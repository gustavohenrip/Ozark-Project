#include "give_weapon_tint.hpp"
#include "menu/base/submenu_handler.hpp"
#include "give_weapons.hpp"
#include "global/arrays.hpp"
#include "give_weapon_tint_selected.hpp"

using namespace GiveWeaponTintMenuVars;

namespace GiveWeaponTintMenuVars {
	Vars_ m_Vars;

	namespace Scripts {
		bool func_173(int iParam0) {
			switch (iParam0) {
				case joaat("WEAPON_PISTOL_MK2"):
				case joaat("WEAPON_SMG_MK2"):
				case joaat("WEAPON_ASSAULTRIFLE_MK2"):
				case joaat("WEAPON_CARBINERIFLE_MK2"):
				case joaat("WEAPON_COMBATMG_MK2"):
				case joaat("WEAPON_HEAVYSNIPER_MK2"):
				case joaat("WEAPON_PUMPSHOTGUN_MK2"):
				case joaat("WEAPON_SPECIALCARBINE_MK2"):
				case joaat("WEAPON_SNSPISTOL_MK2"):
				case joaat("WEAPON_MARKSMANRIFLE_MK2"):
				case joaat("WEAPON_REVOLVER_MK2"):
				case joaat("WEAPON_BULLPUPRIFLE_MK2"):
					return true;
			}
			return false;
		}

		int func_81(int iParam0, int* Data) {
			int iVar0;
			int iVar1;

			iVar1 = Native::GetNumDlcWeapons();
			iVar0 = 0;
			while (iVar0 < iVar1) {
				if (Native::GetDlcWeaponData(iVar0, Data)) {
					if (Data[2] == iParam0) {
						return iVar0;
					}
				}
				iVar0++;
			}
			return -1;
		}
	}

	bool DoesWeaponHaveTints(uint32_t Weapon) {
		switch (Weapon) {
			case joaat("WEAPON_KNUCKLE"):
			case joaat("WEAPON_DOUBLEACTION"):
				return false;
		}

		uint32_t Group = Native::GetWeapontypeGroup(Weapon);
		if (Group == 0xD49321D4 || Group == 0x5C4C5883 || Group == 0x5F1BE07C || Group == 0xFDBF656C) return false;

		return true;
	}

	std::vector<std::pair<std::string, int>> GetWeaponTints(int Weapon) {
		std::vector<std::pair<std::string, int>> Items;

		switch (Weapon) {
			case joaat("WEAPON_RAYPISTOL"): {
				for (int i = 0; i <= 6; i++) {
					Items.push_back({ "RWT_TINT" + std::to_string(i), i });
				}
				break;
			}

			case joaat("WEAPON_RAYCARBINE"):
			case joaat("WEAPON_RAYMINIGUN"): {
				Items.push_back({ "RWT_TINT7", 0 });
				Items.push_back({ "RWT_TINT1", 1 });
				Items.push_back({ "RWT_TINT2", 2 });
				Items.push_back({ "RWT_TINT3", 3 });
				Items.push_back({ "RWT_TINT4", 4 });
				Items.push_back({ "RWT_TINT5", 5 });
				Items.push_back({ "RWT_TINT6", 6 });
				break;
			}

			default: {
				int Data[100];

				if (Weapon == -61829581) {
					for (int i = 0; i <= 11; i++) {
						Items.push_back({ "FW_TINT" + std::to_string(i), i });
					}
				} else if (Native::NetworkIsGameInProgress() || Scripts::func_81(Weapon, Data) != -1) {
					if (Scripts::func_173(Weapon)) {
						for (int i = 0; i <= 31; i++) {
							Items.push_back({ "WCT_TINT_" + std::to_string(i), i });
						}
					} else {
						if ((((Weapon == joaat("WEAPON_MUSKET") || Weapon == joaat("WEAPON_FLAREGUN")) || Weapon == joaat("WEAPON_FIREWORK")) || Weapon == joaat("WEAPON_MARKSMANPISTOL")) || Weapon == joaat("WEAPON_DBSHOTGUN")) {
							Items.push_back({ "WM_TINTDF", 0 });
						} else {
							Items.push_back({ "WM_TINT0", 0 });
						}

						Items.push_back({ "WM_TINT4", 4 });
						Items.push_back({ "WM_TINT1", 1 });
						Items.push_back({ "WM_TINT6", 6 });
						Items.push_back({ "WM_TINT5", 5 });
						Items.push_back({ "WM_TINT3", 3 });
						Items.push_back({ "WM_TINT2", 2 });
						Items.push_back({ "WM_TINT7", 7 });
						// Items.push_back({ "WM_TINT8", 8 });
					}
				} else {
					Items.push_back({ "WM_TINT0", 0 });
					Items.push_back({ "WM_TINT4", 4 });
					Items.push_back({ "WM_TINT1", 1 });
					Items.push_back({ "WM_TINT6", 6 });
					Items.push_back({ "WM_TINT5", 5 });
					Items.push_back({ "WM_TINT3", 3 });
					Items.push_back({ "WM_TINT2", 2 });
					Items.push_back({ "WM_TINT7", 7 });
					// Items.push_back({ "WM_TINT8", 8 });
				}

				break;
			}
		}

		return Items;
	}
}

void GiveWeaponTintMenu::Init() {
	SetName("Give Weapon Tint");
	SetParentSubmenu<GiveWeaponsMenu>();

	addOption(SubmenuOption("Current Weapon")
		.addTranslation()
		.addOnClick([] { GiveWeaponTintSelectedMenuVars::m_Vars.m_Selected = Menu::GetLocalPlayer().m_WeaponHash; })
		.addSubmenu<GiveWeaponTintSelectedMenu>()
		.addRequirement([] { return Menu::GetLocalPlayer().m_HasWeaponInHand && DoesWeaponHaveTints(Menu::GetLocalPlayer().m_WeaponHash); }));

	addOption(BreakOption("Weapons")
		.addTranslation()
		.addRequirement([] { return Menu::GetLocalPlayer().m_HasWeaponInHand && DoesWeaponHaveTints(Menu::GetLocalPlayer().m_WeaponHash); }));
}

void GiveWeaponTintMenu::Update() {}

/*Called once on submenu open*/
void GiveWeaponTintMenu::UpdateOnce() {
	ClearOptionsOffset(2);

	for (auto& Weapon : Global::Arrays::g_Weapons) {
		if (DoesWeaponHaveTints(Weapon.m_Result)) {
			auto Camos = GetWeaponTints((int)Weapon.m_Result);
			if (Camos.size()) {
				addOption(SubmenuOption(Weapon.m_Name.Get())
					.addSubmenu<GiveWeaponTintSelectedMenu>()
					.addOnClick([=] { GiveWeaponTintSelectedMenuVars::m_Vars.m_Selected = Weapon.m_Result; }));
			}
		}
	}
}

/*Called always*/
void GiveWeaponTintMenu::FeatureUpdate() {}

GiveWeaponTintMenu* _instance;
GiveWeaponTintMenu* GiveWeaponTintMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new GiveWeaponTintMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

GiveWeaponTintMenu::~GiveWeaponTintMenu() { delete _instance; }