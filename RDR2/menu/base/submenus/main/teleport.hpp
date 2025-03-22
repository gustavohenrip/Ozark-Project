#pragma once
#include "menu/base/submenu.hpp"

class teleportMenu : public menu::submenu {
public:
	static teleportMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	teleportMenu()
		: menu::submenu()
	{}

	~teleportMenu();
};

namespace teleportMenuVars {
	struct vars_ {

		bool autoWaypoint;
		//math::vector3<float> cached;
		int closest;
		bool hasTeleportedFlag;
		uint32_t waypointTimer;
	};
	Player getClosestPlayer();
	void teleportToLocation(math::vector3<float> coords);
	extern vars_ vars;
}