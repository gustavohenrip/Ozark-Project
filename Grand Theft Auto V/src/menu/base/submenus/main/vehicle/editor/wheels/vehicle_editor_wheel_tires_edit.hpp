#pragma once
#include "menu/base/submenu.hpp"

class VehicleEditorWheelTiresEditMenu : public Menu::Submenu {
public:
	static VehicleEditorWheelTiresEditMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleEditorWheelTiresEditMenu()
		: Menu::Submenu() {}

	~VehicleEditorWheelTiresEditMenu();
};

namespace VehicleEditorWheelTiresEditMenuVars {
	struct Vars_ {
		int m_Type;
		RadioContext m_Context = { "commonmenu", "shop_garage_icon" };
	};

	extern Vars_ m_Vars;
}