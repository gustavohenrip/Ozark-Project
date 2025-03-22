#pragma once
#include "menu/base/submenu.hpp"

class spawnPlantsMenu : public menu::submenu {
public:
	static spawnPlantsMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	spawnPlantsMenu()
		: menu::submenu()
	{}

	~spawnPlantsMenu();
};

namespace spawnPlantsMenuVars {
	struct vars_ {

	};
	extern vars_ vars;
}