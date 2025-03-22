#pragma once
#include "menu/base/submenu.hpp"

class networkNiceMenu : public menu::submenu {
public:
	static networkNiceMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkNiceMenu()
		: menu::submenu()
	{}

	~networkNiceMenu();
};

namespace networkNiceMenuVars {
	struct vars_ {

	};

	//void antiAim(menu::playerVars& player, int type);
	extern vars_ vars;
}