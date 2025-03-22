#include "network.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../main.hpp"
#include "menu/base/util/timers.hpp"
#include "menu/base/util/player_manager.hpp"
#include "network/players.hpp"
#include "network/friends.hpp"
#include "network/account.hpp"
#include "network/all/all_players.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "network/protection/protection.hpp"
#include "network/overseer.hpp"
#include "network/recent_players.hpp"
#include "menu/base/util/notify.hpp"
#include "network/presence_results.hpp"
#include "rage/engine.hpp"
#include "src/menu/base/util/fiber_manager.hpp"
#include "src/menu/base/util/fiber_pool.hpp"
#include "rage/session/session.hpp"

using namespace networkMenuVars;

namespace networkMenuVars {
	vars_ vars;

	const char* generateName(networkMenu* thisPtr, menu::playerVars player) {
		static char name[100];
		strcpy(name, player.name);
		strcat(name, " ");

		if (player.isScriptHost) strcat(name, thisPtr->getString("~t2~[SH]").c_str());
		if (player.isSessionHost) strcat(name, thisPtr->getString("~t3~[H]").c_str());
		if (player.spectate) strcat(name, thisPtr->getString("~t5~[SPEC]").c_str());
		if (player.modder) strcat(name, thisPtr->getString("~t6~[M]").c_str());
		if(player.isOzarkUserDev)strcat(name, thisPtr->getString("~t7~[Ozark - ~u~D~t7~]").c_str());
		else if (player.isOzarkUser) strcat(name, thisPtr->getString("~t7~[Ozark]").c_str());
		if (player.isFriend) strcat(name, thisPtr->getString("~pa~[F]").c_str());

		return name;
	}
	scrollStruct<int32_t> sessionTypes[] = {
		"Private", 0,
		"Solo", 1,
		"Freeroam", 2,
		"Refresh", 3
	};

	void session(int32_t sessionType) {
		static char sessionID[0x30];
		memset(sessionID, 0, sizeof(sessionID));
		menu::util::getFiberPool()->run([=] {
			while (native::getTransitionStage() != 0x73040199) {
				native::setTransitionStage(0x73040199);
				menu::util::waitFiber(50);
			}

			if (_i<int>(0xfbe782b3165ac8ec, 0x521563CC)) {
				_i<int>(0x2c5bd9a43987aa27, TRUE);
			} else {
				_i<int>(0x2c5bd9a43987aa27, FALSE);
			}

			if (sessionType == 0) {
				native::createPrivateSession(17, 1, 1, sessionID);
				menu::getNotify()->right("Attempting to start a private session");
			} else if (sessionType == 1) {
				native::createSoloSession(17, 0, 109, sessionID);
				menu::getNotify()->right("Attempting to start a solo session");
			} else if (sessionType == 2) {
				native::createPrivateSession(17, 1, 1, sessionID);
				menu::getNotify()->right("Attempting to join a freeroam session | ~e~NOTE~q~: this will take up to 30 seconds");
				menu::util::getFiberPool()->run([=] {
					static uint32_t freemodeTimer = 0;
					freemodeTimer = GetTickCount() + 30000;
					while (true) {
						if (freemodeTimer < GetTickCount()) {
							menu::getNotify()->right("Session Found!");
							rage::network::getNetworkTask()->joinFreemode();
							break;
						}
						menu::util::waitFiber(0);
					}
				});
			} else if (sessionType == 3) {
				rage::network::getNetworkTask()->joinFreemode();
				menu::getNotify()->right("Refreshing current session | ~e~WARNING~q~: This could put you in a new session");
			}
			menu::util::waitFiber(0);
		});
	}
}

void networkMenu::init() {
	setName("Network");
	setParentSubmenu<mainMenu>();

	addString("~t2~[SH]");
	addString("~t3~[H]");
	addString("~t5~[SPEC]");
	addString("~t6~[M]");
	addString("~t7~[Ozark]");
	addString("~pa~[F]");

	addOption(submenuOption("All Players")
		.addTranslation()
		.addSubmenu<networkAllPlayersMenu>());

	addOption(submenuOption("Overseer")
		.addTranslation()
		.addSubmenu<networkOverseerMenu>()
		.addTooltip("Glorified blacklist"));

	addOption(submenuOption("Account")
		.addTranslation()
		.addSubmenu<networkAccountMenu>());

	addOption(submenuOption("Protections")
		.addTranslation()
		.addSubmenu<networkProtectionMenu>());

	addOption(submenuOption("Presence")
		.addTranslation()
		.addSubmenu<networkPresenceSeachResultsMenu>());

	addOption(submenuOption("Friends")
		.addTranslation()
		.addSubmenu<networkFriendsMenu>());

	addOption(submenuOption("Recent Players")
		.addTranslation()
		.addSubmenu<networkRecentPlayersMenu>());

	addOption(scrollOption<int32_t>(SCROLLSELECT, "Session")
		.addScroll(vars.currentSessionType, 0, NUMOF(sessionTypes), sessionTypes)
		.addOnClick([] { session(sessionTypes[vars.currentSessionType].result); })
		.addTooltip("yeet")
		.addOnUpdate([](scrollOption<int32_t>* option) { option->addTooltip(vars.sessionTooltip); }));

	addOption(breakOption("Players")
		.addTranslation());
}

/*Called while in submenu*/
void networkMenu::update() {
	std::vector<menu::playerVars> players;
	static int timer = 0;
	static bool update = true;

	menu::util::getTimers()->runTimedFunction(&timer, 250, [] { update = true; });

	if (update) {
		update = false;
		clearOptionsOffset(9);

		for (int i = 0; i < 32; i++) {
			if (menu::getPlayer(i).exists) {
				players.push_back(menu::getPlayer(i));
			}
		}

		for (std::size_t i = 0; i < players.size(); i++) {
			addOption(submenuOption(generateName(this, players[i]))
				.addSubmenu<networkPlayersMenu>()
				.addOnClick([=] { networkPlayersMenuVars::vars.selectedPlayer = players[i].id; })
				.addOnHover([=] { menu::getPanelManager()->panelPlayer(players[i].id); }));
		}

		if (menu::getBase()->currentOption > options.size()) {
			menu::getBase()->currentOption = 0;
			menu::getBase()->scrollOffset = 0;
		}
	}
	
	if (sessionTypes[vars.currentSessionType].result == 0) {
		vars.sessionTooltip = "Transitions you to your own private session";
	} else if (sessionTypes[vars.currentSessionType].result == 1) {
		vars.sessionTooltip = "Transitions you to your own solo session";
	} else if (sessionTypes[vars.currentSessionType].result == 2) {
		vars.sessionTooltip = "Transitions you to a new Freeroam session";
	} else if (sessionTypes[vars.currentSessionType].result == 3) {
		vars.sessionTooltip = "Refreshes your current session\n~e~NOTE~q~: this occasionally puts you into a new freemode session";
	} 
}

/*Called once on submenu open*/
void networkMenu::updateOnce() {}

/*Called always*/
void networkMenu::featureUpdate() {}

networkMenu* _instance;
networkMenu* networkMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkMenu::~networkMenu() { delete _instance; }