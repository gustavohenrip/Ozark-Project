#pragma once
#include "menu/base/submenu.hpp"

class VehicleGunMenu : public Menu::Submenu {
public:
	static VehicleGunMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleGunMenu()
		: Menu::Submenu() {}

	~VehicleGunMenu();
};

namespace VehicleGunMenuVars {
	struct Vars_ {
		bool m_Toggle = false;
		bool m_SitInside = false;
		Vehicle m_Vehicle = 0;

		RadioContext m_Radio = { "commonmenu", "shop_garage_icon" };
	};

	extern Vars_ m_Vars;
}