#pragma once
#include "menu/base/submenu.hpp"

class WeaponMenu : public Menu::Submenu {
public:
	static WeaponMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WeaponMenu()
		: Menu::Submenu() {}

	~WeaponMenu();
};

namespace WeaponMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}