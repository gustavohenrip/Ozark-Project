#include "vehicle_health.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_mods.hpp"

using namespace VehicleModsHealthMenuVars;

namespace VehicleModsHealthMenuVars {
	Vars_ m_Vars;
}

void VehicleModsHealthMenu::Init() {
	SetName("Health");
	SetParentSubmenu<VehicleModsMenu>();

	addOption(ButtonOption("Repair Vehicle")
		.addTranslation().addHotkey()
		.addOnClick([] { Native::SetVehicleFixed(Menu::GetLocalPlayer().m_Vehicle); }));

	addOption(ButtonOption("Wash Vehicle")
		.addTranslation().addHotkey()
		.addOnClick([] { Native::SetVehicleDirtLevel(Menu::GetLocalPlayer().m_Vehicle, 0.f); }));

	addOption(ButtonOption("Dirty Vehicle")
		.addTranslation().addHotkey()
		.addOnClick([] { Native::SetVehicleDirtLevel(Menu::GetLocalPlayer().m_Vehicle, 15.f); }));

	addOption(ToggleOption("Auto Repair")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_AutoRepair).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Auto Wash")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_AutoWash).canBeSaved(m_ParentNameStack));
}

void VehicleModsHealthMenu::Update() {}

/*Called once on submenu open*/
void VehicleModsHealthMenu::UpdateOnce() {}

/*Called always*/
void VehicleModsHealthMenu::FeatureUpdate() {
	if (Menu::GetLocalPlayer().m_InVehicle) {
		if (m_Vars.m_AutoRepair) {
			if (Native::_IsVehicleDamaged(Menu::GetLocalPlayer().m_Vehicle)) {
				Native::SetVehicleFixed(Menu::GetLocalPlayer().m_Vehicle);
			}
		}

		if (m_Vars.m_AutoWash) {
			Native::SetVehicleDirtLevel(Menu::GetLocalPlayer().m_Vehicle, 0.f);
		}
	}
}

VehicleModsHealthMenu* _instance;
VehicleModsHealthMenu* VehicleModsHealthMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleModsHealthMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleModsHealthMenu::~VehicleModsHealthMenu() { delete _instance; }