#pragma once
#include "menu/base/submenu.hpp"

class networkRoleBoosterMenu : public menu::submenu {
public:
	static networkRoleBoosterMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkRoleBoosterMenu()
		: menu::submenu()
	{}

	~networkRoleBoosterMenu();
};

namespace networkRoleBoosterMenuVars {
	struct vars_ {
		bool traderBooster;
		bool moonshinerBooster;
		bool unlockAllMaxRoleAwards;
		bool infiniteTreasureMaps;
		int originalRoleRank[6];
	};

	extern vars_ vars;
}