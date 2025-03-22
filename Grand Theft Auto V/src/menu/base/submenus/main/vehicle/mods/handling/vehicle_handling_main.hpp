#pragma once
#include "menu/base/submenu.hpp"

class VehicleModsHandlingMainMenu : public Menu::Submenu {
public:
	static VehicleModsHandlingMainMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleModsHandlingMainMenu()
		: Menu::Submenu() {}

	~VehicleModsHandlingMainMenu();
};

namespace VehicleModsHandlingMainMenuVars {
	struct Vars_ {
		int m_Type;
	};

	extern Vars_ m_Vars;
}