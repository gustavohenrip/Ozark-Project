#pragma once
#include "menu/base/submenu.hpp"

class VehicleSaveLoadMenu : public Menu::Submenu {
public:
	static VehicleSaveLoadMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleSaveLoadMenu()
		: Menu::Submenu() {}

	~VehicleSaveLoadMenu();
};

namespace VehicleSaveLoadMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}