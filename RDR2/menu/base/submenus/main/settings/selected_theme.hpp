#pragma once
#include "menu/base/submenu.hpp"

class selectedThemeMenu : public menu::submenu {
public:
	static selectedThemeMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	selectedThemeMenu()
		: menu::submenu()
	{}

	~selectedThemeMenu();
};

namespace selectedThemeMenuVars {
	struct vars_ {
		int selectedTheme;
	};

	void loadTheme(const char* name);
	extern vars_ vars;
}