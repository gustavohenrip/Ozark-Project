#include "network_presence.hpp"
#include "menu/base/submenu_handler.hpp"
#include "network_presence.hpp"
#include "menu/base/util/presence.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "rage/api/socialclub.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/va.hpp"
#include "menu/base/util/fiber_pool.hpp"
#include "rage/session/session.hpp"
#include "../presence_results.hpp"
#include "../overseer.hpp"

using namespace networkPresenceMenuVars;

namespace networkPresenceMenuVars {
	vars_ vars;
}

void networkPresenceMenu::init() {
	setName("Presence");
	setParentSubmenu<networkPresenceSeachResultsMenu>();

	addString("~e~Error~q~: Failed to get rockstar ID from name");
	addString("~e~Error~q~: Set the receiver above, it cannot be 0!");
	addString("~e~Error~q~: No accounts found");
	addString("~e~Error~q~: More than one account found. Using first result");

	// networkPresencePaginationMenu::getInstance();

	/*vars.rockstarMessageButton = addOption(buttonOption("Custom Input: ")
		.addKeyboard("Enter the rid/name to send a r* message to!", 100, [this] (const char* res) {
			vars.rockstarMessageSendToName.clear();
			vars.rockstarMessageSendTo = 0;

			std::string str(res);
			if (std::find_if(str.begin(), str.end(), [] (unsigned char c) { return !std::isdigit(c); }) == str.end()) {
				// numeric
				vars.rockstarMessageSendTo = std::atoi(str.c_str());
			} else {
				// str
				rage::api::getSocialclub()->getProfileFromName(res, [&, this] (bool success, rage::network::rockstarAccount acc) {
					if (success) {
						vars.rockstarMessageSendTo = acc.rockstarId;
						vars.rockstarMessageSendToName = acc.name;

						if (acc.total == 0) {
							menu::getNotify()->right(getString("~e~Error~q~: No accounts found"));
						} else if (acc.total > 1) {
							menu::getNotify()->right(getString("~e~Error~q~: More than one account found. Using first result"));
						}
					} else {
						menu::getNotify()->right(getString("~e~Error~q~: Failed to get rockstar ID from name"));
					}
				});
			}
		}));

	addOption(buttonOption("Send Rockstar Message")
		.addTranslation()
		.addKeyboard("Type in your message", 100, [this] (const char* msg) {
			if (vars.rockstarMessageSendTo) {
				menu::getPresence()->sendRockstarMessage(vars.rockstarMessageSendTo, msg);
			} else {
				menu::getNotify()->right(getString("~e~Error~q~: Set the receiver above, it cannot be 0!"));
			}
		}));

	addOption(breakOption("Crew Message")
		.addTranslation());

	vars.crewMessageButton = addOption(buttonOption("Custom Input: ")
		.addKeyboard("Enter the rid/name to send a crew message to!", 100, [this] (const char* res) {
			vars.crewMessageSendToName.clear();
			vars.crewMessageSendTo = 0;

			std::string str(res);
			if (std::find_if(str.begin(), str.end(), [] (unsigned char c) { return !std::isdigit(c); }) == str.end()) {
				// numeric
				vars.crewMessageSendTo = std::atoi(str.c_str());
			} else {
				// str
				rage::api::getSocialclub()->getProfileFromName(res, [&, this] (bool success, rage::network::rockstarAccount acc) {
					if (success) {
						vars.crewMessageSendTo = acc.rockstarId;
						vars.crewMessageSendToName = acc.name;

						if (acc.total == 0) {
							menu::getNotify()->right(getString("~e~Error~q~: No accounts found"));
						} else if (acc.total > 1) {
							menu::getNotify()->right(getString("~e~Error~q~: More than one account found. Using first result"));
						}
					} else {
						menu::getNotify()->right(getString("~e~Error~q~: Failed to get rockstar ID from name"));
					}
				});
			}
		}));

	addOption(buttonOption("Send Crew Message")
		.addTranslation()
		.addKeyboard("Type in your message", 100, [this] (const char* msg) {
			if (vars.crewMessageSendTo) {
				menu::getPresence()->sendCrewMessage(vars.crewMessageSendTo, msg, "OKAY", 17506743);
			} else {
				menu::getNotify()->right(getString("~e~Error~q~: Set the receiver above, it cannot be 0!"));
			}
		}));

	addOption(breakOption("Join RID")
		.addTranslation());*/
	/*
			rage::api::getSocialclub()->getProfileFromName(msg, [=] (bool success, rage::network::rockstarAccount acc) {
			dev("%s -> %i", msg, acc.rockstarId);
		});
	
	*/
	//vars.joinRidButton = addOption(buttonOption("Input: ")
	//	.addKeyboard("Enter the rid/name to join!", 100, [this] (const char* res) {
	//		vars.joinRidSendToName.clear();
	//		vars.joinRidSendTo = 0;

	//		std::string str(res);
	//		if (std::find_if(str.begin(), str.end(), [] (unsigned char c) { return !std::isdigit(c); }) == str.end()) {
	//			// numeric
	//			vars.joinRidSendTo = std::atoi(str.c_str());
	//		} else {
	//			// str
	//			rage::api::getSocialclub()->getProfileFromName(res, [&, this](bool success, rage::network::rockstarAccount acc) {
	//				if (success) {
	//					vars.joinRidSendTo = acc.rockstarId;
	//					vars.joinRidSendToName = acc.name;
	//					if (acc.total == 0) {
	//						menu::getNotify()->right(getString("~e~Error~q~: No accounts found"));
	//					}
	//					else if (acc.total > 1) {
	//						menu::getNotify()->right(getString("~e~Error~q~: More than one account found. Using first result"));
	//					}
	//					else {
	//						menu::getNotify()->right(getString("~t6~Success~q~: Found account. You're ready to join"));
	//					}
	//				}
	//				else {
	//					menu::getNotify()->right(getString("~e~Error~q~: Failed to get rockstar ID from name"));
	//				}
	//			});
	//		}
	//	}));

	//addOption(submenuOption("Search")
	//	.addTranslation()
	//	.addSubmenu<networkPresenceSeachResultsMenu>());

	//addOption(breakOption("Presence").addTranslation().addRequirement([] { return (bool)vars.joinRidSendTo; }));

	addOption(buttonOption("Add to Overseer")
		.addOnClick([] {
			networkOverseerMenuVars::save(vars.joinRidSendToName, vars.joinRidSendTo, "Manually added");
		})
		.addRequirement([] { return (bool)vars.joinRidSendTo; }));

	addOption(buttonOption("Join Session")
		.addTranslation()
		.addOnClick([] {
			if (vars.joinRidSendTo) {
				menu::getNotify()->right(utils::va::va("Joining ~pa~%i", vars.joinRidSendTo));
				menu::getPresence()->joinThem(vars.joinRidSendTo);
			}
		}).addRequirement([] { return (bool)vars.joinRidSendTo; }));

	addOption(breakOption("Socialclub").addRequirement([] { return (bool)vars.joinRidSendTo; }));

	addOption(buttonOption("Invite to Session")
		.addTranslation()
		.addOnClick([] {
			auto netGamePlayer = rage::engine::getNetGamePlayerFromIndex(menu::getLocalPlayer().id);
			if (netGamePlayer) {
				auto playerInfo = netGamePlayer->playerInfo;
				if (playerInfo) {
					menu::getPresence()->invitePlayer(vars.joinRidSendTo, playerInfo->netInfo.rockstarId2);
				}
			}
		}).addRequirement([] { return (bool)vars.joinRidSendTo; }));

	addOption(buttonOption("Invite Me")
		.addTranslation()
		.addOnClick([] {
			auto netGamePlayer = rage::engine::getNetGamePlayerFromIndex(menu::getLocalPlayer().id);
			if (netGamePlayer) {
				auto playerInfo = netGamePlayer->playerInfo;
				if (playerInfo) {
					menu::getPresence()->invitePlayer(playerInfo->netInfo.rockstarId2, vars.joinRidSendTo);
				}
			}
		}).addRequirement([] { return (bool)vars.joinRidSendTo; }));

#if defined(DEV) || defined(TESTER)
	addOption(breakOption("Dev").addRequirement([] { return (bool)vars.joinRidSendTo; }));

	addOption(buttonOption("Kick Player")
		.addTranslation()
		.addOnClick([] {
			if (vars.joinRidSendTo) {
				menu::getNotify()->right(utils::va::va("Kicking ~pa~%i", vars.joinRidSendTo));
				menu::getPresence()->kickPlayer(vars.joinRidSendTo);
			}
		}).addRequirement([] { return (bool)vars.joinRidSendTo; }));

	addOption(buttonOption("Force Join")
		.addTranslation()
		.addOnClick([] {
			if (vars.joinRidSendTo) {
				menu::getNotify()->right(utils::va::va("Forcing ~pa~%i", vars.joinRidSendTo));
				menu::getPresence()->joinMe(vars.joinRidSendTo);
			}
		}).addRequirement([] { return (bool)vars.joinRidSendTo; }));

	addOption(buttonOption("test")
		.addTranslation()
		.addOnClick([] {
			if (vars.joinRidSendTo) {
				menu::getPresence()->gameTriggerEvent(vars.joinRidSendTo);
			}
		}).addRequirement([] { return (bool)vars.joinRidSendTo; }));

	//gameTriggerEvent
#endif // DEV


	//addOption(buttonOption("Invite Player")
	//	.addTranslation()
	//	.addOnClick([] {
	//		if (vars.joinRidSendTo) {
	//			menu::getNotify()->right(utils::va::va("Inviting ~pa~%i", vars.joinRidSendTo));
	//			menu::getPresence()->invitePlayer(vars.joinRidSendTo);
	//		}
	//		}).addRequirement([] { return (bool)vars.joinRidSendTo; }));

	//addOption(breakOption("Join Me")
	//	.addTranslation());

	//vars.joinMeButton = addOption(buttonOption("Custom Input: ")
	//	.addKeyboard("Enter the rid/name to join!", 100, [this](const char* res) {
	//		vars.joinMeSendToName.clear();
	//		vars.joinMeSendTo = 0;

	//		std::string str(res);
	//		if (std::find_if(str.begin(), str.end(), [](unsigned char c) { return !std::isdigit(c); }) == str.end()) {
	//			// numeric
	//			vars.joinMeSendTo = std::atoi(str.c_str());
	//		} else {
	//			// str
	//			rage::api::getSocialclub()->getProfileFromName(res, [&, this](bool success, rage::network::rockstarAccount acc) {
	//				if (success) {
	//					vars.joinMeSendTo = acc.rockstarId;
	//					vars.joinMeSendToName = acc.name;

	//					if (acc.total == 0) {
	//						menu::getNotify()->right(getString("~e~Error~q~: No accounts found"));
	//					} else if (acc.total > 1) {
	//						menu::getNotify()->right(getString("~e~Error~q~: More than one account found. Using first result"));
	//					}
	//				} else {
	//					menu::getNotify()->right(getString("~e~Error~q~: Failed to get rockstar ID from name"));
	//				}
	//			});
	//		}
	//	}));

	//addOption(buttonOption("Join Me")
	//	.addTranslation()
	//	.addOnClick([] {
	//		if (vars.joinMeSendTo) {
	//			menu::getPresence()->joinMe(vars.joinMeSendTo);
	//		}
	//	}));
}

