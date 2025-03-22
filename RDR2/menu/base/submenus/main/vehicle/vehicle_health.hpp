#pragma once
#include "menu/base/submenu.hpp"

class vehicleHealthMenu : public menu::submenu {
public:
	static vehicleHealthMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	vehicleHealthMenu()
		: menu::submenu()
	{}

	~vehicleHealthMenu();
};

namespace vehicleHealthMenuVars {
	struct vars_ {
		bool autoRepair;
	};

	extern vars_ vars;
}