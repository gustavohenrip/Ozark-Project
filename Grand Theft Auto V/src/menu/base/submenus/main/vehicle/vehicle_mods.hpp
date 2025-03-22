#pragma once
#include "menu/base/submenu.hpp"

class VehicleModsMenu : public Menu::Submenu {
public:
	static VehicleModsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleModsMenu()
		: Menu::Submenu() {}

	~VehicleModsMenu();
};

namespace VehicleModsMenuVars {
	struct Vars_ {
		bool m_Godmode;
		bool m_Invisibility;
		bool m_Seatbelt;
		bool m_BurnShell;
		bool m_AutoFlip;

		int m_Opacity = 5;
	};

	void MaxUpgrades(Vehicle Veh);
	void Downgrade(Vehicle Veh);
	Vehicle CloneVehicle(Vehicle Veh);
	extern Vars_ m_Vars;
}