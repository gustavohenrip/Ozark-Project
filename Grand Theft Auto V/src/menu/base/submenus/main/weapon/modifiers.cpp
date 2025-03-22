#include "modifiers.hpp"
#include "menu/base/submenu_handler.hpp"
#include "weapon_mods.hpp"
#include "utils/log.hpp"
#include "global/arrays.hpp"
#include "rage/engine.hpp"
#include "ammo/ammo_modifier.hpp"

using namespace WeaponModifierMenuVars;

namespace WeaponModifierMenuVars {
	Vars_ m_Vars;

	Rage::Types::Weapon* GetWeaponInfo() {
		auto Ped = Global::Vars::g_PedFactory->m_LocalPed;
		if (Ped) {
			auto WeaponManager = Ped->m_PedWeaponManager;
			if (WeaponManager) {
				return WeaponManager->m_WeaponInfo;
			}
		}

		return nullptr;
	}
}

void WeaponModifierMenu::Init() {
	SetName("Modifiers");
	SetParentSubmenu<WeaponModsMenu>();

	addOption(SubmenuOption("Ammo Modifier")
		.addTranslation().addHotkey()
		.addSubmenu<AmmoModifierMenu>());

	// Multiplier
	// Scaling

	addOption(ToggleOption("Disable Spread")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DisableSpread).canBeSaved(m_ParentNameStack)
		.addOnClick([] {
			if (!m_Vars.m_DisableSpread) {
				if (GetWeaponInfo()) {
					if (m_Vars.m_OriginalSpread.find(GetWeaponInfo()) != m_Vars.m_OriginalSpread.end()) {
						GetWeaponInfo()->m_BatchSpread = m_Vars.m_OriginalSpread[GetWeaponInfo()].first;
						GetWeaponInfo()->m_AccuracySpread = m_Vars.m_OriginalSpread[GetWeaponInfo()].second;
					}
				}
			}
		}));

	addOption(ToggleOption("Disable Recoil")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DisableRecoil).canBeSaved(m_ParentNameStack)
		.addOnClick([] {
			if (!m_Vars.m_DisableRecoil) {
				if (GetWeaponInfo()) {
					if (m_Vars.m_OriginalRecoil.find(GetWeaponInfo()) != m_Vars.m_OriginalRecoil.end()) {
						GetWeaponInfo()->m_RecoilShakeAmptitude = m_Vars.m_OriginalRecoil[GetWeaponInfo()];
					}
				}
			}
		}));

	addOption(ToggleOption("Disable Cooldown")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DisableCooldown).canBeSaved(m_ParentNameStack)
		.addOnClick([] {
			if (!m_Vars.m_DisableCooldown) {
				if (GetWeaponInfo()) {
					if (m_Vars.m_OriginalTimeBetweenShots.find(GetWeaponInfo()) != m_Vars.m_OriginalTimeBetweenShots.end()) {
						GetWeaponInfo()->m_TimeBetweenShots = m_Vars.m_OriginalTimeBetweenShots[GetWeaponInfo()];
					}
				}
			}
		}));

	addOption(ToggleOption("Disable Spin Time")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DisableSpin).canBeSaved(m_ParentNameStack)
		.addOnClick([] {
			if (!m_Vars.m_DisableSpin) {
				if (GetWeaponInfo()) {
					if (m_Vars.m_OriginalInstantSpin.find(GetWeaponInfo()) != m_Vars.m_OriginalInstantSpin.end()) {
						GetWeaponInfo()->m_SpinUpTime = m_Vars.m_OriginalInstantSpin[GetWeaponInfo()].first;
						GetWeaponInfo()->m_SpinDownTime = m_Vars.m_OriginalInstantSpin[GetWeaponInfo()].second;
					}
				}
			}
		}));

	addOption(ToggleOption("Instant Reload Anim")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_InstantReload).canBeSaved(m_ParentNameStack)
		.addOnClick([] {
			if (!m_Vars.m_InstantReload) {
				if (GetWeaponInfo()) {
					if (m_Vars.m_OriginalAnimReloadRate.find(GetWeaponInfo()) != m_Vars.m_OriginalAnimReloadRate.end()) {
						GetWeaponInfo()->m_AnimReloadRate = m_Vars.m_OriginalAnimReloadRate[GetWeaponInfo()];
					}
				}
			}
		}));

	addOption(NumberOption<float>(TOGGLE, "Bullet Damage Multiplier")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_WeaponDamageMultiplier)
		.addNumber(m_Vars.m_WeaponDamageMultiplierVal, "%.0f", 1.f).addMin(1.f).addMax(1000.f).setScrollSpeed(10)
		.addOnClick([] { if (Menu::GetLocalPlayer().m_WeaponHash) Native::SetPlayerWeaponDamageModifier(Menu::GetLocalPlayer().m_ID, 1.f); }).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(TOGGLE, "Melee Damage Multiplier")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_MeleeDamageMultiplier)
		.addNumber(m_Vars.m_MeleeDamageMultiplierVal, "%.0f", 1.f).addMin(1.f).addMax(1000.f).setScrollSpeed(10)
		.addOnClick([] { if (Menu::GetLocalPlayer().m_WeaponHash) Native::SetPlayerMeleeWeaponDamageModifier(Menu::GetLocalPlayer().m_ID, 1.f, 1); }).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(TOGGLE, "Defense Damage Multiplier")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DefenseDamageMultiplier)
		.addNumber(m_Vars.m_DefenseDamageMultiplierVal, "%.0f", 1.f).addMin(1.f).addMax(1000.f).setScrollSpeed(10)
		.addOnClick([] { if (Menu::GetLocalPlayer().m_WeaponHash) Native::SetPlayerWeaponDefenseModifier(Menu::GetLocalPlayer().m_ID, 1.f); }).canBeSaved(m_ParentNameStack));

	addOption(BreakOption("Scaling")
		.addTranslation());

	addOption(NumberOption<float>(TOGGLE, "Width")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Width)
		.addNumber(m_Vars.m_WidthVal, "%.2f", 0.01f).setScrollSpeed(10).addMin(0.f).addMax(100.f)
		.addOnUpdate([] (NumberOption<float>* Option) {
		Rage::Types::Object* Object = Cast<Rage::Types::Object>(Rage::Engine::GetEntityAddress(Menu::GetLocalPlayer().m_WeaponEntity));
		if (Object) {
			m_Vars.m_WidthVal = Object->m_Width;

			if (!m_Vars.m_Width && m_Vars.m_WidthVal != 1.f) {
				Object->m_Width = 1.f;
			}
		}
	}).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(TOGGLE, "Height")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Height)
		.addNumber(m_Vars.m_HeightVal, "%.2f", 0.01f).setScrollSpeed(10).addMin(0.f).addMax(100.f)
		.addOnUpdate([] (NumberOption<float>* Option) {
		Rage::Types::Object* Object = Cast<Rage::Types::Object>(Rage::Engine::GetEntityAddress(Menu::GetLocalPlayer().m_WeaponEntity));
		if (Object) {
			m_Vars.m_HeightVal = Object->m_Height;

			if (!m_Vars.m_Height && m_Vars.m_HeightVal != 1.f) {
				Object->m_Height = 1.f;
			}
		}
	}).canBeSaved(m_ParentNameStack));
}

