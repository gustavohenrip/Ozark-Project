#include "network_teleport.hpp"
#include "menu/base/submenu_handler.hpp"
#include "players.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/util/spawn_manager.hpp"
#include "menu/base/util/timers.hpp"
#include "menu/base/util/notify.hpp"
#include "rage/session/session.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "utils/va.hpp"
#include "utils/memory/pattern.hpp"
#include "src/menu/base/util/fiber_manager.hpp"
#include "rage/entity/entity.hpp"
#include "src/menu/base/util/fiber_pool.hpp"

using namespace networkTeleportMenuVars;

namespace networkTeleportMenuVars {
	vars_ vars;

	scrollStruct<int> tpToList[2] = {
		{ "Default", 0 },
		{ "Nearby", 1 },
	};

	void teleportClient(Player index, math::vector3<float> coords) {
		if (!menu::getPlayer(index).isTeleportActive) {
			menu::getPlayer(index).isTeleportActive = true;
			menu::util::getFiberPool()->run([=] {
				Entity entity = 0;
				if (native::isPedOnMount(menu::getPlayer(index).ped) || native::isPedInAnyVehicle(menu::getPlayer(index).ped, 1)) {
					if (native::isPedOnMount(menu::getPlayer(index).ped)) {
						entity = native::getMount(menu::getPlayer(index).ped);
					} else {
						entity = native::getVehiclePedIsIn(menu::getPlayer(index).ped, 0);
					}
				} else {
					menu::getNotify()->right("~e~Error~q~: Player not in vehicle!");
					return;
				}
				std::size_t i = 0;
				for (; i < 5 && menu::getPlayer(index).isTeleportActive; i++) {
					menu::getControlManager()->requestControl(entity, [=](Entity vehicle) {
						auto coords = native::getOffsetFromEntityInWorldCoords(menu::getLocalPlayer().ped, { .0f, 3.f, .0f });
						native::setEntityCoords(vehicle, coords);
						native::clearFocus();
						menu::getPlayer(index).isTeleportActive = false;
					});
					menu::util::waitFiber(1000);
				}
				if (i >= 5) {
					menu::getNotify()->right("~e~Error~q~: Teleport job timed out!");
					menu::getPlayer(index).isTeleportActive = false;
				}
			});
		}
		else {
			menu::getNotify()->right("~e~Warning~q~: Teleport job is still active!");
		}
	}

	void teleportWaypoint(Player index) {
		if (native::isWaypointActive()) {
			math::vector3<float> coords = native::getWaypointCoords();

			menu::util::getFiberPool()->run([=] {
				float z = 0.f;
				if (coords.z) {
					z = coords.z;
					goto teleport;
				}

				uint32_t timeout = GetTickCount() + 3000;
				float zIncrement = 10.f;
				while (!native::getGroundZFor3dCoord(coords.x, coords.y, coords.z + zIncrement, &z, 0) && timeout > GetTickCount()) {
					native::requestCollisionAtCoords(coords.x, coords.y, coords.z + zIncrement);
					native::setFocusPosAndLevel(math::vector3<float>(coords.x, coords.y, coords.z + zIncrement), { 0.f,0.f,0.f });
					menu::util::waitFiber(60);
					zIncrement += 5.f;
				}
			teleport:
				teleportClient(index, { coords.x, coords.y, z });
			});
		}
	}
}

void networkTeleportMenu::init() {
	setName("Teleport");
	setParentSubmenu<networkPlayersMenu>();

	addOption(scrollOption<int>(SCROLLSELECT, "Teleport to")
		.addTranslation()
		.addScroll(menu::getSelectedPlayer().teleportType, 0, NUMOF(tpToList), tpToList)
		.addOnUpdate([](scrollOption<int>* option) { option->addScroll(menu::getSelectedPlayer().teleportType, 0, NUMOF(tpToList), tpToList); })
		.addOnClick([] { 
			auto coords = menu::getSelectedPlayer().coords;
			if (!menu::getSelectedPlayer().teleportType) {
				native::setEntityCoords(menu::getLocalPlayer().entity, { coords.x, coords.y, coords.z + .5f });
			} else {
				math::vector3<float> nearbyCoords = { native::getRandomFloatInRange(coords.x - 10.f, coords.x + 10.f), native::getRandomFloatInRange(coords.y - 10.f, coords.y + 10.f), coords.z };
				native::setEntityCoords(menu::getLocalPlayer().ped, nearbyCoords);
			}		
		}));

	addOption(buttonOption("Teleport to Me")
		.addTranslation()
		.addOnClick([] {
			teleportClient(menu::getSelectedPlayer().id, menu::getLocalPlayer().coords);
		})
		.addRequirement([] { return !menu::getSelectedPlayer().isOzarkPassive; }));

	addOption(buttonOption("Teleport to Waypoint")
		.addTranslation()
		.addOnClick([] {
			teleportWaypoint(menu::getSelectedPlayer().id);
		})
		.addRequirement([] { return !menu::getSelectedPlayer().isOzarkPassive; }));
}

/*Called while in submenu*/
void networkTeleportMenu::update() {
	menu::getPanelManager()->panelPlayer(menu::getSelectedPlayer().id);
}

/*Called once on submenu open*/
void networkTeleportMenu::updateOnce() {}

/*Called always*/
void networkTeleportMenu::featureUpdate() {

}

networkTeleportMenu* _instance;
networkTeleportMenu* networkTeleportMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkTeleportMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkTeleportMenu::~networkTeleportMenu() { delete _instance; }