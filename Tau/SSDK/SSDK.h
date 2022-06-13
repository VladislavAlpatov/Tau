#pragma once
#include "CBaseEntity.h"
#include <Windows.h>

class matrix4x4
{
public:
    float* operator[ ](int index)
    {
        return m_RawData[index];
    }

private:
    float m_RawData[4][4];
};

namespace SSDK
{
    CBaseEntity* GetLocalPlayer();
    matrix4x4 GetViewMatrix();

	class CUserCmd
	{
	public:

		int pad;
		int command_number;
		int tick_count;
		ImVec3 viewangles;
		ImVec3 aimdirection;
		float forwardmove;
		float sidemove;
		float upmove;
		int buttons;
		char impulse;
		int weaponselect;
		int weaponsubtype;
		int randomseed;
		__int16 mousedx;
		__int16 mousedy;
		bool hasbeenpredicted;
	};
}