#include "vehicle_mods.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle.hpp"
#include "menu/base/util/spawn_manager.hpp"
#include "vehicle_health.hpp"
#include "vehicle_weapons.hpp"
#include "vehicle_particles.hpp"
#include "vehicle_doors.hpp"
#include "menu/base/util/stacked_text_render.hpp"
#include "utils/va.hpp"
#include "menu/base/submenus/main/misc.hpp"

using namespace vehicleModsMenuVars;

namespace vehicleModsMenuVars {
	vars_ vars;

	void copyVehicle() {
		if (menu::getLocalPlayer().inVehicle) {
			math::vector3<float> dim[2];
			native::getModelDimensions(native::getEntityModel(menu::getLocalPlayer().vehicle), &dim[0], &dim[1]);
			menu::util::getSpawnManager()->spawnVehicle(native::getEntityModel(menu::getLocalPlayer().vehicle), native::getOffsetFromEntityInWorldCoords(menu::getLocalPlayer().vehicle, { dim[1].x * 2.8f, 0.f, 0.f }), native::getEntityHeading(menu::getLocalPlayer().vehicle));
		}
	}
}

void vehicleModsMenu::init() {
	setName("Vehicle");
	setParentSubmenu<vehicleMenu>();

	// Boost

	addOption(submenuOption("Health")
		.addTranslation()
		.addSubmenu<vehicleHealthMenu>());

	addOption(submenuOption("Weapons")
		.addTranslation()
		.addSubmenu<vehicleWeaponsMenu>());

	addOption(submenuOption("Particles")
		.addTranslation()
		.addSubmenu<vehicleParticlesMenu>());

	addOption(submenuOption("Doors")
		.addTranslation()
		.addSubmenu<vehicleDoorsMenu>());

	addOption(toggleOption("Invincibility")
		.addTranslation()
		.addToggle(vars.invincibility).canBeSaved(name.getOriginal())
		.addOnClick([] { native::setEntityInvincible(menu::getLocalPlayer().vehicle, FALSE); }));

	addOption(toggleOption("Invisibility")
		.addTranslation()
		.addToggle(vars.invisible).canBeSaved(name.getOriginal())
		.addOnClick([] { native::setEntityVisible(menu::getLocalPlayer().vehicle, TRUE); }));

	addOption(toggleOption("Seatbelt")
		.addTranslation()
		.addToggle(vars.seatbelt).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Racecar Mode")
		.addTranslation()
		.addToggle(vars.fastVehicle).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Speedometer")
		.addTranslation()
		.addToggle(vars.speedometer)
		.addOnClick([] {
			if (vars.speedometer) {
				menu::util::getStackedTextRenderer()->alloc("StackedRenderer_Speedo");
			} else {
				menu::util::getStackedTextRenderer()->dealloc("StackedRenderer_Speedo");
			}
		}));

	addOption(toggleOption("No Collision")
		.addTranslation()
		.addToggle(vars.noCollision)
		.addOnClick([] { if (!vars.noCollision) native::setEntityCollision(menu::getLocalPlayer().vehicle, TRUE, TRUE); }));

	addOption(numberOption<int>(SCROLL, "Opacity")
		.addTranslation()
		.addNumber(vars.opacity, "%d", 1).addMin(0).addMax(100).setScrollSpeed(20)
		.addOnClick([] { if (menu::getLocalPlayer().inVehicle) native::setEntityAlpha(menu::getLocalPlayer().vehicle, (vars.opacity * 2.55f)); }));

	addOption(toggleOption("Auto Flip")
		.addTranslation()
		.addToggle(vars.autoFlip).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Stick to Ground")
		.addTranslation()
		.addToggle(vars.stickToGround).canBeSaved(name.getOriginal()));

	addOption(buttonOption("Flip Upright")
		.addTranslation()
		.addOnClick([] { if (menu::getLocalPlayer().inVehicle) native::setVehicleOnGroundProperly(menu::getLocalPlayer().vehicle); }));

	addOption(buttonOption("Clone")
		.addTranslation()
		.addOnClick([] { copyVehicle(); }));

	addOption(buttonOption("Delete")
		.addTranslation()
		.addOnClick([] {
			if (menu::getLocalPlayer().inVehicle) {
				native::setEntityAsMissionEntity(menu::getLocalPlayer().vehicle, TRUE, TRUE);
				native::deleteEntity(&menu::getLocalPlayer().vehicle);
			}
		}));
}

/*Called while in submenu*/
void vehicleModsMenu::update() {}

/*Called once on submenu open*/
void vehicleModsMenu::updateOnce() {}

/*Called always*/
void vehicleModsMenu::featureUpdate() {
	if (menu::getLocalPlayer().inVehicle) {
		if (vars.stickToGround) {
			native::setVehicleOnGroundProperly(menu::getLocalPlayer().vehicle);
		}

		if (vars.autoFlip) {
			if (!native::isEntityUpright(menu::getLocalPlayer().vehicle, 120.f)) {
				auto rotation = native::getEntityRotation(menu::getLocalPlayer().vehicle, 0);
				native::setEntityRotation(menu::getLocalPlayer().vehicle, { 0.f, rotation.y, rotation.z }, 0, 1);
			}
		}

		if (vars.noCollision) {
			native::setEntityCollision(menu::getLocalPlayer().vehicle, FALSE, FALSE);
		}

		if (vars.speedometer) {
			menu::util::getStackedTextRenderer()->update("StackedRenderer_Speedo", utils::va::va("Vehicle Speed: %.0f %s", native::getEntitySpeed(menu::getLocalPlayer().vehicle) * miscMenuVars::metric[miscMenuVars::vars.selectedMetric].result, miscMenuVars::metric[miscMenuVars::vars.selectedMetric].name.c_str()));
		}

		if (vars.invincibility) {
			native::setEntityInvincible(menu::getLocalPlayer().vehicle, TRUE);
		}

		if (vars.invisible) {
			native::setEntityVisible(menu::getLocalPlayer().vehicle, FALSE);
		}

		if (vars.seatbelt) {
			native::setPedCanBeKnockedOffVehicle(menu::getLocalPlayer().ped, 0);
		}

		if (vars.fastVehicle) {
			static float speed = 0.f;
			float move = native::getControlNormal(0, INPUT_MOVE_UD);

			if (move && move < .0f) {
				if (speed < 20.f) {
					speed += .2f;
				}
				native::setVehicleForwardSpeed(menu::getLocalPlayer().vehicle, speed);
			} else if (move && move > .0f) {
				if (speed > .0f) {
					speed -= .2f;
					native::setVehicleForwardSpeed(menu::getLocalPlayer().vehicle, speed);
				}
			} else {
				if (speed > .0f) {
					speed -= .2f;
				}
			}
		}
	}
}

vehicleModsMenu* _instance;
vehicleModsMenu* vehicleModsMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new vehicleModsMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

vehicleModsMenu::~vehicleModsMenu() { delete _instance; }