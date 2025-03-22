#pragma once
#include "menu/base/submenu.hpp"
#include "src/menu/base/util/player_manager.hpp"

class networkEventReactionsMenu : public menu::submenu {
public:
	static networkEventReactionsMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkEventReactionsMenu()
		: menu::submenu()
	{}

	~networkEventReactionsMenu();
};

namespace networkEventReactionsMenuVars {
	struct eventReactions {
		bool addToOverseer;
		bool crash;
		bool explode;
	};

	struct vars_ {
		eventReactions onReport;
		eventReactions onAim;
		eventReactions onDeath;
		eventReactions onCashSpawn;
	};

	extern vars_ vars;
}