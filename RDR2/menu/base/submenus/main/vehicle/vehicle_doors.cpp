#include "vehicle_doors.hpp"
#include "menu/base/submenu_handler.hpp"
#include "vehicle_mods.hpp"

using namespace vehicleDoorsMenuVars;

namespace vehicleDoorsMenuVars {
	vars_ vars;
}

void vehicleDoorsMenu::init() {
	setName("Doors");
	setParentSubmenu<vehicleModsMenu>();

	addOption(numberOption<int>(SCROLLSELECT, "Open Door")
		.addTranslation()
		.addNumber(vars.openDoor, "%i", 1).addMin(0).addMax(6)
		.addOnClick([] { if (menu::getLocalPlayer().inVehicle) native::setVehicleDoorOpen(menu::getLocalPlayer().vehicle, vars.openDoor, FALSE, FALSE); }));

	addOption(numberOption<int>(SCROLLSELECT, "Close Door")
		.addTranslation()
		.addNumber(vars.closeDoor, "%i", 1).addMin(0).addMax(6)
		.addOnClick([] { if (menu::getLocalPlayer().inVehicle) native::setVehicleDoorShut(menu::getLocalPlayer().vehicle, vars.closeDoor, FALSE); }));

	addOption(numberOption<int>(SCROLLSELECT, "Delete Door")
		.addTranslation()
		.addNumber(vars.deleteDoor, "%i", 1).addMin(0).addMax(6)
		.addOnClick([] { if (menu::getLocalPlayer().inVehicle) native::setVehicleDoorBroken(menu::getLocalPlayer().vehicle, vars.deleteDoor, FALSE); }));

	addOption(buttonOption("Lock Doors")
		.addTranslation()
		.addOnClick([] { if (menu::getLocalPlayer().inVehicle) native::setVehicleDoorsLocked(menu::getLocalPlayer().vehicle, 4); }));

	addOption(buttonOption("Unlock Doors")
		.addTranslation()
		.addOnClick([] { if (menu::getLocalPlayer().inVehicle) native::setVehicleDoorsLocked(menu::getLocalPlayer().vehicle, 0); }));
}

/*Called while in submenu*/
void vehicleDoorsMenu::update() {}

/*Called once on submenu open*/
void vehicleDoorsMenu::updateOnce() {}

/*Called always*/
void vehicleDoorsMenu::featureUpdate() {}

vehicleDoorsMenu* _instance;
vehicleDoorsMenu* vehicleDoorsMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new vehicleDoorsMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

vehicleDoorsMenu::~vehicleDoorsMenu() { delete _instance; }