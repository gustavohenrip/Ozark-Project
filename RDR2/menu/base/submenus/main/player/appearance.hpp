#pragma once
#include "menu/base/submenu.hpp"

class appearanceMenu : public menu::submenu {
public:
	static appearanceMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	appearanceMenu()
		: menu::submenu()
	{}

	~appearanceMenu();
};

namespace appearanceMenuVars {
	struct vars_ {
		float scale = 1.f;
		int gender;
	};

	extern vars_ vars;
}