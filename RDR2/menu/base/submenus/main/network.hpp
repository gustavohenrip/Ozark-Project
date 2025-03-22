#pragma once
#include "menu/base/submenu.hpp"

class networkMenu : public menu::submenu {
public:
	static networkMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkMenu()
		: menu::submenu()
	{}

	~networkMenu();
};

namespace networkMenuVars {
	struct vars_ {
		int currentSessionType;
		std::string sessionTooltip;
	};

	extern vars_ vars;
}