#include "players.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network.hpp"
#include "nasty/nasty.hpp"
#include "network_spawn.hpp"
#include "network_teleport.hpp"
#include "nice/nice.hpp"
#include "player_esp.hpp"
#include "network_vehicle.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "rage/entity/entity.hpp"
#include "src/menu/base/util/fiber_manager.hpp"
#include "src/menu/base/util/fiber_pool.hpp"
#include "menu/base/util/control_manager.hpp"
#include "utils/log.hpp"
#include "menu/base/util/spawn_manager.hpp"
#include "overseer.hpp"
#include "network_weather.hpp"
#include "network_message.hpp"
#include "utils/memory/pattern.hpp"
#include "rage/session/session.hpp"
#include "menu/base/util/presence.hpp"

using namespace networkPlayersMenuVars;

namespace networkPlayersMenuVars {
	vars_ vars;

	scrollStruct<int> tpList[5] = {
		{ "To Player", 0 },
		{ "To You", 1 },
		{ "To Waypoint", 2 },
		{ "Nearby", 3 },
		{ "Into Vehicle", 4 }
	};

	void teleportPlayerToWaypoint(menu::playerVars& player) {
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
				menu::getControlManager()->requestControl(player.entity, [=](Entity entity) {
					native::setEntityCoords(entity, { coords.x, coords.y, z });
					native::clearFocus();
				});
			});
		}
	}

	void teleport(menu::playerVars& player, int type) {
		if (type == 0) {
			native::setEntityCoords(menu::getLocalPlayer().entity, { player.coords.x,player.coords.y,player.coords.z + .5f });
		} else if (type == 1) {
			if (native::isPedOnMount(player.ped) || native::isPedInAnyVehicle(player.ped, 1)) {
				Entity entity = 0;
				if (native::isPedOnMount(player.ped)) {
					entity = native::getMount(player.ped);
				}
				else {
					entity = native::getVehiclePedIsIn(player.ped, 0);
				}
				//rage::entity::getEntityTask()->changeNetObjOwner(entity, menu::getLocalPlayer().id);
				menu::getControlManager()->requestControl(entity, [](Entity vehicle) {
				auto coords = native::getOffsetFromEntityInWorldCoords(menu::getLocalPlayer().ped, { .0f, 3.f, .0f });
				native::setEntityCoords(vehicle, coords);
				});
			}
		}else if (type == 2) {
			teleportPlayerToWaypoint(player);
		}
		else if (type == 3) {
			math::vector3<float> coords = { native::getRandomFloatInRange(player.coords.x - 10.f, player.coords.x + 10.f), native::getRandomFloatInRange(player.coords.y - 10.f, player.coords.y + 10.f), player.coords.z };
			native::setEntityCoords(menu::getLocalPlayer().ped, coords);
		} else if (type == 4) {
			if (Vehicle vehicle = native::getVehiclePedIsIn(player.ped, 0)) {
				native::setPedIntoVehicle(menu::getLocalPlayer().ped, vehicle, -2);
			} else {
				menu::getNotify()->bottom("~e~Error~q~: Player not in vehicle!");
			}
		}
	}

	bool addFriend(menu::playerVars& player) {
		int handle[13];
		native::networkHandleFromPlayer(player.id, &handle[0], 13);
		if (native::networkIsHandleValid(&handle[0], 13)) {
			if (native::networkAddFriend(&handle[0], "Hello")) {
				return true;
			}
		}

		return false;
	}

	void kickPlayer(menu::playerVars& player) {
		auto netGamePlayer = rage::engine::getNetGamePlayerFromIndex(player.id);
		if (netGamePlayer) {
			auto info = netGamePlayer->playerInfo;
			menu::getPresence()->kickPlayer(info->netInfo.rockstarId2);
		}
	}

	void gtriPlayer(menu::playerVars& player) {
		auto netGamePlayer = rage::engine::getNetGamePlayerFromIndex(player.id);
		if (netGamePlayer) {
			auto info = netGamePlayer->playerInfo;
			menu::getPresence()->gameTriggerEvent(info->netInfo.rockstarId2);
		}
	}
}

