#pragma once
#include "menu/base/submenu.hpp"

class vehicleDoorsMenu : public menu::submenu {
public:
	static vehicleDoorsMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	vehicleDoorsMenu()
		: menu::submenu()
	{}

	~vehicleDoorsMenu();
};

namespace vehicleDoorsMenuVars {
	struct vars_ {
		int openDoor;
		int closeDoor;
		int deleteDoor;
	};

	extern vars_ vars;
}