#include "ammunation.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../weapon.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/global.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/va.hpp"
#include "utils/fiber_pool.hpp"
#include "utils/fibers.hpp"

using namespace AmmunationMenuVars;

namespace AmmunationMenuVars {
	Vars_ m_Vars;

	std::vector<uint32_t> BlacklistedSubmenuWeapons = {
		0x92a27487,
		0x958a4a8f,
		0xf9e6aa4b,
		0x84bd7bfd,
		0x8bb05fd7,
		0x440e4788,
		0x4e875f73,
		0xf9dcbf2d,
		0xd8df3c3c,
		0x99b507ea,
		0xdd5df8d9,
		0xdfe37640,
		0x678b81b1,
		0x19044ee0,
		0xcd274149,
		0x94117305,
		0x3813fc08,
		0x34a67b97,
		0x24b17070,
		0xa0973d5e,
		0x2c3731d9,
		0xab564b93,
		0xba45e8b8,
		0xfdbc8a50,
		0x93e220bd
	};

	std::vector<uint32_t> BlacklistedWeapons = {
		0x497facc3,
		0xba536372,
		0xfdbadced,
		0x60ec506,
		0x23c9f95c,
		0x787f0bb,
		0x3656c8c1,
	};

	namespace Scripts {
		void StringIntConCat(char* Input, int Val, int Size) {
			strncat(Input, std::to_string(Val).c_str(), 64);
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

		const char* func_181(int iParam0, int iParam1) {
			int iVar0;
			int iVar1;
			static int Var41[100];

			if (iParam1 == joaat("WEAPON_KNUCKLE")) {
				switch (iParam0) {
					case joaat("COMPONENT_KNUCKLE_VARMOD_BASE"):
						return "WT_KNUCKLE";
					case joaat("COMPONENT_KNUCKLE_VARMOD_PIMP"):
						return "WCT_KNUCK_02";
					case joaat("COMPONENT_KNUCKLE_VARMOD_BALLAS"):
						return "WCT_KNUCK_BG";
					case joaat("COMPONENT_KNUCKLE_VARMOD_DOLLAR"):
						return "WCT_KNUCK_DLR";
					case joaat("COMPONENT_KNUCKLE_VARMOD_DIAMOND"):
						return "WCT_KNUCK_DMD";
					case joaat("COMPONENT_KNUCKLE_VARMOD_HATE"):
						return "WCT_KNUCK_HT";
					case joaat("COMPONENT_KNUCKLE_VARMOD_LOVE"):
						return "WCT_KNUCK_LV";
					case joaat("COMPONENT_KNUCKLE_VARMOD_PLAYER"):
						return "WCT_KNUCK_PC";
					case joaat("COMPONENT_KNUCKLE_VARMOD_KING"):
						return "WCT_KNUCK_SLG";
					case joaat("COMPONENT_KNUCKLE_VARMOD_VAGOS"):
						return "WCT_KNUCK_VG";
				}
			}
			switch (iParam0) {
				case 0:
					return "WCT_NONE";
				case joaat("COMPONENT_AT_RAILCOVER_01"):
					return "WCT_RAIL";
				case joaat("COMPONENT_AT_AR_AFGRIP"):
					return "WCT_GRIP";
				case joaat("COMPONENT_AT_PI_FLSH"):
					return "WCT_FLASH";
				case joaat("COMPONENT_AT_AR_FLSH"):
					return "WCT_FLASH";
				case joaat("COMPONENT_AT_SCOPE_MACRO"):
					return "WCT_SCOPE_MAC";
				case joaat("COMPONENT_AT_SCOPE_MACRO_02"):
					return "WCT_SCOPE_MAC";
				case joaat("COMPONENT_AT_SCOPE_SMALL"):
					return "WCT_SCOPE_SML";
				case joaat("COMPONENT_AT_SCOPE_SMALL_02"):
					return "WCT_SCOPE_SML";
				case joaat("COMPONENT_AT_SCOPE_MEDIUM"):
					return "WCT_SCOPE_MED";
				case joaat("COMPONENT_AT_SCOPE_LARGE"):
					return "WCT_SCOPE_LRG";
				case joaat("COMPONENT_AT_SCOPE_MAX"):
					return "WCT_SCOPE_MAX";
				case joaat("COMPONENT_AT_PI_SUPP"):
					return "WCT_SUPP";
				case joaat("COMPONENT_AT_AR_SUPP"):
					return "WCT_SUPP";
				case joaat("COMPONENT_AT_AR_SUPP_02"):
					return "WCT_SUPP";
				case joaat("COMPONENT_AT_SR_SUPP"):
					return "WCT_SUPP";
				case joaat("COMPONENT_PISTOL_CLIP_01"):
					return "WCT_CLIP1";
				case joaat("COMPONENT_PISTOL_CLIP_02"):
					return "WCT_CLIP2";
				case joaat("COMPONENT_COMBATPISTOL_CLIP_01"):
					return "WCT_CLIP1";
				case joaat("COMPONENT_COMBATPISTOL_CLIP_02"):
					return "WCT_CLIP2";
				case joaat("COMPONENT_APPISTOL_CLIP_01"):
					return "WCT_CLIP1";
				case joaat("COMPONENT_APPISTOL_CLIP_02"):
					return "WCT_CLIP2";
				case joaat("COMPONENT_MICROSMG_CLIP_01"):
					return "WCT_CLIP1";
				case joaat("COMPONENT_MICROSMG_CLIP_02"):
					return "WCT_CLIP2";
				case joaat("COMPONENT_SMG_CLIP_01"):
					return "WCT_CLIP1";
				case joaat("COMPONENT_SMG_CLIP_02"):
					return "WCT_CLIP2";
				case joaat("COMPONENT_ASSAULTRIFLE_CLIP_01"):
					return "WCT_CLIP1";
				case joaat("COMPONENT_ASSAULTRIFLE_CLIP_02"):
					return "WCT_CLIP2";
				case joaat("COMPONENT_CARBINERIFLE_CLIP_01"):
					return "WCT_CLIP1";
				case joaat("COMPONENT_CARBINERIFLE_CLIP_02"):
					return "WCT_CLIP2";
				case joaat("COMPONENT_ADVANCEDRIFLE_CLIP_01"):
					return "WCT_CLIP1";
				case joaat("COMPONENT_ADVANCEDRIFLE_CLIP_02"):
					return "WCT_CLIP2";
				case joaat("COMPONENT_MG_CLIP_01"):
					return "WCT_CLIP1";
				case joaat("COMPONENT_MG_CLIP_02"):
					return "WCT_CLIP2";
				case joaat("COMPONENT_COMBATMG_CLIP_01"):
					return "WCT_CLIP1";
				case joaat("COMPONENT_COMBATMG_CLIP_02"):
					return "WCT_CLIP2";
				case joaat("COMPONENT_ASSAULTSHOTGUN_CLIP_01"):
					return "WCT_CLIP1";
				case joaat("COMPONENT_ASSAULTSHOTGUN_CLIP_02"):
					return "WCT_CLIP2";
				case joaat("COMPONENT_SNIPERRIFLE_CLIP_01"):
					return "WCT_CLIP1";
				case joaat("COMPONENT_HEAVYSNIPER_CLIP_01"):
					return "WCT_CLIP1";
				case joaat("COMPONENT_MINIGUN_CLIP_01"):
					return "WCT_CLIP2";
				case joaat("COMPONENT_ASSAULTSMG_CLIP_01"):
					return "WCT_CLIP1";
				case joaat("COMPONENT_ASSAULTSMG_CLIP_02"):
					return "WCT_CLIP2";
				case joaat("COMPONENT_PISTOL50_CLIP_01"):
					return "WCT_CLIP1";
				case joaat("COMPONENT_PISTOL50_CLIP_02"):
					return "WCT_CLIP2";
				case 195735895:
					return "WCT_CLIP1";
				case 1525977990:
					return "WCT_CLIP1";
				case 1824470811:
					return "WCT_CLIP2";
				case -890514874:
					return "WCT_CLIP1";
				case -507117574:
					return "WCT_CLIP2";
				case -124428919:
					return "WCT_CLIP1";
				case 1048471894:
					return "WCT_CLIP2";
				case joaat("COMPONENT_SNSPISTOL_CLIP_01"):
					return "WCT_CLIP1";
				case joaat("COMPONENT_SNSPISTOL_CLIP_02"):
					return "WCT_CLIP2";
				case joaat("COMPONENT_VINTAGEPISTOL_CLIP_01"):
					return "WCT_CLIP1";
				case joaat("COMPONENT_VINTAGEPISTOL_CLIP_02"):
					return "WCT_CLIP2";
				case joaat("COMPONENT_HEAVYSHOTGUN_CLIP_01"):
					return "WCT_CLIP1";
				case joaat("COMPONENT_MARKSMANRIFLE_CLIP_01"):
					return "WCT_CLIP1";
				case joaat("COMPONENT_HEAVYSHOTGUN_CLIP_02"):
					return "WCT_CLIP2";
				case joaat("COMPONENT_MARKSMANRIFLE_CLIP_02"):
					return "WCT_CLIP2";
				case joaat("COMPONENT_AT_SCOPE_LARGE_FIXED_ZOOM"):
					return "WCT_SCOPE_LRG";
				case joaat("COMPONENT_AT_PI_SUPP_02"):
					return "WCT_SUPP";
				case joaat("COMPONENT_COMBATPDW_CLIP_01"):
					return "WCT_CLIP1";
				case joaat("COMPONENT_COMBATPDW_CLIP_02"):
					return "WCT_CLIP2";
				case joaat("COMPONENT_MARKSMANPISTOL_CLIP_01"):
					return "WCT_CLIP1";
				case joaat("COMPONENT_MACHINEPISTOL_CLIP_01"):
					return "WCT_CLIP1";
				case joaat("COMPONENT_MACHINEPISTOL_CLIP_02"):
					return "WCT_CLIP2";
				case joaat("COMPONENT_ASSAULTRIFLE_VARMOD_LUXE"):
					return "WCT_VAR_GOLD";
				case joaat("COMPONENT_ADVANCEDRIFLE_VARMOD_LUXE"):
					return "WCT_VAR_METAL";
				case joaat("COMPONENT_CARBINERIFLE_VARMOD_LUXE"):
					return "WCT_VAR_GOLD";
				case joaat("COMPONENT_APPISTOL_VARMOD_LUXE"):
					return "WCT_VAR_METAL";
				case joaat("COMPONENT_PISTOL_VARMOD_LUXE"):
					return "WCT_VAR_GOLD";
				case joaat("COMPONENT_PISTOL50_VARMOD_LUXE"):
					return "WCT_VAR_SIL";
				case joaat("COMPONENT_HEAVYPISTOL_VARMOD_LUXE"):
					return "WCT_VAR_WOOD";
				case joaat("COMPONENT_SMG_VARMOD_LUXE"):
					return "WCT_VAR_GOLD";
				case joaat("COMPONENT_MICROSMG_VARMOD_LUXE"):
					return "WCT_VAR_GOLD";
				case joaat("COMPONENT_SAWNOFFSHOTGUN_VARMOD_LUXE"):
					return "WCT_VAR_METAL";
				case joaat("COMPONENT_SNIPERRIFLE_VARMOD_LUXE"):
					return "WCT_VAR_WOOD";
				case joaat("COMPONENT_MARKSMANRIFLE_VARMOD_LUXE"):
					return "WCT_VAR_GOLD";
				case joaat("COMPONENT_ASSAULTSMG_VARMOD_LOWRIDER"):
					return "WCT_VAR_GOLD";
				case joaat("COMPONENT_BULLPUPRIFLE_VARMOD_LOW"):
					return "WCT_VAR_METAL";
				case joaat("COMPONENT_COMBATMG_VARMOD_LOWRIDER"):
					return "WCT_VAR_ETCHM";
				case joaat("COMPONENT_COMBATPISTOL_VARMOD_LOWRIDER"):
					return "WCT_VAR_GOLD";
				case joaat("COMPONENT_MG_VARMOD_LOWRIDER"):
					return "WCT_VAR_GOLD";
				case joaat("COMPONENT_PUMPSHOTGUN_VARMOD_LOWRIDER"):
					return "WCT_VAR_GOLD";
				case joaat("COMPONENT_SNSPISTOL_VARMOD_LOWRIDER"):
					return "WCT_VAR_WOOD";
				case joaat("COMPONENT_SPECIALCARBINE_VARMOD_LOWRIDER"):
					return "WCT_VAR_ETCHM";
				case joaat("COMPONENT_SWITCHBLADE_VARMOD_BASE"):
					return "WCT_SB_BASE";
				case joaat("COMPONENT_SWITCHBLADE_VARMOD_VAR1"):
					return "WCT_SB_VAR1";
				case joaat("COMPONENT_SWITCHBLADE_VARMOD_VAR2"):
					return "WCT_SB_VAR2";
				case joaat("COMPONENT_REVOLVER_CLIP_01"):
					return "WCT_CLIP1";
				case joaat("COMPONENT_REVOLVER_VARMOD_BOSS"):
					return "WCT_REV_VARB";
				case joaat("COMPONENT_REVOLVER_VARMOD_GOON"):
					return "WCT_REV_VARG";
				case joaat("COMPONENT_SMG_CLIP_03"):
					return "WCT_CLIP_DRM";
				case joaat("COMPONENT_CARBINERIFLE_CLIP_03"):
					return "WCT_CLIP_BOX";
				case joaat("COMPONENT_ASSAULTRIFLE_CLIP_03"):
					return "WCT_CLIP_DRM";
				case joaat("COMPONENT_HEAVYSHOTGUN_CLIP_03"):
					return "WCT_CLIP_DRM";
				case joaat("COMPONENT_GUNRUN_MK2_UPGRADE"):
					return "WCT_VAR_GUN";
				case joaat("COMPONENT_MARKSMANRIFLE_MK2_CAMO_IND_01"):
					return "WCT_CAMO_IND";
				case joaat("COMPONENT_SNSPISTOL_MK2_CAMO_IND_01"):
					return "WCT_CAMO_IND";
				case joaat("COMPONENT_SNSPISTOL_MK2_CLIP_02"):
					return "WCT_CLIP2";
				case joaat("COMPONENT_AT_SCOPE_MACRO_02_MK2"):
					return "WCT_SCOPE_MAC2";
				case joaat("COMPONENT_AT_SCOPE_SMALL_MK2"):
					return "WCT_SCOPE_SML2";
				case joaat("COMPONENT_RAYPISTOL_VARMOD_XMAS18"):
					return "WCT_VAR_RAY18";
				case joaat("COMPONENT_CERAMICPISTOL_CLIP_01"):
					return "WCT_CLIP1";
				case joaat("COMPONENT_CERAMICPISTOL_CLIP_02"):
					return "WCT_CLIP2";
				case joaat("COMPONENT_CERAMICPISTOL_SUPP"):
					return "WCT_SUPP";
				default:
					if (iParam1 != 0) {
						iVar0 = func_81(iParam1, Var41);
						if (iVar0 != -1) {
							iVar1 = 0;
							while (iVar1 < Native::GetNumDlcWeaponComponents(iVar0)) {
								if (Native::GetDlcWeaponComponentData(iVar0, iVar1, Var41)) {
									if (Var41[6] == iParam0) {
										return (const char*)&Var41[12];
									}
								}
								iVar1++;
							}
						}
					}
					break;
			}
			return "WCT_INVALID";
		}

		int func_58(int iParam0) {
			switch (iParam0) {
				case joaat("WEAPON_UNARMED"):
					return 0;
				case joaat("WEAPON_PISTOL"):
					return 1;
				case joaat("WEAPON_COMBATPISTOL"):
					return 2;
				case joaat("WEAPON_APPISTOL"):
					return 3;
				case joaat("WEAPON_PISTOL50"):
					return 4;
				case joaat("WEAPON_SMG"):
					return 5;
				case joaat("WEAPON_ASSAULTSMG"):
					return 6;
				case joaat("WEAPON_MICROSMG"):
					return 7;
				case joaat("WEAPON_ASSAULTRIFLE"):
					return 8;
				case joaat("WEAPON_CARBINERIFLE"):
					return 9;
				case joaat("WEAPON_HEAVYRIFLE"):
					return 10;
				case joaat("WEAPON_ADVANCEDRIFLE"):
					return 11;
				case joaat("WEAPON_MG"):
					return 12;
				case joaat("WEAPON_COMBATMG"):
					return 13;
				case joaat("WEAPON_ASSAULTMG"):
					return 14;
				case joaat("WEAPON_STICKYBOMB"):
					return 15;
				case joaat("WEAPON_GRENADE"):
					return 16;
				case joaat("WEAPON_SMOKEGRENADE"):
					return 17;
				case joaat("WEAPON_REMOTESNIPER"):
					return 18;
				case joaat("WEAPON_ASSAULTSNIPER"):
					return 19;
				case joaat("WEAPON_SNIPERRIFLE"):
					return 20;
				case joaat("WEAPON_HEAVYSNIPER"):
					return 21;
				case joaat("WEAPON_PUMPSHOTGUN"):
					return 22;
				case joaat("WEAPON_BULLPUPSHOTGUN"):
					return 23;
				case joaat("WEAPON_ASSAULTSHOTGUN"):
					return 24;
				case joaat("WEAPON_SAWNOFFSHOTGUN"):
					return 25;
				case joaat("WEAPON_GRENADELAUNCHER"):
					return 26;
				case joaat("WEAPON_RPG"):
					return 27;
				case joaat("WEAPON_MINIGUN"):
					return 28;
				case joaat("WEAPON_LOUDHAILER"):
					return 29;
				case joaat("WEAPON_PROGRAMMABLEAR"):
					return 30;
				case joaat("WEAPON_STUNGUN"):
					return 31;
				case joaat("WEAPON_RUBBERGUN"):
					return 32;
				case joaat("GADGET_PARACHUTE"):
					return 33;
				case joaat("WEAPON_KNIFE"):
					return 34;
				case joaat("WEAPON_NIGHTSTICK"):
					return 35;
				case joaat("WEAPON_HAMMER"):
					return 36;
				case joaat("WEAPON_BAT"):
					return 37;
				case joaat("WEAPON_CROWBAR"):
					return 38;
				case joaat("WEAPON_GOLFCLUB"):
					return 39;
				case joaat("WEAPON_GRENADELAUNCHER_SMOKE"):
					return 40;
				case joaat("WEAPON_MOLOTOV"):
					return 41;
				case joaat("WEAPON_FIREEXTINGUISHER"):
					return 42;
				case joaat("WEAPON_PETROLCAN"):
					return 43;
				case joaat("WEAPON_DIGISCANNER"):
					return 44;
				case joaat("WEAPON_BOTTLE"):
					return 45;
				case joaat("WEAPON_SPECIALCARBINE"):
					return 46;
				case joaat("WEAPON_SNSPISTOL"):
					return 47;
				case joaat("WEAPON_HEAVYPISTOL"):
					return 49;
				case joaat("WEAPON_BULLPUPRIFLE"):
					return 48;
				case joaat("WEAPON_GUSENBERG"):
					return 50;
				case joaat("WEAPON_DAGGER"):
					return 51;
				case joaat("WEAPON_VINTAGEPISTOL"):
					return 52;
				case joaat("WEAPON_FLAREGUN"):
					return 57;
				case joaat("WEAPON_MUSKET"):
					return 53;
				case joaat("WEAPON_FIREWORK"):
					return 54;
				case joaat("WEAPON_MARKSMANRIFLE"):
					return 56;
				case joaat("WEAPON_HEAVYSHOTGUN"):
					return 55;
				case joaat("WEAPON_PROXMINE"):
					return 60;
				case joaat("WEAPON_HOMINGLAUNCHER"):
					return 61;
				case joaat("WEAPON_HATCHET"):
					return 58;
				case joaat("WEAPON_RAILGUN"):
					return 59;
				case joaat("WEAPON_COMBATPDW"):
					return 64;
				case joaat("WEAPON_KNUCKLE"):
					return 62;
				case joaat("WEAPON_MARKSMANPISTOL"):
					return 63;
				case joaat("WEAPON_MACHETE"):
					return 65;
				case joaat("WEAPON_MACHINEPISTOL"):
					return 68;
				case joaat("WEAPON_DBSHOTGUN"):
					return 66;
				case joaat("WEAPON_COMPACTRIFLE"):
					return 67;
				case joaat("WEAPON_FLASHLIGHT"):
					return 69;
				case joaat("WEAPON_REVOLVER"):
					return 70;
				case joaat("WEAPON_SWITCHBLADE"):
					return 71;
				case joaat("WEAPON_AUTOSHOTGUN"):
					return 72;
				case joaat("WEAPON_MINISMG"):
					return 73;
				case joaat("WEAPON_COMPACTLAUNCHER"):
					return 74;
				case joaat("WEAPON_BATTLEAXE"):
					return 75;
				case joaat("WEAPON_PIPEBOMB"):
					return 76;
				case joaat("WEAPON_POOLCUE"):
					return 77;
				case joaat("WEAPON_WRENCH"):
					return 78;
				case joaat("WEAPON_ASSAULTRIFLE_MK2"):
					return 8;
				case joaat("WEAPON_CARBINERIFLE_MK2"):
					return 9;
				case joaat("WEAPON_COMBATMG_MK2"):
					return 13;
				case joaat("WEAPON_HEAVYSNIPER_MK2"):
					return 21;
				case joaat("WEAPON_PISTOL_MK2"):
					return 1;
				case joaat("WEAPON_SMG_MK2"):
					return 5;
				case joaat("WEAPON_PUMPSHOTGUN_MK2"):
					return 22;
				case joaat("WEAPON_SPECIALCARBINE_MK2"):
					return 46;
				case joaat("WEAPON_SNSPISTOL_MK2"):
					return 47;
				case joaat("WEAPON_MARKSMANRIFLE_MK2"):
					return 56;
				case joaat("WEAPON_REVOLVER_MK2"):
					return 70;
				case joaat("WEAPON_BULLPUPRIFLE_MK2"):
					return 48;
				case joaat("WEAPON_DOUBLEACTION"):
					return 79;
				case joaat("WEAPON_STONE_HATCHET"):
					return 80;
				case joaat("WEAPON_RAYPISTOL"):
					return 81;
				case joaat("WEAPON_RAYCARBINE"):
					return 82;
				case joaat("WEAPON_RAYMINIGUN"):
					return 83;
				case joaat("WEAPON_NAVYREVOLVER"):
					return 84;
				case joaat("WEAPON_CERAMICPISTOL"):
					return 85;
			}
			return 0;
		}

		int func_57(int iParam0) {
			return func_58(iParam0);
		}

		int func_117(int iParam0) {
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
					return 2;
			}
			return -1;
		}