/*Called while in submenu*/
void networkPresenceMenu::update() {
	/*char buffer[50];
	snprintf(buffer, 50, "Custom Input: %s", vars.rockstarMessageSendToName.empty() ? std::to_string(vars.rockstarMessageSendTo).c_str() : vars.rockstarMessageSendToName.c_str());
	vars.rockstarMessageButton->setName(buffer);

	char buffer2[50];
	snprintf(buffer2, 50, "Custom Input: %s", vars.crewMessageSendToName.empty() ? std::to_string(vars.crewMessageSendTo).c_str() : vars.crewMessageSendToName.c_str());
	vars.crewMessageButton->setName(buffer2);*/

	//char buffer3[50];
	//snprintf(buffer3, 50, "Input: %s", vars.joinRidSendToName.empty() ? std::to_string(vars.joinRidSendTo).c_str() : vars.joinRidSendToName.c_str());
	//vars.joinRidButton->setName(buffer3);

	if ((bool)vars.joinRidSendTo) {
		//menu::getPanelManager()->panelPresence();
	}
	//char buffer4[50];
	//snprintf(buffer4, 50, "Custom Input: %s", vars.joinMeSendToName.empty() ? std::to_string(vars.joinMeSendTo).c_str() : vars.joinMeSendToName.c_str());
	//vars.joinMeButton->setName(buffer4);
}

/*Called once on submenu open*/
void networkPresenceMenu::updateOnce() {
	setName(vars.joinRidSendToName);
}

/*Called always*/
void networkPresenceMenu::featureUpdate() {}

networkPresenceMenu* _instance;
networkPresenceMenu* networkPresenceMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkPresenceMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkPresenceMenu::~networkPresenceMenu() { delete _instance; }