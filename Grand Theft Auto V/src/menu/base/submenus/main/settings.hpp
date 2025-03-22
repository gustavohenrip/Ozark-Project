#pragma once
#include "menu/base/submenu.hpp"

class SettingsMenu : public Menu::Submenu {
public:
	static SettingsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	SettingsMenu()
		: Menu::Submenu() {}

	~SettingsMenu();
};

namespace SettingsMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}