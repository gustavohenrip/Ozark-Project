#include "spawn.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../main.hpp"
#include "spawn/spawn_ped.hpp"
#include "spawn/spawn_vehicle.hpp"
#include "spawn/spawn_plants.hpp"

using namespace spawnMenuVars;

namespace spawnMenuVars {
	vars_ vars;
}

void spawnMenu::init() {
	setName("Spawn");
	setParentSubmenu<mainMenu>();

	addOption(submenuOption("Spawn Ped")
		.addTranslation()
		.addSubmenu<spawnPedMenu>());

	addOption(submenuOption("Spawn Vehicle")
		.addTranslation()
		.addSubmenu<spawnVehicleMenu>());

	addOption(submenuOption("Spawn Plants")
		.addTranslation()
		.addSubmenu<spawnPlantsMenu>());

	// Spawn Object
}

/*Called while in submenu*/
void spawnMenu::update() {}

/*Called once on submenu open*/
void spawnMenu::updateOnce() {}

/*Called always*/
void spawnMenu::featureUpdate() {}

spawnMenu* _instance;
spawnMenu* spawnMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new spawnMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

spawnMenu::~spawnMenu() { delete _instance; }