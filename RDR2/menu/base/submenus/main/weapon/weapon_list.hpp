#pragma once
#include "menu/base/submenu.hpp"

class weaponsListMenu : public menu::submenu {
public:
	static weaponsListMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	weaponsListMenu()
		: menu::submenu()
	{}

	~weaponsListMenu();
};

namespace weaponsListMenuVars {
	struct vars_ {

	};


	extern vars_ vars;
}