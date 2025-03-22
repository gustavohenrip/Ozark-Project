#pragma once
#include "menu/base/submenu.hpp"

class VehicleEditorLightsMenu : public Menu::Submenu {
public:
	static VehicleEditorLightsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleEditorLightsMenu()
		: Menu::Submenu() {}

	~VehicleEditorLightsMenu();
};

namespace VehicleEditorLightsMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}