#pragma once
#include "menu/base/submenu.hpp"

class ESPSettingsMenu : public Menu::Submenu {
public:
	static ESPSettingsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ESPSettingsMenu()
		: Menu::Submenu() {}

	~ESPSettingsMenu();
};

namespace ESPSettingsMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}