		void func_116(char* sParam0, const char* cParam1, uint32_t iParam5, int iParam6, int iParam7, int iParam8, int iParam9, int iParam10, bool bParam11, bool bParam12) {
			switch ((int)Native::GetHashKey(cParam1)) {
				case 64715401: /* GXTEntry: "Round" */
				case -2016195117: /* GXTEntry: "Rounds" */
					cParam1 = "GSA_TYPE_R";
					break;
				case -414529079: /* GXTEntry: "Cartridge" */
				case -1253005183: /* GXTEntry: "Cartridges" */
					cParam1 = "GSA_TYPE_C";
					break;
				case 336264847: /* GXTEntry: "Rocket" */
				case -1119195824: /* GXTEntry: "Rockets" */
					cParam1 = "GSA_TYPE_RO";
					break;
				case 531395379: /* GXTEntry: "Charge" */
				case -672224483: /* GXTEntry: "Charges" */
					cParam1 = "GSA_TYPE_CH";
					break;
				case 1034118160: /* GXTEntry: "Grenade" */
				case 600999375: /* GXTEntry: "Grenades" */
					cParam1 = "GSA_TYPE_G";
					break;
				case -218834291: /* GXTEntry: "Bomb" */
				case -859056265: /* GXTEntry: "Bombs" */
					cParam1 = "GSA_TYPE_B";
					break;
				case 1779531303: /* GXTEntry: "Firework" */
				case 82051787: /* GXTEntry: "Fireworks" */
					cParam1 = "GSA_TYPE_FW";
					break;
			}

			strncpy(sParam0, "", 64);
			switch (iParam6) {
				case 0:
					if (iParam5 == joaat("MP_M_FREEMODE_01")) {
						strncpy(sParam0, "M", 64);
					} else if (iParam5 == joaat("MP_F_FREEMODE_01")) {
						strncpy(sParam0, "F", 64);
					}
					strncat(sParam0, "_HA_", 64);
					strncat(sParam0, cParam1, 64);
					strncat(sParam0, "_t", 64);
					StringIntConCat(sParam0, iParam7, 64);
					strncat(sParam0, "_v", 64);
					StringIntConCat(sParam0, iParam8, 64);
					break;
				case 1:
					if (iParam5 == joaat("MP_M_FREEMODE_01")) {
						strncpy(sParam0, "M", 64);
					} else if (iParam5 == joaat("MP_F_FREEMODE_01")) {
						strncpy(sParam0, "F", 64);
					}
					strncat(sParam0, "_CL_", 64);
					if (iParam7 == 12) {
						strncat(sParam0, "OUTFIT_", 64);
					}
					strncat(sParam0, cParam1, 64);
					if (iParam9 != -1) {
						strncat(sParam0, "_n", 64);
						StringIntConCat(sParam0, iParam9, 64);
					}
					strncat(sParam0, "_t", 64);
					StringIntConCat(sParam0, iParam7, 64);
					strncat(sParam0, "_v", 64);
					StringIntConCat(sParam0, iParam8, 64);
					if (iParam10 != -1) {
						strncat(sParam0, "_p", 64);
						StringIntConCat(sParam0, iParam10, 64);
					}
					break;
				case 2:
					if (iParam5 == joaat("MP_M_FREEMODE_01")) {
						strncpy(sParam0, "M", 64);
					} else if (iParam5 == joaat("MP_F_FREEMODE_01")) {
						strncpy(sParam0, "F", 64);
					}
					strncat(sParam0, "_TA_", 64);
					strncat(sParam0, cParam1, 64);
					strncat(sParam0, "_t", 64);
					StringIntConCat(sParam0, iParam7, 64);
					strncat(sParam0, "_v", 64);
					StringIntConCat(sParam0, iParam8, 64);
					break;
				case 3:
					strncat(sParam0, "WP_", 64);
					strncat(sParam0, cParam1, 64);
					strncat(sParam0, "_t", 64);
					StringIntConCat(sParam0, iParam7, 64);
					strncat(sParam0, "_v", 64);
					StringIntConCat(sParam0, iParam8, 64);
					if (iParam9 != -1) {
						strncat(sParam0, "_n", 64);
						StringIntConCat(sParam0, iParam9, 64);
					}
					break;
				case 4:
					if (iParam5 == Menu::Global(74009).As<int>()) {
						strncpy(sParam0, "VE_BIG_YACHT_t0_v0", 64);
					} else if (iParam5 == Menu::Global(74010).As<int>()) {
						strncpy(sParam0, "VE_BIG_TRUCK_t0_v0", 64);
					} else if (iParam5 == Menu::Global(74011).As<int>()) {
						strncpy(sParam0, "VE_BALLISTIC_EQUIPMENT_t0_v0", 64);
					} else if (iParam5 == Menu::Global(74012).As<int>()) {
						strncpy(sParam0, "VE_BIG_PLANE_t0_v0", 64);
					} else if (iParam5 == Menu::Global(74013).As<int>()) {
						strncpy(sParam0, "VE_HACKER_TRUCK_t0_v0", 64);
					} else if (iParam5 == 0) {
						strncat(sParam0, "VEM_", 64);
						if (iParam7 == 24 || iParam7 == 25) {
							if (iParam9 == 1) {
								strncat(sParam0, "COLOUR_1_", 64);
							} else if (iParam9 == 2) {
								strncat(sParam0, "COLOUR_2_", 64);
							} else if (iParam9 == 3) {
								strncat(sParam0, "COLOUR_EXTRA_1_", 64);
							}
							if (iParam10 == 5) {
								strncat(sParam0, "MCT_CHROME_", 64);
							} else if (iParam10 == 1) {
								strncat(sParam0, "MCT_CLASSIC_", 64);
							} else if (iParam10 == 0) {
								strncat(sParam0, "MCT_METALLIC_", 64);
							} else if (iParam10 == 4) {
								strncat(sParam0, "MCT_METALS_", 64);
							} else if (iParam10 == 3) {
								strncat(sParam0, "MCT_MATTE_", 64);
							} else if (iParam10 == 2) {
								strncat(sParam0, "MCT_PEARLESCENT_", 64);
							} else if (iParam10 == 6) {
								strncat(sParam0, "MCT_NONE_", 64);
							}
							iParam9 = -1;
						} else if (iParam7 == 38) {
							strncat(sParam0, "COLOUR_5_", 64);
						} else if (iParam7 == 65) {
							strncat(sParam0, "COLOUR_6_", 64);
						}
						strncat(sParam0, cParam1, 64);
						if (iParam9 != -1) {
							strncat(sParam0, "_n", 64);
							StringIntConCat(sParam0, iParam9, 64);
						}
						strncat(sParam0, "_t", 64);
						StringIntConCat(sParam0, iParam7, 64);
						strncat(sParam0, "_v", 64);
						StringIntConCat(sParam0, iParam8, 64);
					} else if (iParam7 == 22) {
						cParam1 = Native::GetDisplayNameFromVehicleModel(iParam5);
						if (Native::IsStringNullOrEmpty(cParam1) || Native::GetHashKey(cParam1) == -515263000) {
							return;
						}

						strncat(sParam0, "VEM_INSURANCE_", 64);
						strncat(sParam0, cParam1, 64);
						if (!bParam11) {
							switch (iParam5) {
								case joaat("DUBSTA2"):
								case joaat("CAVALCADE2"):
								case joaat("MESA2"):
								case joaat("RAPIDGT2"):
								case joaat("EMPEROR2"):
								case joaat("CARGOBOB2"):
									strncat(sParam0, "2", 64);
									break;
								case joaat("MESA3"):
								case joaat("EMPEROR3"):
								case joaat("BURRITO3"):
								case joaat("MULE3"):
									strncat(sParam0, "3", 64);
									break;
								case joaat("TORNADO4"):
									strncat(sParam0, "4", 64);
									break;
							}
						}
					} else if (iParam7 == 40) {
						switch (iParam5) {
							case joaat("FCR2"):
								cParam1 = "FCR2";
								break;
							case joaat("DIABLOUS2"):
								cParam1 = "DIABLOUS2";
								break;
							case joaat("COMET3"):
								cParam1 = "COMET3";
								break;
							default:
								cParam1 = Native::GetDisplayNameFromVehicleModel(iParam5);
								if (Native::IsStringNullOrEmpty(cParam1)) {
									return;
								}

								break;
						}
						strncat(sParam0, "VEU_", 64);
						strncat(sParam0, cParam1, 64);
						strncat(sParam0, "_t0_v", 64);
						StringIntConCat(sParam0, iParam8, 64);
					} else {
						strncat(sParam0, "VE_", 64);
						strncat(sParam0, cParam1, 64);
						if (!bParam11) {
							switch (iParam5) {
								case joaat("DUBSTA2"):
								case joaat("CAVALCADE2"):
								case joaat("MESA2"):
								case joaat("RAPIDGT2"):
								case joaat("EMPEROR2"):
								case joaat("CARGOBOB2"):
									strncat(sParam0, "2", 64);
									break;
								case joaat("MESA3"):
								case joaat("EMPEROR3"):
								case joaat("BURRITO3"):
								case joaat("MULE3"):
									strncat(sParam0, "3", 64);
									break;
								case joaat("TORNADO4"):
									strncat(sParam0, "4", 64);
									break;
							}
						}
						if (iParam9 != -1) {
							strncat(sParam0, "_n", 64);
							StringIntConCat(sParam0, iParam9, 64);
						}
						strncat(sParam0, "_t", 64);
						StringIntConCat(sParam0, iParam7, 64);
						strncat(sParam0, "_v", 64);
						StringIntConCat(sParam0, iParam8, 64);
						if (iParam10 != -1) {
							strncat(sParam0, "_p", 64);
							StringIntConCat(sParam0, iParam10, 64);
						}
					}
					break;
			}
			if (bParam12) {
				strncat(sParam0, "_CESP", 64);
			}
		}

