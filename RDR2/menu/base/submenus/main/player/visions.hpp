#pragma once
#include "menu/base/submenu.hpp"

class visionMenu : public menu::submenu {
public:
	static visionMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	visionMenu()
		: menu::submenu()
	{}

	~visionMenu();
};

namespace visionMenuVars {
	struct vars_ {
		int vision;
	};

	extern vars_ vars;
}