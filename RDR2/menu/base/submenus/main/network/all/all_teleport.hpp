#pragma once
#include "menu/base/util/player_manager.hpp"
#include "menu/base/submenu.hpp"

class networkAllTeleportMenu : public menu::submenu {
public:
	static networkAllTeleportMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkAllTeleportMenu()
		: menu::submenu()
	{}

	~networkAllTeleportMenu();
};

namespace networkAllTeleportMenuVars {
	struct vars_ {
		int listSize;
		int player;
		scrollStruct<int> playerList[32];
	};

	extern vars_ vars;
}