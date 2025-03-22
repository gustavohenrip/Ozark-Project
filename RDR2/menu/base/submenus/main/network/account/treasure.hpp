#pragma once
#include "menu/base/submenu.hpp"

class networkTreasureMenu : public menu::submenu {
public:
	static networkTreasureMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkTreasureMenu()
		: menu::submenu()
	{}

	~networkTreasureMenu();
};

namespace networkTreasureMenuVars {
	struct vars_ {
		int set;
		int chestType;
		int pickupType;
		int chestCount = 1;
		int pickupCount = 1;
		bool infiniteItems;
		bool speedyloot;
		bool autoSellCoinSet;
	};
	void treasure(Ped ped, Hash chestType, Hash pickupType, int chestCount, int barCount);
	extern scrollStruct<Hash> chests[18];
	extern vars_ vars;
}