#pragma once
#include "menu/base/submenu.hpp"

class miniGamesMenu : public menu::submenu {
public:
	static miniGamesMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	miniGamesMenu()
		: menu::submenu()
	{}

	~miniGamesMenu();
};

namespace miniGamesMenuVars {
	struct vars_ {
		bool undeadNightmare;
		bool stopMiniGame;
		int undeadNightmareLevel = 1;
		int undeadNightmareMaxLevel = 6;
		std::vector<Ped> undeadPeds[2];
	};

	extern vars_ vars;
}