#pragma once
#include "menu/base/submenu.hpp"

class VehicleEditorWheelTypeEditMenu : public Menu::Submenu {
public:
	static VehicleEditorWheelTypeEditMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleEditorWheelTypeEditMenu()
		: Menu::Submenu() {}

	~VehicleEditorWheelTypeEditMenu();
};

namespace VehicleEditorWheelTypeEditMenuVars {
	struct Vars_ {
		int m_WheelType;
		RadioContext m_Context = { "commonmenu", "shop_garage_icon" };
	};

	extern Vars_ m_Vars;
}