#pragma once
#include "menu/base/submenu.hpp"
#include "src/menu/base/util/player_manager.hpp"

class networkPlayersMenu : public menu::submenu {
public:
	static networkPlayersMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkPlayersMenu()
		: menu::submenu()
	{}

	~networkPlayersMenu();
};

namespace networkPlayersMenuVars {
	struct vars_ {
		int selectedPlayer;
		int reportIndex;
	};
	void teleportPlayerToWaypoint(menu::playerVars& player);
	extern vars_ vars;
}