#include "recent_permanent.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../recent_players.hpp"
#include "menu/base/util/timers.hpp"
#include "utils/va.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "selected_recent_player.hpp"

using namespace networkRecentPlayerPermanentMenuVars;

namespace networkRecentPlayerPermanentMenuVars {
	vars_ vars;

	void loadIntoVector() {
		std::ifstream file(utils::getConfig()->getRecentPlayersPath());
		if (file.good()) {
			nlohmann::json j;
			file >> j;
			file.close();

			j = j["perm"];

			networkRecentPlayersMenuVars::vars.permanentPlayers.clear();
			for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
				networkRecentPlayersMenuVars::recentPlayer o;
				o.name = it.value()["name"].get<std::string>();
				o.ip = it.value()["ip"].get<std::string>();
				o.firstEncountered = it.value()["first encountered"].get<std::string>();
				o.lastEncountered = it.value()["last encountered"].get<std::string>();
				o.internalEncountered = it.value()["internal"].get<int>();
				o.rid = std::stoull(it.key());

				if (networkRecentPlayersMenuVars::vars.permanentPlayers.size() == 100) break;
				vars.queue.push_back(o.rid);
				networkRecentPlayersMenuVars::vars.permanentPlayers.push_back({ it.key(), o });
			}

			std::sort(networkRecentPlayersMenuVars::vars.permanentPlayers.begin(), networkRecentPlayersMenuVars::vars.permanentPlayers.end(), [] (std::pair<std::string, networkRecentPlayersMenuVars::recentPlayer>& first, std::pair<std::string, networkRecentPlayersMenuVars::recentPlayer>& second) { return first.second.internalEncountered > second.second.internalEncountered; });
		}
	}
}

void networkRecentPlayerPermanentMenu::init() {
	setName("Permanent");
	setParentSubmenu<networkRecentPlayersMenu>();
}

/*Called while in submenu*/
void networkRecentPlayerPermanentMenu::update() {
	static int TIMERS[2];
	static bool updatePlayers = true;
	static bool updateOnline = true;
	menu::util::getTimers()->runTimedFunction(&TIMERS[0], 1000, [] { updatePlayers = true; });
	menu::util::getTimers()->runTimedFunction(&TIMERS[1], 7500, [] { updateOnline = true; });

	if (updatePlayers) {
		updatePlayers = false;

		clearOptionsOffset(0);
		if (networkRecentPlayersMenuVars::vars.permanentPlayers.size()) {
			for (std::size_t i = 0; i < networkRecentPlayersMenuVars::vars.permanentPlayers.size(); i++) {
				auto& player = networkRecentPlayersMenuVars::vars.permanentPlayers[i];

				bool inSession = false;
				menu::getPlayerManager()->foreach([&] (menu::playerVars& p) {
					auto net = rage::engine::getNetGamePlayerFromIndex(p.id);
					if (net) {
						auto info = net->playerInfo;
						if (info) {
							if (info->netInfo.rockstarId2 == player.second.rid) {
								inSession = true;
							}
						}
					}
				});

				addOption(submenuOption(utils::va::va("%s%s", (global::vars::g_onlineRockstarIds[player.second.rid] || inSession) ? "" : "~m~", player.second.name.c_str()))
					.addOnClick([=] {
						networkSelectedRecentPlayerMenuVars::vars.type = 2;
						networkSelectedRecentPlayerMenuVars::vars.selectedPlayer = i;
						networkRecentPlayersMenuVars::vars.selectedList = &networkRecentPlayersMenuVars::vars.permanentPlayers;
					})
					.addSubmenu<networkSelectedRecentPlayerMenu>()
					.addOnHover([&] { menu::getPanelManager()->panelRecentPlayer(player.second.name, player.second.firstEncountered, player.second.lastEncountered, player.second.ip, player.first); }));
			}
		} else {
			addOption(buttonOption(networkRecentPlayersMenu::getInstance()->getString("~m~[~e~No Recent Players~m~]")));
		}
	}

	if (updateOnline) {
		updateOnline = false;

		for (std::size_t i = 0; i < vars.queue.size(); i++) {
			auto vit = std::find_if(global::vars::g_onlineRockstarIdQueue.begin(), global::vars::g_onlineRockstarIdQueue.end(), [=] (uint64_t value) {
				return value == vars.queue[i];
			});

			if (vit == global::vars::g_onlineRockstarIdQueue.end()) {
				global::vars::g_onlineRockstarIdQueue.push_back(vars.queue[i]);
			}
		}

		std::size_t processSize = vars.queue.size() > 32 ? 32 : vars.queue.size();

		presenceInfo* info = (presenceInfo*)malloc(processSize * sizeof(presenceInfo));
		memset(info, 0, processSize * sizeof(presenceInfo));

		for (std::size_t i = 0; i < processSize; i++) {
			info[i].rockstarID = vars.queue.back();
			info[i].type = 3;
			vars.queue.pop_back();
		}

		_i<int>(0x86E0660E4F5C956D); // reset
		rage::engine::sendSessionInfoRequestOld(info, (int)processSize);
		free(info);

		// recreate the queue
		if (vars.queue.size() == 0) {
			for (std::size_t i = 0; i < networkRecentPlayersMenuVars::vars.permanentPlayers.size(); i++) {
				auto& player = networkRecentPlayersMenuVars::vars.permanentPlayers[i];
				vars.queue.push_back(player.second.rid);
			}
		}
	}
}

/*Called once on submenu open*/
void networkRecentPlayerPermanentMenu::updateOnce() {
	loadIntoVector();
}

/*Called always*/
void networkRecentPlayerPermanentMenu::featureUpdate() {}

networkRecentPlayerPermanentMenu* _instance;
networkRecentPlayerPermanentMenu* networkRecentPlayerPermanentMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkRecentPlayerPermanentMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkRecentPlayerPermanentMenu::~networkRecentPlayerPermanentMenu() { delete _instance; }