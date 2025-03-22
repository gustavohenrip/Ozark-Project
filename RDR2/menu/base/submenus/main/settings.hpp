#pragma once
#include "menu/base/submenu.hpp"

class settingsMenu : public menu::submenu {
public:
	static settingsMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	settingsMenu()
		: menu::submenu()
	{}

	~settingsMenu();
};

namespace settingsMenuVars {
	struct vars_ {
		std::shared_ptr<buttonOption> timeButton;
	};

	extern vars_ vars;
}