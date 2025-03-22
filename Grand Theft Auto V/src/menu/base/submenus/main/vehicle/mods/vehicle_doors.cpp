#include "vehicle_doors.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_mods.hpp"

using namespace VehicleModsDoorMenuVars;

namespace VehicleModsDoorMenuVars {
	Vars_ m_Vars;
}

void VehicleModsDoorMenu::Init() {
	SetName("Doors");
	SetParentSubmenu<VehicleModsMenu>();

	addOption(NumberOption<int>(SCROLLSELECT, "Open Door")
		.addTranslation().addHotkey()
		.addNumber(m_Vars.m_OpenDoor, "%i", 1).addMin(0).addMax(1)
		.addOnUpdate([] (NumberOption<int>* Option) {
			if (Menu::GetLocalPlayer().m_InVehicle) {
				Option->addMax(Native::_GetNumberOfVehicleDoors(Menu::GetLocalPlayer().m_Vehicle));
			}
		})
		.addOnClick([] {
			if (Menu::GetLocalPlayer().m_InVehicle) {
				Native::SetVehicleDoorOpen(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_OpenDoor, false, false);
			}
		}));

	addOption(NumberOption<int>(SCROLLSELECT, "Close Door")
		.addTranslation().addHotkey()
		.addNumber(m_Vars.m_CloseDoor, "%i", 1).addMin(0).addMax(1)
		.addOnUpdate([] (NumberOption<int>* Option) {
			if (Menu::GetLocalPlayer().m_InVehicle) {
				Option->addMax(Native::_GetNumberOfVehicleDoors(Menu::GetLocalPlayer().m_Vehicle));
			}
		})
		.addOnClick([] {
			if (Menu::GetLocalPlayer().m_InVehicle) {
				Native::SetVehicleDoorShut(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_CloseDoor, false);
			}
		}));

	addOption(NumberOption<int>(SCROLLSELECT, "Delete Door")
		.addTranslation().addHotkey()
		.addNumber(m_Vars.m_DeleteDoor, "%i", 1).addMin(0).addMax(1)
		.addOnUpdate([] (NumberOption<int>* Option) {
			if (Menu::GetLocalPlayer().m_InVehicle) {
				Option->addMax(Native::_GetNumberOfVehicleDoors(Menu::GetLocalPlayer().m_Vehicle));
			}
		})
		.addOnClick([] {
			if (Menu::GetLocalPlayer().m_InVehicle) {
				Native::SetVehicleDoorBroken(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_DeleteDoor, true);
			}
		}));

	addOption(ButtonOption("Lock Doors")
		.addTranslation().addHotkey()
		.addOnClick([] { if (Menu::GetLocalPlayer().m_InVehicle) Native::SetVehicleDoorsLocked(Menu::GetLocalPlayer().m_Vehicle, 4); }));

	addOption(ButtonOption("Unlock Doors")
		.addTranslation().addHotkey()
		.addOnClick([] { if (Menu::GetLocalPlayer().m_InVehicle) Native::SetVehicleDoorsLocked(Menu::GetLocalPlayer().m_Vehicle, 0); }));
}

void VehicleModsDoorMenu::Update() {}

/*Called once on submenu open*/
void VehicleModsDoorMenu::UpdateOnce() {}

/*Called always*/
void VehicleModsDoorMenu::FeatureUpdate() {}

VehicleModsDoorMenu* _instance;
VehicleModsDoorMenu* VehicleModsDoorMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleModsDoorMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleModsDoorMenu::~VehicleModsDoorMenu() { delete _instance; }