void WeaponModifierMenu::Update() {}

/*Called once on submenu open*/
void WeaponModifierMenu::UpdateOnce() {}

/*Called always*/
void WeaponModifierMenu::FeatureUpdate() {
	if (m_Vars.m_WeaponDamageMultiplier) {
		if (Menu::GetLocalPlayer().m_WeaponHash) {
			Native::SetPlayerWeaponDamageModifier(Menu::GetLocalPlayer().m_ID, m_Vars.m_WeaponDamageMultiplierVal);
		}
	}

	if (m_Vars.m_MeleeDamageMultiplier) {
		if (Menu::GetLocalPlayer().m_WeaponHash) {
			Native::SetPlayerMeleeWeaponDamageModifier(Menu::GetLocalPlayer().m_ID, m_Vars.m_MeleeDamageMultiplierVal, 1);
		}
	}

	if (m_Vars.m_DefenseDamageMultiplier) {
		if (Menu::GetLocalPlayer().m_WeaponHash) {
			Native::SetPlayerWeaponDefenseModifier(Menu::GetLocalPlayer().m_ID, m_Vars.m_DefenseDamageMultiplierVal);
		}
	}

	if (m_Vars.m_DisableSpread) {
		auto WeaponInfo = GetWeaponInfo();
		if (WeaponInfo) {
			if (m_Vars.m_OriginalSpread.find(GetWeaponInfo()) == m_Vars.m_OriginalSpread.end()) {
				m_Vars.m_OriginalSpread[WeaponInfo].first = WeaponInfo->m_BatchSpread;
				m_Vars.m_OriginalSpread[WeaponInfo].second = WeaponInfo->m_AccuracySpread;
			}

			WeaponInfo->m_BatchSpread = 0.f;
			WeaponInfo->m_AccuracySpread = 0.f;
		}
	}

	if (m_Vars.m_DisableRecoil) {
		auto WeaponInfo = GetWeaponInfo();
		if (WeaponInfo) {
			if (m_Vars.m_OriginalRecoil.find(GetWeaponInfo()) == m_Vars.m_OriginalRecoil.end()) {
				m_Vars.m_OriginalRecoil[WeaponInfo] = WeaponInfo->m_RecoilShakeAmptitude;
			}

			WeaponInfo->m_RecoilShakeAmptitude = 0.f;
		}
	}

	if (m_Vars.m_InstantReload) {
		auto WeaponInfo = GetWeaponInfo();
		if (WeaponInfo) {
			if (m_Vars.m_OriginalAnimReloadRate.find(GetWeaponInfo()) == m_Vars.m_OriginalAnimReloadRate.end()) {
				m_Vars.m_OriginalAnimReloadRate[WeaponInfo] = WeaponInfo->m_AnimReloadRate;
			}

			WeaponInfo->m_AnimReloadRate = 100.f;
		}
	}

	if (m_Vars.m_DisableCooldown) {
		if (Menu::GetLocalPlayer().m_WeaponHash == 0x6d544c99
			|| Menu::GetLocalPlayer().m_WeaponHash == 0xaf3696a1
			|| Menu::GetLocalPlayer().m_WeaponHash == 0x3656c8c1) {
			auto WeaponInfo = GetWeaponInfo();
			if (WeaponInfo) {
				if (m_Vars.m_OriginalTimeBetweenShots.find(GetWeaponInfo()) == m_Vars.m_OriginalTimeBetweenShots.end()) {
					m_Vars.m_OriginalTimeBetweenShots[WeaponInfo] = WeaponInfo->m_TimeBetweenShots;
				}

				WeaponInfo->m_TimeBetweenShots = 0.f;
			}
		}
	}

	if (m_Vars.m_DisableSpin) {
		if (Menu::GetLocalPlayer().m_WeaponHash == 0x42bf8a85) {
			auto WeaponInfo = GetWeaponInfo();
			if (WeaponInfo) {
				if (m_Vars.m_OriginalInstantSpin.find(GetWeaponInfo()) == m_Vars.m_OriginalInstantSpin.end()) {
					m_Vars.m_OriginalInstantSpin[WeaponInfo].first = WeaponInfo->m_SpinUpTime;
					m_Vars.m_OriginalInstantSpin[WeaponInfo].second = WeaponInfo->m_SpinDownTime;
				}

				WeaponInfo->m_SpinUpTime = 0.f;
				WeaponInfo->m_SpinDownTime = 0.f;
			}
		}
	}

	if (m_Vars.m_Width) {
		Rage::Types::Object* Object = Cast<Rage::Types::Object>(Rage::Engine::GetEntityAddress(Menu::GetLocalPlayer().m_WeaponEntity));
		if (Object) {
			Object->m_Width = m_Vars.m_WidthVal;
		}
	}

	if (m_Vars.m_Height) {
		Rage::Types::Object* Object = Cast<Rage::Types::Object>(Rage::Engine::GetEntityAddress(Menu::GetLocalPlayer().m_WeaponEntity));
		if (Object) {
			Object->m_Height = m_Vars.m_HeightVal;
		}
	}
}

WeaponModifierMenu* _instance;
WeaponModifierMenu* WeaponModifierMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WeaponModifierMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WeaponModifierMenu::~WeaponModifierMenu() { delete _instance; }