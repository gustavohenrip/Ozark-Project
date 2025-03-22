#pragma once
#include "menu/base/submenu.hpp"

class spawnMenu : public menu::submenu {
public:
	static spawnMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	spawnMenu()
		: menu::submenu()
	{}

	~spawnMenu();
};

namespace spawnMenuVars {
	struct vars_ {

	};

	extern vars_ vars;
}