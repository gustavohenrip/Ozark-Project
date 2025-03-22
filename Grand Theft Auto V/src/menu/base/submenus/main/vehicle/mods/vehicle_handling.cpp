#include "vehicle_handling.hpp"
#include "menu/base/submenu_handler.hpp"
#include "vehicle_modifiers.hpp"
#include "handling/vehicle_handling_attributes.hpp"
#include "handling/vehicle_handling_main.hpp"
#include "handling/vehicle_handling_save_load.hpp"

using namespace VehicleModsHandlingMenuVars;

namespace VehicleModsHandlingMenuVars {
	Vars_ m_Vars;
}

void VehicleModsHandlingMenu::Init() {
	SetName("Vehicle Handling");
	SetParentSubmenu<VehicleModsModifiersMenu>();

	addString("Boat Handling");
	addString("Bike Handling");
	addString("Flying Handling");

	addOption(SubmenuOption("Save and Load")
		.addTranslation()
		.addSubmenu<VehicleModsHandlingSaveLoadMenu>());

	addOption(SubmenuOption("Main Handling")
		.addTranslation()
		.addSubmenu<VehicleModsHandlingMainMenu>()
		.addOnClick([] { VehicleModsHandlingMainMenuVars::m_Vars.m_Type = 0; }));

	addOption(SubmenuOption(getString("Boat Handling"))
		.addSubmenu<VehicleModsHandlingMainMenu>()
		.addOnClick([] { VehicleModsHandlingMainMenuVars::m_Vars.m_Type = 1; })
		.addRequirement([] { return m_Vars.m_InBoat; }));

	addOption(SubmenuOption(getString("Bike Handling"))
		.addSubmenu<VehicleModsHandlingMainMenu>()
		.addOnClick([] { VehicleModsHandlingMainMenuVars::m_Vars.m_Type = 2; })
		.addRequirement([] { return m_Vars.m_InBike; }));

	addOption(SubmenuOption(getString("Flying Handling"))
		.addSubmenu<VehicleModsHandlingMainMenu>()
		.addOnClick([] { VehicleModsHandlingMainMenuVars::m_Vars.m_Type = 3; })
		.addRequirement([] { return m_Vars.m_InFlying; }));

	// Greyed out
	addOption(ButtonOption("~c~" + getString("Boat Handling"))
		.addRequirement([] { return !m_Vars.m_InBoat; }));

	addOption(ButtonOption("~c~" + getString("Bike Handling"))
		.addRequirement([] { return !m_Vars.m_InBike; }));

	addOption(ButtonOption("~c~" + getString("Flying Handling"))
		.addRequirement([] { return !m_Vars.m_InFlying; }));
}

void VehicleModsHandlingMenu::Update() {
	if (!Menu::GetLocalPlayer().m_InVehicle) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}
}

/*Called once on submenu open*/
void VehicleModsHandlingMenu::UpdateOnce() {
	m_Vars.m_InBoat = false;
	m_Vars.m_InBike = false;
	m_Vars.m_InFlying = false;

	if (Menu::GetLocalPlayer().m_InVehicle) {
		uint32_t Model = Native::GetEntityModel(Menu::GetLocalPlayer().m_Vehicle);
		m_Vars.m_InBoat = Native::IsThisModelABoat(Model) || Native::_IsThisModelAnEmergencyBoat(Model);
		m_Vars.m_InBike = Native::IsThisModelABicycle(Model) || Native::IsThisModelABike(Model);
		m_Vars.m_InFlying = Native::IsThisModelAPlane(Model) | Native::IsThisModelAHeli(Model);
	}
}

/*Called always*/
void VehicleModsHandlingMenu::FeatureUpdate() {}

VehicleModsHandlingMenu* _instance;
VehicleModsHandlingMenu* VehicleModsHandlingMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleModsHandlingMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleModsHandlingMenu::~VehicleModsHandlingMenu() { delete _instance; }