#include "shooter.hpp"
#include "menu/base/submenu_handler.hpp"
#include "weapon_mods.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/camera.hpp"
#include "menu/base/util/spawn_manager.hpp"

using namespace shooterMenuVars;

namespace shooterMenuVars {
	vars_ vars;

	scrollStruct<int> shooter[2] = {
		{ "Vehicle", 0 },
		{ "Ped", 1 }
	};

	void objShooter(Hash hash, int type) {
		if (native::isPlayerFreeAiming(menu::getLocalPlayer().id)) {
			math::vector3<float> pos = native::getOffsetFromEntityInWorldCoords(menu::getLocalPlayer().ped, { 0.4f, 0.0f, 0.8f });
			math::vector3<float> cam = menu::getCamera()->getCameraDirection();
			
			if (!native::doesEntityExist(vars.shooterObj)) {
				switch (type) {
					case 0: vars.shooterObj = menu::util::getSpawnManager()->spawnVehicle(hash, (pos + (cam * 8.f)), 0.f); break;
					case 1: vars.shooterObj = menu::util::getSpawnManager()->spawnPed(hash, (pos + (cam * 8.f)), 0.f); break;
				}
			}

			if (native::isEntityAPed(vars.shooterObj)) {
				native::setPedToRagdoll(vars.shooterObj, -1, -1, 0, 1, 1, 0);
			}

			native::setEntityVelocity(vars.shooterObj, ((pos + (cam * (native::isPedShooting(menu::getLocalPlayer().ped) ? 1000.0f : 9.0f))) - native::getEntityCoords(vars.shooterObj)) * 4);
			auto rot = native::getGameplayCamRotation(0);
			native::setEntityRotation(vars.shooterObj, rot, 0, 0);
			
			if (native::isPedShooting(menu::getLocalPlayer().ped)) {
				vars.shooterObj = 0;
			}
		}
	}
}

