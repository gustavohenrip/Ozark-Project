#include "vehicle_health.hpp"
#include "menu/base/submenu_handler.hpp"
#include "vehicle_mods.hpp"
#include "menu/base/util/timers.hpp"

using namespace vehicleHealthMenuVars;

namespace vehicleHealthMenuVars {
	vars_ vars;

	void repair() {
		if (menu::getLocalPlayer().inVehicle) {
			native::setVehicleFixed(menu::getLocalPlayer().vehicle);
		}
	}
}

void vehicleHealthMenu::init() {
	setName("Health");
	setParentSubmenu<vehicleModsMenu>();

	addOption(toggleOption("Auto Repair")
		.addTranslation()
		.addToggle(vars.autoRepair));

	addOption(buttonOption("Repair")
		.addTranslation()
		.addOnClick(repair));

	addOption(buttonOption("Wash")
		.addTranslation()
		.addOnClick([] { if (menu::getLocalPlayer().inVehicle) native::setVehicleDirtLevel(menu::getLocalPlayer().vehicle, 0.f); }));

	addOption(buttonOption("Dirty")
		.addTranslation()
		.addOnClick([] { if (menu::getLocalPlayer().inVehicle) native::setVehicleDirtLevel(menu::getLocalPlayer().vehicle, 1.f); }));
}

/*Called while in submenu*/
void vehicleHealthMenu::update() {}

/*Called once on submenu open*/
void vehicleHealthMenu::updateOnce() {}

/*Called always*/
void vehicleHealthMenu::featureUpdate() {
	if (menu::getLocalPlayer().inVehicle) {
		if (vars.autoRepair) {
			static int timer = 0;
			menu::util::getTimers()->runTimedFunction(&timer, 500, [] {
				if (_i<BOOL>(0x9934E9C42D52D87E, menu::getLocalPlayer().vehicle)
					|| _i<BOOL>(0x695D7C26DE65C423, menu::getLocalPlayer().vehicle)) {
					repair();
				}
			});
		}
	}
}

vehicleHealthMenu* _instance;
vehicleHealthMenu* vehicleHealthMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new vehicleHealthMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

vehicleHealthMenu::~vehicleHealthMenu() { delete _instance; }