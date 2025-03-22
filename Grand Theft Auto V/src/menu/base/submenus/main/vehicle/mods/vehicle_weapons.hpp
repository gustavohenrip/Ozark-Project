#pragma once
#include "menu/base/submenu.hpp"

class VehicleModsWeaponsMenu : public Menu::Submenu {
public:
	static VehicleModsWeaponsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleModsWeaponsMenu()
		: Menu::Submenu() {}

	~VehicleModsWeaponsMenu();
};

namespace VehicleModsWeaponsMenuVars {
	struct Vars_ {
		bool m_Toggle;
		bool m_AimingLines;
		float m_Speed = 2000.f;
		int m_AimingLineType;

		RadioContext m_Context = { "commonmenu", "shop_ammo_icon" };
	};

	extern Vars_ m_Vars;
}