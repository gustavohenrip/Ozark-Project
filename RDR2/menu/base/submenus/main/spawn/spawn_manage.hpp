#pragma once
#include "menu/base/submenu.hpp"

class spawnManageMenu : public menu::submenu {
public:
	static spawnManageMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	spawnManageMenu()
		: menu::submenu()
	{}

	~spawnManageMenu();
};

namespace spawnManageMenuVars {
	struct vars_ {};

	extern vars_ vars;
}