		char* func_229(int iParam0, bool bParam1) {
			int Var0[0x100];

			switch (iParam0) {
				case 0:
					if (bParam1) {
						return "WTU_INVALID";
					} else {
						return "WT_INVALID";
					}
					break;
				case joaat("WEAPON_UNARMED"):
					if (bParam1) {
						return "WTU_UNARMED";
					} else {
						return "WT_UNARMED";
					}
					break;
				case joaat("WEAPON_PISTOL"):
					if (bParam1) {
						return "WTU_PIST";
					} else {
						return "WT_PIST";
					}
					break;
				case joaat("WEAPON_COMBATPISTOL"):
					if (bParam1) {
						return "WTU_PIST_CBT";
					} else {
						return "WT_PIST_CBT";
					}
					break;
				case joaat("WEAPON_APPISTOL"):
					if (bParam1) {
						return "WTU_PIST_AP";
					} else {
						return "WT_PIST_AP";
					}
					break;
				case joaat("WEAPON_SMG"):
					if (bParam1) {
						return "WTU_SMG";
					} else {
						return "WT_SMG";
					}
					break;
				case joaat("WEAPON_MICROSMG"):
					if (bParam1) {
						return "WTU_SMG_MCR";
					} else {
						return "WT_SMG_MCR";
					}
					break;
				case joaat("WEAPON_ASSAULTRIFLE"):
					if (bParam1) {
						return "WTU_RIFLE_ASL";
					} else {
						return "WT_RIFLE_ASL";
					}
					break;
				case joaat("WEAPON_CARBINERIFLE"):
					if (bParam1) {
						return "WTU_RIFLE_CBN";
					} else {
						return "WT_RIFLE_CBN";
					}
					break;
				case joaat("WEAPON_ADVANCEDRIFLE"):
					if (bParam1) {
						return "WTU_RIFLE_ADV";
					} else {
						return "WT_RIFLE_ADV";
					}
					break;
				case joaat("WEAPON_MG"):
					if (bParam1) {
						return "WTU_MG";
					} else {
						return "WT_MG";
					}
					break;
				case joaat("WEAPON_COMBATMG"):
					if (bParam1) {
						return "WTU_MG_CBT";
					} else {
						return "WT_MG_CBT";
					}
					break;
				case joaat("WEAPON_PUMPSHOTGUN"):
					if (bParam1) {
						return "WTU_SG_PMP";
					} else {
						return "WT_SG_PMP";
					}
					break;
				case joaat("WEAPON_SAWNOFFSHOTGUN"):
					if (bParam1) {
						return "WTU_SG_SOF";
					} else {
						return "WT_SG_SOF";
					}
					break;
				case joaat("WEAPON_ASSAULTSHOTGUN"):
					if (bParam1) {
						return "WTU_SG_ASL";
					} else {
						return "WT_SG_ASL";
					}
					break;
				case joaat("WEAPON_HEAVYSNIPER"):
					if (bParam1) {
						return "WTU_SNIP_HVY";
					} else {
						return "WT_SNIP_HVY";
					}
					break;
				case joaat("WEAPON_REMOTESNIPER"):
					if (bParam1) {
						return "WTU_SNIP_RMT";
					} else {
						return "WT_SNIP_RMT";
					}
					break;
				case joaat("WEAPON_SNIPERRIFLE"):
					if (bParam1) {
						return "WTU_SNIP_RIF";
					} else {
						return "WT_SNIP_RIF";
					}
					break;
				case joaat("WEAPON_GRENADELAUNCHER"):
					if (bParam1) {
						return "WTU_GL";
					} else {
						return "WT_GL";
					}
					break;
				case joaat("WEAPON_RPG"):
					if (bParam1) {
						return "WTU_RPG";
					} else {
						return "WT_RPG";
					}
					break;
				case joaat("WEAPON_MINIGUN"):
					if (bParam1) {
						return "WTU_MINIGUN";
					} else {
						return "WT_MINIGUN";
					}
					break;
				case joaat("WEAPON_GRENADE"):
					if (bParam1) {
						return "WTU_GNADE";
					} else {
						return "WT_GNADE";
					}
					break;
				case joaat("WEAPON_SMOKEGRENADE"):
					if (bParam1) {
						return "WTU_GNADE_SMK";
					} else {
						return "WT_GNADE_SMK";
					}
					break;
				case joaat("WEAPON_STICKYBOMB"):
					if (bParam1) {
						return "WTU_GNADE_STK";
					} else {
						return "WT_GNADE_STK";
					}
					break;
				case joaat("WEAPON_MOLOTOV"):
					if (bParam1) {
						return "WTU_MOLOTOV";
					} else {
						return "WT_MOLOTOV";
					}
					break;
				case joaat("WEAPON_STUNGUN"):
					if (bParam1) {
						return "WTU_STUN";
					} else {
						return "WT_STUN";
					}
					break;
				case joaat("WEAPON_PETROLCAN"):
					if (bParam1) {
						return "WTU_PETROL";
					} else {
						return "WT_PETROL";
					}
					break;
				case joaat("WEAPON_ELECTRIC_FENCE"):
					if (bParam1) {
						return "WTU_ELCFEN";
					} else {
						return "WT_ELCFEN";
					}
					break;
				case joaat("VEHICLE_WEAPON_TANK"):
					if (bParam1) {
						return "WTU_V_TANK";
					} else {
						return "WT_V_TANK";
					}
					break;
				case joaat("VEHICLE_WEAPON_SPACE_ROCKET"):
					if (bParam1) {
						return "WTU_V_SPACERKT";
					} else {
						return "WT_V_SPACERKT";
					}
					break;
				case joaat("VEHICLE_WEAPON_PLAYER_LASER"):
					if (bParam1) {
						return "WTU_V_PLRLSR";
					} else {
						return "WT_V_PLRLSR";
					}
					break;
				case joaat("OBJECT"):
					if (bParam1) {
						return "WTU_OBJECT";
					} else {
						return "WT_OBJECT";
					}
					break;
				case joaat("GADGET_PARACHUTE"):
					if (bParam1) {
						return "WTU_PARA";
					} else {
						return "WT_PARA";
					}
					break;
				case joaat("AMMO_RPG"):
					if (bParam1) {
						return "WTU_A_RPG";
					} else {
						return "WT_A_RPG";
					}
					break;
				case joaat("AMMO_TANK"):
					if (bParam1) {
						return "WTU_A_TANK";
					} else {
						return "WT_A_TANK";
					}
					break;
				case joaat("AMMO_SPACE_ROCKET"):
					if (bParam1) {
						return "WTU_A_SPACERKT";
					} else {
						return "WT_A_SPACERKT";
					}
					break;
				case joaat("AMMO_PLAYER_LASER"):
					if (bParam1) {
						return "WTU_A_PLRLSR";
					} else {
						return "WT_A_PLRLSR";
					}
					break;
				case joaat("AMMO_ENEMY_LASER"):
					if (bParam1) {
						return "WTU_A_ENMYLSR";
					} else {
						return "WT_A_ENMYLSR";
					}
					break;
				case joaat("WEAPON_KNIFE"):
					if (bParam1) {
						return "WTU_KNIFE";
					} else {
						return "WT_KNIFE";
					}
					break;
				case joaat("WEAPON_NIGHTSTICK"):
					if (bParam1) {
						return "WTU_NGTSTK";
					} else {
						return "WT_NGTSTK";
					}
					break;
				case joaat("WEAPON_HAMMER"):
					if (bParam1) {
						return "WTU_HAMMER";
					} else {
						return "WT_HAMMER";
					}
					break;
				case joaat("WEAPON_BAT"):
					if (bParam1) {
						return "WTU_BAT";
					} else {
						return "WT_BAT";
					}
					break;
				case joaat("WEAPON_CROWBAR"):
					if (bParam1) {
						return "WTU_CROWBAR";
					} else {
						return "WT_CROWBAR";
					}
					break;
				case joaat("WEAPON_GOLFCLUB"):
					if (bParam1) {
						return "WTU_GOLFCLUB";
					} else {
						return "WT_GOLFCLUB";
					}
					break;
				case joaat("WEAPON_RAMMED_BY_CAR"):
					if (bParam1) {
						return "WTU_PIST";
					} else {
						return "WT_PIST";
					}
					break;
				case joaat("WEAPON_RUN_OVER_BY_CAR"):
					if (bParam1) {
						return "WTU_PIST";
					} else {
						return "WT_PIST";
					}
					break;
				case joaat("WEAPON_ASSAULTSMG"):
					if (bParam1) {
						return "WTU_SMG_ASL";
					} else {
						return "WT_SMG_ASL";
					}
					break;
				case joaat("WEAPON_BULLPUPSHOTGUN"):
					if (bParam1) {
						return "WTU_SG_BLP";
					} else {
						return "WT_SG_BLP";
					}
					break;
				case joaat("WEAPON_PISTOL50"):
					if (bParam1) {
						return "WTU_PIST_50";
					} else {
						return "WT_PIST_50";
					}
					break;
				case joaat("WEAPON_BOTTLE"):
					if (bParam1) {
						return "WTU_BOTTLE";
					} else {
						return "WT_BOTTLE";
					}
					break;
				case joaat("WEAPON_GUSENBERG"):
					if (bParam1) {
						return "WTU_GUSENBERG";
					} else {
						return "WT_GUSENBERG";
					}
					break;
				case joaat("WEAPON_SNSPISTOL"):
					if (bParam1) {
						return "WTU_SNSPISTOL";
					} else {
						return "WT_SNSPISTOL";
					}
					break;
				case joaat("WEAPON_VINTAGEPISTOL"):
					if (bParam1) {
						return "WTU_VPISTOL";
					} else {
						return "WT_VPISTOL";
					}
					break;
				case joaat("WEAPON_DAGGER"):
					if (bParam1) {
						return "WTU_DAGGER";
					} else {
						return "WT_DAGGER";
					}
					break;
				case joaat("WEAPON_FLAREGUN"):
					if (bParam1) {
						return "WTU_FLAREGUN";
					} else {
						return "WT_FLAREGUN";
					}
					break;
				case joaat("WEAPON_HEAVYPISTOL"):
					if (bParam1) {
						return "WTU_HEAVYPSTL";
					} else {
						return "WT_HEAVYPSTL";
					}
					break;
				case joaat("WEAPON_SPECIALCARBINE"):
					if (bParam1) {
						return "WTU_RIFLE_SCBN";
					} else {
						return "WT_RIFLE_SCBN";
					}
					break;
				case joaat("WEAPON_MUSKET"):
					if (bParam1) {
						return "WTU_MUSKET";
					} else {
						return "WT_MUSKET";
					}
					break;
				case joaat("WEAPON_FIREWORK"):
					if (bParam1) {
						return "WTU_FWRKLNCHR";
					} else {
						return "WT_FWRKLNCHR";
					}
					break;
				case joaat("WEAPON_MARKSMANRIFLE"):
					if (bParam1) {
						return "WTU_MKRIFLE";
					} else {
						return "WT_MKRIFLE";
					}
					break;
				case joaat("WEAPON_HEAVYSHOTGUN"):
					if (bParam1) {
						return "WTU_HVYSHOT";
					} else {
						return "WT_HVYSHOT";
					}
					break;
				case joaat("WEAPON_PROXMINE"):
					if (bParam1) {
						return "WTU_PRXMINE";
					} else {
						return "WT_PRXMINE";
					}
					break;
				case joaat("WEAPON_HOMINGLAUNCHER"):
					if (bParam1) {
						return "WTU_HOMLNCH";
					} else {
						return "WT_HOMLNCH";
					}
					break;
				case joaat("WEAPON_HATCHET"):
					if (bParam1) {
						return "WTU_HATCHET";
					} else {
						return "WT_HATCHET";
					}
					break;
				case joaat("WEAPON_RAILGUN"):
					if (bParam1) {
						return "WTU_RAILGUN";
					} else {
						return "WT_RAILGUN";
					}
					break;
				case joaat("WEAPON_COMBATPDW"):
					if (bParam1) {
						return "WTU_COMBATPDW";
					} else {
						return "WT_COMBATPDW";
					}
					break;
				case joaat("WEAPON_KNUCKLE"):
					if (bParam1) {
						return "WTU_KNUCKLE";
					} else {
						return "WT_KNUCKLE";
					}
					break;
				case joaat("WEAPON_MARKSMANPISTOL"):
					if (bParam1) {
						return "WTU_MKPISTOL";
					} else {
						return "WT_MKPISTOL";
					}
					break;
				case joaat("WEAPON_BULLPUPRIFLE"):
					if (bParam1) {
						return "WTU_BULLRIFLE";
					} else {
						return "WT_BULLRIFLE";
					}
					break;
				case joaat("WEAPON_MACHETE"):
					if (bParam1) {
						return "WTU_MACHETE";
					} else {
						return "WT_MACHETE";
					}
					break;
				case joaat("WEAPON_MACHINEPISTOL"):
					if (bParam1) {
						return "WTU_MCHPIST";
					} else {
						return "WT_MCHPIST";
					}
					break;
				case joaat("WEAPON_FLASHLIGHT"):
					if (bParam1) {
						return "WTU_FLASHLIGHT";
					} else {
						return "WT_FLASHLIGHT";
					}
					break;
				case joaat("WEAPON_DBSHOTGUN"):
					if (bParam1) {
						return "WTU_DBSHGN";
					} else {
						return "WT_DBSHGN";
					}
					break;
				case joaat("WEAPON_COMPACTRIFLE"):
					if (bParam1) {
						return "WTU_CMPRIFLE";
					} else {
						return "WT_CMPRIFLE";
					}
					break;
				case joaat("WEAPON_SWITCHBLADE"):
					if (bParam1) {
						return "WTU_SWBLADE";
					} else {
						return "WT_SWBLADE";
					}
					break;
				case joaat("WEAPON_REVOLVER"):
					if (bParam1) {
						return "WTU_REVOLVER";
					} else {
						return "WT_REVOLVER";
					}
					break;
				case joaat("WEAPON_AUTOSHOTGUN"):
					if (bParam1) {
						return "WTU_AUTOSHGN";
					} else {
						return "WT_AUTOSHGN";
					}
					break;
				case joaat("WEAPON_BATTLEAXE"):
					if (bParam1) {
						return "WTU_BATTLEAXE";
					} else {
						return "WT_BATTLEAXE";
					}
					break;
				case joaat("WEAPON_COMPACTLAUNCHER"):
					if (bParam1) {
						return "WTU_CMPGL";
					} else {
						return "WT_CMPGL";
					}
					break;
				case joaat("WEAPON_MINISMG"):
					if (bParam1) {
						return "WTU_MINISMG";
					} else {
						return "WT_MINISMG";
					}
					break;
				case joaat("WEAPON_PIPEBOMB"):
					if (bParam1) {
						return "WTU_PIPEBOMB";
					} else {
						return "WT_PIPEBOMB";
					}
					break;
				case joaat("WEAPON_POOLCUE"):
					if (bParam1) {
						return "WTU_POOLCUE";
					} else {
						return "WT_POOLCUE";
					}
					break;
				case joaat("WEAPON_WRENCH"):
					if (bParam1) {
						return "WTU_WRENCH";
					} else {
						return "WT_WRENCH";
					}
					break;
				case joaat("WEAPON_COUGAR"):
					return "WT_RAGE";
				case -159960575:
					return "WT_VEH_WEP";
				case joaat("WEAPON_PISTOL_MK2"):
					if (bParam1) {
						return "WTU_PIST2";
					} else {
						return "WT_PIST2";
					}
					break;
				case joaat("WEAPON_SMG_MK2"):
					if (bParam1) {
						return "WTU_SMG2";
					} else {
						return "WT_SMG2";
					}
					break;
				case joaat("WEAPON_HEAVYSNIPER_MK2"):
					if (bParam1) {
						return "WTU_SNIP_HVY2";
					} else {
						return "WT_SNIP_HVY2";
					}
					break;
				case joaat("WEAPON_COMBATMG_MK2"):
					if (bParam1) {
						return "WTU_MG_CBT2";
					} else {
						return "WT_MG_CBT2";
					}
					break;
				case joaat("WEAPON_ASSAULTRIFLE_MK2"):
					if (bParam1) {
						return "WTU_RIFLE_ASL2";
					} else {
						return "WT_RIFLE_ASL2";
					}
					break;
				case joaat("WEAPON_CARBINERIFLE_MK2"):
					if (bParam1) {
						return "WTU_RIFLE_CBN2";
					} else {
						return "WT_RIFLE_CBN2";
					}
					break;
				case joaat("WEAPON_PUMPSHOTGUN_MK2"):
					if (bParam1) {
						return "WTU_SG_PMP2";
					} else {
						return "WT_SG_PMP2";
					}
					break;
				case joaat("WEAPON_SPECIALCARBINE_MK2"):
					if (bParam1) {
						return "WTU_SPCARBINE2";
					} else {
						return "WT_SPCARBINE2";
					}
					break;
				case joaat("WEAPON_SNSPISTOL_MK2"):
					if (bParam1) {
						return "WTU_SNSPISTOL2";
					} else {
						return "WT_SNSPISTOL2";
					}
					break;
				case joaat("WEAPON_MARKSMANRIFLE_MK2"):
					if (bParam1) {
						return "WTU_MKRIFLE2";
					} else {
						return "WT_MKRIFLE2";
					}
					break;
				case joaat("WEAPON_REVOLVER_MK2"):
					if (bParam1) {
						return "WTU_REVOLVER2";
					} else {
						return "WT_REVOLVER2";
					}
					break;
				case joaat("WEAPON_BULLPUPRIFLE_MK2"):
					if (bParam1) {
						return "WTU_BULLRIFLE2";
					} else {
						return "WT_BULLRIFLE2";
					}
					break;
				case joaat("WEAPON_DOUBLEACTION"):
					if (bParam1) {
						return "WTU_REV_DA";
					} else {
						return "WT_REV_DA";
					}
					break;
				case joaat("WEAPON_STONE_HATCHET"):
					if (bParam1) {
						return "WTU_SHATCHET";
					} else {
						return "WT_SHATCHET";
					}
					break;
				case joaat("WEAPON_RAYPISTOL"):
					if (bParam1) {
						return "WTU_RAYPISTOL";
					} else {
						return "WT_RAYPISTOL";
					}
					break;
				case joaat("WEAPON_RAYCARBINE"):
					if (bParam1) {
						return "WTU_RAYCARBINE";
					} else {
						return "WT_RAYCARBINE";
					}
					break;
				case joaat("WEAPON_RAYMINIGUN"):
					if (bParam1) {
						return "WTU_RAYMINIGUN";
					} else {
						return "WT_RAYMINIGUN";
					}
					break;
				case joaat("WEAPON_NAVYREVOLVER"):
					if (bParam1) {
						return "WTU_REV_NV";
					} else {
						return "WT_REV_NV";
					}
					break;
				case joaat("WEAPON_CERAMICPISTOL"):
					if (bParam1) {
						return "WTU_CERPST";
					} else {
						return "WT_CERPST";
					}
					break;
				default:
					if (func_81(iParam0, Var0) != -1) {
						if (bParam1) {
							return (char*)&Var0[32];
						} else {
							return (char*)&Var0[8];
						}
					}
					break;
			}
			return "WT_INVALID";
		}

