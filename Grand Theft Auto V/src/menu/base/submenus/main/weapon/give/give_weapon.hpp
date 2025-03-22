#pragma once
#include "menu/base/submenu.hpp"

class GiveWeaponMenu : public Menu::Submenu {
public:
	static GiveWeaponMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	GiveWeaponMenu()
		: Menu::Submenu() {}

	~GiveWeaponMenu();
};

namespace GiveWeaponMenuVars {
	struct Vars_ {
		bool m_GiveWeapons;
	};

	void UpgradeWeaponComponents(Ped Ped, uint32_t WeaponHash);
	void RemoveWeaponUpgrades(Ped Ped, uint32_t WeaponHash);
	void GiveWeapon(Ped Ped, uint32_t WeaponHash, bool EquipNow, bool FullyUpgraded);
	extern Vars_ m_Vars;
}