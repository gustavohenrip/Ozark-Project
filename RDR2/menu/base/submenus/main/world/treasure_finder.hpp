#pragma once
#include "menu/base/submenu.hpp"

class treasureFinderMenu : public menu::submenu {
public:
	static treasureFinderMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	treasureFinderMenu()
		: menu::submenu()
	{}

	~treasureFinderMenu();
};

namespace treasureFinderMenuVars {
	struct vars_ {};

	extern vars_ vars;
}