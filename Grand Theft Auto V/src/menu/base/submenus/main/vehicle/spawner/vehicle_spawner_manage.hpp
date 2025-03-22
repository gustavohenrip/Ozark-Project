#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/submenus/main/helpers/esp.hpp"

class VehicleSpawnerManageMenu : public Menu::Submenu {
public:
	static VehicleSpawnerManageMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleSpawnerManageMenu()
		: Menu::Submenu() {}

	~VehicleSpawnerManageMenu();
};

namespace VehicleSpawnerManageMenuVars {
	struct SpawnedVehicleContext {
		Vehicle m_Vehicle = 0;
		uint32_t m_ModelHash = 0;
		std::string m_Name;

		int m_CreationTime = 0;
		int m_Gravity = 0;

		ESPMenuVars::ESP m_ESP;
	};

	struct Vars_ {
		std::vector<SpawnedVehicleContext> m_SpawnedVehicles;
	};

	extern Vars_ m_Vars;
}