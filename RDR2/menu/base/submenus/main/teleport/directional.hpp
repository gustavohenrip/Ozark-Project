#pragma once
#include "menu/base/submenu.hpp"

class directionalMenu : public menu::submenu {
public:
	static directionalMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	directionalMenu()
		: menu::submenu()
	{}

	~directionalMenu();
};

namespace directionalMenuVars {
	struct vars_ {
		float up = 1.f;
		float down = 1.f;
		float left = 1.f;
		float right = 1.f;
		float forward = 1.f;
		float back = 1.f;
	};

	extern vars_ vars;
}