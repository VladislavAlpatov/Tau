#include "SSDK.h"
#include "../Utils/memory.h"

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
