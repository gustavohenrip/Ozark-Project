#pragma once
#include "menu/base/submenu.hpp"

class VehicleEditorHornMenu : public Menu::Submenu {
public:
	static VehicleEditorHornMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleEditorHornMenu()
		: Menu::Submenu() {}

	~VehicleEditorHornMenu();
};

namespace VehicleEditorHornMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}