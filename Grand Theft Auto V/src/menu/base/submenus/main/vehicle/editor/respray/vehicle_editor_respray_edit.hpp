#pragma once
#include "menu/base/submenu.hpp"

class VehicleEditorResprayEditMenu : public Menu::Submenu {
public:
	static VehicleEditorResprayEditMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleEditorResprayEditMenu()
		: Menu::Submenu() {}

	~VehicleEditorResprayEditMenu();
};

namespace VehicleEditorResprayEditMenuVars {
	struct Vars_ {
		int m_Type;
		RadioContext m_Context = { "commonmenu", "shop_garage_icon" };
	};

	extern Vars_ m_Vars;
}