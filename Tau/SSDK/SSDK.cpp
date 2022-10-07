#include "SSDK.h"
#include "../Utils/memory.h"
#include "../Utils/Math.h"

SSDK::CBaseEntity* SSDK::GetLocalPlayer()
{
   static auto pLocalPlayerPtr = (CBaseEntity***)(Memory::FindPattern("client.dll", "A1 ? ? ? ? 33 C9 83 C4 04") + 1);

   return **pLocalPlayerPtr;
}

matrix4x4 SSDK::GetViewMatrix()
{
	static auto viewMatrix = *(matrix4x4**)(Memory::FindPattern("engine.dll", "68 ? ? ? ? 8B 01 FF 50 50") + 1);
	return *viewMatrix;
}

void viewangles::Normalize()
{
	if (x > 180)
		x -= 360.0f;

	if (x < 180)
		x += 360.0f;

	if (y > 180)
		y -= 360.0f;

	if (y < 180)
		y += 360.0f;
}

viewangles viewangles::Normalized() const
{
	auto tmp = *this;
	tmp.Normalize();

	return tmp;
}

SSDK::fix_usercmd::fix_usercmd(CUserCmd* pUserCmd)
{
	m_pUserCmd = pUserCmd;
	m_oldUserCmd = *pUserCmd;
}

SSDK::fix_usercmd::~fix_usercmd()
{
	float fDeltaYaw;
	float f1;
	float f2;

	if (m_oldUserCmd.m_vecViewAngles.y < 0.f)
		f1 = 360.f + m_oldUserCmd.m_vecViewAngles.y;
	else
		f1 = m_oldUserCmd.m_vecViewAngles.y;


	if (m_pUserCmd->m_vecViewAngles.y < 0.f)
		f2 = 360.f + m_pUserCmd->m_vecViewAngles.y;
	else
		f2 = m_pUserCmd->m_vecViewAngles.y;

	if (f2 < f1)
		fDeltaYaw = abs(f2 - f1);
	else
		fDeltaYaw = 360.f - abs(f1 - f2);

	fDeltaYaw = 360.f - fDeltaYaw;

	m_pUserCmd->m_fForwardMove = cos(DegToRad(fDeltaYaw)) * m_oldUserCmd.m_fForwardMove + cos(DegToRad(fDeltaYaw + 90.f)) * m_oldUserCmd.m_fSideMove;
	m_pUserCmd->m_fSideMove    = sin(DegToRad(fDeltaYaw)) * m_oldUserCmd.m_fForwardMove + sin(DegToRad(fDeltaYaw + 90.f)) * m_oldUserCmd.m_fSideMove;
}

