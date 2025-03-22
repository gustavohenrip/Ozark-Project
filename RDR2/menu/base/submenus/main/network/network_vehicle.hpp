#pragma once
#include "menu/base/submenu.hpp"

class networkVehicleMenu : public menu::submenu {
public:
	static networkVehicleMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkVehicleMenu()
		: menu::submenu()
	{}

	~networkVehicleMenu();
};

namespace networkVehicleMenuVars {
	struct vars_ {

	};

	extern vars_ vars;
}