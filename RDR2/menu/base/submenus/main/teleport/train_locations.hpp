#pragma once
#include "menu/base/submenu.hpp"

class trainLocationsMenu : public menu::submenu {
public:
	static trainLocationsMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	trainLocationsMenu()
		: menu::submenu()
	{}

	~trainLocationsMenu();
};

namespace trainLocationsMenuVars {
	struct vars_ {

	};

	extern vars_ vars;
}