void networkPlayersMenu::init() {
	setName("Players");
	setParentSubmenu<networkMenu>();
	
	addOption(submenuOption("Teleport")
		.addTranslation()
		.addSubmenu<networkTeleportMenu>());

	addOption(submenuOption("ESP")
		.addTranslation()
		.addRequirement([] { return !menu::getSelectedPlayer().isOzarkPassive; })
		.addSubmenu<networkPlayerEspMenu>());

	addOption(submenuOption("Nasty")
		.addTranslation()
		.addRequirement([] { return !menu::getSelectedPlayer().isOzarkPassive; })
		.addSubmenu<networkNastyMenu>());

	addOption(submenuOption("Nice")
		.addTranslation()
		.addRequirement([] { return !menu::getSelectedPlayer().isOzarkPassive; })
		.addSubmenu<networkNiceMenu>());

	addOption(submenuOption("Spawn")
		.addTranslation()
		.addRequirement([] { return !menu::getSelectedPlayer().isOzarkPassive; })
		.addSubmenu<networkSpawnMenu>());

	addOption(submenuOption("Vehicle")
		.addTranslation()
		.addRequirement([] { return !menu::getSelectedPlayer().isOzarkPassive; })
		.addSubmenu<networkVehicleMenu>());

	//addOption(submenuOption("Weather")
	//	.addTranslation()
	//	.addRequirement([] { return !menu::getSelectedPlayer().isOzarkPassive; })
	//	.addSubmenu<networkWeatherMenu>());

	//addOption(submenuOption("Message")
	//	.addTranslation()
	//	.addRequirement([] { return !menu::getSelectedPlayer().isOzarkPassive; })
	//	.addOnClick([] { 
	//		networkMessageMenuVars::vars.submenuIdentifier = 0;
	//		networkMessageMenuVars::vars.rockstarID = 0;

	//		auto net = rage::engine::getNetGamePlayerFromIndex(menu::getSelectedPlayer().id);
	//		if (net) {
	//			auto info = net->playerInfo;
	//			if (info) {
	//				networkMessageMenuVars::vars.rockstarID = info->netInfo.rockstarId2;
	//			}
	//		}
	//	})
	//	.addSubmenu<networkMessageMenu>());

	addOption(buttonOption("Add Friend")
		.addTranslation()
		.addOnClick([] { addFriend(menu::getSelectedPlayer()); }));

	addOption(toggleOption("Spectate")
		.addTranslation()
		.addToggle(menu::getSelectedPlayer().spectate)
		.addOnUpdate([] (toggleOption* option) { option->addToggle(menu::getSelectedPlayer().spectate); })
		.addOnClick([] { native::networkSetInSpectatorMode(menu::getSelectedPlayer().spectate, menu::getSelectedPlayer().ped); }));

	addOption(buttonOption("Add to Overseer")
		.addTranslation()
		.addOnClick([] {
			auto net = rage::engine::getNetGamePlayerFromIndex(menu::getSelectedPlayer().id);
			if (net) {
				auto info = net->playerInfo;
				if (info) {
					networkOverseerMenuVars::save(info->netInfo.name, info->netInfo.rockstarId2, "Manually added");
				}
			}
		}));
#if defined(DEV) || defined(TESTER)
	addOption(breakOption("Dev"));

	addOption(buttonOption("Kick Player")
		.addTranslation()
		.addOnClick([] {
			kickPlayer(menu::getSelectedPlayer());
		}));

#endif
}

/*Called while in submenu*/
void networkPlayersMenu::update() { 
	menu::getPanelManager()->panelPlayer(menu::getSelectedPlayer().id); 
}

/*Called once on submenu open*/
void networkPlayersMenu::updateOnce() {}

/*Called always*/
void networkPlayersMenu::featureUpdate() {}

networkPlayersMenu* _instance;
networkPlayersMenu* networkPlayersMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkPlayersMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkPlayersMenu::~networkPlayersMenu() { delete _instance; }