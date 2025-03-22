#pragma once
#include "menu/base/submenu.hpp"

class VehicleModsMovementParachuteMenu : public Menu::Submenu {
public:
	static VehicleModsMovementParachuteMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleModsMovementParachuteMenu()
		: Menu::Submenu() {}

	~VehicleModsMovementParachuteMenu();
};

namespace VehicleModsMovementParachuteMenuVars {
	struct Vars_ {
		bool m_AutoDeploy;
	};

	extern Vars_ m_Vars;
}