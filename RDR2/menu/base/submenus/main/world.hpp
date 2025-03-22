#pragma once
#include "menu/base/submenu.hpp"

class worldMenu : public menu::submenu {
public:
	static worldMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	worldMenu()
		: menu::submenu()
	{}

	~worldMenu();
};

namespace worldMenuVars {
	struct vars_ {
		bool giantMode;
		bool groundSnow;
		bool disableGravity;
		bool bypassRestrictedAreas;
		uint32_t gravityTimer;
	};

	extern vars_ vars;
}