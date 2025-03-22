#pragma once
#include "menu/base/submenu.hpp"

class WorldAdvancedUIVehicleNeonsMenu : public Menu::Submenu {
public:
	static WorldAdvancedUIVehicleNeonsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WorldAdvancedUIVehicleNeonsMenu()
		: Menu::Submenu() {}

	~WorldAdvancedUIVehicleNeonsMenu();
};

namespace WorldAdvancedUIVehicleNeonsMenuVars {
	struct Vars_ {
		float m_Intensity;
		float m_Radius;
		float m_FalloffExponent;
		float m_CapsuleExtentSides;
		float m_CapsuleExtentFrontBack;
		float m_ClipPlaneHeight;
		float m_BikeClipPlaneHeight;
	};

	extern Vars_ m_Vars;
}