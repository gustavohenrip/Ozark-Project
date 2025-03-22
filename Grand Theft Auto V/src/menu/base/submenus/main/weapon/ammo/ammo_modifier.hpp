#pragma once
#include "menu/base/submenu.hpp"

class AmmoModifierMenu : public Menu::Submenu {
public:
	static AmmoModifierMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	AmmoModifierMenu()
		: Menu::Submenu() {}

	~AmmoModifierMenu();
};

namespace AmmoModifierMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}