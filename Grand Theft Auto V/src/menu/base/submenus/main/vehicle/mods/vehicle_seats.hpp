#pragma once
#include "menu/base/submenu.hpp"

class VehicleModsSeatMenu : public Menu::Submenu {
public:
	static VehicleModsSeatMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleModsSeatMenu()
		: Menu::Submenu() {}

	~VehicleModsSeatMenu();
};

namespace VehicleModsSeatMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}