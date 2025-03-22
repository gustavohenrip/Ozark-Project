#pragma once
#include "menu/base/submenu.hpp"

class VehicleModsRampMenu : public Menu::Submenu {
public:
	static VehicleModsRampMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleModsRampMenu()
		: Menu::Submenu() {}

	~VehicleModsRampMenu();
};

namespace VehicleModsRampMenuVars {
	struct Vars_ {
		bool m_Transparency;
		bool m_FrontRamp;
		bool m_RearRamp;
		bool m_LeftRamp;
		bool m_RightRamp;

		int m_RampType = 1;
	};

	extern Vars_ m_Vars;
}