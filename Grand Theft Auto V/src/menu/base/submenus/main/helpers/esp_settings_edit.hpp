#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/util/rainbow.hpp"

class ESPSettingsEditMenu : public Menu::Submenu {
public:
	static ESPSettingsEditMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ESPSettingsEditMenu()
		: Menu::Submenu() {}

	~ESPSettingsEditMenu();
};

namespace ESPSettingsEditMenuVars {
	struct Vars_ {
		float* m_Scale;

		Color* m_Color;
		Color* m_LineOfSightColor;

		Menu::Rainbow::Rainbow* m_Rainbow;
		Menu::Rainbow::Rainbow* m_LineOfSightRainbow;

		bool* m_RainbowToggle;
		bool* m_LineOfSightToggle;
	};

	extern Vars_ m_Vars;
}