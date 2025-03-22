#pragma once
#include "menu/base/submenu.hpp"

class weaponsListFullMenu : public menu::submenu {
public:
	static weaponsListFullMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	weaponsListFullMenu()
		: menu::submenu()
	{}

	~weaponsListFullMenu();
};

namespace weaponsListFullMenuVars {
	struct vars_ {

	};


	extern vars_ vars;
}