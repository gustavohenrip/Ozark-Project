#pragma once
#include "menu/base/submenu.hpp"

class positionMenu : public menu::submenu {
public:
	static positionMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	positionMenu()
		: menu::submenu()
	{}

	~positionMenu();
};

namespace positionMenuVars {
	struct vars_ {

	};

	extern vars_ vars;
}