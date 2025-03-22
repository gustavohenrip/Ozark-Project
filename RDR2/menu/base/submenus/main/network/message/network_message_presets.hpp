#pragma once
#include "menu/base/submenu.hpp"

class networkMessagePresetsMenu : public menu::submenu {
public:
	static networkMessagePresetsMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkMessagePresetsMenu()
		: menu::submenu()
	{}

	~networkMessagePresetsMenu();
};

namespace networkMessagePresetsMenuVars {
	struct vars_ {
		
	};

	extern vars_ vars;
}