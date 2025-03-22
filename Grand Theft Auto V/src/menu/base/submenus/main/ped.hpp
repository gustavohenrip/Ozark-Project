#pragma once
#include "menu/base/submenu.hpp"

class PedMenu : public Menu::Submenu {
public:
	static PedMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	PedMenu()
		: Menu::Submenu() {}

	~PedMenu();
};

namespace PedMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}