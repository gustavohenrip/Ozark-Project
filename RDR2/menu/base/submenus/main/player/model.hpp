#pragma once
#include "menu/base/submenu.hpp"

class modelMenu : public menu::submenu {
public:
	static modelMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	modelMenu()
		: menu::submenu()
	{}

	~modelMenu();
};

namespace modelMenuVars {
	struct vars_ {};

	void changeModel(Hash hash);
	extern vars_ vars;
}