#pragma once
#include "../../SSDK/SSDK.h"
#include <vector>

namespace Hacks
{
	class CAimBot
	{
	public:
		CAimBot(SSDK::CUserCmd* pUsrCmd);
		virtual void Work();
		static ImVec3                    CalcAimViewAngles(const SSDK::CBaseEntity* pEntity);
		static ImVec3 Predict(const SSDK::CBaseEntity* pEnt, float projSpeed);
	private:
		SSDK::CUserCmd* m_pCUsrCmd = nullptr;

		bool                      IfEntityInFov(const SSDK::CBaseEntity* pEntity) const;
		void                      AimSmooth(const SSDK::CBaseEntity* pEntity);
		void                      AimPlain(const  SSDK::CBaseEntity* pEntity);
		SSDK::CBaseEntity*        GetClosestTargetByDistance();
		SSDK::CBaseEntity*          GetClosestTargetByFov();

		// Get All enemy entites that are Alive, In fov range and can be seen 
		std::vector<SSDK::CBaseEntity*> GetValidEntities() const;
	};
}