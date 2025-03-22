#include "gravity.hpp"
#include "menu/base/submenu_handler.hpp"
#include "weapon_mods.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "menu/base/util/camera.hpp"
#include "rage/entity/entity.hpp"
#include "menu/base/util/raycast.hpp"

using namespace gravityMenuVars;

namespace gravityMenuVars {
	vars_ vars;
	
	void gravityGun() {
		static int timer = 0;
		auto position = native::getOffsetFromEntityInWorldCoords(menu::getLocalPlayer().ped, { 0.4f, 0.0f, 0.8f });

		if (native::isPlayerFreeAiming(menu::getLocalPlayer().id)) {
			if (native::doesEntityExist(vars.entity)) {
				native::setInputExclusive(0, INPUT_FRONTEND_UP);
				native::setInputExclusive(0, INPUT_FRONTEND_DOWN);

				if (native::isControlPressed(0, INPUT_FRONTEND_UP)) {
					vars.distance++;
				}
				else if (native::isControlPressed(0, INPUT_FRONTEND_DOWN) && vars.distance > 3.f) {
					vars.distance--;
				}

				if (native::networkIsInSession()) {
					//rage::entity::getEntityTask()->changeNetObjOwner(vars.entity, menu::getLocalPlayer().id);
				}

				//native::freezeEntityPosition(vars.gravityEnt, false);
				native::setEntityVelocity(vars.entity, ((position + (menu::getCamera()->getCameraDirection() * vars.distance)) - native::getEntityCoords(vars.entity)) * vars.force);

				if (native::isPedShooting(menu::getLocalPlayer().ped)) {
					native::setEntityVelocity(vars.entity, ((position + (menu::getCamera()->getCameraDirection() * 5000.f)) - native::getEntityCoords(vars.entity)) * vars.force);
					vars.entity = 0;
					timer = native::getGameTimer() + 1000;
				}
			}
			else {
				vars.entity = 0;

				if (native::getGameTimer() > timer) {
					menu::raycast cast = menu::raycastInfront(9999.f);
					if (cast.didHit()) {
						vars.entity = cast.getEntity();
						if (native::isEntityAPed(vars.entity)) {
							vars.entity = native::isPedInAnyVehicle(vars.entity, false) ? native::getVehiclePedIsIn(vars.entity, false) : vars.entity;
							native::setPedRagdollForceFall(vars.entity);
							native::setPedToRagdoll(vars.entity, -1, -1, 0, 1, 1, 0);
							if (native::isPedAPlayer(vars.entity)) {
								vars.entity = 0;
							}
						}

						timer = 0;
					}
				}
			}
		}
		else {
			vars.entity = 0;
		}
	}

	void attraction(bool ped, bool pickup, bool vehicle, bool object) {
		std::vector<Entity> pool;
		auto position = native::getOffsetFromEntityInWorldCoords(menu::getLocalPlayer().ped, { 0.4f, 0.0f, 0.8f });
		if (native::isPlayerFreeAiming(menu::getLocalPlayer().id)) {
			
			native::setInputExclusive(0, INPUT_FRONTEND_UP);
			native::setInputExclusive(0, INPUT_FRONTEND_DOWN);

			if (native::isControlPressed(0, INPUT_FRONTEND_UP)) {
				vars.distance++;
			} else if (native::isControlPressed(0, INPUT_FRONTEND_DOWN) && vars.distance > 4.f) {
				vars.distance--;
			}
			if (vars.buffer < GetTickCount()) {
				if (ped) {
					menu::pools::getPedPool()->run([&](Ped ped) { if (!native::isPedAPlayer(ped)) { pool.push_back(ped); native::setPedToRagdoll(ped, -1, -1, 0, 1, 1, 0); } });
				}

				//if (pickup) {
				//	menu::pools::getPickupPool()->run([&](Pickup pickup) { pool.push_back(pickup); });
				//}

				if (vehicle) {
					menu::pools::getVehiclePool()->run([&](Vehicle vehicle) { pool.push_back(vehicle); });
				}

				if (object) {
					menu::pools::getObjectPool()->run([&](Object object) { pool.push_back(object); });
				}
			}
			bool shooting = native::isPedShooting(menu::getLocalPlayer().ped);
			for (std::size_t i = 0; pool.size() && i < pool.size(); i++) {
				if (native::isEntityOnScreen(pool[i])) {
					if (native::networkIsInSession()) {
						//native::networkRequestControlOfEntity(pool[i]);
						Player owner = rage::entity::getEntityTask()->getNetObjOwner(pool[i]);
						if (owner != menu::getLocalPlayer().id && owner != -1) {
							//native::networkRequestControlOfEntity(pool[i]);
							rage::entity::getEntityTask()->changeNetObjOwner(pool[i], menu::getLocalPlayer().id);
						}
					}

					native::setEntityVelocity(pool[i], ((position + (menu::getCamera()->getCameraDirection() * (shooting ? 9999.f : vars.distance))) - native::getEntityCoords(pool[i])) * (shooting ? 9999.f : vars.force));
				}
			}

			if (shooting && !pool.empty()) {
				vars.buffer = GetTickCount() + 3000;
			}
			pool.clear();
		} else {
			vars.distance = 10.f;
			vars.buffer = 0;
		}
	}
}

void gravityMenu::init() {
	setName("Gravity Gun");
	setParentSubmenu<weaponModsMenu>();

	addOption(toggleOption("Enable Gravity Gun")
		.addTranslation()
		.addToggle(vars.gravityGun));

	addOption(toggleOption("Attraction")
		.addTranslation()
		.addToggle(vars.attraction)
		.addRequirement([] { return vars.gravityGun; }));

	addOption(breakOption("Config")
		.addTranslation()
		.addRequirement([] { return vars.gravityGun; }));

	addOption(numberOption<float>(SCROLL, "Force")
		.addTranslation()
		.addNumber(vars.force, "%.01f", .1f).addMin(1.f).addMax(20.f)
		.addRequirement([] { return vars.gravityGun; }));

	addOption(toggleOption("Peds")
		.addTranslation()
		.addToggle(vars.peds)
		.addRequirement([] { return vars.gravityGun && vars.attraction; }));

	//addOption(toggleOption("Pickups")
	//	.addTranslation()
	//	.addToggle(vars.pickups)
	//	.addRequirement([] { return vars.gravityGun && vars.attraction; }));

	addOption(toggleOption("Vehicles")
		.addTranslation()
		.addToggle(vars.vehicles)
		.addRequirement([] { return vars.gravityGun && vars.attraction; }));
	
	addOption(toggleOption("Objects")
		.addTranslation()
		.addToggle(vars.objects)
		.addRequirement([] { return vars.gravityGun && vars.attraction; }));
}

/*Called while in submenu*/
void gravityMenu::update() {}

/*Called once on submenu open*/
void gravityMenu::updateOnce() {}

/*Called always*/
void gravityMenu::featureUpdate() {
	if (vars.gravityGun) {
		if (vars.attraction) {
			attraction(vars.peds, vars.pickups, vars.vehicles, vars.objects);
		} else {
			gravityGun();
		}
	}
}

gravityMenu* _instance;
gravityMenu* gravityMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new gravityMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

gravityMenu::~gravityMenu() { delete _instance; }