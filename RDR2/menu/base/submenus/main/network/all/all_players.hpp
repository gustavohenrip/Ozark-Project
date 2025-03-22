#pragma once
#include "menu/base/submenu.hpp"

class networkAllPlayersMenu : public menu::submenu {
public:
	static networkAllPlayersMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkAllPlayersMenu()
		: menu::submenu()
	{}

	~networkAllPlayersMenu();
};

namespace networkAllPlayersMenuVars {
	struct vars_ {
		bool excludeFriends;
		bool otherPlayers;
		bool me = true;
		bool excludeOzarkUsers = true;
		bool showJoinLeave = true;
		bool revealPlayers;
		std::unordered_map<rage::network::netPlayerInfo*, Blip> playerBlip;
	};

	extern vars_ vars;
}