#pragma once
#include "menu/base/submenu.hpp"

class AmmoModifierLoadMenu : public Menu::Submenu {
public:
	static AmmoModifierLoadMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	AmmoModifierLoadMenu()
		: Menu::Submenu() {}

	~AmmoModifierLoadMenu();
};

namespace AmmoModifierLoadMenuVars {
	struct Vars_ {};

	void SaveModifier(const char* Name, uint32_t Model);
	extern Vars_ m_Vars;
}