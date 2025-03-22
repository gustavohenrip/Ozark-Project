#pragma once
#include "menu/base/submenu.hpp"

class VehicleModsHandlingMenu : public Menu::Submenu {
public:
	static VehicleModsHandlingMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleModsHandlingMenu()
		: Menu::Submenu() {}

	~VehicleModsHandlingMenu();
};

namespace VehicleModsHandlingMenuVars {
	struct Vars_ {
		int m_Type = 0;

		bool m_InBoat;
		bool m_InBike;
		bool m_InFlying;
	};

	extern Vars_ m_Vars;
}