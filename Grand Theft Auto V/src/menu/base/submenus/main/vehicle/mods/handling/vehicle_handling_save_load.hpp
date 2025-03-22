#pragma once
#include "menu/base/submenu.hpp"

class VehicleModsHandlingSaveLoadMenu : public Menu::Submenu {
public:
	static VehicleModsHandlingSaveLoadMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleModsHandlingSaveLoadMenu()
		: Menu::Submenu() {}

	~VehicleModsHandlingSaveLoadMenu();
};

namespace VehicleModsHandlingSaveLoadMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}