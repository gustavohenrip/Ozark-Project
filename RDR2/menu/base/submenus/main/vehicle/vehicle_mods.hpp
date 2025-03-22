#pragma once
#include "menu/base/submenu.hpp"

class vehicleModsMenu : public menu::submenu {
public:
	static vehicleModsMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	vehicleModsMenu()
		: menu::submenu()
	{}

	~vehicleModsMenu();
};

namespace vehicleModsMenuVars {
	struct vars_ {
		bool burnedToggle;
		bool invincibility;
		bool invisible;
		bool fastVehicle;
		bool seatbelt;
		bool speedometer;
		std::size_t speedometerId;
		bool noCollision;
		int opacity = 100;
		bool autoFlip;
		bool stickToGround;
	};

	extern vars_ vars;
}