#pragma once
#include "menu/base/submenu.hpp"

class GiveWeaponTintSelectedMenu : public Menu::Submenu {
public:
	static GiveWeaponTintSelectedMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	GiveWeaponTintSelectedMenu()
		: Menu::Submenu() {}

	~GiveWeaponTintSelectedMenu();
};

namespace GiveWeaponTintSelectedMenuVars {
	struct Vars_ {
		RadioContext m_Context = { "commonmenu", "shop_tick_icon" };
		uint32_t m_Selected;
	};

	extern Vars_ m_Vars;
}
