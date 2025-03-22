#pragma once
#include "menu/base/submenu.hpp"

class VehicleEditorNeonMenu : public Menu::Submenu {
public:
	static VehicleEditorNeonMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleEditorNeonMenu()
		: Menu::Submenu() {}

	~VehicleEditorNeonMenu();
};

namespace VehicleEditorNeonMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}