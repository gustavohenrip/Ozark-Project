#pragma once
#include "menu/base/submenu.hpp"

class networkAllNastyMenu : public menu::submenu {
public:
	static networkAllNastyMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkAllNastyMenu()
		: menu::submenu()
	{}

	~networkAllNastyMenu();
};

namespace networkAllNastyMenuVars {
	struct vars_ {
		bool fakeMoney;
		uint32_t explosionTimer;
		bool loopedExplode;
		bool paralyse;
		bool bulletBarrage;
		bool nearbyExplosions;
	};

	extern vars_ vars;
}