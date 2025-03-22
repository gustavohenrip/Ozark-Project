#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/util/rainbow.hpp"
#include "global/ui_vars.hpp"

class RainbowConfigLoadMenu : public Menu::Submenu {
public:
	static RainbowConfigLoadMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	RainbowConfigLoadMenu()
		: Menu::Submenu() {}

	~RainbowConfigLoadMenu();
};

namespace RainbowConfigLoadMenuVars {
	struct Vars_ {
		Menu::Rainbow::Rainbow* m_Rainbow;
	};

	extern Vars_ m_Vars;
}