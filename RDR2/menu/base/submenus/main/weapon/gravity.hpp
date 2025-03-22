#pragma once
#include "menu/base/submenu.hpp"

class gravityMenu : public menu::submenu {
public:
	static gravityMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	gravityMenu()
		: menu::submenu()
	{}

	~gravityMenu();
};

namespace gravityMenuVars {
	struct vars_ {
		float distance = 10.f;
		bool peds = true;
		bool vehicles = true;
		bool objects = false;
		bool pickups = true;
		Entity entity;
		bool gravityGun;
		bool attraction;
		float force = 4.f;
		uint32_t buffer;
	};

	extern vars_ vars;
}