#include "ammo_modifier.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../modifiers.hpp"
#include "global/arrays.hpp"
#include "ammo_modifier_edit.hpp"
#include "load/ammo_modifier_load.hpp"

using namespace AmmoModifierMenuVars;

namespace AmmoModifierMenuVars {
	Vars_ m_Vars;

	void Reset() {
		for (auto& Modifier : AmmoModifierEditMenuVars::m_Vars.m_Modifiers) {
			AmmoModifierEditMenuVars::ResetModifier(Modifier.m_Hash);
		}
	}

	bool HasExistingModifier(uint32_t Hash) {
		for (auto& Modifier : AmmoModifierEditMenuVars::m_Vars.m_Modifiers) {
			if (Modifier.m_Hash == Hash) return true;
		}

		return false;
	}
}

void AmmoModifierMenu::Init() {
	SetName("Ammo Modifier");
	SetParentSubmenu<WeaponModifierMenu>();

	addOption(SubmenuOption("Load Modifier")
		.addTranslation().addHotkey()
		.addSubmenu<AmmoModifierLoadMenu>());

	addOption(ButtonOption("Reset All Modifiers")
		.addTranslation().addHotkey()
		.addOnClick(Reset));

	addOption(SubmenuOption("Current Weapon")
		.addTranslation()
		.addSubmenu<AmmoModifierEditMenu>()
		.addOnClick([] {
			AmmoModifierEditMenuVars::m_Vars.m_CurrentHash = Menu::GetLocalPlayer().m_WeaponHash;
		})
		.addRequirement([] { return Menu::GetLocalPlayer().m_WeaponHash && Menu::GetLocalPlayer().m_WeaponHash != 2725352035; }));

	addOption(BreakOption("All Weapons")
		.addTranslation());

	for (auto& Weapon : Global::Arrays::g_Weapons) {
		addOption(ButtonOption(Weapon.m_Name.Get())
			.addSprite("commonmenu", "shop_ammo_icon", [=] { return HasExistingModifier(Weapon.m_Result); })
			.addOnClick([=] {
				AmmoModifierEditMenuVars::m_Vars.m_CurrentHash = Weapon.m_Result;
				Menu::GetSubmenuHandler()->SetSubmenu(AmmoModifierEditMenu::GetInstance());
			}));
	}
}

void AmmoModifierMenu::Update() {}

/*Called once on submenu open*/
void AmmoModifierMenu::UpdateOnce() {}

/*Called always*/
void AmmoModifierMenu::FeatureUpdate() {}

AmmoModifierMenu* _instance;
AmmoModifierMenu* AmmoModifierMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new AmmoModifierMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

AmmoModifierMenu::~AmmoModifierMenu() { delete _instance; }