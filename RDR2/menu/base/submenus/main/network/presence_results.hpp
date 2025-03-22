#pragma once
#include "menu/base/submenu.hpp"

class networkPresenceSeachResultsMenu : public menu::submenu {
public:
	static networkPresenceSeachResultsMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkPresenceSeachResultsMenu()
		: menu::submenu()
	{}

	~networkPresenceSeachResultsMenu();
};

namespace networkPresenceSeachResultsMenuVars {
	struct vars_ {
		bool loading = false;
		std::vector<std::pair<std::string, uint64_t>> searchList;
	};

	extern vars_ vars;
}