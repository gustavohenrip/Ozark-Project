#pragma once
#include "menu/base/submenu.hpp"

class particleMenu : public menu::submenu {
public:
	static particleMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	particleMenu()
		: menu::submenu()
	{}

	~particleMenu();
};

namespace particleMenuVars {
	struct vars_ {
		bool overrideScale;
		float scale = 2.f;
		int loopedTimeout = 10;
	};

	extern vars_ vars;
}