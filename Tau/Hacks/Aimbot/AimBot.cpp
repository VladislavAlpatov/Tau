#include "AimBot.h"
#include <algorithm>
#include "../../Utils/Math.h"
#include "../../SSDK/IClientEntityList.h"

using namespace Hacks;

ImVec3 LinearPrediction(const SSDK::CBaseEntity* pEnt, float fTime)
{
	const auto pLocalPlayer = SSDK::GetLocalPlayer();
	ImVec3 acceleration = { 0, 0, -800.f };  // Assuming only gravity acts on the entity in the z-direction

	ImVec3 vecOut = pEnt->m_vecOrigin + (pEnt->m_vecVelocity * fTime);

	if (!(pEnt->m_fFlags & 1))
		vecOut += acceleration * 0.5f * fTime * fTime;

	return vecOut;
}

#include <cmath>

// Function to solve the quadratic equation ax^2 + bx + c = 0
bool solveQuadratic(float a, float b, float c, float& x1, float& x2) {
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		return false;
	}
	x1 = (-b + std::sqrt(discriminant)) / (2 * a);
	x2 = (-b - std::sqrt(discriminant)) / (2 * a);
	return true;
}

ImVec3 CAimBot::Predict(const SSDK::CBaseEntity* pEnt, float projSpeed) {
	const float gravity = -800.0f;  // Acceleration due to gravity
	bool isTargetInAir = !(pEnt->m_fFlags & 1);
	ImVec3 myPos = SSDK::GetLocalPlayer()->m_vecOrigin + SSDK::GetLocalPlayer()->m_vecViewOffset;
	ImVec3 targetPos = pEnt->m_vecOrigin + ImVec3(0,0,20);
	ImVec3 targetVel = pEnt->m_vecVelocity;

	// Relative position and velocity
	ImVec3 relPos = targetPos - myPos;
	ImVec3 relVel = targetVel;

	// Constants for the quadratic equation to find the time 't'
	float a = targetVel.LengthSqr() - projSpeed * projSpeed;
	float b = 2 * relPos.Dot(relVel);
	float c = relPos.LengthSqr();

	// Adjust for gravity if the target is in the air
	if (isTargetInAir) {
		a += 0.25f * gravity * gravity;
		b += gravity * relPos.z;
	}

	float t1, t2;
	if (!solveQuadratic(a, b, c, t1, t2)) {
		// No real roots, no intersection
		return targetPos;
	}

	// Choose the smallest positive time
	float intersectTime = (t1 > 0) ? t1 : t2;
	if (t1 > 0 && t2 > 0) {
		intersectTime = min(t1, t2);
	}

	// Calculate the final predicted position
	ImVec3 predictedPos = targetPos + targetVel * intersectTime;

	// If the target is in the air, consider gravity
	if (isTargetInAir) {
		predictedPos += ImVec3(0, 0, 0.5f * gravity * intersectTime * intersectTime);
	}

	return predictedPos;
}

CAimBot::CAimBot(SSDK::CUserCmd* ppUsrCmd)
{
	m_pCUsrCmd = ppUsrCmd;
}

