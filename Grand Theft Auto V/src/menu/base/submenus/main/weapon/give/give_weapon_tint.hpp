#pragma once
#include "menu/base/submenu.hpp"

class GiveWeaponTintMenu : public Menu::Submenu {
public:
	static GiveWeaponTintMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	GiveWeaponTintMenu()
		: Menu::Submenu() {}

	~GiveWeaponTintMenu();
};

namespace GiveWeaponTintMenuVars {
	struct Vars_ {
		// RadioContext m_Context = { "commonmenu", "shop_tick_icon" };
	};

	std::vector<std::pair<std::string, int>> GetWeaponTints(int Weapon);
	extern Vars_ m_Vars;
}