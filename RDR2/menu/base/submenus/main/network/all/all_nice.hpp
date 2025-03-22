#pragma once
#include "menu/base/submenu.hpp"

class networkAllNiceMenu : public menu::submenu {
public:
	static networkAllNiceMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkAllNiceMenu()
		: menu::submenu()
	{}

	~networkAllNiceMenu();
};

namespace networkAllNiceMenuVars {
	struct vars_ {
		bool explosiveAmmo;
		int explosiveType;
	};

	extern vars_ vars;
}