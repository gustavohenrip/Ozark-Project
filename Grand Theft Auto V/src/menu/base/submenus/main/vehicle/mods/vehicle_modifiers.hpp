#pragma once
#include "menu/base/submenu.hpp"

class VehicleModsModifiersMenu : public Menu::Submenu {
public:
	static VehicleModsModifiersMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleModsModifiersMenu()
		: Menu::Submenu() {}

	~VehicleModsModifiersMenu();
};

namespace VehicleModsModifiersMenuVars {
	struct Vars_ {
		float m_Suspension;
		float m_TireSize;
		float m_TireWidth;

		bool m_HasSuspension;
		bool m_HasCustomWheels;
	};

	extern Vars_ m_Vars;
}