#pragma once
#include "menu/base/submenu.hpp"

class vehicleWeaponsMenu : public menu::submenu {
public:
	static vehicleWeaponsMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	vehicleWeaponsMenu()
		: menu::submenu()
	{}

	~vehicleWeaponsMenu();
};

namespace vehicleWeaponsMenuVars {
	struct vars_ {
		bool enabled;
		bool takeTheBlame;
		bool guidelines;
		int weapon;
		float speed = 10000.f;
	};

	extern vars_ vars;
}