		void func_228(int iParam0, int* uParam1, int* iParam2) {
			*iParam2 = 0;
			if (iParam0 == joaat("WEAPON_PETROLCAN")) {
				*uParam1 = 5;
			} else if (iParam0 == joaat("GADGET_PARACHUTE")) {
				*uParam1 = 4;
			} else if (iParam0 == joaat("WEAPON_UNARMED")) {
				*uParam1 = 3;
			} else if ((((iParam0 == joaat("WEAPON_PISTOL50") || iParam0 == joaat("WEAPON_SNSPISTOL")) || iParam0 == joaat("WEAPON_HEAVYPISTOL")) || iParam0 == joaat("WEAPON_VINTAGEPISTOL")) || iParam0 == joaat("WEAPON_FLAREGUN")) {
				*uParam1 = 0;
			} else if (((((((((iParam0 == joaat("WEAPON_BOTTLE") || iParam0 == joaat("WEAPON_DAGGER")) || iParam0 == joaat("WEAPON_HAMMER")) || iParam0 == joaat("WEAPON_HATCHET")) || iParam0 == joaat("WEAPON_KNIFE")) || iParam0 == joaat("WEAPON_NIGHTSTICK")) || iParam0 == joaat("WEAPON_FLASHLIGHT")) || iParam0 == joaat("WEAPON_SWITCHBLADE")) || iParam0 == joaat("WEAPON_KNUCKLE")) || iParam0 == joaat("WEAPON_MACHETE")) {
				*uParam1 = 1;
			} else if ((((iParam0 == joaat("WEAPON_GRENADE") || iParam0 == joaat("WEAPON_SMOKEGRENADE")) || iParam0 == joaat("WEAPON_STICKYBOMB")) || iParam0 == joaat("WEAPON_PROXMINE")) || iParam0 == joaat("WEAPON_PIPEBOMB")) {
				*uParam1 = 2;
				*iParam2 = 1;
			} else {
				*uParam1 = 0;
			}
		}

