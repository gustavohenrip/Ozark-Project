#pragma once
#include "menu/base/submenu.hpp"

class VehicleModsMultipliersMenu : public Menu::Submenu {
public:
	static VehicleModsMultipliersMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleModsMultipliersMenu()
		: Menu::Submenu() {}

	~VehicleModsMultipliersMenu();
};

namespace VehicleModsMultipliersMenuVars {
	struct Vars_ {
		float m_RPM = 1.f;
		float m_Torque = 1.f;
		float m_Headlight = 1.f;
		float m_TractionCurveMax = 1.f;

		bool m_CachedTraction = false;
	};

	extern Vars_ m_Vars;
}