#include "CBaseEntity.h"

ImColor SSDK::CBaseEntity::GetHealthColor() const
{
	auto healthRatio = (float)m_iHealth / (float)m_iMaxHealth;

	if (healthRatio >= 0.5f)
		return ImColor(1.f - (healthRatio - 0.5f) * 2.f, 1.f, 0.f);

	return ImColor(1.f, healthRatio * 2.f, 0.f);
}

float SSDK::CBaseEntity::DistTo(const SSDK::CBaseEntity* pEnt) const
{
	return (m_vecOrigin + m_vecViewOffset).DistTo(pEnt->m_vecOrigin);

}