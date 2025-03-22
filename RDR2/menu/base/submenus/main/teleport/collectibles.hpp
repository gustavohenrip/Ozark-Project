#pragma once
#include "menu/base/submenu.hpp"

class collectiblesMenu : public menu::submenu {
public:
	static collectiblesMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	collectiblesMenu()
		: menu::submenu()
	{}

	~collectiblesMenu();
};

namespace collectiblesMenuVars {
	struct vars_ {
		int cycle = 1;
		int cups;
		int heirlooms;
		int wands;
		int swords;
		int pentacles;
	};

	extern vars_ vars;
}