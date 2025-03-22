#include "spawn_settings.hpp"
#include "menu/base/submenu_handler.hpp"
#include "spawn_ped.hpp"
#include "spawn_vehicle.hpp"

using namespace spawnSettingsMenuVars;

namespace spawnSettingsMenuVars {
	vars_ vars;
	
	void populatePed(spawnSettingsMenu* pthis) {
		pthis->clearOptionsOffset(0);
		pthis->setParentSubmenu<spawnPedMenu>();
		/*
		native::setPedConfigFlag(entity, 580, true);
			native::setPedKeepsTask(entity, true);
		*/
		pthis->addOption(toggleOption(pthis->getString("Spawn Sedated"))
			.addToggle(vars.pedSpawnSedated).canBeSaved(pthis->name.getOriginal())
			.addTooltip("Auto sedate animal"));

		pthis->addOption(toggleOption(pthis->getString("Spawn with Blip"))
			.addToggle(vars.pedSpawnWithBlip).canBeSaved(pthis->name.getOriginal()));

		pthis->addOption(toggleOption(pthis->getString("Spawn Dead"))
			.addToggle(vars.pedSpawnDead).canBeSaved(pthis->name.getOriginal()));

		pthis->addOption(toggleOption(pthis->getString("Spawn with Particle"))
			.addToggle(vars.pedSpawnWithParticle).canBeSaved(pthis->name.getOriginal()));

		pthis->addOption(toggleOption(pthis->getString("Spawn with Invincibility"))
			.addToggle(vars.pedSpawnWithInvincibility).canBeSaved(pthis->name.getOriginal()));

		pthis->addOption(toggleOption(pthis->getString("Spawn with Creation Overlay"))
			.addToggle(vars.pedSpawnWithCreationOverlay).canBeSaved(pthis->name.getOriginal()));

		pthis->addOption(toggleOption(pthis->getString("Spawn Frozen"))
			.addToggle(vars.pedSpawnFrozen).canBeSaved(pthis->name.getOriginal()));

		pthis->addOption(toggleOption(pthis->getString("Add to Spawn Manager"))
			.addToggle(vars.pedAddToManager).canBeSaved(pthis->name.getOriginal()));
	}

	void populateVehicle(spawnSettingsMenu* pthis) {
		pthis->clearOptionsOffset(0);
		pthis->setParentSubmenu<spawnVehicleMenu>();

		pthis->addOption(toggleOption(pthis->getString("Spawn with Blip"))
			.addToggle(vars.vehicleSpawnWithBlip).canBeSaved(pthis->name.getOriginal()));

		pthis->addOption(toggleOption(pthis->getString("Spawn with Particle"))
			.addToggle(vars.vehicleSpawnWithParticle).canBeSaved(pthis->name.getOriginal()));

		pthis->addOption(toggleOption(pthis->getString("Spawn with Invincibility"))
			.addToggle(vars.vehicleSpawnWithInvincibility).canBeSaved(pthis->name.getOriginal()));

		pthis->addOption(toggleOption(pthis->getString("Spawn with Creation Overlay"))
			.addToggle(vars.vehicleSpawnWithCreationOverlay).canBeSaved(pthis->name.getOriginal()));

		pthis->addOption(toggleOption(pthis->getString("Spawn Frozen"))
			.addToggle(vars.vehicleSpawnFrozen).canBeSaved(pthis->name.getOriginal()));

		pthis->addOption(toggleOption(pthis->getString("Spawn Inside"))
			.addToggle(vars.vehicleSpawnInsideVehicle).canBeSaved(pthis->name.getOriginal()));

		pthis->addOption(toggleOption(pthis->getString("Delete Current Vehicle"))
			.addToggle(vars.vehicleDeleteCurrentVehicle).canBeSaved(pthis->name.getOriginal()));

		pthis->addOption(toggleOption(pthis->getString("Add to Spawn Manager"))
			.addToggle(vars.vehicleAddToManager).canBeSaved(pthis->name.getOriginal()));
	}
}

void spawnSettingsMenu::init() {
	setName("Spawn Settings");
	setParentSubmenu<spawnPedMenu>();

	addString("Spawn with Blip");
	addString("Spawn with Particle");
	addString("Spawn with Invincibility");
	addString("Spawn with Creation Overlay");
	addString("Spawn Frozen");
	addString("Spawn Inside");
	addString("Delete Current Vehicle");
	addString("Add to Spawn Manager");

	// init
	populatePed(this);
	populateVehicle(this);
}

/*Called while in submenu*/
void spawnSettingsMenu::update() {}

/*Called once on submenu open*/
void spawnSettingsMenu::updateOnce() {
	if (menu::getSubmenuHandler()->getPreviousSubmenu() == spawnPedMenu::getInstance()) {
		populatePed(this);
	}

	if (menu::getSubmenuHandler()->getPreviousSubmenu() == spawnVehicleMenu::getInstance()) {
		populateVehicle(this);
	}
}

/*Called always*/
void spawnSettingsMenu::featureUpdate() {}

spawnSettingsMenu* _instance;
spawnSettingsMenu* spawnSettingsMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new spawnSettingsMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

spawnSettingsMenu::~spawnSettingsMenu() { delete _instance; }