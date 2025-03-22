#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/util/rainbow.hpp"
#include "global/ui_vars.hpp"

class RainbowConfigMenu : public Menu::Submenu {
public:
	static RainbowConfigMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	RainbowConfigMenu()
		: Menu::Submenu() {}

	~RainbowConfigMenu();
};

namespace RainbowConfigMenuVars {
	struct Vars_ {
		Color m_LocalColor;
		Menu::Rainbow::Rainbow m_LocalRainbow;
		Menu::Rainbow::Rainbow* m_Rainbow;
	};

	extern Vars_ m_Vars;
}