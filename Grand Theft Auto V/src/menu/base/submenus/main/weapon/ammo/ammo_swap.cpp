#include "ammo_swap.hpp"
#include "menu/base/submenu_handler.hpp"
#include "ammo_modifier_edit.hpp"
#include "global/arrays.hpp"

using namespace AmmoSwapMenuVars;

namespace AmmoSwapMenuVars {
	Vars_ m_Vars;

	void Swap(uint32_t First, uint32_t Second) {
		uint64_t Table = *(uint64_t*)Global::Vars::g_WeaponInfoList;
		if (Table) {
			Rage::Types::Weapon* FirstPtr = nullptr;
			Rage::Types::Weapon* SecondPtr = nullptr;

			for (int16_t i = 0; i < (*(int16_t*)Global::Vars::g_WeaponInfoListCount) - 1; i++) {
				Rage::Types::Weapon* Ptr = (Rage::Types::Weapon*) * (uint64_t*)(Table + (i * 8));
				if (Ptr) {
					if (Ptr->m_NameHash == First) {
						FirstPtr = Ptr;
						break;
					}
				}
			}

			for (int16_t i = 0; i < (*(int16_t*)Global::Vars::g_WeaponInfoListCount) - 1; i++) {
				Rage::Types::Weapon* Ptr = (Rage::Types::Weapon*) * (uint64_t*)(Table + (i * 8));
				if (Ptr) {
					if (Ptr->m_NameHash == Second) {
						SecondPtr = Ptr;
						break;
					}
				}
			}

			if (FirstPtr && SecondPtr) {
				auto Vit = std::find_if(AmmoModifierEditMenuVars::m_Vars.m_Modifiers.begin(), AmmoModifierEditMenuVars::m_Vars.m_Modifiers.end(), [=] (AmmoModifierEditMenuVars::EditContext& Context) {
					return Context.m_Hash == First;
				});
				
				if (Vit == AmmoModifierEditMenuVars::m_Vars.m_Modifiers.end()) {
					AmmoModifierEditMenuVars::m_Vars.m_Modifiers.push_back({ FirstPtr->m_AmmoInfo, FirstPtr->m_AimingInfo, FirstPtr->m_FireType, FirstPtr->m_Audio, First, Second });
				}

				int Ammo = -1;
				Native::GetMaxAmmo(Menu::GetLocalPlayer().m_Ped, First, &Ammo);

				FirstPtr->m_AmmoInfo = SecondPtr->m_AmmoInfo;
				FirstPtr->m_AimingInfo = SecondPtr->m_AimingInfo;
				FirstPtr->m_FireType = SecondPtr->m_FireType;
				FirstPtr->m_Audio = SecondPtr->m_Audio;

				Native::AddAmmoToPed(Menu::GetLocalPlayer().m_Ped, First, Ammo);
				Native::GiveWeaponToPed(Menu::GetLocalPlayer().m_Ped, Second, Ammo, false, false);
			}
		}
	}
}

void AmmoSwapMenu::Init() {
	SetName("Swap With...");
	SetParentSubmenu<AmmoModifierEditMenu>();

	for (auto& Weapon : Global::Arrays::g_Weapons) {
		addOption(ButtonOption(Weapon.m_Name.Get())
			.addOnClick([=] {
				Swap(AmmoModifierEditMenuVars::m_Vars.m_CurrentHash, Weapon.m_Result);
				Menu::GetSubmenuHandler()->SetSubmenuPrevious();
			}));
	}
}

void AmmoSwapMenu::Update() {}

/*Called once on submenu open*/
void AmmoSwapMenu::UpdateOnce() {}

/*Called always*/
void AmmoSwapMenu::FeatureUpdate() {}

AmmoSwapMenu* _instance;
AmmoSwapMenu* AmmoSwapMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new AmmoSwapMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

AmmoSwapMenu::~AmmoSwapMenu() { delete _instance; }