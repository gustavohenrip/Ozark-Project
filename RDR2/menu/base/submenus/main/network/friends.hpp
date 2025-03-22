#pragma once
#include "menu/base/submenu.hpp"

class networkFriendsMenu : public menu::submenu {
public:
	static networkFriendsMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkFriendsMenu()
		: menu::submenu()
	{}

	~networkFriendsMenu();
};

namespace networkFriendsMenuVars {

	struct vars_ {
		std::vector<std::pair<friendInfo, std::pair<networkHandle, std::string>>> friends;
	};

	extern vars_ vars;
}