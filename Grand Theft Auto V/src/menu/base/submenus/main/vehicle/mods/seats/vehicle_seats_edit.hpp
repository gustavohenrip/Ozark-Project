#pragma once
#include "menu/base/submenu.hpp"

class VehicleModsSeatEditMenu : public Menu::Submenu {
public:
	static VehicleModsSeatEditMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleModsSeatEditMenu()
		: Menu::Submenu() {}

	~VehicleModsSeatEditMenu();
};

namespace VehicleModsSeatEditMenuVars {
	struct Vars_ {
		int m_Seat;
	};

	extern Vars_ m_Vars;
}