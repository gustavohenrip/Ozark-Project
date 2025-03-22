#pragma once
#include "menu/base/submenu.hpp"

class VehicleEditorBumpersMenu : public Menu::Submenu {
public:
	static VehicleEditorBumpersMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleEditorBumpersMenu()
		: Menu::Submenu() {}

	~VehicleEditorBumpersMenu();
};

namespace VehicleEditorBumpersMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}