#pragma once
#include "menu/base/submenu.hpp"

class GiveWeaponsMenu : public Menu::Submenu {
public:
	static GiveWeaponsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	GiveWeaponsMenu()
		: Menu::Submenu() {}

	~GiveWeaponsMenu();
};

namespace GiveWeaponsMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}