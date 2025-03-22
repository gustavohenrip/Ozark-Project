#include "vehicle_randomization.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_mods.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/timers.hpp"
#include "menu/base/base.hpp"
#include "menu/base/util/instructionals.hpp"
#include "menu/base/submenus/main/helpers/rainbow_config.hpp"

using namespace VehicleModsRandomizationMenuVars;

namespace VehicleModsRandomizationMenuVars {
	Vars_ m_Vars;
}

void VehicleModsRandomizationMenu::Init() {
	SetName("Randomization");
	SetParentSubmenu<VehicleModsMenu>();

	m_DefaultInstructionals = false;

	addOption(ToggleOption("Rainbow Primary Paint")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_RainbowPrimary)
		.addOnClick([] {
			m_Vars.m_RainbowPrimaryRun.Toggle(m_Vars.m_RainbowPrimary);
			if (m_Vars.m_RainbowPrimary) {
				if (Menu::GetLocalPlayer().m_InVehicle) {
					Native::GetVehicleCustomPrimaryColour(Menu::GetLocalPlayer().m_Vehicle, &m_Vars.m_RainbowPrimaryColor.R, &m_Vars.m_RainbowPrimaryColor.G, &m_Vars.m_RainbowPrimaryColor.B);
				}

				m_Vars.m_RainbowPrimaryRun.Add(&m_Vars.m_RainbowPrimaryColor);
			} else {
				m_Vars.m_RainbowPrimaryRun.Remove(&m_Vars.m_RainbowPrimaryColor);
			}
		}));

	addOption(ToggleOption("Rainbow Secondary Paint")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_RainbowSecondary)
		.addOnClick([] {
			m_Vars.m_RainbowSecondaryRun.Toggle(m_Vars.m_RainbowSecondary);
			if (m_Vars.m_RainbowSecondary) {
				if (Menu::GetLocalPlayer().m_InVehicle) {
					Native::GetVehicleCustomSecondaryColour(Menu::GetLocalPlayer().m_Vehicle, &m_Vars.m_RainbowSecondaryColor.R, &m_Vars.m_RainbowSecondaryColor.G, &m_Vars.m_RainbowSecondaryColor.B);
				}

				m_Vars.m_RainbowSecondaryRun.Add(&m_Vars.m_RainbowSecondaryColor);
			} else {
				m_Vars.m_RainbowSecondaryRun.Remove(&m_Vars.m_RainbowSecondaryColor);
			}
		}));

	addOption(ToggleOption("Rainbow Tire Smoke")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_RainbowTireSmoke)
		.addOnClick([] {
			m_Vars.m_RainbowTireSmokeRun.Toggle(m_Vars.m_RainbowTireSmoke);
			if (m_Vars.m_RainbowTireSmoke) {
				if (Menu::GetLocalPlayer().m_InVehicle) {
					Native::GetVehicleTyreSmokeColor(Menu::GetLocalPlayer().m_Vehicle, &m_Vars.m_RainbowTireSmokeColor.R, &m_Vars.m_RainbowTireSmokeColor.G, &m_Vars.m_RainbowTireSmokeColor.B);
				}

				m_Vars.m_RainbowTireSmokeRun.Add(&m_Vars.m_RainbowTireSmokeColor);
			} else {
				m_Vars.m_RainbowTireSmokeRun.Remove(&m_Vars.m_RainbowTireSmokeColor);
			}
		}));

	addOption(ToggleOption("Random Upgrades")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_RandomUpgrades));

	addOption(ToggleOption("Random Acrobatics")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_RandomAcrobatics));
}

void VehicleModsRandomizationMenu::Update() {
	if (Menu::GetBase()->m_CurrentOption <= 2) {
		Instructionals::Rainbow();

		if (Menu::Util::GetInput()->IsJustPressed(false, ControlFrontendX)) {
			RainbowConfigMenuVars::m_Vars.m_Rainbow = Menu::GetBase()->m_CurrentOption == 0 ? &m_Vars.m_RainbowPrimaryRun : &m_Vars.m_RainbowSecondaryRun;
			RainbowConfigMenu::GetInstance()->SetParentSubmenu<VehicleModsRandomizationMenu>();
			Menu::GetSubmenuHandler()->SetSubmenu(RainbowConfigMenu::GetInstance());
		}
	} else Instructionals::Standard();
}

/*Called once on submenu open*/
void VehicleModsRandomizationMenu::UpdateOnce() {}

/*Called always*/
void VehicleModsRandomizationMenu::FeatureUpdate() {
	if (Menu::GetLocalPlayer().m_InVehicle) {
		if (m_Vars.m_RainbowPrimary) {
			m_Vars.m_RainbowPrimaryRun.Run();
			Native::SetVehicleCustomPrimaryColour(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_RainbowPrimaryColor.R, m_Vars.m_RainbowPrimaryColor.G, m_Vars.m_RainbowPrimaryColor.B);
		}

		if (m_Vars.m_RainbowSecondary) {
			m_Vars.m_RainbowSecondaryRun.Run();
			Native::SetVehicleCustomSecondaryColour(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_RainbowSecondaryColor.R, m_Vars.m_RainbowSecondaryColor.G, m_Vars.m_RainbowSecondaryColor.B);
		}

		if (m_Vars.m_RainbowTireSmoke) {
			m_Vars.m_RainbowTireSmokeRun.Run();
			Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
			Native::ToggleVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TIRESMOKE, true);
			Native::SetVehicleTyreSmokeColor(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_RainbowTireSmokeColor.R, m_Vars.m_RainbowTireSmokeColor.G, m_Vars.m_RainbowTireSmokeColor.B);
		}

		if (m_Vars.m_RandomUpgrades) {
			Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
			int Class = Native::GetRandomIntInRange(MOD_SPOILER - 1, MOD_LIVERY - 1);
			int Max = Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, Class);
			int Mod = Native::GetRandomIntInRange(-2, Max + 1);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, Class, Mod, 0);
		}

		if (m_Vars.m_RandomAcrobatics) {
			static int Timer = 0;
			Menu::Timers::RunTimedFunction(&Timer, 5000, [] {
				auto Data = Global::Arrays::g_Acrobatics[Native::GetRandomIntInRange(0, 9)].m_Result;
				Native::ApplyForceToEntity(Menu::GetLocalPlayer().m_Vehicle, 1, Data.first.m_X, Data.first.m_Y, Data.first.m_Z, Data.second.m_X, Data.second.m_Y, Data.second.m_Z, 0, true, true, true, false, true);
			});
		}
	}
}

VehicleModsRandomizationMenu* _instance;
VehicleModsRandomizationMenu* VehicleModsRandomizationMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleModsRandomizationMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleModsRandomizationMenu::~VehicleModsRandomizationMenu() { delete _instance; }