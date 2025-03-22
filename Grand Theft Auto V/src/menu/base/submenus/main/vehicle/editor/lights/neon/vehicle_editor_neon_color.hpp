#pragma once
#include "menu/base/submenu.hpp"

class VehicleEditorNeonColorMenu : public Menu::Submenu {
public:
	static VehicleEditorNeonColorMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleEditorNeonColorMenu()
		: Menu::Submenu() {}

	~VehicleEditorNeonColorMenu();
};

namespace VehicleEditorNeonColorMenuVars {
	struct Vars_ {
		RadioContext m_Context = { "commonmenu", "shop_garage_icon" };
	};

	extern Vars_ m_Vars;
}