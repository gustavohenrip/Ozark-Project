#include "spawn_vehicle.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../spawn.hpp"
#include "spawn_settings.hpp"
#include "menu/base/util/spawn_manager.hpp"
#include "menu/base/renderer.hpp"
#include "utils/va.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/control_manager.hpp"
#include "spawn_manage.hpp"
#include "rage/entity/entity.hpp"

using namespace spawnVehicleMenuVars;

namespace spawnVehicleMenuVars {
	vars_ vars;

	void spawnVehicle(std::string visibleName, Hash hash) {
		menu::getControlManager()->requestModel(hash, [=] (DWORD modelHash) {
			float speed = 0.f;

			if (menu::getLocalPlayer().inVehicle) {
				if (spawnSettingsMenuVars::vars.vehicleDeleteCurrentVehicle) {
					speed = native::getEntitySpeed(menu::getLocalPlayer().vehicle);
					native::setEntityAsMissionEntity(menu::getLocalPlayer().vehicle, TRUE, TRUE);
					native::deleteEntity(&menu::getLocalPlayer().vehicle);
				}
			}
			auto coords = menu::getLocalPlayer().coords + (native::getEntityForwardVector(menu::getLocalPlayer().entity) * 5.f);
			Vehicle vehicle = menu::util::getSpawnManager()->spawnVehicle(modelHash, coords, 0.f);
			if (vehicle) {
				native::decorSetBool(vehicle, "wagon_block_honor", TRUE);

				if (spawnSettingsMenuVars::vars.vehicleSpawnWithBlip) {
					native::setBlipSprite(native::addBlipForEntity(-515518185, vehicle), -1606321000, TRUE);
				}

				if (spawnSettingsMenuVars::vars.vehicleSpawnWithParticle) {
					native::requestNamedPtfxAsset(native::getHashKey("scr_dis_ligtn_trees"));
					native::useParticleFXAsset("scr_dis_ligtn_trees");

					auto copyCoords = coords;
					copyCoords.z -= 5.f;
					native::startNetworkedParticleFXNonLoopedAtCoord("scr_crackpot_rc_lightening_exp", copyCoords, { 0.f, 0.f, 0.f }, 1.8f, FALSE, FALSE, FALSE);
				}

				if (spawnSettingsMenuVars::vars.vehicleSpawnWithInvincibility) {
					native::setEntityInvincible(vehicle, TRUE);
				}

				if (spawnSettingsMenuVars::vars.vehicleSpawnFrozen) {
					native::freezeEntityPosition(vehicle, TRUE);
				}

				if (spawnSettingsMenuVars::vars.vehicleSpawnInsideVehicle) {
					native::setVehicleEngineOn(vehicle, TRUE, TRUE);
					native::setPedIntoVehicle(menu::getLocalPlayer().ped, vehicle, -1);
					native::setVehicleForwardSpeed(vehicle, speed);
				}

				if (spawnSettingsMenuVars::vars.vehicleAddToManager) {
					spawnedVehicleData data;
					data.creationOverlay = spawnSettingsMenuVars::vars.vehicleSpawnWithCreationOverlay;
					data.creationTime = native::getGameTimer();
					data.frozen = spawnSettingsMenuVars::vars.vehicleSpawnFrozen;
					data.handle = vehicle;
					data.hash = modelHash;
					data.invincibility = spawnSettingsMenuVars::vars.vehicleSpawnWithInvincibility;
					data.origin = coords;

					std::string n = visibleName;
					if (n.empty()) {
						std::stringstream stream;
						stream << "0x" << std::hex << modelHash;
						n = stream.str().c_str();
					}

					strcpy(data.name, n.c_str());

					vars.spawnedVehicles.push_back(data);
				}
			}
		});
	}
}