void shooterMenu::init() {
	setName("Entity Shooter");
	setParentSubmenu<weaponModsMenu>();

	addOption(toggleOption("Enable Entity Shooter")
		.addTranslation()
	.addToggle(vars.shooterToggle));

	addOption(scrollOption<int>(SCROLL, "Type")
		.addTranslation()
		.addScroll(vars.shooterType, 0, NUMOF(shooter), shooter)
		.addRequirement([] {return vars.shooterToggle; }));

	addOption(breakOption("Config")
		.addTranslation()
		.addRequirement([] {return vars.shooterToggle; }));

	addOption(scrollOption<Hash>(SCROLL, "Trains")
		.addTranslation()
		.addScroll(vars.trains, 0, NUMOF(global::arrays::g_vehicleModelTrain), global::arrays::g_vehicleModelTrain)
		.addOnClick([] { vars.hash = global::arrays::g_vehicleModelTrain[vars.trains].result; })
		.addRequirement([] {return vars.shooterType == 0 && vars.shooterToggle; }));

	addOption(scrollOption<Hash>(SCROLL, "Wagons")
		.addTranslation()
		.addScroll(vars.wagons, 0, NUMOF(global::arrays::g_vehicleModelWagon), global::arrays::g_vehicleModelWagon)
		.addOnClick([] { vars.hash = global::arrays::g_vehicleModelWagon[vars.wagons].result; })
		.addRequirement([] {return vars.shooterType == 0 && vars.shooterToggle; }));

	addOption(scrollOption<Hash>(SCROLL, "Stagecoach")
		.addTranslation()
		.addScroll(vars.stagecoach, 0, NUMOF(global::arrays::g_vehicleModelStagecoach), global::arrays::g_vehicleModelStagecoach)
		.addOnClick([] { vars.hash = global::arrays::g_vehicleModelStagecoach[vars.stagecoach].result; })
		.addRequirement([] {return vars.shooterType == 0 && vars.shooterToggle; }));

	addOption(scrollOption<Hash>(SCROLL, "Boats")
		.addTranslation()
		.addScroll(vars.boats, 0, NUMOF(global::arrays::g_vehicleModelBoat), global::arrays::g_vehicleModelBoat)
		.addOnClick([] { vars.hash = global::arrays::g_vehicleModelBoat[vars.boats].result; })
		.addRequirement([] {return vars.shooterType == 0 && vars.shooterToggle; }));

	addOption(scrollOption<Hash>(SCROLL, "Alligators")
		.addTranslation()
		.addScroll(vars.alligators, 0, NUMOF(global::arrays::g_pedModelAlligators), global::arrays::g_pedModelAlligators)
		.addOnClick([] { vars.hash = global::arrays::g_pedModelAlligators[vars.alligators].result; })
		.addRequirement([] {return vars.shooterType == 1 && vars.shooterToggle; }));

	addOption(scrollOption<Hash>(SCROLL, "Dogs")
		.addTranslation()
		.addScroll(vars.dogs, 0, NUMOF(global::arrays::g_pedModelDogs), global::arrays::g_pedModelDogs)
		.addOnClick([] { vars.hash = global::arrays::g_pedModelDogs[vars.dogs].result; })
		.addRequirement([] {return vars.shooterType == 1 && vars.shooterToggle; }));

	addOption(scrollOption<Hash>(SCROLL, "Sparrows")
		.addTranslation()
		.addScroll(vars.sparrows, 0, NUMOF(global::arrays::g_pedModelSparrows), global::arrays::g_pedModelSparrows)
		.addOnClick([] { vars.hash = global::arrays::g_pedModelSparrows[vars.sparrows].result; })
		.addRequirement([] {return vars.shooterType == 1 && vars.shooterToggle; }));

	addOption(scrollOption<Hash>(SCROLL, "Snakes")
		.addTranslation()
		.addScroll(vars.snakes, 0, NUMOF(global::arrays::g_pedModelSnake), global::arrays::g_pedModelSnake)
		.addOnClick([] { vars.hash = global::arrays::g_pedModelSnake[vars.snakes].result; })
		.addRequirement([] {return vars.shooterType == 1 && vars.shooterToggle; }));

	addOption(scrollOption<Hash>(SCROLL, "Males")
		.addTranslation()
		.addScroll(vars.males, 0, NUMOF(global::arrays::g_pedModelMales), global::arrays::g_pedModelMales)
		.addOnClick([] { vars.hash = global::arrays::g_pedModelMales[vars.males].result; })
		.addRequirement([] {return vars.shooterType == 1 && vars.shooterToggle; }));

	addOption(scrollOption<Hash>(SCROLL, "Females")
		.addTranslation()
		.addScroll(vars.females, 0, NUMOF(global::arrays::g_pedModelFemales), global::arrays::g_pedModelFemales)
		.addOnClick([] { vars.hash = global::arrays::g_pedModelFemales[vars.females].result; })
		.addRequirement([] {return vars.shooterType == 1 && vars.shooterToggle; }));

	addOption(scrollOption<Hash>(SCROLL, "Females")
		.addTranslation()
		.addScroll(vars.horses, 0, NUMOF(global::arrays::g_pedModelHorses), global::arrays::g_pedModelHorses)
		.addOnClick([] { vars.hash = global::arrays::g_pedModelHorses[vars.horses].result; })
		.addRequirement([] {return vars.shooterType == 1 && vars.shooterToggle; }));
}

/*Called while in submenu*/
void shooterMenu::update() {}

/*Called once on submenu open*/
void shooterMenu::updateOnce() {}

/*Called always*/
void shooterMenu::featureUpdate() {
	if (vars.shooterToggle) {
		objShooter(vars.hash, vars.shooterType);
	}
}

shooterMenu* _instance;
shooterMenu* shooterMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new shooterMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

shooterMenu::~shooterMenu() { delete _instance; }