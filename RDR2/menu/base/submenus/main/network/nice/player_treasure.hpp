#pragma once
#include "menu/base/submenu.hpp"

class networkPlayerTreasureMenu : public menu::submenu {
public:
	static networkPlayerTreasureMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkPlayerTreasureMenu()
		: menu::submenu()
	{}

	~networkPlayerTreasureMenu();
};

namespace networkPlayerTreasureMenuVars {
	struct vars_ {
		bool allPlayers;
		int chestCount = 1;
		int barCount = 1;
		int chestType;
	};

	extern vars_ vars;
}