void spawnVehicleMenu::init() {
	setName("Spawn Vehicle");
	setParentSubmenu<spawnMenu>();

	addOption(submenuOption("Spawn Settings")
		.addTranslation()
		.addSubmenu<spawnSettingsMenu>());

	addOption(submenuOption("Manage Spawned Vehicles")
		.addTranslation()
		.addSubmenu<spawnManageMenu>());

	addOption(buttonOption("Custom Input")
		.addTranslation()
		.addKeyboard("Enter hash or name of model", 60, [] (const char* res) {
		if (res) {
			if (res[0] == '0' && (res[1] == 'x' || res[1] == 'X')) {
				// hash
				spawnVehicle(res, (DWORD)_strtoui64(res, 0, 0));
			} else {
				std::string str(res);
				if (std::find_if(str.begin(), str.end(), [] (unsigned char c) { return !std::isdigit(c); }) == str.end()) {
					// numeric
					spawnVehicle(res, (DWORD)atoi(res));
				} else {
					// str
					spawnVehicle(res, native::getHashKey(res));
				}
			}
		}
	}));

	addOption(breakOption("Vehicles")
		.addTranslation());

	addOption(scrollOption<Hash>(SCROLLSELECT, "Trains")
		.addTranslation()
		.addScroll(vars.trains, 0, NUMOF(global::arrays::g_vehicleModelTrain), global::arrays::g_vehicleModelTrain)
		.addOnClick([] { spawnVehicle(global::arrays::g_vehicleModelTrain[vars.trains].name, global::arrays::g_vehicleModelTrain[vars.trains].result); }));

	addOption(scrollOption<Hash>(SCROLLSELECT, "Wagons")
		.addTranslation()
		.addScroll(vars.wagons, 0, NUMOF(global::arrays::g_vehicleModelWagon), global::arrays::g_vehicleModelWagon)
		.addOnClick([] { spawnVehicle(global::arrays::g_vehicleModelWagon[vars.wagons].name, global::arrays::g_vehicleModelWagon[vars.wagons].result); }));

	addOption(scrollOption<Hash>(SCROLLSELECT, "Boats")
		.addTranslation()
		.addScroll(vars.boats, 0, NUMOF(global::arrays::g_vehicleModelBoat), global::arrays::g_vehicleModelBoat)
		.addOnClick([] { spawnVehicle(global::arrays::g_vehicleModelBoat[vars.boats].name, global::arrays::g_vehicleModelBoat[vars.boats].result); }));

	addOption(scrollOption<Hash>(SCROLLSELECT, "Stagecoaches")
		.addTranslation()
		.addScroll(vars.stagecoaches, 0, NUMOF(global::arrays::g_vehicleModelStagecoach), global::arrays::g_vehicleModelStagecoach)
		.addOnClick([] { spawnVehicle(global::arrays::g_vehicleModelStagecoach[vars.stagecoaches].name, global::arrays::g_vehicleModelStagecoach[vars.stagecoaches].result); }));

	addOption(scrollOption<Hash>(SCROLLSELECT, "Carts")
		.addTranslation()
		.addScroll(vars.carts, 0, NUMOF(global::arrays::g_vehicleModelCart), global::arrays::g_vehicleModelCart)
		.addOnClick([] { spawnVehicle(global::arrays::g_vehicleModelCart[vars.carts].name, global::arrays::g_vehicleModelCart[vars.carts].result); }));

	addOption(scrollOption<Hash>(SCROLLSELECT, "Special")
		.addTranslation()
		.addScroll(vars.special, 0, NUMOF(global::arrays::g_vehicleModelSpecial), global::arrays::g_vehicleModelSpecial)
		.addOnClick([] { spawnVehicle(global::arrays::g_vehicleModelSpecial[vars.special].name, global::arrays::g_vehicleModelSpecial[vars.special].result); }));
}

/*Called while in submenu*/
void spawnVehicleMenu::update() {}

/*Called once on submenu open*/
void spawnVehicleMenu::updateOnce() {}

/*Called always*/
void spawnVehicleMenu::featureUpdate() {
	if (vars.spawnedVehicles.size()) {
		for (std::size_t i = 0; i < vars.spawnedVehicles.size(); i++) {
			auto vehicle = vars.spawnedVehicles[i];

			if (!native::doesEntityExist(vehicle.handle)) {
				vars.spawnedVehicles.erase(vars.spawnedVehicles.begin() + i);
				continue;
			}

			if (vehicle.invincibility) {
				native::setEntityInvincible(vehicle.handle, TRUE);
			}

			if (vehicle.frozen) {
				native::freezeEntityPosition(vehicle.handle, TRUE);
			}

			if (vehicle.creationOverlay) {
				float screenX, screenY;
				if (native::getScreenCoordFromWorldCoord(native::getEntityCoords(vehicle.handle), &screenX, &screenY)) {
					if (menu::getLocalPlayer().coords.getMathmaticalDistance(native::getEntityCoords(vehicle.handle)) < 200.f) {
						int ms = (native::getGameTimer() - vehicle.creationTime) / 1000;
						int hour = ms / 3600;
						int min = (ms % 3600) / 60;
						int sec = ms % 60;

						menu::getRenderer()->drawText(utils::va::va("~o~%s~q~\n%s\n%02dh:%02dm:%02ds", vehicle.name, native::isEntityDead(vehicle.handle) ? "Dead" : "Alive", hour, min, sec), { screenX, screenY }, 0.20f, 4, { 255, 255, 255, 255 }, JUSTIFY_CENTER, true);
					}
				}
			}
		}
	}
}

spawnVehicleMenu* _instance;
spawnVehicleMenu* spawnVehicleMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new spawnVehicleMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

spawnVehicleMenu::~spawnVehicleMenu() { delete _instance; }