#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/submenus/main/helpers/esp.hpp"

class VehicleLocalMenu : public Menu::Submenu {
public:
	static VehicleLocalMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleLocalMenu()
		: Menu::Submenu() {}

	~VehicleLocalMenu();
};

namespace VehicleLocalMenuVars {
	struct Vars_ {
		int m_Driver;
		int m_Boost;
		int m_Acrobatics;
		int m_Gravity;
		int m_Teleport;

		ESPMenuVars::ESP m_ESP;
	};

	extern Vars_ m_Vars;
}