#pragma once
#include "menu/base/submenu.hpp"

class VehicleModsAutopilotMenu : public Menu::Submenu {
public:
	static VehicleModsAutopilotMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleModsAutopilotMenu()
		: Menu::Submenu() {}

	~VehicleModsAutopilotMenu();
};

namespace VehicleModsAutopilotMenuVars {
	struct Vars_ {
		int m_Destinations;
		int m_Styles;

		float m_Speed = 20.f;
	};

	extern Vars_ m_Vars;
}