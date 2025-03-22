#include "overseer.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network.hpp"
#include "utils/va.hpp"
#include "menu/base/util/presence.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "menu/base/util/timers.hpp"
#include "overseer/selected_overseer.hpp"
#include "menu/base/submenus/main/network/nasty/crash.hpp"
#include "rage/engine.hpp"
#include <iomanip>

using namespace networkOverseerMenuVars;

namespace networkOverseerMenuVars {
	vars_ vars;

	void load(networkOverseerMenu* ptr) {
		ptr->clearOptionsOffset(2);

		std::ifstream file(utils::getConfig()->getOverseerPath());
		if (file.good()) {
			nlohmann::json j;
			file >> j;
			file.close();

			vars.players.clear();
			for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
				overseer o;
				o.flags = it.value()["flags"].get<int>();
				o.name = it.value()["name"].get<std::string>();
				o.reason = it.value()["reason added"].get<std::string>();

				vars.players.push_back({ it.key(), o });
			}

			for (std::size_t i = 0; i < vars.players.size(); i++) {
				auto& player = vars.players[i];

				ptr->addOption(submenuOption(utils::va::va("%s%s", (player.second.flags & OVERSEER_ENABLED) ? "~t6~" : "", player.second.name.c_str()))
					.addOnClick([=] { networkSelectedOverseerMenuVars::vars.selectedPlayer = i; })
					.addSubmenu<networkSelectedOverseerMenu>()
					.addOnHover([&] { menu::getPanelManager()->panelOverseer(player.first, player.second.name, player.second.reason, player.second.flags); }));
			}
		}
	}

	bool save(std::string name, int rid, std::string reason) {
		auto vit = std::find_if(networkOverseerMenuVars::vars.players.begin(), networkOverseerMenuVars::vars.players.end(), [=] (const std::pair<std::string, networkOverseerMenuVars::overseer>& elem) -> bool { return elem.first == std::to_string(rid); });
		if (vit == networkOverseerMenuVars::vars.players.end()) {
			load(networkOverseerMenu::getInstance());

			nlohmann::json outputJson;
			for (auto& player : networkOverseerMenuVars::vars.players) {
				outputJson[player.first]["name"] = player.second.name;
				outputJson[player.first]["reason added"] = player.second.reason;
				outputJson[player.first]["flags"] = player.second.flags;
			}

			nlohmann::json newJson;
			newJson["name"] = name;
			newJson["reason added"] = reason;
			newJson["flags"] = OVERSEER_ENABLED | OVERSEER_NOTIFY;

			outputJson[std::to_string(rid)] = newJson;

			std::ofstream output(utils::getConfig()->getOverseerPath());
			if (output.good()) {
				output << outputJson.dump(4);
				output.close();
			}

			load(networkOverseerMenu::getInstance());

			return true;
		}

		return false;
	}

	void process(int playerID, int flag) {
		if (flag & OVERSEER_ENABLED) {
			if (flag & OVERSEER_CRASH) {
				networkCrashMenuVars::v3(playerID);
			}

			if (flag & OVERSEER_EXPLODE_LOOP) {
				menu::getPlayer(playerID).explodeLoop = true;
			}

			if (flag & OVERSEER_PARALYSE) {
				menu::getPlayer(playerID).paralyse = true;
			}
		}
	}
}

void networkOverseerMenu::init() {
	setName("Overseer");
	setParentSubmenu<networkMenu>();

	addOption(toggleOption("Enable Overseer")
		.addTranslation()
		.addToggle(vars.enableOverseer).canBeSaved(name.getOriginal()));

	addOption(breakOption("Players")
		.addTranslation()
		.addRequirement([] { return vars.players.size() > 0; }));

	load(this);

	networkSelectedOverseerMenu::getInstance();
}

/*Called while in submenu*/
void networkOverseerMenu::update() {}

/*Called once on submenu open*/
void networkOverseerMenu::updateOnce() {
	load(this);
}

/*Called always*/
void networkOverseerMenu::featureUpdate() {
	static int timer = 0;
	menu::util::getTimers()->runTimedFunction(&timer, 2000, [] {
		menu::getPlayerManager()->foreach([](menu::playerVars& player) {
			if (player.id == menu::getLocalPlayer().id) return;

			auto net = rage::engine::getNetGamePlayerFromIndex(player.id);
			if (net) {
				auto info = net->playerInfo;
				if (info) {
					auto vit = std::find_if(networkOverseerMenuVars::vars.players.begin(), networkOverseerMenuVars::vars.players.end(), [=](const std::pair<std::string, overseer>& elem) -> bool { return elem.first == std::to_string(info->netInfo.rockstarId2); });
					if (vit != networkOverseerMenuVars::vars.players.end()) {
						int32_t flag = vit->second.flags;
						if (flag & OVERSEER_ENABLED) {
							networkOverseerMenuVars::process(player.id, flag);
						}
					}
				}
			}
			}, true);


		//auto vit = std::find_if(networkOverseerMenuVars::vars.players.begin(), networkOverseerMenuVars::vars.players.end(), [=](const std::pair<std::string, overseer>& elem) -> bool { return elem.first == std::to_string(info->netInfo.rockstarId2); });
		vars.blockJoiners.clear();
		std::for_each(networkOverseerMenuVars::vars.players.begin(), networkOverseerMenuVars::vars.players.end(), [=](const std::pair<std::string, overseer>& elem){
			if (elem.second.flags & OVERSEER_ENABLED && elem.second.flags & OVERSEER_BLOCK_JOIN) {
				vars.blockJoiners.push_back(elem.first);
			}
		});

		vars.blacklist.clear();
		std::for_each(networkOverseerMenuVars::vars.players.begin(), networkOverseerMenuVars::vars.players.end(), [=](const std::pair<std::string, overseer>& elem) {
			if (elem.second.flags & OVERSEER_ENABLED && elem.second.flags & OVERSEER_BLACKLIST) {
				vars.blacklist.push_back(std::make_pair(elem.first, elem.second.name));
			}
		});

	});
}

networkOverseerMenu* _instance;
networkOverseerMenu* networkOverseerMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkOverseerMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkOverseerMenu::~networkOverseerMenu() { delete _instance; }