		bool func_83() {
			int iVar0;

			if (Native::NetworkIsSignedIn()) {
				if (Native::_NetworkAreRosAvailable()) {
					if (Native::NetworkHasRosPrivilege(24)) {
						Native::StatGetInt(joaat("SP_UNLOCK_EXCLUS_CONTENT"), &iVar0, -1);
						Native::SetBit(&iVar0, 2);
						Native::SetBit(&iVar0, 4);
						Native::SetBit(&iVar0, 6);
						Native::SetBit(Menu::Global(25).Get<int>(), 2);
						Native::SetBit(Menu::Global(25).Get<int>(), 4);
						Native::SetBit(Menu::Global(25).Get<int>(), 6);
						Native::StatSetInt(joaat("SP_UNLOCK_EXCLUS_CONTENT"), iVar0, true);

						if (Native::AreProfileSettingsValid()) {
							iVar0 = Native::GetProfileSetting(866);
							Native::SetBit(&iVar0, 0);
							Native::SetHasContentUnlocksFlags(iVar0);
						}

						return true;
					}
				}
			}

			if (Menu::Global(150472).As<int>() == 2) {
				return true;
			} else if (Menu::Global(150472).As<int>() == 3) {
				return false;
			}

			if (Native::AreProfileSettingsValid()) {
				if (Native::IsBitSet(Native::GetProfileSetting(866), 0)) {
					return true;
				}
			}

			return false;
		}

		int func_165() {
			int iVar0;
			int iVar1;
			int iVar2;
			int iVar3;
			int iVar4;

			if (Native::IsBitSet(Menu::Global(25).As<int>(), 5)) {
				if (Native::IsBitSet(Menu::Global(25).As<int>(), 1) || Native::IsBitSet(Menu::Global(25).As<int>(), 3)) {
					return 1;
				} else {
					return 0;
				}
			}
			if (Native::StatGetInt(joaat("SP_UNLOCK_EXCLUS_CONTENT"), &iVar0, -1)) {
				if (Native::IsBitSet(iVar0, 5)) {
					if (Native::IsBitSet(iVar0, 1) || Native::IsBitSet(iVar0, 3)) {
						return 1;
					} else {
						return 0;
					}
				}
			}
			if (Native::StatSlotIsLoaded(false)) {
				if (Menu::Global(150473).At(3).As<int>()) {
					iVar2 = joaat("MPPLY_PLAT_UP_LB_CHECK");
					if (Native::StatGetInt(iVar2, &iVar1, -1)) {
						if (Native::IsBitSet(iVar1, 5)) {
							return 1;
						}
					}
				}
			}
			if (Native::AreProfileSettingsValid()) {
				iVar3 = Native::GetProfileSetting(866);
				if (Native::IsBitSet(iVar3, 1) || Native::IsBitSet(iVar3, 3)) {
					return 1;
				}
			}
			if (Native::NetworkIsSignedIn()) {
				if (Native::_NetworkAreRosAvailable()) {
					if (Native::NetworkHasRosPrivilege(25)) {
						Native::StatGetInt(joaat("SP_UNLOCK_EXCLUS_CONTENT"), &iVar4, -1);
						Native::SetBit(&iVar4, 1);
						Native::SetBit(&iVar4, 3);
						Native::SetBit(&iVar4, 5);
						Native::SetBit(&Menu::Global(25).As<int>(), 1);
						Native::SetBit(&Menu::Global(25).As<int>(), 3);
						Native::SetBit(&Menu::Global(25).As<int>(), 5);
						Native::StatSetInt(joaat("SP_UNLOCK_EXCLUS_CONTENT"), iVar4, true);
						if (Native::AreProfileSettingsValid()) {
							iVar4 = Native::GetProfileSetting(866);
							Native::SetBit(&iVar4, 1);
							Native::SetBit(&iVar4, 3);
							Native::SetHasContentUnlocksFlags(iVar4);
						}
						return 1;
					}
				}
			}
			return 0;
		}

		int func_164() {
			int iVar0;
			int iVar1;
			int iVar2;
			int iVar3;

			if (Native::IsBitSet(Menu::Global(25).As<int>(), 6)) {
				if (Native::IsBitSet(Menu::Global(25).As<int>(), 2) || Native::IsBitSet(Menu::Global(25).As<int>(), 4)) {
					return 1;
				} else {
					return 0;
				}
			}
			if (Native::StatGetInt(joaat("SP_UNLOCK_EXCLUS_CONTENT"), &iVar0, -1)) {
				if (Native::IsBitSet(iVar0, 6)) {
					if (Native::IsBitSet(iVar0, 2) || Native::IsBitSet(iVar0, 4)) {
						return 1;
					} else {
						return 0;
					}
				}
			}
			if (Native::StatSlotIsLoaded(false)) {
				if (Menu::Global(150473).At(3).As<int>()) {
					iVar2 = joaat("MPPLY_PLAT_UP_LB_CHECK");
					if (Native::StatGetInt(iVar2, &iVar1, -1)) {
						if (Native::IsBitSet(iVar1, 8)) {
							return 1;
						}
					}
				}
			}
			if (Native::AreProfileSettingsValid()) {
				iVar3 = Native::GetProfileSetting(866);
				if (Native::IsBitSet(iVar3, 2) || Native::IsBitSet(iVar3, 4)) {
					return 1;
				}
			}
			return 0;
		}

		int func_77() {
			switch (_i<int>(0x155467ACA0F55705)) {
				case 1:
				case 2:
				case 3:
				case 4:
					return 1;
				default:
					break;
			}
			return 0;
		}

		int func_78() {
			switch (_i<int>(0x754615490A029508)) {
				case 1:
				case 2:
				case 3:
				case 4:
					return 1;
				default:
					break;
			}
			return 0;
		}

		bool func_76() {
			return (func_78() || func_77());
		}

		bool func_75(int iParam0) {
			if (Native::NetworkIsGameInProgress()) {
				if (func_76()) {
					if ((iParam0 == joaat("WEAPON_MARKSMANRIFLE") || iParam0 == joaat("WEAPON_COMPACTRIFLE")) || iParam0 == joaat("WEAPON_COMPACTLAUNCHER")) {
						return true;
					}
				}
			}
			return false;
		}

		bool func_226(int iParam0, int iParam1, char* sParam2, char* sParam3) {
			if (!Native::IsStringNullOrEmpty(sParam3)) {
				strncpy(sParam2, sParam3, 16);
			} else {
				strncpy(sParam2, "GSA", 16);
			}

			if (iParam0 == joaat("AMMO_PISTOL")) {
				strncat(sParam2, "_TYPE_R", 16);
			} else if (iParam0 == joaat("AMMO_SMG")) {
				strncat(sParam2, "_TYPE_R", 16);
			} else if (iParam0 == joaat("AMMO_RIFLE")) {
				strncat(sParam2, "_TYPE_R", 16);
			} else if (iParam0 == joaat("AMMO_MG")) {
				strncat(sParam2, "_TYPE_R", 16);
			} else if (iParam0 == -1866678455) {
				strncat(sParam2, "_TYPE_R", 16);
			} else if (iParam0 == joaat("AMMO_SNIPER")) {
				strncat(sParam2, "_TYPE_R", 16);
			} else if (iParam0 == joaat("AMMO_SNIPER_REMOTE")) {
				strncat(sParam2, "_TYPE_R", 16);
			} else if (iParam0 == joaat("AMMO_MINIGUN")) {
				strncat(sParam2, "_TYPE_R", 16);
			} else if (iParam0 == -873900901) {
				strncat(sParam2, "_TYPE_R", 16);
			} else if (iParam0 == joaat("AMMO_SHOTGUN")) {
				strncat(sParam2, "_TYPE_C", 16);
			} else if (iParam0 == joaat("AMMO_STUNGUN")) {
				strncat(sParam2, "_TYPE_CH", 16);
			} else if (iParam0 == joaat("AMMO_GRENADELAUNCHER")) {
				strncat(sParam2, "_TYPE_G", 16);
			} else if (iParam0 == joaat("AMMO_GRENADELAUNCHER_SMOKE")) {
				strncat(sParam2, "_TYPE_G", 16);
			} else if (iParam0 == joaat("AMMO_SMOKEGRENADE")) {
				strncat(sParam2, "_TYPE_G", 16);
			} else if (iParam0 == 214501711) {
				strncat(sParam2, "_TYPE_G", 16);
			} else if (iParam0 == joaat("AMMO_STICKYBOMB")) {
				strncat(sParam2, "_TYPE_B", 16);
			} else if (iParam0 == joaat("AMMO_RPG")) {
				strncat(sParam2, "_TYPE_RO", 16);
			} else if (iParam0 == joaat("AMMO_FLAREGUN")) {
				strncat(sParam2, "_TYPE_F", 16);
			} else if (iParam0 == joaat("AMMO_FIREWORK")) {
				strncat(sParam2, "_TYPE_FW", 16);
			} else if (iParam0 == joaat("AMMO_HOMINGLAUNCHER")) {
				strncat(sParam2, "_TYPE_RO", 16);
			} else if (iParam0 == joaat("AMMO_PROXMINE")) {
				strncat(sParam2, "_TYPE_M", 16);
			} else if (iParam0 == joaat("AMMO_RAILGUN")) {
				strncat(sParam2, "_TYPE_R", 16);
			} else if (iParam0 == joaat("AMMO_PIPEBOMB")) {
				strncat(sParam2, "_TYPE_B", 16);
			} else if (iParam0 == joaat("AMMO_MG_ARMORPIERCING")) {
				strncpy(sParam2, "WCT_CLIP_AP", 16);
			} else if (iParam0 == joaat("AMMO_MG_FMJ")) {
				strncpy(sParam2, "WCT_CLIP_FMJ", 16);
			} else if (iParam0 == joaat("AMMO_MG_INCENDIARY")) {
				strncpy(sParam2, "WCT_CLIP_INC", 16);
			} else if (iParam0 == joaat("AMMO_MG_TRACER")) {
				strncpy(sParam2, "WCT_CLIP_TR", 16);
			} else if (iParam0 == joaat("AMMO_PISTOL_FMJ")) {
				strncpy(sParam2, "WCT_CLIP_FMJ", 16);
			} else if (iParam0 == joaat("AMMO_PISTOL_HOLLOWPOINT")) {
				strncpy(sParam2, "WCT_CLIP_HP", 16);
			} else if (iParam0 == joaat("AMMO_PISTOL_INCENDIARY")) {
				strncpy(sParam2, "WCT_CLIP_INC", 16);
			} else if (iParam0 == joaat("AMMO_PISTOL_TRACER")) {
				strncpy(sParam2, "WCT_CLIP_TR", 16);
			} else if (iParam0 == joaat("AMMO_RIFLE_ARMORPIERCING")) {
				strncpy(sParam2, "WCT_CLIP_AP", 16);
			} else if (iParam0 == joaat("AMMO_RIFLE_FMJ")) {
				strncpy(sParam2, "WCT_CLIP_FMJ", 16);
			} else if (iParam0 == joaat("AMMO_RIFLE_INCENDIARY")) {
				strncpy(sParam2, "WCT_CLIP_INC", 16);
			} else if (iParam0 == joaat("AMMO_RIFLE_TRACER")) {
				strncpy(sParam2, "WCT_CLIP_TR", 16);
			} else if (iParam0 == joaat("AMMO_SMG_FMJ")) {
				strncpy(sParam2, "WCT_CLIP_FMJ", 16);
			} else if (iParam0 == joaat("AMMO_SMG_HOLLOWPOINT")) {
				strncpy(sParam2, "WCT_CLIP_HP", 16);
			} else if (iParam0 == joaat("AMMO_SMG_INCENDIARY")) {
				strncpy(sParam2, "WCT_CLIP_INC", 16);
			} else if (iParam0 == joaat("AMMO_SMG_TRACER")) {
				strncpy(sParam2, "WCT_CLIP_TR", 16);
			} else if (iParam0 == joaat("AMMO_SNIPER_ARMORPIERCING")) {
				strncpy(sParam2, "WCT_CLIP_AP", 16);
			} else if (iParam0 == joaat("AMMO_SNIPER_EXPLOSIVE")) {
				strncpy(sParam2, "WCT_CLIP_EX", 16);
			} else if (iParam0 == joaat("AMMO_SNIPER_FMJ")) {
				strncpy(sParam2, "WCT_CLIP_FMJ", 16);
			} else if (iParam0 == joaat("AMMO_SNIPER_INCENDIARY")) {
				strncpy(sParam2, "WCT_CLIP_INC", 16);
			} else if (iParam0 == joaat("AMMO_SNIPER_TRACER")) {
				strncpy(sParam2, "WCT_CLIP_TR", 16);
			} else if (iParam0 == joaat("AMMO_SHOTGUN_ARMORPIERCING")) {
				strncpy(sParam2, "WCT_SHELL_AP", 16);
			} else if (iParam0 == joaat("AMMO_SHOTGUN_EXPLOSIVE")) {
				strncpy(sParam2, "WCT_SHELL_EX", 16);
			} else if (iParam0 == joaat("AMMO_SHOTGUN_HOLLOWPOINT")) {
				strncpy(sParam2, "WCT_SHELL_HP", 16);
			} else if (iParam0 == joaat("AMMO_SHOTGUN_INCENDIARY")) {
				strncpy(sParam2, "WCT_SHELL_INC", 16);
			} else {
				return false;
			}

			if (iParam1 == 1 && iParam0 != joaat("AMMO_FIREWORK")) {
				strncat(sParam2, "1", 16);
			}

			return Native::GetHashKey(sParam2) != 0;
		}

