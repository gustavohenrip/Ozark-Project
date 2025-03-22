#pragma once
#include "menu/base/submenu.hpp"
#include <array>

class VehicleSpawnerMenu : public Menu::Submenu {
public:
	static VehicleSpawnerMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleSpawnerMenu()
		: Menu::Submenu() {}

	~VehicleSpawnerMenu();
};

namespace VehicleSpawnerMenuVars {
	struct Vars_ {
		std::array<std::vector<uint32_t>, 23> m_VehicleHashes;
	};

	void SpawnVehicle(uint32_t ModelHash);
	extern Vars_ m_Vars;
}