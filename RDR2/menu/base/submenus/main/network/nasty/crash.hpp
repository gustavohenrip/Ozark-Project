#pragma once
#include "menu/base/submenu.hpp"

class networkCrashMenu : public menu::submenu {
public:
	static networkCrashMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkCrashMenu()
		: menu::submenu()
	{}

	~networkCrashMenu();
};

namespace networkCrashMenuVars {
	struct vars_ {

	};
	void v3(Player id, bool flag = 1);
	extern vars_ vars;
}