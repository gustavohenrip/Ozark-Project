#pragma once
#include "menu/base/submenu.hpp"

class pickupsMenu : public menu::submenu {
public:
	static pickupsMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	pickupsMenu()
		: menu::submenu()
	{}

	~pickupsMenu();
};

namespace pickupsMenuVars {
	struct vars_ {
		int teleportIndex;
		bool esp = true;
	};

	void esp();
	extern vars_ vars;
}