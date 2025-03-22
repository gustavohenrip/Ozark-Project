#pragma once
#include "menu/base/submenu.hpp"

class shopLocationsMenu : public menu::submenu {
public:
	static shopLocationsMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	shopLocationsMenu()
		: menu::submenu()
	{}

	~shopLocationsMenu();
};

namespace shopLocationsMenuVars {
	struct vars_ {
		int saintDennis;
		int valentine;
		int blackwater;
		int armadillo;
		int tumbleweed;
	};

	extern vars_ vars;
}