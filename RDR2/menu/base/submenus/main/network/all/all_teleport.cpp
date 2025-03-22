#include "all_teleport.hpp"
#include "menu/base/submenu_handler.hpp"
#include "all_players.hpp"
#include "../players.hpp"
#include "menu/base/util/control_manager.hpp"

using namespace networkAllTeleportMenuVars;

namespace networkAllTeleportMenuVars {
	vars_ vars;
	
	void teleport(int type) {
		menu::getPlayerManager()->foreach([=](menu::playerVars& lobby) { if (lobby.id != menu::getLocalPlayer().id) {
			if (native::isPedOnMount(lobby.ped) || native::isPedInAnyVehicle(lobby.ped, 1)) {
				Entity entity = 0;
				if (native::isPedOnMount(lobby.ped)) {
					entity = native::getMount(lobby.ped);
				} else {
					entity = native::getVehiclePedIsIn(lobby.ped, 0);
				}

				if (type == 0) {
					menu::getControlManager()->requestControl(entity, [](Entity vehicle) {
						auto coords = native::getOffsetFromEntityInWorldCoords(menu::getLocalPlayer().ped, { .0f, 3.f, .0f });
						native::setEntityCoords(vehicle, coords);
					});
				} else if (type == 1) {
					networkPlayersMenuVars::teleportPlayerToWaypoint(lobby);
				} else if (type == 2) {
					menu::getControlManager()->requestControl(entity, [=](Entity vehicle) {
						auto coords = native::getOffsetFromEntityInWorldCoords(lobby.ped, { .0f, .0f, 5000.f });
						native::setEntityCoords(vehicle, coords);
					});
				}
			}
		}});
	}
	
	void teleportAllToPlayer(int index) {
		menu::getPlayerManager()->foreach([=](menu::playerVars& lobby) { if (lobby.id != menu::getLocalPlayer().id) {
			menu::getControlManager()->requestControl(lobby.entity, [=](Entity entity) {
				auto coords = native::getEntityCoords(native::getPlayerPed(index));
				native::setEntityCoords(entity, coords);
			});
		}});
	}
}

void networkAllTeleportMenu::init() {
	setName("Teleport");
	setParentSubmenu<networkAllPlayersMenu>();

	addOption(buttonOption("Teleport to Me")
		.addTranslation()
		.addOnClick([] { teleport(0); }));

	addOption(buttonOption("Teleport to Waypoint")
		.addTranslation()
		.addOnClick([] { teleport(1); }));

	addOption(buttonOption("Teleport up Above")
		.addTranslation()
		.addOnClick([] { teleport(2); }));

	addOption(scrollOption<int>(SCROLLSELECT, "Teleport to Player")
		.addTranslation()
		.addScroll(vars.player, 0, NUMOF(vars.playerList), vars.playerList)
		.addOnUpdate([](scrollOption<int>* option) { option->addScroll(vars.player, 0, vars.listSize, vars.playerList); })
		.addOnClick([] { teleportAllToPlayer(vars.playerList[vars.player].result); }));
}

/*Called while in submenu*/
void networkAllTeleportMenu::update() {
	int size = 0;
	menu::getPlayerManager()->foreach([&](menu::playerVars& var) {
		vars.playerList[size].name = var.name;
		vars.playerList[size].result = var.id;
		size++;
	});

	vars.listSize = size;
}

/*Called once on submenu open*/
void networkAllTeleportMenu::updateOnce() {}

/*Called always*/
void networkAllTeleportMenu::featureUpdate() {}

networkAllTeleportMenu* _instance;
networkAllTeleportMenu* networkAllTeleportMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkAllTeleportMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkAllTeleportMenu::~networkAllTeleportMenu() { delete _instance; }