#pragma once
#include "menu/base/submenu.hpp"

class networkSelectedRecentPlayerMenu : public menu::submenu {
public:
	static networkSelectedRecentPlayerMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkSelectedRecentPlayerMenu()
		: menu::submenu()
	{}

	~networkSelectedRecentPlayerMenu();
};

namespace networkSelectedRecentPlayerMenuVars {
	struct vars_ {
		int type; // 1 = recent, 2 = perma
		std::size_t selectedPlayer;
	};

	extern vars_ vars;
}