#pragma once
#include "menu/base/submenu.hpp"

class popularLocationsMenu : public menu::submenu {
public:
	static popularLocationsMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	popularLocationsMenu()
		: menu::submenu()
	{}

	~popularLocationsMenu();
};

namespace popularLocationsMenuVars {
	struct vars_ {

	};

	extern vars_ vars;
}