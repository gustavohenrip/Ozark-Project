#pragma once
#include "menu/base/submenu.hpp"

class searchResultsMenu : public menu::submenu {
public:
	static searchResultsMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	searchResultsMenu()
		: menu::submenu()
	{}

	~searchResultsMenu();
};

namespace searchResultsMenuVars {
	struct vars_ {

	};

	extern vars_ vars;
}