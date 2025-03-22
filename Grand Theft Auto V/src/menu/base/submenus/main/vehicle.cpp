#include "vehicle.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../main.hpp"
#include "vehicle/vehicle_spawner.hpp"
#include "vehicle/vehicle_editor.hpp"
#include "vehicle/vehicle_mods.hpp"
#include "vehicle/vehicle_local.hpp"
#include "vehicle/vehicle_save_load.hpp"

using namespace VehicleMenuVars;

namespace VehicleMenuVars {
	Vars_ m_Vars;

	bool CanRunLosSantosCustoms() {
		if (!Menu::GetLocalPlayer().m_InVehicle) return false;

		for (int i = MOD_SPOILER; i <= MOD_BACKWHEELS; i++) {
			Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
			if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, i) > 0) {
				return true;
			}
		}

		return false;
	}

	bool CanRunBennys() {
		if (!Menu::GetLocalPlayer().m_InVehicle) return false;

		int Count = 0;

		for (int i = MOD_PLATEHOLDER; i <= MOD_LIVERY; i++) {
			Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
			if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, i) > 0) {
				Count++;
			}
		}

		if (Count > 1) return true;

		// fuck livery
		if (Count == 1) {
			if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_LIVERY) > 0) return false;
			return true;
		}

		return false;
	}
}

void VehicleMenu::Init() {
	SetName("Vehicle");
	SetParentSubmenu<MainMenu>();

	addOption(SubmenuOption("Vehicle Spawner")
		.addTranslation().addHotkey()
		.addOnClick([] { VehicleSpawnerMenu::GetInstance()->SetParentSubmenu<VehicleMenu>(); })
		.addSubmenu<VehicleSpawnerMenu>());

	addOption(SubmenuOption("Vehicle Mods")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleModsMenu>());

	addOption(SubmenuOption("Los Santos Customs")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleEditorMenu>()
		.addOnClick([] {
			VehicleEditorMenuVars::m_Vars.m_LSC = true;
			Global::UIVars::g_InsideLSC = true;
		})
		.addRequirement(CanRunLosSantosCustoms));

	addOption(SubmenuOption("Benny's Original Motorworks")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleEditorMenu>()
		.addOnClick([] {
			VehicleEditorMenuVars::m_Vars.m_LSC = false;
			Global::UIVars::g_InsideBennys = true;
		})
		.addRequirement(CanRunBennys));

	addOption(SubmenuOption("Local Vehicles")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleLocalMenu>());

	addOption(SubmenuOption("Save and Load")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleSaveLoadMenu>()
		.addTooltip("Save and load your modifications"));
}

void VehicleMenu::Update() {}

/*Called once on submenu open*/
void VehicleMenu::UpdateOnce() {
	Global::UIVars::g_InsideLSC = false;
	Global::UIVars::g_InsideBennys = false;
}

/*Called always*/
void VehicleMenu::FeatureUpdate() {}

VehicleMenu* _instance;
VehicleMenu* VehicleMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleMenu::~VehicleMenu() { delete _instance; }