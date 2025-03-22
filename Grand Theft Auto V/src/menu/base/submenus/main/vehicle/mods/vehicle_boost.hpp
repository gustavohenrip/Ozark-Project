#pragma once
#include "menu/base/submenu.hpp"

class VehicleModsBoostMenu : public Menu::Submenu {
public:
	static VehicleModsBoostMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleModsBoostMenu()
		: Menu::Submenu() {}

	~VehicleModsBoostMenu();
};

namespace VehicleModsBoostMenuVars {
	struct Vars_ {
		bool m_BoostFX;
		bool m_BoostSound;
		bool m_NitrousOxide;
		bool m_UnlimitedSpecialAbility;

		int m_BoostForwards = 40;
		int m_BoostBackwards = 40;
	};

	extern Vars_ m_Vars;
}