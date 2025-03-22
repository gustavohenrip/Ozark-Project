#pragma once
#include "menu/base/submenu.hpp"

class weaponTriggerBotMenu : public menu::submenu {
public:
	static weaponTriggerBotMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	weaponTriggerBotMenu()
		: menu::submenu()
	{}

	~weaponTriggerBotMenu();
};

namespace weaponTriggerBotMenuVars {
	struct vars_ {
		bool triggerBot;
		int boneIndex;
		int ms = 360;
		int filter = 3;
	};

	extern vars_ vars;
}