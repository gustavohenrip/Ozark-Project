#pragma once
#include "menu/base/submenu.hpp"

class mainMenu : public menu::submenu {
public:
	static mainMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	mainMenu()
		: menu::submenu()
	{}

	~mainMenu();
};

namespace mainMenuVars {
	struct vars_ {
	};

	extern vars_ vars;
}