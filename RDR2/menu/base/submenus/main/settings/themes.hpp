#pragma once
#include "menu/base/submenu.hpp"

class themeMenu : public menu::submenu {
public:
	static themeMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	themeMenu()
		: menu::submenu()
	{}

	~themeMenu();
};

namespace themeMenuVars {
	struct vars_ {
		std::vector<std::pair<std::string, struct stat>> cachedThemes;
	};

	extern vars_ vars;
}