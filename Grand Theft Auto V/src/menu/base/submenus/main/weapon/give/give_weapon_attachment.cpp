#include "give_weapon_attachment.hpp"
#include "menu/base/submenu_handler.hpp"
#include "give_weapons.hpp"
#include "menu/base/util/notify.hpp"
#include "give_weapon.hpp"
#include "global/arrays.hpp"
#include "rage/engine.hpp"
#include "give_weapon_attachment_selected.hpp"
#include "../ammunation.hpp"

using namespace GiveWeaponAttachmentMenuVars;

namespace GiveWeaponAttachmentMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> GiveAll[] = {
		{ TranslationString("Current", true), 0 },
		{ TranslationString("All", true), 1 }
	};

	bool DoesWeaponHaveComponents(uint32_t Weapon) {
		int Count = 0;
		uint32_t LastComponent = 0;

		auto Hashes = Rage::Engine::GetHashListFromHashTable(*(uint32_t*)(Global::Vars::g_WeaponComponentHashListCount) - 1, Global::Vars::g_WeaponComponentHashList);
		for (auto& Component : Hashes) {
			if (Native::DoesWeaponTakeWeaponComponent(Weapon, Component)) {
				Count++;
				LastComponent = Component;
			}
		}

		if (Count) {
			if (Count == 1) {
				if (!strcmp(AmmunationMenuVars::Scripts::func_181(LastComponent, Weapon), "NULL")) {
					return false;
				}
			}

			return true;
		}

		return false;
	}

	void DoGiveAll(GiveWeaponAttachmentMenu* This, bool Remove) {
		if (Remove) {
			if (m_Vars.m_RemoveAll == 0) {
				if (Menu::GetLocalPlayer().m_HasWeaponInHand) {
					GiveWeaponMenuVars::RemoveWeaponUpgrades(Menu::GetLocalPlayer().m_Ped, Menu::GetLocalPlayer().m_WeaponHash);

				} else Menu::GetNotify()->NotifyStacked(This->getString("You don't have a weapon in hand"), Global::UIVars::g_NotificationError);

				return;
			}

			for (auto& Weapon : Global::Arrays::g_Weapons) {
				GiveWeaponMenuVars::RemoveWeaponUpgrades(Menu::GetLocalPlayer().m_Ped, Weapon.m_Result);
			}

			return;
		}

		if (m_Vars.m_GiveAll == 0) {
			if (Menu::GetLocalPlayer().m_HasWeaponInHand) {
				GiveWeaponMenuVars::UpgradeWeaponComponents(Menu::GetLocalPlayer().m_Ped, Menu::GetLocalPlayer().m_WeaponHash);
			} else Menu::GetNotify()->NotifyStacked(This->getString("You don't have a weapon in hand"), Global::UIVars::g_NotificationError);

			return;
		}

		for (auto& Weapon : Global::Arrays::g_Weapons) {
			GiveWeaponMenuVars::UpgradeWeaponComponents(Menu::GetLocalPlayer().m_Ped, Weapon.m_Result);
		}
	}
}

void GiveWeaponAttachmentMenu::Init() {
	SetName("Give Weapon Attachment");
	SetParentSubmenu<GiveWeaponsMenu>();

	addString("You don't have a weapon in hand");

	addOption(ScrollOption<int>(SCROLLSELECT, "Give All Weapon Attachments")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_GiveAll, 0, NUMOF(GiveAll), GiveAll)
		.addOnClick([this] { DoGiveAll(this, false); }));

	addOption(ScrollOption<int>(SCROLLSELECT, "Remove All Weapon Attachments")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_RemoveAll, 0, NUMOF(GiveAll), GiveAll)
		.addOnClick([this] { DoGiveAll(this, true); }));

	addOption(SubmenuOption("Current Weapon")
		.addTranslation()
		.addOnClick([] { GiveWeaponAttachmentMenuSelectedVars::m_Vars.m_SelectedWeapon = Menu::GetLocalPlayer().m_WeaponHash; })
		.addSubmenu<GiveWeaponAttachmentMenuSelected>()
		.addRequirement([] { return Menu::GetLocalPlayer().m_HasWeaponInHand && DoesWeaponHaveComponents(Menu::GetLocalPlayer().m_WeaponHash); }));

	addOption(BreakOption("Weapons")
		.addTranslation());
}

void GiveWeaponAttachmentMenu::Update() {}

/*Called once on submenu open*/
void GiveWeaponAttachmentMenu::UpdateOnce() {
	ClearOptionsOffset(4);

	for (auto& Weapon : Global::Arrays::g_Weapons) {
		if (DoesWeaponHaveComponents(Weapon.m_Result)) {
			addOption(SubmenuOption(Weapon.m_Name.Get())
				.addOnClick([=] { GiveWeaponAttachmentMenuSelectedVars::m_Vars.m_SelectedWeapon = Weapon.m_Result; })
				.addSubmenu<GiveWeaponAttachmentMenuSelected>());
		}
	}
}

/*Called always*/
void GiveWeaponAttachmentMenu::FeatureUpdate() {}

GiveWeaponAttachmentMenu* _instance;
GiveWeaponAttachmentMenu* GiveWeaponAttachmentMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new GiveWeaponAttachmentMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

GiveWeaponAttachmentMenu::~GiveWeaponAttachmentMenu() { delete _instance; }