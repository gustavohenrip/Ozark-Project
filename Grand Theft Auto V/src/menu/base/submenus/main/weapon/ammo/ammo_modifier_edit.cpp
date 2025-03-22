#include "ammo_modifier_edit.hpp"
#include "menu/base/submenu_handler.hpp"
#include "ammo_modifier.hpp"
#include "ammo_swap.hpp"
#include "../modifiers.hpp"
#include "load/ammo_modifier_load.hpp"

using namespace AmmoModifierEditMenuVars;

namespace AmmoModifierEditMenuVars {
	Vars_ m_Vars;

	void ResetModifier(uint32_t Hash) {
		auto Vit = std::find_if(AmmoModifierEditMenuVars::m_Vars.m_Modifiers.begin(), AmmoModifierEditMenuVars::m_Vars.m_Modifiers.end(), [=] (AmmoModifierEditMenuVars::EditContext& Context) {
			return Context.m_Hash == Hash;
		});

		if (Vit != AmmoModifierEditMenuVars::m_Vars.m_Modifiers.end()) {
			uint64_t Table = *(uint64_t*)Global::Vars::g_WeaponInfoList;
			if (Table) {
				for (int16_t i = 0; i < (*(int16_t*)Global::Vars::g_WeaponInfoListCount) - 1; i++) {
					Rage::Types::Weapon* Ptr = (Rage::Types::Weapon*) * (uint64_t*)(Table + (i * 8));
					if (Ptr) {
						if (Ptr->m_NameHash == Hash) {
							Ptr->m_AmmoInfo = Vit->m_AmmoInfo;
							Ptr->m_AimingInfo = Vit->m_AimingInfo;
							Ptr->m_FireType = Vit->m_FireType;
							Ptr->m_Audio = Vit->m_Audio;
							break;
						}
					}
				}
			}

			AmmoModifierEditMenuVars::m_Vars.m_Modifiers.erase(Vit);
		}

	}

	Rage::Types::Weapon* GetActiveWeapon() {
		if (m_Vars.m_CurrentHash == Menu::GetLocalPlayer().m_WeaponHash) return WeaponModifierMenuVars::GetWeaponInfo();

		uint64_t Table = *(uint64_t*)Global::Vars::g_WeaponInfoList;
		if (Table) {
			for (int16_t i = 0; i < (*(int16_t*)Global::Vars::g_WeaponInfoListCount) - 1; i++) {
				Rage::Types::Weapon* Ptr = (Rage::Types::Weapon*)*(uint64_t*)(Table + (i * 8));
				if (Ptr) {
					if (Ptr->m_NameHash == m_Vars.m_CurrentHash) {
						return Ptr;
					}
				}
			}
		}

		return nullptr;
	}
}

void AmmoModifierEditMenu::Init() {
	SetName("Ammo Modifier");
	SetParentSubmenu<AmmoModifierMenu>();

	addOption(ButtonOption("Save Modifier")
		.addKeyboard("Enter modifier name", 100, [] (const char* Name) { AmmoModifierLoadMenuVars::SaveModifier(Name, m_Vars.m_CurrentHash); }));

	addOption(ButtonOption("Reset Modifier")
		.addTranslation()
		.addOnClick([] { ResetModifier(m_Vars.m_CurrentHash); }));

	addOption(SubmenuOption("Bullet Swap")
		.addTranslation()
		.addSubmenu<AmmoSwapMenu>());

	addOption(NumberOption<float>(SCROLL, "Damage")
		.addTranslation()
		.addNumber(m_Vars.m_Damage, "%.0f", 1.f).setScrollSpeed(5).addMin(1.f).addMax(10000.f)
		.addOnClick([] {
			auto Weapon = GetActiveWeapon();
			if (Weapon) {
				Weapon->m_Damage = m_Vars.m_Damage;
			}
		})
		.addRequirement([] { return GetActiveWeapon() != nullptr; })
		.addTooltip("~y~Notice: ~w~Won't reset with Reset option"));

	addOption(NumberOption<float>(SCROLL, "Force")
		.addTranslation()
		.addNumber(m_Vars.m_Force, "%.0f", 1.f).setScrollSpeed(5).addMin(1.f).addMax(10000.f)
		.addOnClick([] {
			auto Weapon = GetActiveWeapon();
			if (Weapon) {
				Weapon->m_Force = m_Vars.m_Force;
			}
		})
		.addRequirement([] { return GetActiveWeapon() != nullptr; })
		.addTooltip("~y~Notice: ~w~Won't reset with Reset option"));

	addOption(NumberOption<float>(SCROLL, "Speed")
		.addTranslation()
		.addNumber(m_Vars.m_Speed, "%.0f", 1.f).setScrollSpeed(5).addMin(1.f).addMax(10000.f)
		.addOnClick([] {
			auto Weapon = GetActiveWeapon();
			if (Weapon) {
				Weapon->m_Speed = m_Vars.m_Speed;
			}
		})
		.addRequirement([] { return GetActiveWeapon() != nullptr; })
		.addTooltip("~y~Notice: ~w~Won't reset with Reset option"));
}

void AmmoModifierEditMenu::Update() {
	auto Active = GetActiveWeapon();
	if (Active == nullptr) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}

	if (m_Vars.m_CachedWeapon != Active) {
		m_Vars.m_CurrentHash = Menu::GetLocalPlayer().m_WeaponHash;
		m_Vars.m_CachedWeapon = Active;
		if (m_Vars.m_CachedWeapon) {
			m_Vars.m_Damage = m_Vars.m_CachedWeapon->m_Damage;
			m_Vars.m_Force = m_Vars.m_CachedWeapon->m_Force;
			m_Vars.m_Speed = m_Vars.m_CachedWeapon->m_Speed;
		}
	}
}

/*Called once on submenu open*/
void AmmoModifierEditMenu::UpdateOnce() {}

/*Called always*/
void AmmoModifierEditMenu::FeatureUpdate() {}

AmmoModifierEditMenu* _instance;
AmmoModifierEditMenu* AmmoModifierEditMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new AmmoModifierEditMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

AmmoModifierEditMenu::~AmmoModifierEditMenu() { delete _instance; }