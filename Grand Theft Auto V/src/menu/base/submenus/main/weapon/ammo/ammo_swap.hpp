#pragma once
#include "menu/base/submenu.hpp"

class AmmoSwapMenu : public Menu::Submenu {
public:
	static AmmoSwapMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	AmmoSwapMenu()
		: Menu::Submenu() {}

	~AmmoSwapMenu();
};

namespace AmmoSwapMenuVars {
	struct Vars_ {};

	void Swap(uint32_t First, uint32_t Second);
	extern Vars_ m_Vars;
}