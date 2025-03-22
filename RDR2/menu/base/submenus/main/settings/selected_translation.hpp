#pragma once
#include "menu/base/submenu.hpp"

class selectedTranslationMenu : public menu::submenu {
public:
	static selectedTranslationMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	selectedTranslationMenu()
		: menu::submenu()
	{}

	~selectedTranslationMenu();
};

namespace selectedTranslationMenuVars {
	struct vars_ {
		int selectedTranslation;
	};

	extern vars_ vars;
}