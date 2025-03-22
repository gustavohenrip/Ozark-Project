#pragma once
#include "menu/base/submenu.hpp"

class aimbotMenu : public menu::submenu {
public:
	static aimbotMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	aimbotMenu()
		: menu::submenu()
	{}

	~aimbotMenu();
};

namespace aimbotMenuVars {
	struct vars_ {
		bool enable;
		bool paintTarget;
		bool aimingRequired;
		bool autoFire;
		int ms = 250;
		bool autoAim;
		int filter = 3;
		int type = 1;
		int tag;
		int condition;
	};

	extern scrollStruct<int> aimTag[9];
	extern scrollStruct<int> aimFilter[4];
	extern vars_ vars;
}