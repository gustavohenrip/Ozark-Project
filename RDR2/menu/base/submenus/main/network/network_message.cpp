#include "network_message.hpp"
#include "menu/base/submenu_handler.hpp"
#include "players.hpp"
#include "recent/selected_recent_player.hpp"
#include "all/all_players.hpp"
#include "recent_players.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "menu/base/util/presence.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/notify.hpp"
#include "message/network_message_presets.hpp"
#include "message/network_message_save_load.hpp"
#include "friends/selected_friend.hpp"

using namespace networkMessageMenuVars;

namespace networkMessageMenuVars {
	vars_ vars;

	scrollStruct<const char*> textColors[] = {
		"Default", "",
		"White", "~q~",
		"Red", "~e~",
		"Light Red", "~t8~",
		"Yellow", "~o~",
		"Yellow 2", "~t4~",
		"Orange", "~d~",
		"Orange 2", "~t2~",
		"Grey", "~m~",
		"Light Grey", "~t~",
		"Black", "~v~",
		"Pink", "~u~",
		"Light Pink", "~t5~",
		"Blue", "~pa~",
		"Light Blue", "~t3~",
		"Dark Blue", "~t7~",
		"Purple", "~t1~",
		"Green", "~t6~",
	};

	scrollStruct<const char*> textSettings[] = {
		"Normal", "",
		"Bold", "~bold~",
		"Italic", "~italic~",
	};

	scrollStruct<int32_t> messageType[] = {
		"Rockstar", 0,
		"Crew", 1
	};

	void sendMessage() {
		char msg[150];
		snprintf(msg, 150, "%s%s%s", textSettings[vars.style].result, textColors[vars.color].result, vars.message.c_str());
		if (vars.submenuIdentifier == 0 || vars.submenuIdentifier == 1) {
			if (vars.rockstarID) {
				if (!vars.messageIdentifier) {
					menu::getPresence()->sendRockstarMessage(vars.rockstarID, (const char*)msg);
				} else {
					menu::getPresence()->sendCrewMessage(vars.rockstarID, msg, "OKAY", 17506743);
				}
			} else {
				menu::getNotify()->right("~e~Failed to grab players information");
			}
		} else {
			menu::getPlayerManager()->foreach([=](menu::playerVars& player) {
				auto net = rage::engine::getNetGamePlayerFromIndex(player.id);
				if (net) {
					auto info = net->playerInfo;
					if (info) {
						if (!vars.messageIdentifier) {
							menu::getPresence()->sendRockstarMessage(info->netInfo.rockstarId2, (const char*)msg);
						} else {
							menu::getPresence()->sendCrewMessage(info->netInfo.rockstarId2, msg, "OKAY", 17506743);
						}
					}
				}
			});
		}
	}
}

void networkMessageMenu::init() {
	setName("Message");
	setParentSubmenu<networkPlayersMenu>();

	addOption(submenuOption("Save and Load")
		.addTranslation()
		.addSubmenu<networkMessageSaveLoadMenu>());

	addOption(submenuOption("Presets")
		.addTranslation()
		.addSubmenu<networkMessagePresetsMenu>());

	addOption(scrollOption<int32_t>(SCROLL, "Message Type")
		.addTranslation()
		.addScroll(vars.messageIdentifier, 0, NUMOF(messageType), messageType));

	vars.inputButton = addOption(buttonOption("")
		.addKeyboard("Type in your message", 100, [](const char* msg) { vars.message = std::string(msg); }));

	addOption(breakOption("Preferences")
		.addTranslation());

	addOption(scrollOption<const char*>(SCROLL, "Color")
		.addTranslation()
		.addScroll(vars.color, 0, NUMOF(textColors), textColors));

	addOption(scrollOption<const char*>(SCROLL, "Style")
		.addTranslation()
		.addScroll(vars.style, 0, NUMOF(textSettings), textSettings));

	addOption(buttonOption("Send")
		.addOnClick(sendMessage));
}

/*Called while in submenu*/
void networkMessageMenu::update() {
	if (vars.submenuIdentifier == 0) {
		setParentSubmenu<networkPlayersMenu>();
		menu::getPanelManager()->panelPlayer(menu::getSelectedPlayer().id);
	} else if (vars.submenuIdentifier == 1) {
		setParentSubmenu<networkSelectedRecentPlayerMenu>();
		auto player = networkRecentPlayersMenuVars::vars.selectedList->at(networkSelectedRecentPlayerMenuVars::vars.selectedPlayer);
		menu::getPanelManager()->panelRecentPlayer(player.second.name, player.second.firstEncountered, player.second.lastEncountered, player.second.ip, player.first);
	} else if (vars.submenuIdentifier == 2) {
		setParentSubmenu<networkAllPlayersMenu>();
	} else if (vars.submenuIdentifier == 3) {
		setParentSubmenu<networkSelectedFriendMenu>();
	}

	std::string temp = vars.message;
	std::size_t s = temp.size();

	if (temp.size() > 32) {
		temp.erase(temp.begin() + 32, temp.end());
		temp += std::string("...");
	}

	snprintf(vars.inputMessage, 100, "Input: %s%s%s", textSettings[vars.style].result, textColors[vars.color].result, temp.c_str());
	vars.inputButton->setName(vars.inputMessage);
}

/*Called once on submenu open*/
void networkMessageMenu::updateOnce() {}

/*Called always*/
void networkMessageMenu::featureUpdate() {}

networkMessageMenu* _instance;
networkMessageMenu* networkMessageMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkMessageMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkMessageMenu::~networkMessageMenu() { delete _instance; }