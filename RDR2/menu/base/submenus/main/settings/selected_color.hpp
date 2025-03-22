#pragma once
#include "menu/base/submenu.hpp"

class selectedColorMenu : public menu::submenu {
public:
	static selectedColorMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	selectedColorMenu()
		: menu::submenu()
	{}

	~selectedColorMenu();
};

namespace selectedColorMenuVars {
	struct vars_ {
		color tmp;
		color* currentColor;
		std::string colorName;
	};

	extern vars_ vars;
}