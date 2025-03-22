#pragma once
#include "menu/base/submenu.hpp"

class WeaponDisablesMenu : public Menu::Submenu {
public:
	static WeaponDisablesMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WeaponDisablesMenu()
		: Menu::Submenu() {}

	~WeaponDisablesMenu();
};

namespace WeaponDisablesMenuVars {
	struct Vars_ {
		bool m_DisableVehicleWeaponLimits;
		bool m_DisableOrbitalCooldown;
		bool m_InteriorWeapons;
		bool m_PassiveWeapons;
	};

	extern Vars_ m_Vars;
}