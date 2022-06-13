#pragma once
#include "AimBot.h"
#include <algorithm>
#include "../../Utils/Math.h"
#include "../../SSDK/IClientEntityList.h"


using namespace Hacks;

ImVec3 Predict(const SSDK::CBaseEntity* pEnt,float projSpeed)
{
	auto pLocalPlayer = SSDK::GetLocalPlayer();
	ImVec3 vecOut;
	float fTime = pEnt->DistTo(pLocalPlayer) / projSpeed;

	if (pEnt->m_fFlags == 769)
	{
		vecOut = pEnt->m_vecOrigin + (pEnt->m_vecVelocity * fTime);
	}
	else
	{
		vecOut = pEnt->m_vecOrigin + (pEnt->m_vecVelocity * fTime);
		vecOut.z -= 800.f / 2.f * fTime * fTime;
	}

	return vecOut;
}


CAimBot::CAimBot(SSDK::CUserCmd* ppUsrCmd)
{
	m_pCUsrCmd = ppUsrCmd;
}

void CAimBot::Work()
{

	SSDK::CBaseEntity* pEnt = GetClosestTargetByFov();


	if (!pEnt)
	{
		return;
	}

	if (IfEntityInFov(pEnt))
	{
		AimPlain(pEnt);
	}
	
}
void CAimBot::AimPlain(const SSDK::CBaseEntity* pEnt)
{
	auto calcedAngles = CalcAimViewAngles(pEnt);

	if (fabs(calcedAngles.x) <= 89.f and fabs(calcedAngles.y) <= 180.f)
	{
		m_pCUsrCmd->viewangles = calcedAngles;
	}
}
void CAimBot::AimSmooth(const SSDK::CBaseEntity* pEnt)
{


	ImVec3 targetViewAngle = CalcAimViewAngles(pEnt);

	ImVec3 currentAngle = m_pCUsrCmd->viewangles;

	ImVec3 diff = targetViewAngle - currentAngle;

	//diff = Utils::ClampViewAngles(Utils::NormalizeViewAngles(diff));

	ImVec3 angle = currentAngle;

	//angle.x += diff.x / pAimBotSettings->m_fSmooth;
	//angle.y += diff.y / pAimBotSettings->m_fSmooth;

	angle = ImVec3(); //Utils::ClampViewAngles(Utils::NormalizeViewAngles(angle));
	
	if (fabs(angle.x) <= 89.f and fabs(angle.y) <= 180.f)
	{
		m_pCUsrCmd->viewangles = angle;
	}
}
bool CAimBot::IfEntityInFov(const SSDK::CBaseEntity* entity) const
{

	ImVec3  pLocalPlayerAngles   = m_pCUsrCmd->viewangles;
	ImVec3  targetAngles         = CalcAimViewAngles(entity);
	ImVec3  deltaFov             = pLocalPlayerAngles - targetAngles;

	return fabs(deltaFov.x) <= 60 and fabs(deltaFov.y) <= 60;
}
SSDK::CBaseEntity* CAimBot::GetClosestTargetByDistance()
{
	std::vector<SSDK::CBaseEntity*> validEntities = GetValidEntities();
	
	if (validEntities.size() == 0)
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
			ImVec3 diffFirstEntity = (CalcAimViewAngles(pEntityFirst) - m_pCUsrCmd->viewangles).Abs();
			ImVec3 diffSecondEntity = (CalcAimViewAngles(pEntitySecond) - m_pCUsrCmd->viewangles).Abs();

			return diffFirstEntity.Length2D() < diffSecondEntity.Length2D();
		});

	return validEntities[0];
}

ImVec3 CAimBot::CalcAimViewAngles(const SSDK::CBaseEntity* pEntity)
{
	ImVec3 calculated;

	ImVec3 targetPosition = Predict(pEntity, 1980.f);

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