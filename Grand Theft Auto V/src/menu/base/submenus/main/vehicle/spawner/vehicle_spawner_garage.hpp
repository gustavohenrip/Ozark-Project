#pragma once
#include "menu/base/submenu.hpp"

class VehicleSpawnerGarageMenu : public Menu::Submenu {
public:
	static VehicleSpawnerGarageMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleSpawnerGarageMenu()
		: Menu::Submenu() {}

	~VehicleSpawnerGarageMenu();
};

namespace VehicleSpawnerGarageMenuVars {
	struct Vars_ {
		std::vector<int> m_ValidSlots[25];
	};

	const char* GetGarageName(int ID);
	extern Vars_ m_Vars;
}