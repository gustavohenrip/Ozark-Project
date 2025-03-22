#pragma once
#include "menu/base/submenu.hpp"

class MiscHudMenu : public Menu::Submenu {
public:
	static MiscHudMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	MiscHudMenu()
		: Menu::Submenu() {}

	~MiscHudMenu();
};

namespace MiscHudMenuVars {
	struct Vars_ {
		int m_Hud;
		Color m_Color;
	};

	extern Vars_ m_Vars;
}