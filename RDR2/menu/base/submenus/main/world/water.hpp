#pragma once
#include "menu/base/submenu.hpp"

class waterMenu : public menu::submenu {
public:
	static waterMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	waterMenu()
		: menu::submenu()
	{}

	~waterMenu();
};

namespace waterMenuVars {
	struct vars_ {

	};

	extern vars_ vars;
}