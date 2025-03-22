#pragma once
#include "menu/base/submenu.hpp"

class vehiclesMenu : public menu::submenu {
public:
	static vehiclesMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	vehiclesMenu()
		: menu::submenu()
	{}

	~vehiclesMenu();
};

namespace vehiclesMenuVars {
	struct vars_ {
		int teleportIndex;
		bool esp = true;
	};

	void esp();
	extern vars_ vars;
}