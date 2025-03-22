#include "vehicle_handling_main.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_handling.hpp"
#include "vehicle_handling_boat.hpp"
#include "vehicle_handling_flying.hpp"
#include "vehicle_handling_bike.hpp"
#include "vehicle_handling_attributes.hpp"

using namespace VehicleModsHandlingMainMenuVars;

namespace VehicleModsHandlingMainMenuVars {
	Vars_ m_Vars;
}

void VehicleModsHandlingMainMenu::Init() {
	SetName("Handling");
	SetParentSubmenu<VehicleModsHandlingMenu>();

	VehicleModsHandlingAttributesMenu::GetInstance();
	VehicleHandlingBoatMenu::GetInstance();
	VehicleHandlingBikeMenu::GetInstance();
	VehicleHandlingFlyingMenu::GetInstance();

	addString("Physical Attributes");
	addString("Transmission Attributes");
	addString("Wheel Traction Attributes");
	addString("Suspension Attributes");
	addString("Damage Attributes");
	addString("Miscellaneous Attributes");

	addString("Aquaplane Attributes");
	addString("Rudder Attributes");
	addString("Resistance Attributes");

	addString("Balance Attributes");
	addString("Friction Attributes");

	addString("Thrust Attributes");
	addString("Movement Attributes");
	addString("Turbulence Attributes");
	addString("Gear Attributes");
	addString("Ground Attributes");
}

void VehicleModsHandlingMainMenu::Update() {
	if (!Menu::GetLocalPlayer().m_InVehicle) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}
}

/*Called once on submenu open*/
void VehicleModsHandlingMainMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	/* Vehicle Handling */
	if (m_Vars.m_Type == 0) {
		addOption(SubmenuOption(getString("Physical Attributes"))
			.addSubmenu<VehicleModsHandlingAttributesMenu>()
			.addOnClick([] { VehicleModsHandlingAttributesMenuVars::m_Vars.m_Type = 0; }));

		addOption(SubmenuOption(getString("Transmission Attributes"))
			.addSubmenu<VehicleModsHandlingAttributesMenu>()
			.addOnClick([] { VehicleModsHandlingAttributesMenuVars::m_Vars.m_Type = 1; }));

		addOption(SubmenuOption(getString("Wheel Traction Attributes"))
			.addSubmenu<VehicleModsHandlingAttributesMenu>()
			.addOnClick([] { VehicleModsHandlingAttributesMenuVars::m_Vars.m_Type = 2; }));

		addOption(SubmenuOption(getString("Suspension Attributes"))
			.addSubmenu<VehicleModsHandlingAttributesMenu>()
			.addOnClick([] { VehicleModsHandlingAttributesMenuVars::m_Vars.m_Type = 3; }));

		addOption(SubmenuOption(getString("Damage Attributes"))
			.addSubmenu<VehicleModsHandlingAttributesMenu>()
			.addOnClick([] { VehicleModsHandlingAttributesMenuVars::m_Vars.m_Type = 4; }));

		addOption(SubmenuOption(getString("Miscellaneous Attributes"))
			.addSubmenu<VehicleModsHandlingAttributesMenu>()
			.addOnClick([] { VehicleModsHandlingAttributesMenuVars::m_Vars.m_Type = 5; }));
	}

	/* Boat */
	if (m_Vars.m_Type == 1) {
		addOption(SubmenuOption(getString("Physical Attributes"))
			.addSubmenu<VehicleHandlingBoatMenu>()
			.addOnClick([] { VehicleHandlingBoatMenuVars::m_Vars.m_Type = 0; }));

		addOption(SubmenuOption(getString("Aquaplane Attributes"))
			.addSubmenu<VehicleHandlingBoatMenu>()
			.addOnClick([] { VehicleHandlingBoatMenuVars::m_Vars.m_Type = 1; }));

		addOption(SubmenuOption(getString("Rudder Attributes"))
			.addSubmenu<VehicleHandlingBoatMenu>()
			.addOnClick([] { VehicleHandlingBoatMenuVars::m_Vars.m_Type = 2; }));

		addOption(SubmenuOption(getString("Resistance Attributes"))
			.addSubmenu<VehicleHandlingBoatMenu>()
			.addOnClick([] { VehicleHandlingBoatMenuVars::m_Vars.m_Type = 3; }));

		addOption(SubmenuOption(getString("Miscellaneous Attributes"))
			.addSubmenu<VehicleHandlingBoatMenu>()
			.addOnClick([] { VehicleHandlingBoatMenuVars::m_Vars.m_Type = 4; }));
	}

	/* Bike */
	if (m_Vars.m_Type == 2) {
		addOption(SubmenuOption(getString("Physical Attributes"))
			.addSubmenu<VehicleHandlingBikeMenu>()
			.addOnClick([] { VehicleHandlingBikeMenuVars::m_Vars.m_Type = 0; }));

		addOption(SubmenuOption(getString("Balance Attributes"))
			.addSubmenu<VehicleHandlingBikeMenu>()
			.addOnClick([] { VehicleHandlingBikeMenuVars::m_Vars.m_Type = 1; }));

		addOption(SubmenuOption(getString("Friction Attributes"))
			.addSubmenu<VehicleHandlingBikeMenu>()
			.addOnClick([] { VehicleHandlingBikeMenuVars::m_Vars.m_Type = 2; }));
	}

	/* Flying */
	if (m_Vars.m_Type == 3) {
		addOption(SubmenuOption(getString("Thrust Attributes"))
			.addSubmenu<VehicleHandlingFlyingMenu>()
			.addOnClick([] { VehicleHandlingFlyingMenuVars::m_Vars.m_Type = 0; }));

		addOption(SubmenuOption(getString("Movement Attributes"))
			.addSubmenu<VehicleHandlingFlyingMenu>()
			.addOnClick([] { VehicleHandlingFlyingMenuVars::m_Vars.m_Type = 1; }));

		addOption(SubmenuOption(getString("Resistance Attributes"))
			.addSubmenu<VehicleHandlingFlyingMenu>()
			.addOnClick([] { VehicleHandlingFlyingMenuVars::m_Vars.m_Type = 2; }));

		addOption(SubmenuOption(getString("Turbulence Attributes"))
			.addSubmenu<VehicleHandlingFlyingMenu>()
			.addOnClick([] { VehicleHandlingFlyingMenuVars::m_Vars.m_Type = 3; }));

		addOption(SubmenuOption(getString("Gear Attributes"))
			.addSubmenu<VehicleHandlingFlyingMenu>()
			.addOnClick([] { VehicleHandlingFlyingMenuVars::m_Vars.m_Type = 4; }));

		addOption(SubmenuOption(getString("Ground Attributes"))
			.addSubmenu<VehicleHandlingFlyingMenu>()
			.addOnClick([] { VehicleHandlingFlyingMenuVars::m_Vars.m_Type = 5; }));

		addOption(SubmenuOption(getString("Miscellaneous Attributes"))
			.addSubmenu<VehicleHandlingFlyingMenu>()
			.addOnClick([] { VehicleHandlingFlyingMenuVars::m_Vars.m_Type = 6; }));
	}
}

/*Called always*/
void VehicleModsHandlingMainMenu::FeatureUpdate() {}

VehicleModsHandlingMainMenu* _instance;
VehicleModsHandlingMainMenu* VehicleModsHandlingMainMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleModsHandlingMainMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleModsHandlingMainMenu::~VehicleModsHandlingMainMenu() { delete _instance; }