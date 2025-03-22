#pragma once
#include "menu/base/submenu.hpp"

class weaponMenu : public menu::submenu {
public:
	static weaponMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	weaponMenu()
		: menu::submenu()
	{}

	~weaponMenu();
};

namespace weaponMenuVars {
	struct vars_ {

	};

	extern vars_ vars;
}