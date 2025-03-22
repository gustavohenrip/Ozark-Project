#pragma once
#include "menu/base/submenu.hpp"

class VehicleModsHealthMenu : public Menu::Submenu {
public:
	static VehicleModsHealthMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleModsHealthMenu()
		: Menu::Submenu() {}

	~VehicleModsHealthMenu();
};

namespace VehicleModsHealthMenuVars {
	struct Vars_ {
		bool m_AutoRepair;
		bool m_AutoWash;
	};

	extern Vars_ m_Vars;
}