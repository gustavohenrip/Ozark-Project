#pragma once
#include "menu/base/submenu.hpp"

class VehicleEditorFendersMenu : public Menu::Submenu {
public:
	static VehicleEditorFendersMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleEditorFendersMenu()
		: Menu::Submenu() {}

	~VehicleEditorFendersMenu();
};

namespace VehicleEditorFendersMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}