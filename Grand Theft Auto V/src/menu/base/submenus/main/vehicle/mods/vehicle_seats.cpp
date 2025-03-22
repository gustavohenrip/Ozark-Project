#include "vehicle_seats.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_mods.hpp"
#include "seats/vehicle_seats_edit.hpp"

using namespace VehicleModsSeatMenuVars;

namespace VehicleModsSeatMenuVars {
	Vars_ m_Vars;

	void Kick(bool ExcludeMe) {
		if (!Menu::GetLocalPlayer().m_InVehicle) return;
		
		for (int i = -1; i < Native::GetVehicleMaxNumberOfPassengers(Menu::GetLocalPlayer().m_Vehicle); i++) {
			if (!Native::IsVehicleSeatFree(Menu::GetLocalPlayer().m_Vehicle, i, 0)) {
				if (ExcludeMe && Native::GetPedInVehicleSeat(Menu::GetLocalPlayer().m_Vehicle, i, 0) == Menu::GetLocalPlayer().m_Ped) break;
				Native::ClearPedTasksImmediately(Native::GetPedInVehicleSeat(Menu::GetLocalPlayer().m_Vehicle, i, 0));
			}
		}
	}

	void KickDriver() {
		if (!Native::IsVehicleSeatFree(Menu::GetLocalPlayer().m_Vehicle, -1, 0)) {
			Native::ClearPedTasksImmediately(Native::GetPedInVehicleSeat(Menu::GetLocalPlayer().m_Vehicle, -1, 0));
		}
	}
}

void VehicleModsSeatMenu::Init() {
	SetName("Seats");
	SetParentSubmenu<VehicleModsMenu>();

	addOption(ButtonOption("Kick All Seats")
		.addTranslation().addHotkey()
		.addOnClick([] { Kick(false); }));

	addOption(ButtonOption("Kick All Seats (Exclude Me)")
		.addTranslation().addHotkey()
		.addOnClick([] { Kick(true); }));

	addOption(ButtonOption("Kick Driver")
		.addTranslation().addHotkey()
		.addOnClick(KickDriver));

	addOption(BreakOption("Seats")
		.addTranslation());
}

void VehicleModsSeatMenu::Update() {}

/*Called once on submenu open*/
void VehicleModsSeatMenu::UpdateOnce() {
	ClearOptionsOffset(4);

	if (!Menu::GetLocalPlayer().m_InVehicle) return;

	for (int i = -1; i < Native::GetVehicleMaxNumberOfPassengers(Menu::GetLocalPlayer().m_Vehicle); i++) {
		bool Free = Native::IsVehicleSeatFree(Menu::GetLocalPlayer().m_Vehicle, i, 0);
		bool Driver = i == -1;

		std::string N = "";
		if (Free) N = "Free Seat";
		else {
			auto Ped = Native::GetPedInVehicleSeat(Menu::GetLocalPlayer().m_Vehicle, i, 0);
			Menu::GetPlayerManager()->Foreach([&] (Menu::PlayerVars& Player) {
				if (Player.m_Ped == Ped) {
					N = Player.m_Name;
					if (Player.m_ID == Menu::GetLocalPlayer().m_ID) {
						N = "Me";
					}
				}
			}, true);

			if (N.empty()) {
				N = "Ped - " + std::to_string(Ped);
			}
		}

		char Name[100];
		sprintf(Name, "[Seat %i] %s%s", i + 1, Driver ? "[Driver] " : "", N.c_str());

		addOption(SubmenuOption(Name)
			.addSubmenu<VehicleModsSeatEditMenu>()
			.addOnClick([=] { VehicleModsSeatEditMenuVars::m_Vars.m_Seat = i; }));
	}
}

/*Called always*/
void VehicleModsSeatMenu::FeatureUpdate() {}

VehicleModsSeatMenu* _instance;
VehicleModsSeatMenu* VehicleModsSeatMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleModsSeatMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleModsSeatMenu::~VehicleModsSeatMenu() { delete _instance; }