void CAimBot::Work()
{

	SSDK::CBaseEntity* pEnt = GetClosestTargetByFov();
	if (pEnt)
		AimPlain(pEnt);
}
void CAimBot::AimPlain(const SSDK::CBaseEntity* pEnt)
{
	auto calcedAngles = CalcAimViewAngles(pEnt);

	if (fabs(calcedAngles.x) <= 89.f and fabs(calcedAngles.y) <= 180.f)
	{
		m_pCUsrCmd->m_vecViewAngles = calcedAngles;
	}
}
void CAimBot::AimSmooth(const SSDK::CBaseEntity* pEnt)
{


	ImVec3 targetViewAngle = CalcAimViewAngles(pEnt);

	ImVec3 currentAngle = m_pCUsrCmd->m_vecViewAngles;

	ImVec3 diff = targetViewAngle - currentAngle;

	//diff = Utils::ClampViewAngles(Utils::NormalizeViewAngles(diff));

	ImVec3 angle = currentAngle;

	//angle.x += diff.x / pAimBotSettings->m_fSmooth;
	//angle.y += diff.y / pAimBotSettings->m_fSmooth;

	angle = ImVec3(); //Utils::ClampViewAngles(Utils::NormalizeViewAngles(angle));
	
	if (fabs(angle.x) <= 89.f and fabs(angle.y) <= 180.f)
	{
		m_pCUsrCmd->m_vecViewAngles = angle;
	}
}
bool CAimBot::IfEntityInFov(const SSDK::CBaseEntity* entity) const
{

	ImVec3  pLocalPlayerAngles   = m_pCUsrCmd->m_vecViewAngles;
	ImVec3  targetAngles         = CalcAimViewAngles(entity);
	ImVec3  deltaFov             = pLocalPlayerAngles - targetAngles;

	return fabs(deltaFov.x) <= 60 and fabs(deltaFov.y) <= 60;
}
SSDK::CBaseEntity* CAimBot::GetClosestTargetByDistance()
{
	std::vector<SSDK::CBaseEntity*> validEntities = GetValidEntities();
	
	if (validEntities.empty())
		return NULL;
	std::sort(validEntities.begin(), validEntities.end(),

		[](const SSDK::CBaseEntity* first, const SSDK::CBaseEntity* second)
		{
			auto pLocalPlayer = SSDK::GetLocalPlayer();
			return pLocalPlayer->DistTo(first) < pLocalPlayer->DistTo(second);
		});
	return validEntities[0];
}

SSDK::CBaseEntity* CAimBot::GetClosestTargetByFov()
{
	std::vector<SSDK::CBaseEntity*> validEntities = GetValidEntities();

	if (!validEntities.size())
		return NULL;

	auto localPlayer = SSDK::GetLocalPlayer();

	std::sort(validEntities.begin(), validEntities.end(),

		[this](SSDK::CBaseEntity* pEntityFirst, SSDK::CBaseEntity* pEntitySecond)
		{
			ImVec3 diffFirstEntity = (CalcAimViewAngles(pEntityFirst) - m_pCUsrCmd->m_vecViewAngles).Abs();
			ImVec3 diffSecondEntity = (CalcAimViewAngles(pEntitySecond) - m_pCUsrCmd->m_vecViewAngles).Abs();

			return diffFirstEntity.Length2D() < diffSecondEntity.Length2D();
		});

	return validEntities[0];
}

ImVec3 CAimBot::CalcAimViewAngles(const SSDK::CBaseEntity* pEntity)
{
	ImVec3 calculated;

	ImVec3 targetPosition = Predict(pEntity, 1100.f);

	auto pLocalPlayer = SSDK::GetLocalPlayer();
	ImVec3 localCameraPosition = pLocalPlayer->m_vecOrigin + pLocalPlayer->m_vecViewOffset;


	float distance = localCameraPosition.DistTo(targetPosition);

	calculated.x = - RadToDeg(asinf((targetPosition.z - localCameraPosition.z) / distance));
	calculated.y =   RadToDeg(atan2f(targetPosition.y - localCameraPosition.y, targetPosition.x - localCameraPosition.x));

	return calculated;
}

std::vector<SSDK::CBaseEntity*> CAimBot::GetValidEntities() const
{
	static auto pEntList = SSDK::GetInterface<SSDK::IClientEntityList>("client.dll", "VClientEntityList003");


	std::vector<SSDK::CBaseEntity*> validEntities;
	const auto localPlayer = SSDK::GetLocalPlayer();


	for (int i = 1; i < 33; i++)
	{
		SSDK::CBaseEntity* pEnt = reinterpret_cast<SSDK::CBaseEntity*>(pEntList->GetClientEntity(i));

		if (!pEnt)
			continue;

		if (!pEnt->m_bDead and !pEnt->m_bDormant and localPlayer->m_iTeamNum != pEnt->m_iTeamNum and IfEntityInFov(pEnt) and pEnt->m_iHealth)
			validEntities.push_back(pEnt);

	}
	return validEntities;

}