#pragma once
#include "../imgui/ImVec3.h"

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

namespace SSDK
{
	class CBaseEntity
	{
	public:
		union
		{
			DEFINE_MEMBER_N(int, m_iTeamNum, 0xB0);
			DEFINE_MEMBER_N(int, m_iHealth, 0xA8);
			DEFINE_MEMBER_N(ImVec3, m_vecOrigin, 0x364);
			DEFINE_MEMBER_N(bool, m_bDead, 0xA5);
			DEFINE_MEMBER_N(ImVec3, m_vecVelocity, 0x120);
			DEFINE_MEMBER_N(bool, m_bDormant, 0x1AA);
			DEFINE_MEMBER_N(int, m_iMaxHealth, 0x17A0);
			DEFINE_MEMBER_N(ImVec3, m_vecViewOffset, 0xfc);
			DEFINE_MEMBER_N(int, m_fFlags, 0x037C);
		};
		ImColor GetHealthColor() const;
		float DistTo(const SSDK::CBaseEntity* pEnt) const;
	};


}

