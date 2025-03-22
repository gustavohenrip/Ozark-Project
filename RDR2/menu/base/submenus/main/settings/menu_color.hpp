#pragma once
#include "menu/base/submenu.hpp"

class colorMenu : public menu::submenu {
public:
	static colorMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	colorMenu()
		: menu::submenu()
	{}

	~colorMenu();
};

namespace colorMenuVars {
	struct vars_ {
		//color titleText = { 255, 255, 255, 255 };
		//color menuHeaderText = { 255, 255, 255, 255 };
		//color openToolTip = { 74, 127, 184, 130 };
		//color mainText = { 255, 255, 255, 255 };
		//color breakText = { 255, 255, 255, 255 };
		//color selectedText = { 0, 0, 0, 255 };
		//color toggleOn = { 130, 214, 157, 255 };
		//color toggleOff = { 200, 55, 80, 255 };
		//color submenuIndicator = { 74, 127, 184, 150 };
		//color titleHeader = { 74, 127, 184, 200 };
		//color menuHeader = { 0, 0, 0, 200 };
		//color scroller = { 255, 255, 255, 255 };
		//color background = { 0, 0, 0, 165 };
		//color panelBackground;
	};

	extern vars_ vars;
}