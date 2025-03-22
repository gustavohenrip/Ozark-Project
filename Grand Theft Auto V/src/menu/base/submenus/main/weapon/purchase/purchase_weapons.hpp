#pragma once
#include "menu/base/submenu.hpp"

class PurchaseWeaponsMenu : public Menu::Submenu {
public:
	static PurchaseWeaponsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	PurchaseWeaponsMenu()
		: Menu::Submenu() {}

	~PurchaseWeaponsMenu();
};

namespace PurchaseWeaponsMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}