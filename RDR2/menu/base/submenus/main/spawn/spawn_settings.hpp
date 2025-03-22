#pragma once
#include "menu/base/submenu.hpp"

class spawnSettingsMenu : public menu::submenu {
public:
	static spawnSettingsMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	spawnSettingsMenu()
		: menu::submenu()
	{}

	~spawnSettingsMenu();
};

namespace spawnSettingsMenuVars {
	struct vars_ {
		bool pedSpawnWithBlip;
		bool pedSpawnSedated;
		bool pedSpawnDead;
		bool pedSpawnWithParticle;
		bool pedSpawnWithInvincibility;
		bool pedSpawnWithCreationOverlay;
		bool pedSpawnFrozen;
		bool pedAddToManager = true;

		bool vehicleSpawnWithBlip;
		bool vehicleSpawnWithParticle;
		bool vehicleSpawnWithInvincibility;
		bool vehicleSpawnWithCreationOverlay;
		bool vehicleSpawnFrozen;
		bool vehicleSpawnInsideVehicle;
		bool vehicleDeleteCurrentVehicle;
		bool vehicleAddToManager = true;
	};

	extern vars_ vars;
}