#pragma once
#include "menu/base/submenu.hpp"

class networkTeleportMenu : public menu::submenu {
public:
	static networkTeleportMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkTeleportMenu()
		: menu::submenu()
	{}

	~networkTeleportMenu();
};

namespace networkTeleportMenuVars {
	struct vars_ {

	};
	extern vars_ vars;
}