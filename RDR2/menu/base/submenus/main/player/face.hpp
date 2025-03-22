#pragma once
#include "menu/base/submenu.hpp"

class faceMenu : public menu::submenu {
public:
	static faceMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	faceMenu()
		: menu::submenu()
	{}

	~faceMenu();
};

namespace faceMenuVars {
	struct vars_ {
		int id;
		float value;
	};

	extern vars_ vars;
}