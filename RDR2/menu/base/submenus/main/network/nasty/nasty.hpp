#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/util/player_manager.hpp"

class networkNastyMenu : public menu::submenu {
public:
	static networkNastyMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkNastyMenu()
		: menu::submenu()
	{}

	~networkNastyMenu();
};

namespace networkNastyMenuVars {
	struct vars_ {
		int explodeWithExplosives;
	};

	void explode(menu::playerVars& player);
	void kill(menu::playerVars& player);
	void airstrike(menu::playerVars& player);
	void forcefield(menu::playerVars& player);
	void setOnFire(menu::playerVars& player);
	void hostilePeds(menu::playerVars& player);
	void smite(menu::playerVars& player);
	void hostileClone(menu::playerVars& player);
	void killLobby(menu::playerVars& player);
	void fakeMoney(menu::playerVars& player);
	void nearbyExplosions(menu::playerVars& player);
	void bulletBarrage(menu::playerVars& player);
	extern vars_ vars;
}