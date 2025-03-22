#pragma once
#include "stdafx.hpp"

struct color {
	int r;
	int g;
	int b;
	int a;

	color()
		: r(0), g(0), b(0), a(0)
	{}

	color(int r, int g, int b, int a = 255)
		: r(r), g(g), b(b), a(a)
	{}
};

namespace global::uivars {
	extern color titleText;
	extern color menuHeaderText;
	extern color openToolTip;
	extern color mainText;
	extern color breakText;
	extern color selectedText;
	extern color toggleOn;
	extern color toggleOff;
	extern color submenuIndicator;
	extern color titleHeader;
	extern color menuHeader;
	extern color menuFooter;
	extern color menuFooterArrows;
	extern color scroller;
	extern color background;
	extern color panelBackground;
	extern int titleFont;
	extern int headerFont;
	extern int optionFont;
	extern int breakFont;
	extern float menuX;
	extern float menuY;
}