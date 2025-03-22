#pragma once
#include "menu/base/submenu.hpp"

class fontMenu : public menu::submenu {
public:
	static fontMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	fontMenu()
		: menu::submenu()
	{}

	~fontMenu();
};

namespace fontMenuVars {
	struct vars_ {

	};

	extern vars_ vars;
}