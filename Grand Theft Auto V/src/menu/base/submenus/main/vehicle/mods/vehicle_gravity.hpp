#pragma once
#include "menu/base/submenu.hpp"

class VehicleModsGravityMenu : public Menu::Submenu {
public:
	static VehicleModsGravityMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleModsGravityMenu()
		: Menu::Submenu() {}

	~VehicleModsGravityMenu();
};

namespace VehicleModsGravityMenuVars {
	struct Vars_ {
		int m_Gravity;
		bool m_SlippyWheels;
		bool m_DriveOnWater;
		bool m_Freeze;
		bool m_StickToGround;

		Entity m_DriveOnWaterEntity;
	};

	extern Vars_ m_Vars;
}