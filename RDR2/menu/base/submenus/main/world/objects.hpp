#pragma once
#include "menu/base/submenu.hpp"

class objectMenu : public menu::submenu {
public:
	static objectMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	objectMenu()
		: menu::submenu()
	{}

	~objectMenu();
};

namespace objectMenuVars {
	struct vars_ {
		bool esp;
		int teleportIndex;
		int listSize;
		scrollStruct<int> teleportPlayers[32];
	};

	void esp();
	extern vars_ vars;
}