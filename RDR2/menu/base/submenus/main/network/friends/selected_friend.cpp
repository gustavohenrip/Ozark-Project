#include "selected_friend.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../friends.hpp"
#include "menu/base/util/presence.hpp"
#include "../network_message.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "rage/session/session.hpp"

using namespace networkSelectedFriendMenuVars;

namespace networkSelectedFriendMenuVars {
	vars_ vars;

}

void networkSelectedFriendMenu::init() {
	setParentSubmenu<networkFriendsMenu>();

	addOption(buttonOption("Join Player")
		.addTranslation()
		.addOnClick([] { menu::getPresence()->joinThem(vars.currentFriend.first.rid); }));

	addOption(breakOption("Socialclub"));

	addOption(buttonOption("Invite to Session")
		.addTranslation()
		.addOnClick([] {
			auto netGamePlayer = rage::engine::getNetGamePlayerFromIndex(menu::getLocalPlayer().id);
			if (netGamePlayer) {
				auto playerInfo = netGamePlayer->playerInfo;
				if (playerInfo) {
					menu::getPresence()->invitePlayer(vars.currentFriend.first.rid, playerInfo->netInfo.rockstarId2);
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
					menu::getPresence()->invitePlayer(playerInfo->netInfo.rockstarId2, vars.currentFriend.first.rid);
				}
			}
			}));
#if defined(DEV) || defined(TESTER)
	addOption(breakOption("Dev"));

	addOption(buttonOption("Kick Player")
		.addTranslation()
		.addOnClick([] { menu::getPresence()->kickPlayer(vars.currentFriend.first.rid); }));

	addOption(buttonOption("Force Join")
		.addOnClick([] { menu::getPresence()->joinMe(vars.currentFriend.first.rid); }));
#endif // DEV

	/*addOption(submenuOption("Message")
		.addTranslation()
		.addOnClick([] {
			networkMessageMenuVars::vars.submenuIdentifier = 3;
			networkMessageMenuVars::vars.rockstarID = vars.currentFriend.first.rid;
			})
		.addSubmenu<networkMessageMenu>());*/

}

/*Called while in submenu*/
void networkSelectedFriendMenu::update() {
	auto player = vars.currentFriend;
	menu::getPanelManager()->panelFriend(player.second.second, std::to_string(player.first.rid), player.second.first, player.first.status == 0x06);
}

/*Called once on submenu open*/
void networkSelectedFriendMenu::updateOnce() {
	setName(vars.currentFriend.second.second);
}

/*Called always*/
void networkSelectedFriendMenu::featureUpdate() {}

networkSelectedFriendMenu* _instance;
networkSelectedFriendMenu* networkSelectedFriendMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkSelectedFriendMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkSelectedFriendMenu::~networkSelectedFriendMenu() { delete _instance; }