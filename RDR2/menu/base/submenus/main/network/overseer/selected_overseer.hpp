#pragma once
#include "menu/base/submenu.hpp"

class networkSelectedOverseerMenu : public menu::submenu {
public:
	static networkSelectedOverseerMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkSelectedOverseerMenu()
		: menu::submenu()
	{}

	~networkSelectedOverseerMenu();
};

namespace networkSelectedOverseerMenuVars {
	struct vars_ {
		bool enabled;
		bool notify;
		bool crash;
		bool explodeLoop;
		bool paralyse;
		bool blockJoin;
		bool blacklist;
		std::size_t selectedPlayer;
	};

	extern vars_ vars;
}