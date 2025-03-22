#pragma once
#include "menu/base/submenu.hpp"

class visualMenu : public menu::submenu {
public:
	static visualMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	visualMenu()
		: menu::submenu()
	{}

	~visualMenu();
};

namespace visualMenuVars {
	struct vars_ {
		bool invisibility;
		bool crosshair;
		bool weaponOpacity;
		int weaponOpacityVar = 100;
	};

	extern vars_ vars;
}