		bool func_227(int iParam0, int* iParam1) {
			*iParam1 = -1;
			switch (iParam0) {
				case joaat("WEAPON_PISTOL"):
					*iParam1 = joaat("AMMO_PISTOL");
					break;
				case joaat("WEAPON_COMBATPISTOL"):
					*iParam1 = joaat("AMMO_PISTOL");
					break;
				case joaat("WEAPON_APPISTOL"):
					*iParam1 = joaat("AMMO_PISTOL");
					break;
				case joaat("WEAPON_PISTOL50"):
					*iParam1 = joaat("AMMO_PISTOL");
					break;
				case joaat("WEAPON_SNSPISTOL"):
					*iParam1 = joaat("AMMO_PISTOL");
					break;
				case joaat("WEAPON_HEAVYPISTOL"):
					*iParam1 = joaat("AMMO_PISTOL");
					break;
				case joaat("WEAPON_VINTAGEPISTOL"):
					*iParam1 = joaat("AMMO_PISTOL");
					break;
				case joaat("WEAPON_MICROSMG"):
					*iParam1 = joaat("AMMO_SMG");
					break;
				case joaat("WEAPON_SMG"):
					*iParam1 = joaat("AMMO_SMG");
					break;
				case joaat("WEAPON_ASSAULTSMG"):
					*iParam1 = joaat("AMMO_SMG");
					break;
				case joaat("WEAPON_COMBATPDW"):
					*iParam1 = joaat("AMMO_SMG");
					break;
				case joaat("WEAPON_ASSAULTRIFLE"):
					*iParam1 = joaat("AMMO_RIFLE");
					break;
				case joaat("WEAPON_CARBINERIFLE"):
					*iParam1 = joaat("AMMO_RIFLE");
					break;
				case joaat("WEAPON_ADVANCEDRIFLE"):
					*iParam1 = joaat("AMMO_RIFLE");
					break;
				case joaat("WEAPON_SPECIALCARBINE"):
					*iParam1 = joaat("AMMO_RIFLE");
					break;
				case joaat("WEAPON_HEAVYRIFLE"):
					*iParam1 = joaat("AMMO_RIFLE");
					break;
				case joaat("WEAPON_BULLPUPRIFLE"):
					*iParam1 = joaat("AMMO_RIFLE");
					break;
				case joaat("WEAPON_MG"):
					*iParam1 = joaat("AMMO_MG");
					break;
				case joaat("WEAPON_COMBATMG"):
					*iParam1 = joaat("AMMO_MG");
					break;
				case joaat("WEAPON_ASSAULTMG"):
					*iParam1 = joaat("AMMO_MG");
					break;
				case joaat("WEAPON_GUSENBERG"):
					*iParam1 = joaat("AMMO_MG");
					break;
				case joaat("WEAPON_PUMPSHOTGUN"):
					*iParam1 = joaat("AMMO_SHOTGUN");
					break;
				case joaat("WEAPON_SAWNOFFSHOTGUN"):
					*iParam1 = joaat("AMMO_SHOTGUN");
					break;
				case joaat("WEAPON_ASSAULTSHOTGUN"):
					*iParam1 = joaat("AMMO_SHOTGUN");
					break;
				case joaat("WEAPON_BULLPUPSHOTGUN"):
					*iParam1 = joaat("AMMO_SHOTGUN");
					break;
				case joaat("WEAPON_HEAVYSHOTGUN"):
					*iParam1 = joaat("AMMO_SHOTGUN");
					break;
				case joaat("WEAPON_SNIPERRIFLE"):
					*iParam1 = joaat("AMMO_SNIPER");
					break;
				case joaat("WEAPON_HEAVYSNIPER"):
					*iParam1 = joaat("AMMO_SNIPER");
					break;
				case joaat("WEAPON_MUSKET"):
					*iParam1 = joaat("AMMO_SHOTGUN");
					break;
				case joaat("WEAPON_MARKSMANRIFLE"):
					*iParam1 = joaat("AMMO_SNIPER");
					break;
				case joaat("WEAPON_GRENADELAUNCHER"):
					*iParam1 = joaat("AMMO_GRENADELAUNCHER");
					break;
				case joaat("WEAPON_RPG"):
					*iParam1 = joaat("AMMO_RPG");
					break;
				case joaat("WEAPON_MINIGUN"):
					*iParam1 = joaat("AMMO_MINIGUN");
					break;
				case joaat("WEAPON_FIREWORK"):
					*iParam1 = joaat("AMMO_FIREWORK");
					break;
				case joaat("WEAPON_FLAREGUN"):
					*iParam1 = joaat("AMMO_FLAREGUN");
					break;
				case joaat("WEAPON_HOMINGLAUNCHER"):
					*iParam1 = joaat("AMMO_HOMINGLAUNCHER");
					break;
				case joaat("WEAPON_PROXMINE"):
					*iParam1 = joaat("AMMO_PROXMINE");
					break;
				case joaat("WEAPON_RAILGUN"):
					*iParam1 = joaat("AMMO_RAILGUN");
					break;
				case joaat("WEAPON_MARKSMANPISTOL"):
					*iParam1 = joaat("AMMO_PISTOL");
					break;
				case joaat("WEAPON_MACHINEPISTOL"):
					*iParam1 = joaat("AMMO_SMG");
					break;
				case joaat("WEAPON_REVOLVER"):
					*iParam1 = joaat("AMMO_PISTOL");
					break;
				case joaat("WEAPON_DBSHOTGUN"):
					*iParam1 = joaat("AMMO_SHOTGUN");
					break;
				case joaat("WEAPON_COMPACTRIFLE"):
					*iParam1 = joaat("AMMO_RIFLE");
					break;
				case joaat("WEAPON_AUTOSHOTGUN"):
					*iParam1 = joaat("AMMO_SHOTGUN");
					break;
				case joaat("WEAPON_COMPACTLAUNCHER"):
					*iParam1 = joaat("AMMO_GRENADELAUNCHER");
					break;
				case joaat("WEAPON_MINISMG"):
					*iParam1 = joaat("AMMO_SMG");
					break;
				case joaat("WEAPON_PIPEBOMB"):
					*iParam1 = joaat("AMMO_PIPEBOMB");
					break;
				case joaat("WEAPON_DOUBLEACTION"):
					*iParam1 = joaat("AMMO_PISTOL");
					break;
				case joaat("WEAPON_PISTOL_MK2"):
					*iParam1 = joaat("AMMO_PISTOL");
					if (Native::DoesEntityExist(Menu::GetLocalPlayer().m_Ped) && !Native::IsEntityDead(Menu::GetLocalPlayer().m_Ped, false)) {
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_PISTOL_MK2_CLIP_FMJ"))) {
							*iParam1 = joaat("AMMO_PISTOL_FMJ");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_PISTOL_MK2_CLIP_HOLLOWPOINT"))) {
							*iParam1 = joaat("AMMO_PISTOL_HOLLOWPOINT");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_PISTOL_MK2_CLIP_INCENDIARY"))) {
							*iParam1 = joaat("AMMO_PISTOL_INCENDIARY");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_PISTOL_MK2_CLIP_TRACER"))) {
							*iParam1 = joaat("AMMO_PISTOL_TRACER");
						}
					}
					break;
				case joaat("WEAPON_SMG_MK2"):
					*iParam1 = joaat("AMMO_SMG");
					if (Native::DoesEntityExist(Menu::GetLocalPlayer().m_Ped) && !Native::IsEntityDead(Menu::GetLocalPlayer().m_Ped, false)) {
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_SMG_MK2_CLIP_FMJ"))) {
							*iParam1 = joaat("AMMO_SMG_FMJ");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_SMG_MK2_CLIP_HOLLOWPOINT"))) {
							*iParam1 = joaat("AMMO_SMG_HOLLOWPOINT");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_SMG_MK2_CLIP_INCENDIARY"))) {
							*iParam1 = joaat("AMMO_SMG_INCENDIARY");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_SMG_MK2_CLIP_TRACER"))) {
							*iParam1 = joaat("AMMO_SMG_TRACER");
						}
					}
					break;
				case joaat("WEAPON_HEAVYSNIPER_MK2"):
					*iParam1 = joaat("AMMO_SNIPER");
					if (Native::DoesEntityExist(Menu::GetLocalPlayer().m_Ped) && !Native::IsEntityDead(Menu::GetLocalPlayer().m_Ped, false)) {
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_HEAVYSNIPER_MK2_CLIP_ARMORPIERCING"))) {
							*iParam1 = joaat("AMMO_SNIPER_ARMORPIERCING");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_HEAVYSNIPER_MK2_CLIP_EXPLOSIVE"))) {
							*iParam1 = joaat("AMMO_SNIPER_EXPLOSIVE");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_HEAVYSNIPER_MK2_CLIP_FMJ"))) {
							*iParam1 = joaat("AMMO_SNIPER_FMJ");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_HEAVYSNIPER_MK2_CLIP_INCENDIARY"))) {
							*iParam1 = joaat("AMMO_SNIPER_INCENDIARY");
						}
					}
					break;
				case joaat("WEAPON_COMBATMG_MK2"):
					*iParam1 = joaat("AMMO_MG");
					if (Native::DoesEntityExist(Menu::GetLocalPlayer().m_Ped) && !Native::IsEntityDead(Menu::GetLocalPlayer().m_Ped, false)) {
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_COMBATMG_MK2_CLIP_ARMORPIERCING"))) {
							*iParam1 = joaat("AMMO_MG_ARMORPIERCING");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_COMBATMG_MK2_CLIP_FMJ"))) {
							*iParam1 = joaat("AMMO_MG_FMJ");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_COMBATMG_MK2_CLIP_INCENDIARY"))) {
							*iParam1 = joaat("AMMO_MG_INCENDIARY");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_COMBATMG_MK2_CLIP_TRACER"))) {
							*iParam1 = joaat("AMMO_MG_TRACER");
						}
					}
					break;
				case joaat("WEAPON_ASSAULTRIFLE_MK2"):
					*iParam1 = joaat("AMMO_RIFLE");
					if (Native::DoesEntityExist(Menu::GetLocalPlayer().m_Ped) && !Native::IsEntityDead(Menu::GetLocalPlayer().m_Ped, false)) {
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_ASSAULTRIFLE_MK2_CLIP_ARMORPIERCING"))) {
							*iParam1 = joaat("AMMO_RIFLE_ARMORPIERCING");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_ASSAULTRIFLE_MK2_CLIP_FMJ"))) {
							*iParam1 = joaat("AMMO_RIFLE_FMJ");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_ASSAULTRIFLE_MK2_CLIP_INCENDIARY"))) {
							*iParam1 = joaat("AMMO_RIFLE_INCENDIARY");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_ASSAULTRIFLE_MK2_CLIP_TRACER"))) {
							*iParam1 = joaat("AMMO_RIFLE_TRACER");
						}
					}
					break;
				case joaat("WEAPON_CARBINERIFLE_MK2"):
					*iParam1 = joaat("AMMO_RIFLE");
					if (Native::DoesEntityExist(Menu::GetLocalPlayer().m_Ped) && !Native::IsEntityDead(Menu::GetLocalPlayer().m_Ped, false)) {
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_CARBINERIFLE_MK2_CLIP_ARMORPIERCING"))) {
							*iParam1 = joaat("AMMO_RIFLE_ARMORPIERCING");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_CARBINERIFLE_MK2_CLIP_FMJ"))) {
							*iParam1 = joaat("AMMO_RIFLE_FMJ");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_CARBINERIFLE_MK2_CLIP_INCENDIARY"))) {
							*iParam1 = joaat("AMMO_RIFLE_INCENDIARY");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_CARBINERIFLE_MK2_CLIP_TRACER"))) {
							*iParam1 = joaat("AMMO_RIFLE_TRACER");
						}
					}
					break;
				case joaat("WEAPON_PUMPSHOTGUN_MK2"):
					*iParam1 = joaat("AMMO_SHOTGUN");
					if (Native::DoesEntityExist(Menu::GetLocalPlayer().m_Ped) && !Native::IsEntityDead(Menu::GetLocalPlayer().m_Ped, false)) {
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_PUMPSHOTGUN_MK2_CLIP_ARMORPIERCING"))) {
							*iParam1 = joaat("AMMO_SHOTGUN_ARMORPIERCING");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_PUMPSHOTGUN_MK2_CLIP_EXPLOSIVE"))) {
							*iParam1 = joaat("AMMO_SHOTGUN_EXPLOSIVE");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_PUMPSHOTGUN_MK2_CLIP_HOLLOWPOINT"))) {
							*iParam1 = joaat("AMMO_SHOTGUN_HOLLOWPOINT");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_PUMPSHOTGUN_MK2_CLIP_INCENDIARY"))) {
							*iParam1 = joaat("AMMO_SHOTGUN_INCENDIARY");
						}
					}
					break;
				case joaat("WEAPON_SPECIALCARBINE_MK2"):
					*iParam1 = joaat("AMMO_RIFLE");
					if (Native::DoesEntityExist(Menu::GetLocalPlayer().m_Ped) && !Native::IsEntityDead(Menu::GetLocalPlayer().m_Ped, false)) {
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_SPECIALCARBINE_MK2_CLIP_ARMORPIERCING"))) {
							*iParam1 = joaat("AMMO_RIFLE_ARMORPIERCING");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_SPECIALCARBINE_MK2_CLIP_FMJ"))) {
							*iParam1 = joaat("AMMO_RIFLE_FMJ");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_SPECIALCARBINE_MK2_CLIP_INCENDIARY"))) {
							*iParam1 = joaat("AMMO_RIFLE_INCENDIARY");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_SPECIALCARBINE_MK2_CLIP_TRACER"))) {
							*iParam1 = joaat("AMMO_RIFLE_TRACER");
						}
					}
					break;
				case joaat("WEAPON_SNSPISTOL_MK2"):
					*iParam1 = joaat("AMMO_PISTOL");
					if (Native::DoesEntityExist(Menu::GetLocalPlayer().m_Ped) && !Native::IsEntityDead(Menu::GetLocalPlayer().m_Ped, false)) {
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_SNSPISTOL_MK2_CLIP_FMJ"))) {
							*iParam1 = joaat("AMMO_PISTOL_FMJ");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_SNSPISTOL_MK2_CLIP_HOLLOWPOINT"))) {
							*iParam1 = joaat("AMMO_PISTOL_HOLLOWPOINT");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_SNSPISTOL_MK2_CLIP_INCENDIARY"))) {
							*iParam1 = joaat("AMMO_PISTOL_INCENDIARY");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_SNSPISTOL_MK2_CLIP_TRACER"))) {
							*iParam1 = joaat("AMMO_PISTOL_TRACER");
						}
					}
					break;
				case joaat("WEAPON_MARKSMANRIFLE_MK2"):
					*iParam1 = joaat("AMMO_SNIPER");
					if (Native::DoesEntityExist(Menu::GetLocalPlayer().m_Ped) && !Native::IsEntityDead(Menu::GetLocalPlayer().m_Ped, false)) {
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_MARKSMANRIFLE_MK2_CLIP_ARMORPIERCING"))) {
							*iParam1 = joaat("AMMO_SNIPER_ARMORPIERCING");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_MARKSMANRIFLE_MK2_CLIP_FMJ"))) {
							*iParam1 = joaat("AMMO_SNIPER_FMJ");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_MARKSMANRIFLE_MK2_CLIP_INCENDIARY"))) {
							*iParam1 = joaat("AMMO_SNIPER_INCENDIARY");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_MARKSMANRIFLE_MK2_CLIP_TRACER"))) {
							*iParam1 = joaat("AMMO_SNIPER_TRACER");
						}
					}
					break;
				case joaat("WEAPON_REVOLVER_MK2"):
					*iParam1 = joaat("AMMO_PISTOL");
					if (Native::DoesEntityExist(Menu::GetLocalPlayer().m_Ped) && !Native::IsEntityDead(Menu::GetLocalPlayer().m_Ped, false)) {
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_REVOLVER_MK2_CLIP_FMJ"))) {
							*iParam1 = joaat("AMMO_PISTOL_FMJ");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_REVOLVER_MK2_CLIP_HOLLOWPOINT"))) {
							*iParam1 = joaat("AMMO_PISTOL_HOLLOWPOINT");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_REVOLVER_MK2_CLIP_INCENDIARY"))) {
							*iParam1 = joaat("AMMO_PISTOL_INCENDIARY");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_REVOLVER_MK2_CLIP_TRACER"))) {
							*iParam1 = joaat("AMMO_PISTOL_TRACER");
						}
					}
					break;
				case joaat("WEAPON_BULLPUPRIFLE_MK2"):
					*iParam1 = joaat("AMMO_RIFLE");
					if (Native::DoesEntityExist(Menu::GetLocalPlayer().m_Ped) && !Native::IsEntityDead(Menu::GetLocalPlayer().m_Ped, false)) {
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_BULLPUPRIFLE_MK2_CLIP_ARMORPIERCING"))) {
							*iParam1 = joaat("AMMO_RIFLE_ARMORPIERCING");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_BULLPUPRIFLE_MK2_CLIP_FMJ"))) {
							*iParam1 = joaat("AMMO_RIFLE_FMJ");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_BULLPUPRIFLE_MK2_CLIP_INCENDIARY"))) {
							*iParam1 = joaat("AMMO_RIFLE_INCENDIARY");
						}
						if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, iParam0, joaat("COMPONENT_BULLPUPRIFLE_MK2_CLIP_TRACER"))) {
							*iParam1 = joaat("AMMO_RIFLE_TRACER");
						}
					}
					break;
				case joaat("WEAPON_RAYPISTOL"):
					*iParam1 = joaat("AMMO_RAYPISTOL");
					break;
				case joaat("WEAPON_RAYCARBINE"):
					*iParam1 = joaat("AMMO_MG");
					break;
				case joaat("WEAPON_RAYMINIGUN"):
					*iParam1 = joaat("AMMO_MINIGUN");
					break;
				case joaat("WEAPON_CERAMICPISTOL"):
					*iParam1 = joaat("AMMO_PISTOL");
					break;
				case joaat("WEAPON_NAVYREVOLVER"):
					*iParam1 = joaat("AMMO_PISTOL");
					break;
			}

			return *iParam1 != -1;
		}

		int func_464(int iParam0) {
			switch (iParam0) {
				case joaat("WEAPON_PISTOL_MK2"):
					return joaat("WEAPON_PISTOL");
				case joaat("WEAPON_SMG_MK2"):
					return joaat("WEAPON_SMG");
				case joaat("WEAPON_ASSAULTRIFLE_MK2"):
					return joaat("WEAPON_ASSAULTRIFLE");
				case joaat("WEAPON_CARBINERIFLE_MK2"):
					return joaat("WEAPON_CARBINERIFLE");
				case joaat("WEAPON_COMBATMG_MK2"):
					return joaat("WEAPON_COMBATMG");
				case joaat("WEAPON_HEAVYSNIPER_MK2"):
					return joaat("WEAPON_HEAVYSNIPER");
				case joaat("WEAPON_BULLPUPRIFLE_MK2"):
					return joaat("WEAPON_BULLPUPRIFLE");
				case joaat("WEAPON_MARKSMANRIFLE_MK2"):
					return joaat("WEAPON_MARKSMANRIFLE");
				case joaat("WEAPON_PUMPSHOTGUN_MK2"):
					return joaat("WEAPON_PUMPSHOTGUN");
				case joaat("WEAPON_REVOLVER_MK2"):
					return joaat("WEAPON_REVOLVER");
				case joaat("WEAPON_SNSPISTOL_MK2"):
					return joaat("WEAPON_SNSPISTOL");
				case joaat("WEAPON_SPECIALCARBINE_MK2"):
					return joaat("WEAPON_SPECIALCARBINE");
				default:
					break;
			}
			return 0;
		}

		// is weapon ammo full
		bool func_882(int iParam0, bool IsMark2 = false) {
			int iVar0;
			bool iVar1 = false;

			if (Native::HasPedGotWeapon(Menu::GetLocalPlayer().m_Ped, iParam0, false)) {
				if (IsMark2 && func_464(iParam0) != 0) {
					bool bVar2 = Native::_GetPedAmmoTypeFromWeapon2(Menu::GetLocalPlayer().m_Ped, iParam0);
					if (Native::GetMaxAmmoByType(Menu::GetLocalPlayer().m_Ped, bVar2, &iVar0)) {
						if (Native::GetPedAmmoByType(Menu::GetLocalPlayer().m_Ped, bVar2) >= iVar0) {
							iVar1 = true;
						}
					}
				} else if (!Native::IsEntityDead(Menu::GetLocalPlayer().m_Ped, false)) {
					if (Native::GetMaxAmmo(Menu::GetLocalPlayer().m_Ped, iParam0, &iVar0)) {
						if (Native::GetAmmoInPedWeapon(Menu::GetLocalPlayer().m_Ped, iParam0) >= iVar0) {
							iVar1 = true;
						}
					}
				}
			}

			return iVar1;
		}
	}

	int GetRoundCount(uint32_t Weapon) {
		switch (Weapon) {
			case joaat("WEAPON_MICROSMG"): return 32;
			case joaat("WEAPON_SMG"): return 60;
			case joaat("WEAPON_ASSAULTSMG"): return 60;
			case joaat("WEAPON_SAWNOFFSHOTGUN"): return 16;
			case joaat("WEAPON_PUMPSHOTGUN"): return 16;
			case joaat("WEAPON_ASSAULTSHOTGUN"): return 16;
			case joaat("WEAPON_ASSAULTRIFLE"): return 60;
			case joaat("WEAPON_CARBINERIFLE"): return 60;
			case joaat("WEAPON_ADVANCEDRIFLE"): return 60;
			case joaat("WEAPON_SNIPERRIFLE"): return 20;
			case joaat("WEAPON_HEAVYSNIPER"): return 12;
			case joaat("WEAPON_MG"): return 108;
			case joaat("WEAPON_COMBATMG"): return 200;
			case joaat("WEAPON_RPG"): return 2;
			case joaat("WEAPON_GRENADELAUNCHER"): return 20;
			case joaat("WEAPON_GRENADELAUNCHER_SMOKE"): return 20;
			case joaat("WEAPON_MINIGUN"): return 100;
			case joaat("WEAPON_RAYCARBINE"): return 100;
			case joaat("WEAPON_RAYMINIGUN"): return 100;
			case joaat("WEAPON_APPISTOL"): return 36;
			case joaat("WEAPON_COMBATPISTOL"): return 24;
			case joaat("WEAPON_PISTOL"): return 24;
			case joaat("WEAPON_PISTOL50"): return 48;
			case joaat("WEAPON_SMOKEGRENADE"): return 1;
			case joaat("WEAPON_GRENADE"): return 1;
			case joaat("WEAPON_STICKYBOMB"): return 1;
			case joaat("WEAPON_PROXMINE"): return 1;
			case joaat("WEAPON_PIPEBOMB"): return 1;
			case joaat("WEAPON_DOUBLEACTION"): return 12;
			case joaat("WEAPON_SNSPISTOL"): return 24;
			case joaat("WEAPON_HEAVYPISTOL"): return 72;
			case joaat("WEAPON_VINTAGEPISTOL"): return 28;
			case joaat("WEAPON_FLAREGUN"): return 2;
			case joaat("WEAPON_REVOLVER"): return 24;
			case joaat("WEAPON_MACHINEPISTOL"): return 48;
			case joaat("WEAPON_COMPACTLAUNCHER"): return 2;
			case joaat("WEAPON_HOMINGLAUNCHER"): return 2;
			case joaat("WEAPON_CERAMICPISTOL"): return 48;
			case joaat("WEAPON_MARKSMANRIFLE"): return 16;
			case joaat("WEAPON_HEAVYSHOTGUN"): return 12;
			case joaat("WEAPON_SPECIALCARBINE"): return 60;
			case joaat("WEAPON_PUMPSHOTGUN_MK2"): return 16;
			case joaat("WEAPON_COMBATPDW"): return 60;
			case joaat("WEAPON_MINISMG"): return 40;
			case joaat("WEAPON_RAINGUN"): return 1;
			case joaat("WEAPON_FIREWORK"): return 2;
			case joaat("WEAPON_GUSENBERG"): return 60;
			case joaat("WEAPON_MILITARYRIFLE"): return 60;
			case joaat("WEAPON_COMPACTRIFLE"): return 60;
			case joaat("WEAPON_BULLPUPRIFLE"): return 60;
			case joaat("WEAPON_AUTOSHOTGUN"): return 20;
			case joaat("WEAPON_DBSHOTGUN"): return 4;
			case joaat("WEAPON_MUSKET"): return 2;
			case joaat("WEAPON_BULLPUPSHOTGUN"): return 16;
			case joaat("WEAPON_NAVYREVOLVER"): return 1;
			case joaat("WEAPON_MARKSMANPISTOL"): return 1;

			case joaat("WEAPON_ASSAULTRIFLE_MK2"): {
				if (Native::DoesEntityExist(Menu::GetLocalPlayer().m_Ped) && !Native::IsEntityDead(Menu::GetLocalPlayer().m_Ped, false)) {
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_ASSAULTRIFLE_MK2_CLIP_ARMORPIERCING"))) {
						return 40;
					}
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_ASSAULTRIFLE_MK2_CLIP_FMJ"))) {
						return 40;
					}
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_ASSAULTRIFLE_MK2_CLIP_INCENDIARY"))) {
						return 40;
					}
				}

				return 60;
			}
			
			case joaat("WEAPON_PISTOL_MK2"): {
				if (Native::DoesEntityExist(Menu::GetLocalPlayer().m_Ped) && !Native::IsEntityDead(Menu::GetLocalPlayer().m_Ped, false)) {
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_PISTOL_MK2_CLIP_FMJ"))) {
						return 16;
					}
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_PISTOL_MK2_CLIP_HOLLOWPOINT"))) {
						return 16;
					}
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_PISTOL_MK2_CLIP_INCENDIARY"))) {
						return 16;
					}
				}

				return 24;
			}

			case joaat("WEAPON_SMG_MK2"): {
				if (Native::DoesEntityExist(Menu::GetLocalPlayer().m_Ped) && !Native::IsEntityDead(Menu::GetLocalPlayer().m_Ped, false)) {
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_SMG_MK2_CLIP_FMJ"))) {
						return 40;
					}
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_SMG_MK2_CLIP_HOLLOWPOINT"))) {
						return 40;
					}
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_SMG_MK2_CLIP_INCENDIARY"))) {
						return 40;
					}
				}

				return 60;
			}

			case joaat("WEAPON_HEAVYSNIPER_MK2"): {
				if (Native::DoesEntityExist(Menu::GetLocalPlayer().m_Ped) && !Native::IsEntityDead(Menu::GetLocalPlayer().m_Ped, false)) {
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_HEAVYSNIPER_MK2_CLIP_ARMORPIERCING"))) {
						return 8;
					}
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_HEAVYSNIPER_MK2_CLIP_EXPLOSIVE"))) {
						return 8;
					}
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_HEAVYSNIPER_MK2_CLIP_FMJ"))) {
						return 8;
					}
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_HEAVYSNIPER_MK2_CLIP_INCENDIARY"))) {
						return 8;
					}
				}

				return 12;
			}

			case joaat("WEAPON_COMBATMG_MK2"): {
				if (Native::DoesEntityExist(Menu::GetLocalPlayer().m_Ped) && !Native::IsEntityDead(Menu::GetLocalPlayer().m_Ped, false)) {
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_COMBATMG_MK2_CLIP_ARMORPIERCING"))) {
						return 160;
					}
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_COMBATMG_MK2_CLIP_FMJ"))) {
						return 160;
					}
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_COMBATMG_MK2_CLIP_INCENDIARY"))) {
						return 160;
					}
				}

				return 200;
			}

			case joaat("WEAPON_CARBINERIFLE_MK2"): {
				if (Native::DoesEntityExist(Menu::GetLocalPlayer().m_Ped) && !Native::IsEntityDead(Menu::GetLocalPlayer().m_Ped, false)) {
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_CARBINERIFLE_MK2_CLIP_ARMORPIERCING"))) {
						return 40;
					}
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_CARBINERIFLE_MK2_CLIP_FMJ"))) {
						return 40;
					}
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_CARBINERIFLE_MK2_CLIP_INCENDIARY"))) {
						return 40;
					}
				}

				return 60;
			}

			case joaat("WEAPON_SPECIALCARBINE_MK2"): {
				if (Native::DoesEntityExist(Menu::GetLocalPlayer().m_Ped) && !Native::IsEntityDead(Menu::GetLocalPlayer().m_Ped, false)) {
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_SPECIALCARBINE_MK2_CLIP_ARMORPIERCING"))) {
						return 40;
					}
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_SPECIALCARBINE_MK2_CLIP_FMJ"))) {
						return 40;
					}
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_SPECIALCARBINE_MK2_CLIP_INCENDIARY"))) {
						return 40;
					}
				}

				return 60;
			}

			case joaat("WEAPON_SNSPISTOL_MK2"): {
				if (Native::DoesEntityExist(Menu::GetLocalPlayer().m_Ped) && !Native::IsEntityDead(Menu::GetLocalPlayer().m_Ped, false)) {
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_SNSPISTOL_MK2_CLIP_FMJ"))) {
						return 12;
					}
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_SNSPISTOL_MK2_CLIP_HOLLOWPOINT"))) {
						return 12;
					}
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_SNSPISTOL_MK2_CLIP_INCENDIARY"))) {
						return 12;
					}
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_SNSPISTOL_MK2_CLIP_TRACER"))) {
						return 12;
					}
				}

				return 24;
			}

			case joaat("WEAPON_MARKSMANRIFLE_MK2"): {
				if (Native::DoesEntityExist(Menu::GetLocalPlayer().m_Ped) && !Native::IsEntityDead(Menu::GetLocalPlayer().m_Ped, false)) {
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_MARKSMANRIFLE_MK2_CLIP_ARMORPIERCING"))) {
						return 10;
					}
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_MARKSMANRIFLE_MK2_CLIP_FMJ"))) {
						return 10;
					}
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_MARKSMANRIFLE_MK2_CLIP_INCENDIARY"))) {
						return 10;
					}
				}

				return 16;
			}

			case joaat("WEAPON_REVOLVER_MK2"): {
				if (Native::DoesEntityExist(Menu::GetLocalPlayer().m_Ped) && !Native::IsEntityDead(Menu::GetLocalPlayer().m_Ped, false)) {
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_REVOLVER_MK2_CLIP_FMJ"))) {
						return 12;
					}
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_REVOLVER_MK2_CLIP_HOLLOWPOINT"))) {
						return 12;
					}
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_REVOLVER_MK2_CLIP_INCENDIARY"))) {
						return 12;
					}
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_REVOLVER_MK2_CLIP_TRACER"))) {
						return 12;
					}
				}

				return 24;
			}

			case joaat("WEAPON_BULLPUPRIFLE_MK2"): {
				if (Native::DoesEntityExist(Menu::GetLocalPlayer().m_Ped) && !Native::IsEntityDead(Menu::GetLocalPlayer().m_Ped, false)) {
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_BULLPUPRIFLE_MK2_CLIP_ARMORPIERCING"))) {
						return 40;
					}
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_BULLPUPRIFLE_MK2_CLIP_FMJ"))) {
						return 40;
					}
					if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, joaat("COMPONENT_BULLPUPRIFLE_MK2_CLIP_INCENDIARY"))) {
						return 40;
					}
				}

				return 60;
			}
		}

		return 0;
	}

	std::string GetTransactionNameForWeapon(uint32_t WeaponHash) {
		char Var96[0x50];
		int iVar101 = 0;
		int iVar100 = 0;
		int iVar102 = 0;

		int iParam0 = (int)WeaponHash;//hash

		strncpy(Var96, Scripts::func_229(iParam0, 0), 16);
		Scripts::func_228(iParam0, &iVar100, &iVar101);

		if (Scripts::func_83() && (Scripts::func_165() || Scripts::func_164())) {
			if ((iParam0 == joaat("WEAPON_HAMMER") || iParam0 == joaat("WEAPON_PISTOL50")) || iParam0 == joaat("WEAPON_BULLPUPSHOTGUN")) {
				iVar101 = 1;
			}
		}

		if (Scripts::func_83()) {
			if (iParam0 == joaat("WEAPON_HATCHET")) {
				iVar101 = 1;
			}
		}

		iVar102 = 0;
		if (Scripts::func_75(iParam0)) {
			iVar102 = 1;
			iVar101 = 2;
		}

		char Output[0x50];
		Scripts::func_116(Output, Var96, Native::GetEntityModel(Native::PlayerPedId()), 3, iVar100, iVar101, Scripts::func_117(iParam0), -1, 0, 0);

		return std::string(Output);
	}

	std::string GetTransactionNameForWeaponAmmo(uint32_t WeaponHash) {
		if ((((WeaponHash == joaat("WEAPON_GRENADE") || WeaponHash == joaat("WEAPON_SMOKEGRENADE")) || WeaponHash == joaat("WEAPON_STICKYBOMB")) || WeaponHash == joaat("WEAPON_PROXMINE")) || WeaponHash == joaat("WEAPON_PIPEBOMB")) {
			switch (WeaponHash) {
				case joaat("WEAPON_GRENADE"):
					return "WP_WT_GNADE_t2_v1";
				case joaat("WEAPON_SMOKEGRENADE"):
					return "WP_WT_GNADE_SMK_t2_v1";
				case joaat("WEAPON_STICKYBOMB"):
					return "WP_WT_GNADE_STK_t2_v1";
				case joaat("WEAPON_PROXMINE"):
					return "WP_WT_PRXMINE_t2_v1";
				case joaat("WEAPON_PIPEBOMB"):
					return "WP_WT_PIPEBOMB_t2_v1";
			}
		}

		char Output[0x50];
		char Var96[0x50];
		int Hash = 0;

		Scripts::func_227((int)WeaponHash, &Hash);
		Scripts::func_226(Hash, 2, Var96, 0); // Native::_GetLabelText(Var96)
		Scripts::func_116(Output, Var96, Native::GetEntityModel(Native::PlayerPedId()), 3, 0, Scripts::func_57((int)WeaponHash), Scripts::func_117((int)WeaponHash), -1, 0, 0);

		return std::string(Output);
	}

	int GetWeaponPrice(std::string TransactionName) {
		return Native::_NetworkShopGetPrice((int)Native::GetHashKey(TransactionName.c_str()), -1829708865, true);
	}

	bool PurchaseWeapon(std::string TransactionName, std::string WeaponName, int Price) {
		if (Rage::Engine::ProcessBasketTransaction(-1829708865, 537254313, 4, { { (int)Native::GetHashKey(TransactionName.c_str()), -1, Price, 1, 1 } })) {
			AmmunationMenu* This = AmmunationMenu::GetInstance();
			Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s %s%i", This->getString("Purchased").c_str(), WeaponName.c_str(), This->getString("for $").c_str(), Price));
			return true;
		}

		return false;
	}

	int CalculateTotalPrice() {
		int TotalPrice = 0;

		for (auto& Weapon : Global::Arrays::g_Weapons) {
			if (!Native::HasPedGotWeapon(Menu::GetLocalPlayer().m_Ped, Weapon.m_Result, false)) {
				std::string TransactionName = GetTransactionNameForWeapon(Weapon.m_Result);
				if (!TransactionName.empty()) {
					int Price = GetWeaponPrice(TransactionName);
					if (Price > 0 || Weapon.m_Result == 0x7846a318) {
						TotalPrice += Price;
					}
				}
			}
		}

		return TotalPrice;
	}

	void PurchaseAllWeapons() {
		std::vector<Transaction> Weapons;

		for (auto& Weapon : Global::Arrays::g_Weapons) {
			if (!Native::HasPedGotWeapon(Menu::GetLocalPlayer().m_Ped, Weapon.m_Result, false)) {
				std::string TransactionName = GetTransactionNameForWeapon(Weapon.m_Result);
				if (!TransactionName.empty()) {
					int Price = GetWeaponPrice(TransactionName);
					if (Price > 0 || Weapon.m_Result == 0x7846a318) {
						Weapons.push_back({ (int)Native::GetHashKey(TransactionName.c_str()), -1, Price, 1, 1 });
					}
				}
			}
		}

		Rage::Engine::ProcessBasketTransaction(-1829708865, 537254313, 4, Weapons);
		AmmunationMenu::GetInstance()->UpdateOnce();
	}

	const char* GetRoundName(const char* Name) {
		std::pair<const char*, const char*> Pairs[] = {
			{ "GSA_TYPE_R", "Rounds" },
			{ "GSA_TYPE_C", "Cartridges" },
			{ "GSA_TYPE_CH", "Charges" },
			{ "GSA_TYPE_G", "Grenades" },
			{ "GSA_TYPE_B", "Bombs" },
			{ "GSA_TYPE_RO", "Rockets" },
			{ "GSA_TYPE_F", "Flares" },
			{ "GSA_TYPE_FW", "Fireworks" }
		};

		for (auto& P : Pairs) {
			if (!strcmp(P.first, Name)) {
				return P.second;
			}
		}

		return Native::_GetLabelText(Name);
	}
}

void AmmunationMenu::Init() {
	SetName("Ammunation");
	SetParentSubmenu<WeaponMenu>();
}

void AmmunationMenu::Update() {}

/*Called once on submenu open*/
void AmmunationMenu::UpdateOnce() {
	ClearOptionsOffset(3);
}

/*Called always*/
void AmmunationMenu::FeatureUpdate() {}

AmmunationMenu* _instance;
AmmunationMenu* AmmunationMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new AmmunationMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

AmmunationMenu::~AmmunationMenu() { delete _instance; }