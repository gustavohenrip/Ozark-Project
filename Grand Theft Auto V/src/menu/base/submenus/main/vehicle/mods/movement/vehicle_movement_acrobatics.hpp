#pragma once
#include "menu/base/submenu.hpp"

class VehicleModsMovementAcrobaticsMenu : public Menu::Submenu {
public:
	static VehicleModsMovementAcrobaticsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleModsMovementAcrobaticsMenu()
		: Menu::Submenu() {}

	~VehicleModsMovementAcrobaticsMenu();
};

namespace VehicleModsMovementAcrobaticsMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}