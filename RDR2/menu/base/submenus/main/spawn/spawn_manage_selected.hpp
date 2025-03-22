#pragma once
#include "menu/base/submenu.hpp"
#include "spawn_ped.hpp"
#include "spawn_vehicle.hpp"

class spawnManageSelectedMenu : public menu::submenu {
public:
	static spawnManageSelectedMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	spawnManageSelectedMenu()
		: menu::submenu()
	{}

	~spawnManageSelectedMenu();
};

namespace spawnManageSelectedMenuVars {
	struct vars_ {
		int type; // 1 = ped, 2 = veh, 3 = obj
		Entity handle;
		int weapon;
		float* scale;
		bool* creationOverlay;
		bool* invincibility;
		int attachVal;
		int listSize;
		scrollStruct<int> attachList[32];

		spawnPedMenuVars::spawnedPedData* pedData;
		spawnVehicleMenuVars::spawnedVehicleData* vehicleData;
	};

	extern vars_ vars;
}