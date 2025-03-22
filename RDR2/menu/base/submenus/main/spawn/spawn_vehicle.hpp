#pragma once
#include "menu/base/submenu.hpp"

class spawnVehicleMenu : public menu::submenu {
public:
	static spawnVehicleMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	spawnVehicleMenu()
		: menu::submenu()
	{}

	~spawnVehicleMenu();
};

namespace spawnVehicleMenuVars {
	struct spawnedVehicleData {
		char name[60];
		Hash hash;
		Vehicle handle;
		int creationTime;
		bool invincibility;
		bool creationOverlay;
		bool frozen;
		math::vector3<float> origin;
	};

	struct vars_ {
		int trains;
		int wagons;
		int boats;
		int stagecoaches;
		int carts;
		int special;

		std::vector<spawnedVehicleData> spawnedVehicles;
	};

	void spawnVehicle(std::string visibleName, Hash hash);
	extern vars_ vars;
}