#pragma once
#include "menu/base/submenu.hpp"

class nearbyMenu : public menu::submenu {
public:
	static nearbyMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	nearbyMenu()
		: menu::submenu()
	{}

	~nearbyMenu();
};

namespace nearbyMenuVars {
	struct vars_ {

	};

	extern vars_ vars;
}