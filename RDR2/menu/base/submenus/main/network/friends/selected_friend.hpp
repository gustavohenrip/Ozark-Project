#pragma once
#include "menu/base/submenu.hpp"

class networkSelectedFriendMenu : public menu::submenu {
public:
	static networkSelectedFriendMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkSelectedFriendMenu()
		: menu::submenu()
	{}

	~networkSelectedFriendMenu();
};

namespace networkSelectedFriendMenuVars {

	struct vars_ {
		//std::pair<friendInfo, std::string> currentFriend;
		std::pair<friendInfo, std::pair<networkHandle, std::string>> currentFriend;
	};

	extern vars_ vars;
}