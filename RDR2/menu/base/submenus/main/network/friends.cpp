#include "friends.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network.hpp"
#include "friends/selected_friend.hpp"
#include "menu/base/util/panel_manager.hpp"

using namespace networkFriendsMenuVars;

namespace networkFriendsMenuVars {
	vars_ vars;

	//void initializeFriendList() {
	//	vars.friends.clear();
	//	vars.friends.resize(native::getFriendCount());
	//	static int count = 0;
	//	std::for_each(vars.friends.begin(), vars.friends.end(),
	//		[&](std::pair<networkHandle, std::string>& data) {
	//			networkHandle handle;
	//			native::getNetHandleFromFriendIndex(count++, &handle.handle[0]);
	//			data = std::make_pair(handle, std::string(native::getFriendNameFromNetHandle(&handle.handle[0])));
	//		});
	//	count = 0;
	//}

	void initializeFriendList() {
		vars.friends.clear();
		vars.friends.resize(native::getFriendCount());
		static int count = 0;
		std::for_each(vars.friends.begin(), vars.friends.end(),
			[&](std::pair<friendInfo, std::pair<networkHandle, std::string>>& data) {
				uint64_t ptr = *(uint64_t*)(global::vars::g_friendData + (count * 8));
				if (ptr) {
					friendInfo fr = *(friendInfo*)ptr;
					networkHandle netHandle;
					native::getNetHandleFromFriendIndex(count++, &netHandle.handle[0]);
					data = std::make_pair(fr, std::make_pair(netHandle, std::string(fr.name)));
				}
			});
		count = 0;
	}

}

void networkFriendsMenu::init() {
	setName("Friends");
	setParentSubmenu<networkMenu>();

}

/*Called while in submenu*/
void networkFriendsMenu::update() {
	auto player = networkSelectedFriendMenuVars::vars.currentFriend;
	menu::getPanelManager()->panelFriend(player.second.second, std::to_string(player.first.rid), player.second.first, player.first.status == 0x06);
}

/*Called once on submenu open*/
void networkFriendsMenu::updateOnce() {

	clearOptionsOffset(0);
	initializeFriendList();
	std::for_each(vars.friends.begin(), vars.friends.end(),
		[&](std::pair<friendInfo, std::pair<networkHandle, std::string>>& data) {
			char name[32];
			snprintf(name, 32, _i<BOOL>(0xE348D1404BD80146, &data.second.first.handle[0]) ? "%s" : "~m~%s", data.second.second.c_str());
			addOption(submenuOption(std::string(name))
				.addTranslation()
				.addSubmenu<networkSelectedFriendMenu>()
				.addOnHover([=] { networkSelectedFriendMenuVars::vars.currentFriend = data; }));
		});
}

/*Called always*/
void networkFriendsMenu::featureUpdate() {}

networkFriendsMenu* _instance;
networkFriendsMenu* networkFriendsMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkFriendsMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkFriendsMenu::~networkFriendsMenu() { delete _instance; }