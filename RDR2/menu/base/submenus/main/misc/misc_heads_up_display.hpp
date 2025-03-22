#pragma once
#include "menu/base/submenu.hpp"

class miscHeadsUpDisplayMenu : public menu::submenu {
public:
	static miscHeadsUpDisplayMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	miscHeadsUpDisplayMenu()
		: menu::submenu()
	{}

	~miscHeadsUpDisplayMenu();
};

namespace miscHeadsUpDisplayMenuVars {
	struct vars_ {
		bool disableMinimap;
		bool disableHUD;
	};

	extern vars_ vars;
}