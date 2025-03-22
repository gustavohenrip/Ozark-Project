#pragma once
#include "menu/base/submenu.hpp"

class VehicleEditorWheelColorMenu : public Menu::Submenu {
public:
	static VehicleEditorWheelColorMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleEditorWheelColorMenu()
		: Menu::Submenu() {}

	~VehicleEditorWheelColorMenu();
};

namespace VehicleEditorWheelColorMenuVars {
	struct Vars_ {
		RadioContext m_Context = { "commonmenu", "shop_garage_icon" };
	};

	extern Vars_ m_Vars;
}