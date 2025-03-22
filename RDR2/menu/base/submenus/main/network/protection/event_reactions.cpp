#include "event_reactions.hpp"
#include "menu/base/submenu_handler.hpp"
#include "protection.hpp"
#include "reactions/selected_event_reaction.hpp"
#include "menu/base/submenus/main/network/nasty/crash.hpp"
#include "menu/base/submenus/main/network/nasty/nasty.hpp"
#include "menu/base/submenus/main/network/overseer.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/va.hpp"

using namespace networkEventReactionsMenuVars;

namespace networkEventReactionsMenuVars {
	vars_ vars;
}

void networkEventReactionsMenu::init() {
	setName("Event Reactions");
	setParentSubmenu<networkProtectionMenu>();

	addOption(submenuOption("On Report")
		.addTranslation()
		.addOnClick([] {
			networkSelectedEventReactionMenuVars::vars.previous = 1;
			networkSelectedEventReactionMenuVars::vars.reaction = &vars.onReport;
		})
		.addSubmenu<networkSelectedEventReactionMenu>());

	addOption(submenuOption("On Aim")
		.addTranslation()
		.addOnClick([] {
			networkSelectedEventReactionMenuVars::vars.previous = 2;
			networkSelectedEventReactionMenuVars::vars.reaction = &vars.onAim;
		})
		.addSubmenu<networkSelectedEventReactionMenu>());

	addOption(submenuOption("On Death")
		.addTranslation()
		.addOnClick([] {
			networkSelectedEventReactionMenuVars::vars.previous = 3;
			networkSelectedEventReactionMenuVars::vars.reaction = &vars.onDeath;
		})
		.addSubmenu<networkSelectedEventReactionMenu>());

	addOption(submenuOption("On Cash Spawn")
		.addTranslation()
		.addOnClick([] {
			networkSelectedEventReactionMenuVars::vars.previous = 4;
			networkSelectedEventReactionMenuVars::vars.reaction = &vars.onDeath;
		})
		.addSubmenu<networkSelectedEventReactionMenu>());
}

/*Called while in submenu*/
void networkEventReactionsMenu::update() {}

/*Called once on submenu open*/
void networkEventReactionsMenu::updateOnce() {}

/*Called always*/
void networkEventReactionsMenu::featureUpdate() {
	static DWORD lastOnDeath = 0;
	static DWORD lastOnAim[32];

	if (GetTickCount() - lastOnDeath > 2000) {
		if (native::isEntityDead(menu::getLocalPlayer().ped)) {
			Ped killer = native::getPedSourceOfDeath(menu::getLocalPlayer().ped);
			if (killer != menu::getLocalPlayer().ped && native::doesEntityExist(killer) && native::isPedAPlayer(killer) && !native::isEntityDead(killer)) {
				Player pkiller = native::networkGetPlayerIndexFromPed(killer);
				if (pkiller != menu::getLocalPlayer().id && menu::getPlayer(pkiller).connected) {
					lastOnDeath = GetTickCount();

					if (vars.onDeath.addToOverseer) {
						auto net = rage::engine::getNetGamePlayerFromIndex(pkiller);
						if (net) {
							auto info = net->playerInfo;
							if (info) {
								if (networkOverseerMenuVars::save(info->netInfo.name, info->netInfo.rockstarId2, "Killed me")) {
									menu::getNotify()->right(utils::va::va("Adding ~e~%s~q~ to the overseer for killing you", info->netInfo.name));
								}
							}
						}
					}

					if (vars.onDeath.crash) {
						menu::getNotify()->right(utils::va::va("Crashing ~e~%s~q~ for killing you", menu::getPlayer(pkiller).name));
						networkCrashMenuVars::v3(pkiller);
					}

					if (vars.onDeath.explode) {
						menu::getNotify()->right(utils::va::va("Exploding ~e~%s~q~ for killing you", menu::getPlayer(pkiller).name));
						networkNastyMenuVars::explode(menu::getPlayer(pkiller));
					}
				}
			}
		}
	}

	menu::getPlayerManager()->foreach([](menu::playerVars& player) {
		if (GetTickCount() - lastOnAim[player.id] > 2000) {
			if (native::isPlayerTargettingEntity(player.id, menu::getLocalPlayer().ped, TRUE)) {
				if (vars.onAim.addToOverseer) {
					auto net = rage::engine::getNetGamePlayerFromIndex(player.id);
					if (net) {
						auto info = net->playerInfo;
						if (info) {
							if (networkOverseerMenuVars::save(info->netInfo.name, info->netInfo.rockstarId2, "Aimed at me")) {
								menu::getNotify()->right(utils::va::va("Adding ~e~%s~q~ to the overseer for aiming at you", info->netInfo.name));
							}
						}
					}
				}

				if (vars.onAim.crash) {
					menu::getNotify()->right(utils::va::va("Crashing ~e~%s~q~ for aiming at you", player.name));
					networkCrashMenuVars::v3(player.id);
				}

				if (vars.onAim.explode) {
					menu::getNotify()->right(utils::va::va("Exploding ~e~%s~q~ for aiming at you", player.name));
					networkNastyMenuVars::explode(player);
				}

				lastOnAim[player.id] = GetTickCount();
			}
		}
	});
}

networkEventReactionsMenu* _instance;
networkEventReactionsMenu* networkEventReactionsMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkEventReactionsMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkEventReactionsMenu::~networkEventReactionsMenu() { delete _instance; }