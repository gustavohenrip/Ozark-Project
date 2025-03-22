#include "selected_overseer.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../overseer.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "menu/base/util/presence.hpp"
#include <iomanip>
#include "rage/session/session.hpp"

using namespace networkSelectedOverseerMenuVars;

namespace networkSelectedOverseerMenuVars {
	vars_ vars;

	void save() {
		int flag = networkOverseerMenuVars::vars.players[vars.selectedPlayer].second.flags;

		if (vars.enabled) flag |= OVERSEER_ENABLED;
		else flag &= ~OVERSEER_ENABLED;

		if (vars.notify) flag |= OVERSEER_NOTIFY;
		else flag &= ~OVERSEER_NOTIFY;

		if (vars.crash) flag |= OVERSEER_CRASH;
		else flag &= ~OVERSEER_CRASH;

		if (vars.explodeLoop) flag |= OVERSEER_EXPLODE_LOOP;
		else flag &= ~OVERSEER_EXPLODE_LOOP;

		if (vars.paralyse) flag |= OVERSEER_PARALYSE;
		else flag &= ~OVERSEER_PARALYSE;

		if (vars.blockJoin) flag |= OVERSEER_BLOCK_JOIN;
		else flag &= ~OVERSEER_BLOCK_JOIN;

		if (vars.blacklist) flag |= OVERSEER_BLACKLIST;
		else flag &= ~OVERSEER_BLACKLIST;

		networkOverseerMenuVars::vars.players[vars.selectedPlayer].second.flags = flag;

		nlohmann::json outputJson;
		for (auto& player : networkOverseerMenuVars::vars.players) {
			outputJson[player.first]["name"] = player.second.name;
			outputJson[player.first]["reason added"] = player.second.reason;
			outputJson[player.first]["flags"] = player.second.flags;
		}

		std::ofstream output(utils::getConfig()->getOverseerPath());
		if (output.good()) {
			output << outputJson.dump(4);
			output.close();
		}
	}

	void deletePlayer() {
		networkOverseerMenuVars::vars.players.erase(networkOverseerMenuVars::vars.players.begin() + vars.selectedPlayer);

		nlohmann::json outputJson;
		for (auto& player : networkOverseerMenuVars::vars.players) {
			outputJson[player.first]["name"] = player.second.name;
			outputJson[player.first]["reason added"] = player.second.reason;
			outputJson[player.first]["flags"] = player.second.flags;
		}

		std::ofstream output(utils::getConfig()->getOverseerPath());
		if (output.good()) {
			output << outputJson.dump(4);
			output.close();
		}
	}
}

void networkSelectedOverseerMenu::init() {
	setName("Overseer");
	setParentSubmenu<networkOverseerMenu>();

	addOption(toggleOption("Enabled")
		.addTranslation()
		.addToggle(vars.enabled)
		.addOnClick(save));

	addOption(toggleOption("Notify")
		.addTranslation()
		.addToggle(vars.notify)
		.addOnClick(save));

	addOption(toggleOption("Crash")
		.addTranslation()
		.addToggle(vars.crash)
		.addOnClick(save));

	addOption(toggleOption("Explode Loop")
		.addTranslation()
		.addToggle(vars.explodeLoop)
		.addOnClick(save));

	addOption(toggleOption("Paralyse")
		.addTranslation()
		.addToggle(vars.paralyse)
		.addOnClick(save));

	addOption(toggleOption("Block RID Joining")
		.addTranslation()
		.addToggle(vars.blockJoin)
		.addOnClick(save)
		.addTooltip("Block current overseer player from joining you using your RID"));

	addOption(toggleOption("Blacklist")
		.addTranslation()
		.addToggle(vars.blacklist)
		.addOnClick(save)
		.addTooltip("You will never see this player again with this toggled."));

	//addOption(toggleOption("Auto Join me")
	//	.addTranslation()
	//	.addToggle(vars.autoJoinMe)
	//	.addOnClick(save));
	//addOption(buttonOption("Save Settings")
	//	.addTranslation()
	//	.addOnClick(save));

	addOption(buttonOption("~e~Delete")
		.addTranslation()
		.addOnClick([] {
			deletePlayer();
			menu::getSubmenuHandler()->setSubmenuPrevious(true);
		}));

	addOption(breakOption("Presence"));

	addOption(buttonOption("Join Session")
		.addTranslation()
		.addOnClick([] { menu::getPresence()->joinThem(std::atoi(networkOverseerMenuVars::vars.players[vars.selectedPlayer].first.c_str())); }));

	addOption(breakOption("Socialclub"));

	addOption(buttonOption("Invite to Session")
		.addTranslation()
		.addOnClick([] {
			auto netGamePlayer = rage::engine::getNetGamePlayerFromIndex(menu::getLocalPlayer().id);
			if (netGamePlayer) {
				auto playerInfo = netGamePlayer->playerInfo;
				if (playerInfo) {
					menu::getPresence()->invitePlayer(std::atoi(networkOverseerMenuVars::vars.players[vars.selectedPlayer].first.c_str()), playerInfo->netInfo.rockstarId2);
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
					menu::getPresence()->invitePlayer(playerInfo->netInfo.rockstarId2, std::atoi(networkOverseerMenuVars::vars.players[vars.selectedPlayer].first.c_str()));
				}
			}
		}));


#if defined(DEV) || defined(TESTER)
	addOption(breakOption("Dev"));

	addOption(buttonOption("Kick Player")
		.addTranslation()
		.addOnClick([] { menu::getPresence()->kickPlayer(std::atoi(networkOverseerMenuVars::vars.players[vars.selectedPlayer].first.c_str())); }));

	addOption(buttonOption("Join Me")
		.addOnClick([] { menu::getPresence()->joinMe(std::atoi(networkOverseerMenuVars::vars.players[vars.selectedPlayer].first.c_str())); }));
#endif // DEV

	//addOption(buttonOption("Join Me")
	//	.addOnClick([] { menu::getPresence()->joinMe(networkOverseerMenuVars::vars.players[vars.selectedPlayer].second.rid); }));



}

/*Called while in submenu*/
void networkSelectedOverseerMenu::update() {
	auto p = networkOverseerMenuVars::vars.players[vars.selectedPlayer];
	menu::getPanelManager()->panelOverseer(p.first, p.second.name, p.second.reason, p.second.flags);
}

/*Called once on submenu open*/
void networkSelectedOverseerMenu::updateOnce() {
	int flags = networkOverseerMenuVars::vars.players[vars.selectedPlayer].second.flags;

	vars.crash = flags & OVERSEER_CRASH;
	vars.enabled = flags & OVERSEER_ENABLED;
	vars.explodeLoop = flags & OVERSEER_EXPLODE_LOOP;
	vars.notify = flags & OVERSEER_NOTIFY;
	vars.paralyse = flags & OVERSEER_PARALYSE;
	vars.blockJoin = flags & OVERSEER_BLOCK_JOIN;
	vars.blacklist = flags & OVERSEER_BLACKLIST;
}

/*Called always*/
void networkSelectedOverseerMenu::featureUpdate() {}

networkSelectedOverseerMenu* _instance;
networkSelectedOverseerMenu* networkSelectedOverseerMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkSelectedOverseerMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkSelectedOverseerMenu::~networkSelectedOverseerMenu() { delete _instance; }