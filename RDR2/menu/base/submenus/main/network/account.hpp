#pragma once
#include "menu/base/submenu.hpp"

class networkAccountMenu : public menu::submenu {
public:
	static networkAccountMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkAccountMenu()
		: menu::submenu()
	{}

	~networkAccountMenu();
};

namespace networkAccountMenuVars {
	struct vars_ {

	};

	extern vars_ vars;
}