#include "world_time.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../world.hpp"
#include "rage/engine.hpp"
#include "utils/memory/memory.hpp"
#include "utils/caller.hpp"

using namespace WorldTimeMenuVars;

namespace WorldTimeMenuVars {
	Vars_ m_Vars;

	ScrollStruct<float> Timescales[] = {
		{ TranslationString("Default", true), 1.f },
		{ TranslationString("Slow", true), 0.7f },
		{ TranslationString("Slower", true), 0.4f },
		{ TranslationString("Slowest", true), 0.f }
	};

	void SyncTime() {
		Memory::Write(Global::Vars::g_SendClockTime + 0x65, (uint8_t)0xEB);
		Caller::Call<int>(Global::Vars::g_SyncClockTime, 0, 0);
		Memory::Write(Global::Vars::g_SendClockTime + 0x65, (uint8_t)0x75);
	}

	void PrecisionTime() {
		if (Menu::Util::GetInput()->IsLeftPressed() || Menu::Util::GetInput()->IsRightPressed()) {
			m_Vars.m_Accelerate = false;
			m_Vars.m_AlwaysDay = false;
			m_Vars.m_AlwaysNight = false;
			m_Vars.m_Freeze = false;
			m_Vars.m_SyncWithSystem = false;

			if (Menu::Util::GetInput()->IsLeftPressed()) {
				Rage::Engine::AddToClockTime(0, -1, 0);
			} else {
				Rage::Engine::AddToClockTime(0, 1, 0);
			}

			Native::NetworkOverrideClockTime(Native::GetClockHours(), Native::GetClockMinutes(), Native::GetClockSeconds());
			if (m_Vars.m_Sync) SyncTime();
		}
	}
}

void WorldTimeMenu::Init() {
	SetName("Time");
	SetParentSubmenu<WorldMenu>();

	addOption(ButtonOption("Advance Time")
		.addTranslation()
		.addOnClick([] {
			int Hour = Native::GetClockHours() + 1;
			if (Hour > 23) Hour = 0;
			Native::NetworkOverrideClockTime(Hour, Native::GetClockMinutes(), Native::GetClockSeconds());
		}));

	addOption(ButtonOption("Rewind Time")
		.addTranslation()
		.addOnClick([] {
			int Hour = Native::GetClockHours() - 1;
			if (Hour < 0) Hour = 23;
			Native::NetworkOverrideClockTime(Hour, Native::GetClockMinutes(), Native::GetClockSeconds());
		}));

	addOption(ButtonOption("Precision Time")
		.addTranslation()
		.addOnHover(PrecisionTime)
		.addTooltip("Use LEFT & RIGHT, only works while hovering on this option"));

	addOption(ToggleOption("Always Day")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_AlwaysDay)
		.addOnClick([] {
			m_Vars.m_Accelerate = false;
			m_Vars.m_Freeze = false;
			m_Vars.m_AlwaysNight = false;
			m_Vars.m_SyncWithSystem = false;
		}));

	addOption(ToggleOption("Always Night")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_AlwaysNight)
		.addOnClick([] {
			m_Vars.m_Accelerate = false;
			m_Vars.m_Freeze = false;
			m_Vars.m_AlwaysDay = false;
			m_Vars.m_SyncWithSystem = false;
		}));

	addOption(ToggleOption("Freeze Time")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Freeze)
		.addOnClick([] {
			m_Vars.m_Accelerate = false;
			m_Vars.m_AlwaysDay = false;
			m_Vars.m_AlwaysNight = false;
			m_Vars.m_SyncWithSystem = false;

			m_Vars.m_FreezeTime[0] = Native::GetClockHours();
			m_Vars.m_FreezeTime[1] = Native::GetClockMinutes();
			m_Vars.m_FreezeTime[2] = Native::GetClockSeconds();
		}));

	addOption(ToggleOption("Accelerate Time")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Accelerate)
		.addOnClick([] {
			m_Vars.m_AlwaysDay = false;
			m_Vars.m_AlwaysNight = false;
			m_Vars.m_Freeze = false;
			m_Vars.m_SyncWithSystem = false;
		}));

	addOption(ToggleOption("Time Epilepsy")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Epilepsy));

	addOption(ScrollOption<float>(SCROLL, "Timescale")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_Timescale, 0, NUMOF(Timescales), Timescales)
		.addOnClick([] { if (m_Vars.m_Timescale == 0) Native::SetTimeScale(1.f); }));

	addOption(ToggleOption("Sync with System Time")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_SyncWithSystem)
		.addOnClick([] {
			m_Vars.m_Accelerate = false;
			m_Vars.m_AlwaysDay = false;
			m_Vars.m_AlwaysNight = false;
			m_Vars.m_Freeze = false;
		}));

	addOption(ToggleOption("Sync to Session")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Sync));
}

void WorldTimeMenu::Update() {}

/*Called once on submenu open*/
void WorldTimeMenu::UpdateOnce() {}

/*Called always*/
void WorldTimeMenu::FeatureUpdate() {
	if (m_Vars.m_Timescale != 0) {
		Native::SetTimeScale(Timescales[m_Vars.m_Timescale].m_Result);
	}

	if (m_Vars.m_SyncWithSystem) {
		int day, month, year, hour, min, sec;
		Native::GetLocalTime(&year, &month, &day, &hour, &min, &sec);
		Native::NetworkOverrideClockTime(hour, min, sec);
		if (m_Vars.m_Sync) SyncTime();
	}

	if (m_Vars.m_Freeze) {
		Native::NetworkOverrideClockTime(m_Vars.m_FreezeTime[0], m_Vars.m_FreezeTime[1], m_Vars.m_FreezeTime[2]);
		if (m_Vars.m_Sync) SyncTime();
	}
	
	if (m_Vars.m_AlwaysNight) {
		Native::NetworkOverrideClockTime(23, 0, 0);
		if (m_Vars.m_Sync) SyncTime();
	}

	if (m_Vars.m_AlwaysDay) {
		Native::NetworkOverrideClockTime(12, 0, 0);
		if (m_Vars.m_Sync) SyncTime();
	}

	if (m_Vars.m_Accelerate) {
		Rage::Engine::AddToClockTime(0, 2, 0);
		Native::NetworkOverrideClockTime(Native::GetClockHours(), Native::GetClockMinutes(), Native::GetClockSeconds());
		if (m_Vars.m_Sync) SyncTime();
	}

	if (m_Vars.m_Epilepsy) {
		static bool Flip = false;
		Flip ^= true;
		Native::NetworkOverrideClockTime(Flip ? 12 : 23, Native::GetClockMinutes(), Native::GetClockSeconds());
		if (m_Vars.m_Sync) SyncTime();
	}
}

WorldTimeMenu* _instance;
WorldTimeMenu* WorldTimeMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WorldTimeMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WorldTimeMenu::~WorldTimeMenu() { delete _instance; }