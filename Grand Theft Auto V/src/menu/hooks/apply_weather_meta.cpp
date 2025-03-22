#include "hooks.hpp"
#include "menu/base/submenus/main/world/world_weather.hpp"

void Menu::Hooks::ApplyWeatherMetaHook(uint64_t Meta) {
	OriginalApplyWeatherMeta(Meta);

	if (WorldWeatherMenuVars::m_Vars.m_WindSpeed == 100000.f) {
		WorldWeatherMenuVars::m_Vars.m_WindSpeed = *(float*)(Meta + 0xEA8);
	}

	*(float*)(Meta + 0xEB4) = WorldWeatherMenuVars::m_Vars.m_WindSpeed * 12.f;
	*(float*)(Meta + 0xEA8) = WorldWeatherMenuVars::m_Vars.m_WindSpeed;
}