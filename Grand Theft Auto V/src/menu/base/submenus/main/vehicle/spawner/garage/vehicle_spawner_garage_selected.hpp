#pragma once
#include "menu/base/submenu.hpp"

class VehicleSpawnerGarageSelectedMenu : public Menu::Submenu {
public:
	static VehicleSpawnerGarageSelectedMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleSpawnerGarageSelectedMenu()
		: Menu::Submenu() {}

	~VehicleSpawnerGarageSelectedMenu();
};

namespace VehicleSpawnerGarageSelectedMenuVars {
	struct Vars_ {
		int m_Slot;
	};

	extern Vars_ m_Vars;
}