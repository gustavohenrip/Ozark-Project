#include "vehicle_seats_edit.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_seats.hpp"

using namespace VehicleModsSeatEditMenuVars;

namespace VehicleModsSeatEditMenuVars {
	Vars_ m_Vars;

	bool IsPlayer() {
		if (Menu::GetLocalPlayer().m_InVehicle) {
			return Native::IsPedAPlayer(Native::GetPedInVehicleSeat(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_Seat, 0));
		}

		return false;
	}

	void Click() {
		if (Menu::GetLocalPlayer().m_InVehicle) {
			Native::ClearPedTasksImmediately(Native::GetPedInVehicleSeat(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_Seat, 0));
		}
	}
}

void VehicleModsSeatEditMenu::Init() {
	SetName("Seats");
	SetParentSubmenu<VehicleModsSeatMenu>();

	addOption(ButtonOption("Kick")
		.addTranslation()
		.addOnClick(Click));
}

void VehicleModsSeatEditMenu::Update() {}

/*Called once on submenu open*/
void VehicleModsSeatEditMenu::UpdateOnce() {}

/*Called always*/
void VehicleModsSeatEditMenu::FeatureUpdate() {}

VehicleModsSeatEditMenu* _instance;
VehicleModsSeatEditMenu* VehicleModsSeatEditMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleModsSeatEditMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleModsSeatEditMenu::~VehicleModsSeatEditMenu() { delete _instance; }