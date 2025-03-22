#include "vehicle.hpp"
#include "menu/base/submenu_handler.hpp"
#include "menu/base/submenus/main.hpp"
#include "vehicle/mount.hpp"
#include "vehicle/vehicle_mods.hpp"

using namespace vehicleMenuVars;

namespace vehicleMenuVars {
	vars_ vars;
}

void vehicleMenu::init() {
	setName("Vehicle");
	setParentSubmenu<mainMenu>();

	addOption(submenuOption("Mount")
		.addTranslation()
		.addSubmenu<mountMenu>());

	addOption(submenuOption("Vehicle")
		.addTranslation()
		.addSubmenu<vehicleModsMenu>());
}

/*Called while in submenu*/
void vehicleMenu::update() {}

/*Called once on submenu open*/
void vehicleMenu::updateOnce() {}

/*Called always*/
void vehicleMenu::featureUpdate() {}

vehicleMenu* _instance;
vehicleMenu* vehicleMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new vehicleMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

vehicleMenu::~vehicleMenu() { delete _instance; }