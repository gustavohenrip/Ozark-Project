#pragma once
#include "menu/base/submenu.hpp"

class VehicleModsSpeedometerMenu : public Menu::Submenu {
public:
	static VehicleModsSpeedometerMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleModsSpeedometerMenu()
		: Menu::Submenu() {}

	~VehicleModsSpeedometerMenu();
};

namespace VehicleModsSpeedometerMenuVars {
	struct Vars_ {
		bool m_BasicSpeedometer;
		bool m_NumberplateSpeedometer;

		std::string m_PreviousNumberplateText;
	};

	extern Vars_ m_Vars;
}