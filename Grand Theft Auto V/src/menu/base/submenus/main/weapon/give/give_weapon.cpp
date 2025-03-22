#include "give_weapon.hpp"
#include "menu/base/submenu_handler.hpp"
#include "give_weapons.hpp"
#include "rage/engine.hpp"
#include "global/arrays.hpp"

using namespace GiveWeaponMenuVars;

namespace GiveWeaponMenuVars {
	Vars_ m_Vars;

    void UpgradeWeaponComponents(Ped Ped, uint32_t WeaponHash) {
        auto Hashes = Rage::Engine::GetHashListFromHashTable(*(uint32_t*)(Global::Vars::g_WeaponComponentHashListCount) - 1, Global::Vars::g_WeaponComponentHashList);
        for (auto& Component : Hashes) {
            if (Native::DoesWeaponTakeWeaponComponent(WeaponHash, Component)) {
                Native::GiveWeaponComponentToPed(Ped, WeaponHash, Component);
            }
        }
    }

    void RemoveWeaponUpgrades(Ped Ped, uint32_t WeaponHash) {
        auto Hashes = Rage::Engine::GetHashListFromHashTable(*(uint32_t*)(Global::Vars::g_WeaponComponentHashListCount) - 1, Global::Vars::g_WeaponComponentHashList);
        for (auto& Component : Hashes) {
            if (Native::DoesWeaponTakeWeaponComponent(WeaponHash, Component)) {
                Native::RemoveWeaponComponentFromPed(Ped, WeaponHash, Component);
            }
        }
    }

    void GiveWeapon(Ped Ped, uint32_t WeaponHash, bool EquipNow, bool FullyUpgraded) {
        int Ammo = -1;
        Native::GetMaxAmmo(Ped, WeaponHash, &Ammo);
        Native::GiveWeaponToPed(Ped, WeaponHash, Ammo, false, EquipNow);

        if (FullyUpgraded) {
            UpgradeWeaponComponents(Ped, WeaponHash);
        }
    }
}

void GiveWeaponMenu::Init() {
	SetName("Give Weapon");
	SetParentSubmenu<GiveWeaponsMenu>();

	addOption(ToggleOption("Give All Weapons")
		.addTranslation().addHotkey()
        .addToggle(m_Vars.m_GiveWeapons)
		.addOnClick([] {
            if (m_Vars.m_GiveWeapons) {
                for (auto& Weapon : Global::Arrays::g_Weapons) {
                    GiveWeapon(Menu::GetLocalPlayer().m_Ped, Weapon.m_Result, false, false);
                }
        }
        }).canBeSaved(m_ParentNameStack)
        .addTooltip("When toggled, weapons are reapplied on model change and session change"));

    addOption(BreakOption("Weapons")
        .addTranslation());

    for (auto& Weapon : Global::Arrays::g_Weapons) {
        addOption(ButtonOption(Weapon.m_Name.Get())
            .addTranslation()
            .addOnClick([=] { GiveWeapon(Menu::GetLocalPlayer().m_Ped, Weapon.m_Result, true, false); })
            .addOnUpdate([=] (ButtonOption* Option) {
                if (Native::HasPedGotWeapon(Menu::GetLocalPlayer().m_Ped, Weapon.m_Result, false)) {
                    Option->addSprite("commonmenu", "shop_gunclub_icon");
                    Option->addSpriteScale(0.02112f, 0.03744f);
                } else Option->removeSprite();
            }));
    }
}

void GiveWeaponMenu::Update() {}

/*Called once on submenu open*/
void GiveWeaponMenu::UpdateOnce() {}

/*Called always*/
void GiveWeaponMenu::FeatureUpdate() {
    if (m_Vars.m_GiveWeapons) {
        if (Global::Vars::g_ReapplyWeapons) {
            Global::Vars::g_ReapplyWeapons = false;

            for (auto& Weapon : Global::Arrays::g_Weapons) {
                GiveWeapon(Menu::GetLocalPlayer().m_Ped, Weapon.m_Result, false, false);
            }
        }
    }
}

GiveWeaponMenu* _instance;
GiveWeaponMenu* GiveWeaponMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new GiveWeaponMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

GiveWeaponMenu::~GiveWeaponMenu() { delete _instance; }