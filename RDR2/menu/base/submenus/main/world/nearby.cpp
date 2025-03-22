#include "nearby.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../world.hpp"
#include "ped.hpp"
#include "objects.hpp"
#include "vehicles.hpp"
#include "pickups.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "menu/base/util/pools/pool_manager.hpp"

using namespace nearbyMenuVars;

namespace nearbyMenuVars {
	vars_ vars;
}

void nearbyMenu::init() {
	setName("Nearby entities");
	setParentSubmenu<worldMenu>();

	addOption(submenuOption("Ped Manager")
		.addTranslation()
		.addSubmenu<pedMenu>()
		.addOnHover([] { pedMenuVars::esp(); menu::getPanelManager()->panelPool("Ped", menu::pools::getPedPool()->getPoolPointer(), 1); }));

	addOption(submenuOption("Vehicle Manager")
		.addTranslation()
		.addSubmenu<vehiclesMenu>()
		.addOnHover([] { vehiclesMenuVars::esp(); menu::getPanelManager()->panelPool("Vehicle", menu::pools::getVehiclePool()->getPoolPointer(), 2); }));

	addOption(submenuOption("Object Manager")
		.addTranslation()
		.addSubmenu<objectMenu>()
		.addOnHover([] { objectMenuVars::esp(); menu::getPanelManager()->panelPool("Object", menu::pools::getObjectPool()->getPoolPointer(), 3); }));

	//addOption(submenuOption("Pickup Manager")
	//	.addTranslation()
	//	.addSubmenu<pickupsMenu>()
	//	.addOnHover([] { pickupsMenuVars::esp(); menu::getPanelManager()->panelPool("Pickup", menu::pools::getPickupPool()->getPoolPointer(), 4); }));
}

/*Called while in submenu*/
void nearbyMenu::update() {}

/*Called once on submenu open*/
void nearbyMenu::updateOnce() {}

/*Called always*/
void nearbyMenu::featureUpdate() {}

nearbyMenu* _instance;
nearbyMenu* nearbyMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new nearbyMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

nearbyMenu::~nearbyMenu() { delete _instance; }