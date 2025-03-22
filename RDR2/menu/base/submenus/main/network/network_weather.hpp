#pragma once
#include "menu/base/submenu.hpp"

class networkWeatherMenu : public menu::submenu {
public:
	static networkWeatherMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkWeatherMenu()
		: menu::submenu()
	{}

	~networkWeatherMenu();
};

namespace networkWeatherMenuVars {
	struct vars_ {

	};

	extern vars_ vars;
}