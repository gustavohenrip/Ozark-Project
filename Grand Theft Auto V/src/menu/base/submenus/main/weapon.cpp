#include "weapon.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../main.hpp"
#include "global/arrays.hpp"
#include "weapon/weapon_mods.hpp"
#include "menu/base/util/camera.hpp"
#include "weapon/give/give_weapons.hpp"
#include "weapon/purchase/purchase_weapons.hpp"

using namespace WeaponMenuVars;

namespace WeaponMenuVars {
	Vars_ m_Vars;
}

void WeaponMenu::Init() {
	SetName("Weapon");
	SetParentSubmenu<MainMenu>();

	addOption(SubmenuOption("Weapon Mods")
		.addTranslation().addHotkey()
		.addSubmenu<WeaponModsMenu>());

	addOption(SubmenuOption("Give Weapons")
		.addTranslation().addHotkey()
		.addSubmenu<GiveWeaponsMenu>());

	addOption(SubmenuOption("Purchase Weapons")
		.addTranslation().addHotkey()
		.addSubmenu<PurchaseWeaponsMenu>());

	addOption(ButtonOption("Max Weapon Ammo")
		.addTranslation().addHotkey()
		.addOnClick([] {
			for (auto& Weapon : Global::Arrays::g_Weapons) {
				int Ammo = -1;
				Native::GetMaxAmmo(Menu::GetLocalPlayer().m_Ped, Weapon.m_Result, &Ammo);
				Native::AddAmmoToPed(Menu::GetLocalPlayer().m_Ped, Weapon.m_Result, Ammo);
			}
		}));

	addOption(ButtonOption("Remove All Weapons")
		.addTranslation().addHotkey()
		.addOnClick([] { Native::RemoveAllPedWeapons(Menu::GetLocalPlayer().m_Ped, true); }));
}

void WeaponMenu::Update() {}

/*Called once on submenu open*/
void WeaponMenu::UpdateOnce() {}

/*Called always*/
void WeaponMenu::FeatureUpdate() {}

WeaponMenu* _instance;
WeaponMenu* WeaponMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WeaponMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WeaponMenu::~WeaponMenu() { delete _instance; }