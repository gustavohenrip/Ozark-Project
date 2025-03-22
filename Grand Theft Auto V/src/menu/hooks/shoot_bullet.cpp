#include "hooks.hpp"
#include "menu/base/submenus/main/world.hpp"

bool Menu::Hooks::ShootBulletHook(uint64_t WeaponInstance, uint64_t Rdx, float* StartPos, float* TargetPos) {
	WorldMenuVars::Tracer Trace;
	Trace.m_SpawnTime = GetTickCount();
	Trace.m_SpawnPosition = { StartPos[0], StartPos[1], StartPos[2] };
	Trace.m_EndPosition = { TargetPos[0], TargetPos[1], TargetPos[2] };
	Trace.m_Populated = true;

	if (WorldMenuVars::m_Vars.m_TracerCurrentIndex == 500) {
		WorldMenuVars::m_Vars.m_TracerCurrentIndex = 0;
	}

	memcpy(&WorldMenuVars::m_Vars.m_Tracers[WorldMenuVars::m_Vars.m_TracerCurrentIndex], &Trace, sizeof(Trace));
	WorldMenuVars::m_Vars.m_TracerCurrentIndex++;

	return OriginalShootBullet(WeaponInstance, Rdx, StartPos, TargetPos);
}