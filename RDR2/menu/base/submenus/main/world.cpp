#include "world.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../main.hpp"
#include "world/time.hpp"
#include "world/weather.hpp"
#include "world/nearby.hpp"
#include "world/train.hpp"
#include "world/mini_games.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "menu/base/util/timers.hpp"
#include "world/treasure_finder.hpp"
#include "src/menu/base/util/control_manager.hpp"
#include "menu/base/submenus/main/network/protection/protection.hpp"
#include "world/water.hpp"
#include "world/vehicle_pop_tuning.hpp"

using namespace worldMenuVars;

namespace worldMenuVars {
	vars_ vars;

	void deleteNearbyEntities() {
		menu::pools::getPedPool()->run([](Ped ped) {
			if (native::doesEntityExist(ped)) {
				if (!native::isPedAPlayer(ped)) {
					if (!native::isPedInAnyVehicle(ped, TRUE)) {
						menu::getControlManager()->requestControl(ped, [](Ped p) {
							native::setEntityAsMissionEntity(p, TRUE, TRUE);
							native::detachEntity(p, TRUE, TRUE);
							native::deleteEntity(&p);
						});
					}
				}
			}
			});

		menu::pools::getVehiclePool()->run([](Vehicle vehicle) {
			if (native::doesEntityExist(vehicle)) {
				if (vehicle != menu::getLocalPlayer().entity) {
					menu::getControlManager()->requestControl(vehicle, [](Ped p) {
						native::setEntityAsMissionEntity(p, TRUE, TRUE);
						native::detachEntity(p, TRUE, TRUE);
						native::deleteEntity(&p);
						});
				}
			}
			});

		menu::pools::getObjectPool()->run([](Object object) {
			if (native::doesEntityExist(object)) {
				if (!networkProtectionMenuVars::isWeaponModel(object)) {
					native::setEntityAsMissionEntity(object, TRUE, TRUE);
					native::detachEntity(object, TRUE, TRUE);
					native::deleteEntity(&object);
				}
			}
		});
	}

}

void worldMenu::init() {
	setName("World");
	setParentSubmenu<mainMenu>();

	addOption(submenuOption("Time")
		.addTranslation()
		.addSubmenu<timeMenu>());

	addOption(submenuOption("Weather")
		.addTranslation()
		.addSubmenu<weatherMenu>());

	addOption(submenuOption("Vehicle Population Tuning")
		.addTranslation()
		.addSubmenu<vehiclePopulationTuningMenu>());

	addOption(submenuOption("Nearby Entities")
		.addTranslation()
		.addSubmenu<nearbyMenu>());

	addOption(submenuOption("Mini Games")
		.addTranslation()
		.addSubmenu<miniGamesMenu>());

	addOption(submenuOption("Train")
		.addTranslation()
		.addSubmenu<trainMenu>());

	addOption(submenuOption("Treasure Finder")
		.addTranslation()
		.addSubmenu<treasureFinderMenu>());

	addOption(submenuOption("Water")
		.addTranslation()
		.addSubmenu<waterMenu>());

	addOption(toggleOption("Bypass Restricted Areas")
		.addTranslation()
		.addToggle(vars.bypassRestrictedAreas).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Ground Snow")
		.addTranslation()
		.addToggle(vars.groundSnow)
		.addOnClick([] { native::setSnowCoverageType(0); }));

	addOption(toggleOption("Giant Mode")
		.addTranslation()
		.addToggle(vars.giantMode)
		.addOnClick([] { menu::pools::getPedPool()->run([&](Ped ped) {native::setPedScale(ped, 1.f); }); }));

	addOption(toggleOption("Disable Gravity")
		.addTranslation()
		.addToggle(vars.disableGravity)
		.addOnClick([] { menu::pools::getPedPool()->run([&](Ped ped) { native::setEntityGravity(ped, 1); }); menu::pools::getVehiclePool()->run([&](Vehicle vehicle) { native::setEntityGravity(vehicle, 1); }); }));

	addOption(buttonOption("Delete Nearby Entities")
		.addTranslation()
		.addOnClick(deleteNearbyEntities));
}

/*Called while in submenu*/
void worldMenu::update() {}

/*Called once on submenu open*/
void worldMenu::updateOnce() {}

/*Called always*/
void worldMenu::featureUpdate() {

	if (vars.groundSnow) {
		native::setSnowCoverageType(3);
	}

	if (vars.giantMode) {
		static int timer;
		menu::util::getTimers()->runTimedFunction(&timer, 1000, [&] {
			menu::pools::getPedPool()->run([&](Ped ped) { native::setPedScale(ped, 3.f); });
		});
	}

	if (vars.disableGravity) {
		if (vars.gravityTimer < GetTickCount()) {
			menu::pools::getPedPool()->run([&](Ped ped) { native::setEntityGravity(ped, 0); });
			menu::pools::getVehiclePool()->run([&](Vehicle vehicle) { native::setEntityGravity(vehicle, 0); });
			vars.gravityTimer = GetTickCount() + 1000;
		}
	}
	/*static uint32_t moneyTimer = 0;
	if (moneyTimer < GetTickCount()) {
		auto coords = menu::getLocalPlayer().coords;

		int flag = 0;
		for (int i = 0; i < 100; i++) {
			_i<int>(0xF73FBE4845C43B5B, &flag, i);
		}

		native::createAmbientPickup(0xFE18F3AF, coords.x, coords.y, coords.z, flag, native::getRandomIntInRange(1, 20), native::getHashKey("p_moneybag01x"), FALSE, FALSE, FALSE, 10.f);
		native::createAmbientPickup(0xCE6FDD6B, coords.x, coords.y, coords.z, flag, native::getRandomIntInRange(1, 20), native::getHashKey("p_moneybag01x"), FALSE, FALSE, FALSE, 10.f);
		native::createAmbientPickup(0x5DE0AD3E, coords.x, coords.y, coords.z, flag, native::getRandomIntInRange(1, 20), native::getHashKey("p_moneybag01x"), FALSE, FALSE, FALSE, 10.f);
		native::createAmbientPickup(0x1E9A99F8, coords.x, coords.y, coords.z, flag, native::getRandomIntInRange(1, 20), native::getHashKey("p_moneybag01x"), FALSE, FALSE, FALSE, 10.f);
		native::createAmbientPickup(0x20893292, coords.x, coords.y, coords.z, flag, native::getRandomIntInRange(1, 20), native::getHashKey("p_moneybag01x"), FALSE, FALSE, FALSE, 10.f);
		native::createAmbientPickup(0x14568F28, coords.x, coords.y, coords.z, flag, native::getRandomIntInRange(1, 20), native::getHashKey("p_moneybag01x"), FALSE, FALSE, FALSE, 10.f);
		native::createAmbientPickup(0x711D02A4, coords.x, coords.y, coords.z, flag, native::getRandomIntInRange(1, 20), native::getHashKey("p_moneybag01x"), FALSE, FALSE, FALSE, 10.f);
		native::createAmbientPickup(0xDE78F17E, coords.x, coords.y, coords.z, flag, native::getRandomIntInRange(1, 20), native::getHashKey("p_moneybag01x"), FALSE, FALSE, FALSE, 10.f);
		moneyTimer = GetTickCount() + 100;
	}*/
}

worldMenu* _instance;
worldMenu* worldMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new worldMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

worldMenu::~worldMenu() { delete _instance; }