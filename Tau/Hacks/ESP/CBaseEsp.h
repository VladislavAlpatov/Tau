#pragma once
#include "../../SSDK/CBaseEntity.h"
#include "../../imgui/ImVec3.h"

namespace Esp
{

	class EntityBox
	{
	public:

		ImVec2 m_vecTopLeft;
		ImVec2 m_vecTopRight;

		ImVec2 m_vecBottomLeft;
		ImVec2 m_vecBottomRight;

		ImVec2 GetSize() const;
	};

	using namespace SSDK;

	class CBaseEsp
	{
	public:
		__forceinline void RenderAt(SSDK::CBaseEntity* pEntity)
		{
			if (IsEntityOnScreen(pEntity))
				InternalRenderAt(pEntity);

		}
		static ImVec3 WorldToScreen(const ImVec3& pos);
	protected:
		EntityBox CalcEspBox(const SSDK::CBaseEntity* pEntity);
		virtual void InternalRenderAt(SSDK::CBaseEntity* pEntity) = 0;

		template <typename Type>
		__forceinline Type* GetSettings()
		{
			return NULL;
		}

	private:

		bool IsEntityOnScreen(const SSDK::CBaseEntity* pEnt)
		{
			if (WorldToScreen(pEnt->m_vecOrigin).z > 0.f)
				return true;
			return false;
		}
	};
}