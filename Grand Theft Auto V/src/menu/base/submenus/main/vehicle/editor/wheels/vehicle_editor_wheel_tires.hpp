#pragma once
#include "menu/base/submenu.hpp"

class VehicleEditorWheelTiresMenu : public Menu::Submenu {
public:
	static VehicleEditorWheelTiresMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleEditorWheelTiresMenu()
		: Menu::Submenu() {}

	~VehicleEditorWheelTiresMenu();
};

namespace VehicleEditorWheelTiresMenuVars {
	struct Vars_ {
		bool m_UpdateCache;
		bool m_CustomTires;
		bool m_TireSmoke;
		Color m_TireSmokeColor;
	};

	extern Vars_ m_Vars;
}