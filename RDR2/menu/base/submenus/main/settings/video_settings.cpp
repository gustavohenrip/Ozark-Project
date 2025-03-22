#include "video_settings.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../settings.hpp"
#include "rage/types/network_types.hpp"
#include "rage/session/session.hpp"
#include "utils/va.hpp"


using namespace videoSettingsMenuVars;

namespace videoSettingsMenuVars {
	vars_ vars;
	
	void hideNames() {
		menu::getPlayerManager()->foreach([=](menu::playerVars& lobby) {
			if (lobby.connected) {
				rage::network::netGamePlayer* netGamePlayer = rage::engine::getNetGamePlayerFromIndex(lobby.id);
				if (vars.cachedPlayerInfo[netGamePlayer->playerInfo].empty()) {
					vars.cachedPlayerInfo[netGamePlayer->playerInfo] = netGamePlayer->playerInfo->netInfo.name;
					strcpy(netGamePlayer->playerInfo->netInfo.name, utils::va::va("Player %i", netGamePlayer->playerID));
				}
			}
		}, true);
	}

	void restoreNames() {
		menu::getPlayerManager()->foreach([=](menu::playerVars& lobby) {
			if (lobby.connected) {
				rage::network::netGamePlayer* netGamePlayer = rage::engine::getNetGamePlayerFromIndex(lobby.id);
				if (!vars.cachedPlayerInfo[netGamePlayer->playerInfo].empty()) {
					strcpy(netGamePlayer->playerInfo->netInfo.name, vars.cachedPlayerInfo[netGamePlayer->playerInfo].c_str());
					vars.cachedPlayerInfo[netGamePlayer->playerInfo].clear();
				}
			}
		}, true);
	}
}

void videoSettingsMenu::init() {
	setName("Video Settings");
	setParentSubmenu<settingsMenu>();

	addOption(toggleOption("Hide Me")
		.addTranslation()
		.addToggle(vars.hideMe));

	addOption(toggleOption("Hide Players")
		.addTranslation()
		.addToggle(vars.hideNames)
		.addOnClick([] { 
			
			if (vars.hideNames) {
				hideNames();
			}
			else {
				restoreNames();
			}
			
			}));

	addOption(toggleOption("Hide IPs")
		.addTranslation()
		.addToggle(vars.hideIPs));

	addOption(toggleOption("Hide Rockstar IDs")
		.addTranslation()
		.addToggle(vars.hideRID));
}

/*Called while in submenu*/
void videoSettingsMenu::update() {}

/*Called once on submenu open*/
void videoSettingsMenu::updateOnce() {}

/*Called always*/
void videoSettingsMenu::featureUpdate() {}

videoSettingsMenu* _instance;
videoSettingsMenu* videoSettingsMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new videoSettingsMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

videoSettingsMenu::~videoSettingsMenu() { delete _instance; }