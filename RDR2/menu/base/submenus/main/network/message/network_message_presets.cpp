#include "network_message_presets.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_message.hpp"
#include "../recent_players.hpp"
#include "../recent/selected_recent_player.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "rage/engine.hpp"
#include "utils/va.hpp"

using namespace networkMessagePresetsMenuVars;

namespace networkMessagePresetsMenuVars {
	vars_ vars;
}

void networkMessagePresetsMenu::init() {
	setName("Message Presets");
	setParentSubmenu<networkMessageMenu>();

	addOption(buttonOption("Send IP")
		.addTranslation()
		.addOnClick([] {
			auto net = rage::engine::getNetGamePlayerFromIndex(menu::getSelectedPlayer().id);
			if (net) {
				auto info = net->playerInfo;
				if (info) {
					if (global::vars::g_relayUsage.find(info->netInfo.rockstarId2) != global::vars::g_relayUsage.end()) {
						uint32_t ip = *(uint32_t*)&info->netInfo.externalIp;
						if (global::vars::g_relayUsage[info->netInfo.rockstarId2] != ip) {
							ip = global::vars::g_relayUsage[info->netInfo.rockstarId2];
							networkMessageMenuVars::vars.message = utils::va::va("I know that your real IP is %i.%i.%i.%i ;) You're not fooling ~t3~Ozark~q~!", ((ip & 0xFF000000) >> 24), ((ip & 0xFF0000) >> 16), ((ip & 0xFF00) >> 8), (ip & 0xFF));
						} else {
							networkMessageMenuVars::vars.message = utils::va::va("I know that your IP is %i.%i.%i.%i ;)", info->netInfo.externalIp[3], info->netInfo.externalIp[2], info->netInfo.externalIp[1], info->netInfo.externalIp[0]);
						}
					} else {
						networkMessageMenuVars::vars.message = utils::va::va("I know that your IP is %i.%i.%i.%i ;)", info->netInfo.externalIp[3], info->netInfo.externalIp[2], info->netInfo.externalIp[1], info->netInfo.externalIp[0]);
					}
				}
			}
			menu::getSubmenuHandler()->setSubmenuPrevious(); 
		})
		.addRequirement([] {return networkMessageMenuVars::vars.submenuIdentifier == 0; }));

	addOption(buttonOption("Advertisement")
		.addTranslation()
		.addOnClick([=] {
			networkMessageMenuVars::vars.message = "Purchase ~pa~Ozark~q~ at ~t5~ozarkengine.cc~q~!";
			menu::getSubmenuHandler()->setSubmenuPrevious();
		}));

	addOption(buttonOption("Mock Shit Cheat")
		.addTranslation()
		.addOnClick([=] {
			networkMessageMenuVars::vars.message = "Cool cheats. Sike.";
			menu::getSubmenuHandler()->setSubmenuPrevious();
		}));

	addOption(buttonOption("I boke up wif my X gurl")
		.addTranslation()
		.addOnClick([=] {
			networkMessageMenuVars::vars.message = "I boke up wif my X gurl, heres her numba. SIKE. Dats the wrong nuMBA!!";
			menu::getSubmenuHandler()->setSubmenuPrevious();
		}));

	addOption(buttonOption("Cool Stream")
		.addTranslation()
		.addOnClick([=] {
			networkMessageMenuVars::vars.message = "Cool stream, you're killing it";
			menu::getSubmenuHandler()->setSubmenuPrevious();
		}));

	addOption(buttonOption("Ozark #1")
		.addTranslation()
		.addOnClick([=] {
			networkMessageMenuVars::vars.message = "~t8~Ozark #1";
			menu::getSubmenuHandler()->setSubmenuPrevious();
		}));

	addOption(buttonOption("Enjoy the Crash")
		.addTranslation()
		.addOnClick([=] {
			networkMessageMenuVars::vars.message = "Enjoy the crash :)";
			menu::getSubmenuHandler()->setSubmenuPrevious();
		}));

	addOption(buttonOption("Your crash sucks")
		.addTranslation()
		.addOnClick([=] {
			networkMessageMenuVars::vars.message = "Jheeze, what cheat is that? I blocked the crash ezpz. GET OZARK!";
			menu::getSubmenuHandler()->setSubmenuPrevious();
		}));

	addOption(buttonOption("Big brain time")
		.addTranslation()
		.addOnClick([=] {
			networkMessageMenuVars::vars.message = "Big brain time";
			menu::getSubmenuHandler()->setSubmenuPrevious();
		}));
}

/*Called while in submenu*/
void networkMessagePresetsMenu::update() {
	if (networkMessageMenuVars::vars.submenuIdentifier == 0) {
		menu::getPanelManager()->panelPlayer(menu::getSelectedPlayer().id);
	} else if (networkMessageMenuVars::vars.submenuIdentifier == 1) {
		auto player = networkRecentPlayersMenuVars::vars.selectedList->at(networkSelectedRecentPlayerMenuVars::vars.selectedPlayer);
		menu::getPanelManager()->panelRecentPlayer(player.second.name, player.second.firstEncountered, player.second.lastEncountered, player.second.ip, player.first);
	}
}

/*Called once on submenu open*/
void networkMessagePresetsMenu::updateOnce() {}

/*Called always*/
void networkMessagePresetsMenu::featureUpdate() {}

networkMessagePresetsMenu* _instance;
networkMessagePresetsMenu* networkMessagePresetsMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkMessagePresetsMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkMessagePresetsMenu::~networkMessagePresetsMenu() { delete _instance; }