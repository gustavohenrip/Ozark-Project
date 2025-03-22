#pragma once
#include "menu/base/submenu.hpp"

class networkAttackMenu : public menu::submenu {
public:
	static networkAttackMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkAttackMenu()
		: menu::submenu()
	{}

	~networkAttackMenu();
};

namespace networkAttackMenuVars {
	struct vars_ {
		bool allPlayers;
		bool spawnInvincible;
	};

	extern vars_ vars;
}