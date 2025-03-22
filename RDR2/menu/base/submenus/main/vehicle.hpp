#pragma once
#include "menu/base/submenu.hpp"

class vehicleMenu : public menu::submenu {
public:
	static vehicleMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	vehicleMenu()
		: menu::submenu()
	{}

	~vehicleMenu();
};

namespace vehicleMenuVars {
	struct vars_ {

	};

	extern vars_ vars;
}