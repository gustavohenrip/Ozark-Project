#pragma once
#include "menu/base/submenu.hpp"
#include "src/menu/base/util/player_manager.hpp"
#include "../event_reactions.hpp"

class networkSelectedEventReactionMenu : public menu::submenu {
public:
	static networkSelectedEventReactionMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkSelectedEventReactionMenu()
		: menu::submenu()
	{}

	~networkSelectedEventReactionMenu();
};

namespace networkSelectedEventReactionMenuVars {
	struct vars_ {
		networkEventReactionsMenuVars::eventReactions tmp;
		networkEventReactionsMenuVars::eventReactions* reaction;
		int previous;
	};

	extern vars_ vars;
}