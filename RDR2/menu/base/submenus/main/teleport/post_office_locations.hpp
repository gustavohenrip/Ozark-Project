#pragma once
#include "menu/base/submenu.hpp"

class postOfficeLocationsMenu : public menu::submenu {
public:
	static postOfficeLocationsMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	postOfficeLocationsMenu()
		: menu::submenu()
	{}

	~postOfficeLocationsMenu();
};

namespace postOfficeLocationsMenuVars {
	struct vars_ {

	};

	extern vars_ vars;
}