#include "selected_recent_player.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../recent_players.hpp"
#include "recent_permanent.hpp"
#include "utils/va.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "menu/base/util/presence.hpp"
#include "../network_message.hpp"
#include "../join_player.hpp"
#include "rage/session/session.hpp"
#include "../overseer.hpp"

using namespace networkSelectedRecentPlayerMenuVars;

namespace networkSelectedRecentPlayerMenuVars {
	vars_ vars;

	void makePermanent() {
		nlohmann::json j;

		std::ifstream file(utils::getConfig()->getRecentPlayersPath());
		if (file.good()) {
			file >> j;
			file.close();

			std::ofstream output(utils::getConfig()->getRecentPlayersPath());
			if (output.good()) {
				auto p = networkRecentPlayersMenuVars::vars.selectedList->at(vars.selectedPlayer).second;
				j["perm"][std::to_string(p.rid)]["name"] = p.name;
				j["perm"][std::to_string(p.rid)]["ip"] = p.ip;
				j["perm"][std::to_string(p.rid)]["first encountered"] = p.firstEncountered;
				j["perm"][std::to_string(p.rid)]["last encountered"] = p.lastEncountered;
				j["perm"][std::to_string(p.rid)]["internal"] = p.internalEncountered;
				output << j.dump(4);
				output.close();
			}
		}
	}
}

void networkSelectedRecentPlayerMenu::init() {
	setName("Recent Player");
	setParentSubmenu<networkRecentPlayersMenu>();

	addOption(buttonOption("Add to Permanent List")
		.addTranslation()
		.addRequirement([] { return vars.type == 1; })
		.addOnClick(makePermanent));

	addOption(buttonOption("Add to Overseer")
		.addTranslation()
		.addOnClick([] {
			networkOverseerMenuVars::save(networkRecentPlayersMenuVars::vars.selectedList->at(vars.selectedPlayer).second.name, networkRecentPlayersMenuVars::vars.selectedList->at(vars.selectedPlayer).second.rid, "Manually added");
		}));

	addOption(buttonOption("Join Player")
		.addTranslation()
		.addOnClick([] { menu::getPresence()->joinThem(networkRecentPlayersMenuVars::vars.selectedList->at(vars.selectedPlayer).second.rid); }));

	addOption(breakOption("Socialclub"));

	addOption(buttonOption("Invite to Session")
		.addTranslation()
		.addOnClick([] {
			auto netGamePlayer = rage::engine::getNetGamePlayerFromIndex(menu::getLocalPlayer().id);
			if (netGamePlayer) {
				auto playerInfo = netGamePlayer->playerInfo;
				if (playerInfo) {
					menu::getPresence()->invitePlayer(networkRecentPlayersMenuVars::vars.selectedList->at(vars.selectedPlayer).second.rid, playerInfo->netInfo.rockstarId2);
				}
			}
		}));

	addOption(buttonOption("Invite Me")
		.addTranslation()
		.addOnClick([] {
			auto netGamePlayer = rage::engine::getNetGamePlayerFromIndex(menu::getLocalPlayer().id);
			if (netGamePlayer) {
				auto playerInfo = netGamePlayer->playerInfo;
				if (playerInfo) {
					menu::getPresence()->invitePlayer(playerInfo->netInfo.rockstarId2, networkRecentPlayersMenuVars::vars.selectedList->at(vars.selectedPlayer).second.rid);
				}
			}
		}));


#if defined(DEV) || defined(TESTER)
	addOption(breakOption("Dev"));

	addOption(buttonOption("Kick Player")
		.addTranslation()
		.addOnClick([] { menu::getPresence()->kickPlayer(networkRecentPlayersMenuVars::vars.selectedList->at(vars.selectedPlayer).second.rid); }));

	addOption(buttonOption("Force Join")
		.addOnClick([] { menu::getPresence()->joinMe(networkRecentPlayersMenuVars::vars.selectedList->at(vars.selectedPlayer).second.rid); }));
#endif // DEV

	//addOption(buttonOption("Join Me")
	//	.addOnClick([] { menu::getPresence()->joinMe(networkRecentPlayersMenuVars::vars.selectedList->at(vars.selectedPlayer).second.rid); }));

	/*addOption(submenuOption("Message")
		.addTranslation()
		.addOnClick([] {
			networkMessageMenuVars::vars.submenuIdentifier = 1;
			networkMessageMenuVars::vars.rockstarID = networkRecentPlayersMenuVars::vars.selectedList->at(vars.selectedPlayer).second.rid;
		})
		.addSubmenu<networkMessageMenu>());*/
}

/*Called while in submenu*/
void networkSelectedRecentPlayerMenu::update() {
	auto player = networkRecentPlayersMenuVars::vars.selectedList->at(vars.selectedPlayer);
	menu::getPanelManager()->panelRecentPlayer(player.second.name, player.second.firstEncountered, player.second.lastEncountered, player.second.ip, player.first);

	if (vars.type == 1) {
		setParentSubmenu<networkRecentPlayersMenu>();
	} else {
		setParentSubmenu<networkRecentPlayerPermanentMenu>();
	}
}

/*Called once on submenu open*/
void networkSelectedRecentPlayerMenu::updateOnce() {}

/*Called always*/
void networkSelectedRecentPlayerMenu::featureUpdate() {}

networkSelectedRecentPlayerMenu* _instance;
networkSelectedRecentPlayerMenu* networkSelectedRecentPlayerMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkSelectedRecentPlayerMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkSelectedRecentPlayerMenu::~networkSelectedRecentPlayerMenu() { delete _instance; }