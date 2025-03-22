#pragma once
#include "menu/base/submenu.hpp"
#include "world_advanced_ui_struct.hpp"

class WorldAdvancedUIVehicleMenu : public Menu::Submenu {
public:
	static WorldAdvancedUIVehicleMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WorldAdvancedUIVehicleMenu()
		: Menu::Submenu() {}

	~WorldAdvancedUIVehicleMenu();
};

namespace WorldAdvancedUIVehicleMenuVars {
	struct Vars_ {
		WorldAdvancedUIStructMenuVars::CustomColor m_InteriorLightColor;
		WorldAdvancedUIStructMenuVars::CustomColor m_PlateLightColor;
		WorldAdvancedUIStructMenuVars::CustomColor m_DashLightColor;
		WorldAdvancedUIStructMenuVars::CustomColor m_DoorLightColor;
	};

	extern Vars_ m_Vars;
}