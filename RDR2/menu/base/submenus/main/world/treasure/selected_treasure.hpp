#pragma once
#include "menu/base/submenu.hpp"

class selectedTreasureMenu : public menu::submenu {
public:
	static selectedTreasureMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	selectedTreasureMenu()
		: menu::submenu()
	{}

	~selectedTreasureMenu();
};

namespace selectedTreasureMenuVars {
	struct vars_ {
		Object selectedChest;
	};

	extern vars_ vars;
}