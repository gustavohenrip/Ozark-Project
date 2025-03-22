#pragma once
#include "menu/base/submenu.hpp"

class VehicleEditorLSCMenu : public Menu::Submenu {
public:
	static VehicleEditorLSCMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleEditorLSCMenu()
		: Menu::Submenu() {}

	~VehicleEditorLSCMenu();
};

namespace VehicleEditorLSCMenuVars {
	struct Vars_ {
		eVehicleModTypes m_ModType;
		int m_XenonColor;
		int* m_Value;

		RadioContext m_Context = { "commonmenu", "shop_garage_icon" };
	};

	extern Vars_ m_Vars;
}