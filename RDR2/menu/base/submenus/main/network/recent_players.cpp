#include "recent_players.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network.hpp"
#include "utils/va.hpp"
#include "menu/base/util/timers.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "recent/selected_recent_player.hpp"
#include "recent/recent_permanent.hpp"
#include <queue>

using namespace networkRecentPlayersMenuVars;

namespace networkRecentPlayersMenuVars {
	vars_ vars;

	std::string getCurrentTime() {
		time_t curr_time;
		time(&curr_time);
		tm* curr_tm = localtime(&curr_time);

		char buffer[40];
		strftime(buffer, 40, "%a %d %b %Y %H:%M:%S", curr_tm);
		return std::string(buffer);
	}

	void save(std::string name, std::string ip, uint64_t rid) {
		nlohmann::json j;
		std::vector<recentPlayer> players;
		std::vector<recentPlayer> permPlayers;

		std::ifstream file(utils::getConfig()->getRecentPlayersPath());
		if (file.good()) {
			file >> j;
			file.close();
		}

		if (!j.is_null() && !j["recent"].is_null()) {
			for (nlohmann::json::iterator it = j["recent"].begin(); it != j["recent"].end(); ++it) {
				recentPlayer o;
				o.name = it.value()["name"].get<std::string>();
				o.ip = it.value()["ip"].get<std::string>();
				o.firstEncountered = it.value()["first encountered"].get<std::string>();
				o.lastEncountered = it.value()["last encountered"].get<std::string>();
				o.rid = std::stoull(it.key());
				o.internalEncountered = it.value()["internal"].get<int>();

				players.push_back(o);
			}
		}

		if (!j.is_null() && !j["perm"].is_null()) {
			for (nlohmann::json::iterator it = j["perm"].begin(); it != j["perm"].end(); ++it) {
				recentPlayer o;
				o.name = it.value()["name"].get<std::string>();
				o.ip = it.value()["ip"].get<std::string>();
				o.firstEncountered = it.value()["first encountered"].get<std::string>();
				o.lastEncountered = it.value()["last encountered"].get<std::string>();
				o.rid = std::stoull(it.key());
				o.internalEncountered = it.value()["internal"].get<int>();

				permPlayers.push_back(o);
			}
		}

		bool exists = false;
		for (auto& p : players) {
			if (p.rid == rid) {
				exists = true;
				p.lastEncountered = getCurrentTime();
				p.internalEncountered = (int)time(0);
				break;
			}
		}

		if (!exists) {
			recentPlayer newPlayer;
			newPlayer.name = name;
			newPlayer.ip = ip;
			newPlayer.internalEncountered = (int)time(0);
			newPlayer.rid = rid;
			newPlayer.firstEncountered = getCurrentTime();
			newPlayer.lastEncountered = getCurrentTime();
			players.push_back(newPlayer);
		}

		std::sort(players.begin(), players.end(), [] (recentPlayer& first, recentPlayer& second) { return first.internalEncountered > second.internalEncountered; });

		std::ofstream output(utils::getConfig()->getRecentPlayersPath());
		if (output.good()) {
			nlohmann::json resp;

			for (auto& p : permPlayers) {
				resp["perm"][std::to_string(p.rid)]["name"] = p.name;
				resp["perm"][std::to_string(p.rid)]["ip"] = p.ip;
				resp["perm"][std::to_string(p.rid)]["first encountered"] = p.firstEncountered;
				resp["perm"][std::to_string(p.rid)]["last encountered"] = p.lastEncountered;
				resp["perm"][std::to_string(p.rid)]["internal"] = p.internalEncountered;
			}

			int count = 0;
			for (auto& p : players) {
				if (count > 100) break;
				resp["recent"][std::to_string(p.rid)]["name"] = p.name;
				resp["recent"][std::to_string(p.rid)]["ip"] = p.ip;
				resp["recent"][std::to_string(p.rid)]["first encountered"] = p.firstEncountered;
				resp["recent"][std::to_string(p.rid)]["last encountered"] = p.lastEncountered;
				resp["recent"][std::to_string(p.rid)]["internal"] = p.internalEncountered;
				count++;
			}

			output << resp.dump(4);
			output.close();
		}
	}

	void loadIntoVector() {
		std::ifstream file(utils::getConfig()->getRecentPlayersPath());
		if (file.good()) {
			nlohmann::json j;
			file >> j;
			file.close();

			j = j["recent"];

			vars.players.clear();
			for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
				recentPlayer o;
				o.name = it.value()["name"].get<std::string>();
				o.ip = it.value()["ip"].get<std::string>();
				o.firstEncountered = it.value()["first encountered"].get<std::string>();
				o.lastEncountered = it.value()["last encountered"].get<std::string>();
				o.internalEncountered = it.value()["internal"].get<int>();
				o.rid = std::stoull(it.key());

				if (vars.players.size() == 100) break;
				vars.queue.push_back(o.rid);
				vars.players.push_back({ it.key(), o });
			}

			std::sort(vars.players.begin(), vars.players.end(), [] (std::pair<std::string, recentPlayer>& first, std::pair<std::string, recentPlayer>& second) { return first.second.internalEncountered > second.second.internalEncountered; });
		}
	}
}

void networkRecentPlayersMenu::init() {
	setName("Recent Players");
	setParentSubmenu<networkMenu>();

	addString("You don't have any recent players :(");

	addOption(submenuOption("Permanent")
		.addTranslation()
		.addSubmenu<networkRecentPlayerPermanentMenu>());

	addOption(breakOption("Players")
		.addTranslation()
		.addRequirement([] { return vars.players.size() > 0; }));

	networkSelectedRecentPlayerMenu::getInstance();
}

/*Called while in submenu*/
void networkRecentPlayersMenu::update() {
	static int TIMERS[2];
	static bool updatePlayers = true;
	static bool updateOnline = true;
	menu::util::getTimers()->runTimedFunction(&TIMERS[0], 1000, [] { updatePlayers = true; });
	menu::util::getTimers()->runTimedFunction(&TIMERS[1], 7500, [] { updateOnline = true; });

	if (updatePlayers) {
		updatePlayers = false;

		clearOptionsOffset(2);
		if (vars.players.size()) {
			for (std::size_t i = 0; i < vars.players.size(); i++) {
				auto& player = vars.players[i];

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
						networkSelectedRecentPlayerMenuVars::vars.type = 1;
						networkSelectedRecentPlayerMenuVars::vars.selectedPlayer = i;
						networkRecentPlayersMenuVars::vars.selectedList = &networkRecentPlayersMenuVars::vars.players;
					})
					.addSubmenu<networkSelectedRecentPlayerMenu>()
					.addOnHover([&] { menu::getPanelManager()->panelRecentPlayer(player.second.name, player.second.firstEncountered, player.second.lastEncountered, player.second.ip, player.first); }));
			}
		} else {
			addOption(buttonOption(getString("~m~[~e~No Recent Players~m~]")));
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
			for (std::size_t i = 0; i < vars.players.size(); i++) {
				auto& player = vars.players[i];
				vars.queue.push_back(player.second.rid);
			}
		}
	}
}

/*Called once on submenu open*/
void networkRecentPlayersMenu::updateOnce() {
	loadIntoVector();
}

/*Called always*/
void networkRecentPlayersMenu::featureUpdate() {}

networkRecentPlayersMenu* _instance;
networkRecentPlayersMenu* networkRecentPlayersMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkRecentPlayersMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkRecentPlayersMenu::~networkRecentPlayersMenu() { delete _instance; }