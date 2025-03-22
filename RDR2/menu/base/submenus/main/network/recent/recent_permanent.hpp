#pragma once
#include "menu/base/submenu.hpp"

class networkRecentPlayerPermanentMenu : public menu::submenu {
public:
	static networkRecentPlayerPermanentMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkRecentPlayerPermanentMenu()
		: menu::submenu()
	{}

	~networkRecentPlayerPermanentMenu();
};

namespace networkRecentPlayerPermanentMenuVars {
	struct vars_ {
		std::vector<uint64_t> queue;
	};

	extern vars_ vars;
}