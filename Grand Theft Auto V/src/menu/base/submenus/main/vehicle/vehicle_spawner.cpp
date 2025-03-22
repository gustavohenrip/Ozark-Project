#include "vehicle_spawner.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle.hpp"
#include "utils/memory/memory.hpp"
#include "spawner/vehicle_spawner_select.hpp"
#include "spawner/vehicle_spawner_settings.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/util/global.hpp"
#include "spawner/vehicle_spawner_manage.hpp"
#include "vehicle_mods.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/va.hpp"
#include "spawner/vehicle_spawner_menyoo.hpp"
#include "menu/base/submenus/main/network/players/network_players_spawn.hpp"
#include "menu/base/util/panels.hpp"
#include "spawner/vehicle_spawner_garage.hpp"

using namespace VehicleSpawnerMenuVars;

namespace VehicleSpawnerMenuVars {
	Vars_ m_Vars;

	void PopulateHashList() {
		struct HashNode {
			uint32_t m_ModelHash;
			uint16_t m_Data;
			uint16_t m_Padding;
			HashNode* m_Next;
		};

		for (auto& Hashes : m_Vars.m_VehicleHashes) {
			Hashes.clear();
		}

		uint16_t HashTableCount = *(uint16_t*)Memory::GetAddressFromInstruction(Global::Vars::g_GetModelInfo);
		uint64_t HashTable = *(uint64_t*)Memory::GetAddressFromInstruction(Global::Vars::g_GetModelInfo + 0x7E);
		HashNode** HashMap = *(HashNode***)Memory::GetAddressFromInstruction(Global::Vars::g_GetModelInfo + 0x21);

		for (uint16_t i = 0; i < HashTableCount; i++) {
			for (HashNode* Current = HashMap[i]; Current; Current = Current->m_Next) {
				uint64_t Element = *(uint64_t*)(HashTable + (Current->m_Data * 8));
				if (Element) {
					uint8_t ModelType = *(uint8_t*)(Element + 0x9D) & 0x1F;
					if (ModelType == 5) {
						if (Current->m_ModelHash) {
							uint8_t VehicleType = *(uint8_t*)(Element + 0x548) & 0x1F;
							m_Vars.m_VehicleHashes[VehicleType].push_back(Current->m_ModelHash);
						}
					}
				}
			}
		}
	}

	const char* GetVehicleClassName(int Class) {
		char Buffer[32];
		sprintf(Buffer, "VEH_CLASS_%d", Class);
		return !strcmp(Native::_GetLabelText(Buffer), "NULL") ? "" : Native::_GetLabelText(Buffer);
	}

	namespace Script {
		// Freemode, search for "case joaat("TECHNICAL"):"
		bool func_1081(int iParam0) {
			return iParam0 == 49;
		}

		bool func_1080(int iParam0) {
			return iParam0 == 50;
		}

		int func_8789(int iParam0) {
			switch (iParam0) {
				case 0x3af76f4a:
					return 533;

				case 0x381e10bd:
					return 530;

				case 0xceb28249:
					return 531;

				case 0xed62bfa9:
					return 531;

				case 0x9dae1398:
					return 528;

				case 0x4662bcbb:
					return 534;

				case 0x50d4d19f:
					return 534;

				case 0x28ad20e1:
					return 529;

				case 0x8e08ec82:
					return 532;

				case 0xa1355f67:
					return 512;

				case 0x2189d250:
					return 558;

				case 0x34b82784:
					return 559;

				case 0xfe141da6:
					return 560;

				case 0x711d4738:
					return 561;

				case 0xb7d9f7f1:
					return 562;

				case 0x8fd54ebb:
					return 563;

				case 0xa52f6866:
					return 572;

				case 0xfe0a508c:
					return 573;

				case 0x89ba59f5:
					return 574;

				case 0xc3f25753:
					return 575;

				case 0xfd707ede:
					return 576;

				case 0x96e24857:
					return 577;

				case 0xd35698ef:
					return 578;

				case 0x5d56f01b:
					return 579;

				case 0x3dc92356:
					return 580;

				case 0xad6065c0:
					return 581;

				case 0xc5dd6967:
					return 582;

				case 0x9a9eb7de:
					return 583;

				case 0xe8983f9f:
					return 584;

				case 0x3e2e4f8a:
					return 585;

				case 0x19dd9ed1:
					if (func_1081(Menu::Global(4456448).At(129348).As<int>()) || func_1080(Menu::Global(4456448).At(129348).As<int>())) {
						return 225;
					}
					break;

				case 0x83051506:
					if (func_1081(Menu::Global(4456448).At(129348).As<int>())) {
						return 534;
					}
					break;

				case 0x34dba661:
					return 595;

				case 0x586765fb:
					return 596;

				case 0x58cdaf30:
					return 597;

				case 0xaa6f980a:
					return 598;

				case 0x9b16a3b4:
					return 599;

				case 0x1aad0ded:
					return 600;

				case 0xf34dfb25:
					return 601;

				case 0x46699f47:
					return 602;

				case 0xd6bc7523:
					return 603;

				case 0x81bd2ed0:
					return 589;

				case 0x64de07a1:
					return 640;

				case 0xd17099d:
					return 637;

				case 0x73f4110e:
					return 636;

				case 0x6290f15b:
					return 635;

				case 0x7b54a9d3:
					return 639;

				case 0xd4ae63d9:
					return 612;

				case 0x4abebf23:
					return 613;

				case 0x36a167e0:
					return 348;
			}
			return -1;
		}

		int func_8783(int iParam0) {
			switch (iParam0) {
				case 0x711d4738:
					return 0;

				case 0xfe141da6:
					return 1;

				case 0x8fd54ebb:
					return 2;

				case 0x2189d250:
					return 3;

				case 0xb7d9f7f1:
					return 4;

				case 0x34b82784:
					return 5;
			}

			return -1;
		}

		int func_8782(uint32_t iParam0) {
			switch (iParam0) { // good
				case 0x96e24857:
					return Menu::Global(262145).At(22223).As<int>();

				case 0xc5dd6967:
					return Menu::Global(262145).At(22224).As<int>();

				case 0xa52f6866:
					return Menu::Global(262145).At(22225).As<int>();

				case 0x89ba59f5:
					return Menu::Global(262145).At(22226).As<int>();

				case 0x9a9eb7de:
					return Menu::Global(262145).At(22227).As<int>();

				case 0x5d56f01b:
					return Menu::Global(262145).At(22228).As<int>();

				case 0x3e2e4f8a:
					return Menu::Global(262145).At(22229).As<int>();

				case 0xfe0a508c:
					return Menu::Global(262145).At(22230).As<int>();

				case 0xc3f25753:
					return Menu::Global(262145).At(22231).As<int>();

				case 0xd35698ef:
					return Menu::Global(262145).At(22232).As<int>();

				case 0xad6065c0:
					return Menu::Global(262145).At(22233).As<int>();

				case 0xe8983f9f:
					return Menu::Global(262145).At(22234).As<int>();

				case 0x3dc92356:
					return Menu::Global(262145).At(22235).As<int>();

				case 0xfd707ede:
					return Menu::Global(262145).At(22236).As<int>();
			}

			return 0;
		}

		int func_8788(int iParam0) {
			switch (iParam0) {
				case 0x586765fb:
					return 0;
					break;

				case 0x46699f47:
					return 1;
					break;

				case 0x9b16a3b4:
					return 2;
					break;

				case 0x34dba661:
					return 3;
					break;

				case 0xd6bc7523:
					return 4;
					break;

				case 0xf34dfb25:
					return 5;
					break;

				case 0xaa6f980a:
					return 6;
					break;

				case 0x1aad0ded:
					return 7;
					break;

				case 0x58cdaf30:
					return 8;
					break;
			}
			return -1;
		}

		int func_8785(int iParam0) {
			if ((iParam0 == joaat("LAZER") || iParam0 == joaat("BESRA")) || iParam0 == joaat("HYDRA")) {
				return 1;
			}
			return 0;
		}

		int func_8787(int iParam0) {
			if ((((((((((iParam0 == joaat("TECHNICAL") || iParam0 == joaat("INSURGENT")) || iParam0 == joaat("INSURGENT2")) || iParam0 == joaat("LIMO2")) || iParam0 == joaat("TECHNICAL2")) || iParam0 == joaat("BOXVILLE5")) || iParam0 == joaat("INSURGENT3")) || iParam0 == joaat("TECHNICAL3")) || iParam0 == joaat("barrage")) || iParam0 == joaat("caracara")) || iParam0 == joaat("HALFTRACK")) {
				return 1;
			}
			return 0;
		}

		int func_1079(int iParam0, bool bParam1) {
			switch (iParam0) {
				case 0x3af76f4a:
				case 0x381e10bd:
				case 0xceb28249:
				case 0xed62bfa9:
				case 0x9dae1398:
				case 0x4662bcbb:
				case 0x28ad20e1:
				case 0x8e08ec82:
				case 0xa1355f67:
				case 0xd17099d:
				case 0x73f4110e:
				case 0x6290f15b:
				case 0x8fd54ebb:
					return 1;

				case 0x19dd9ed1:
					// might need updating
					if (func_1081(Menu::Global(4456448).At(129348).As<int>()) || func_1080(Menu::Global(4456448).At(129348).As<int>())) {
						if (!bParam1) {
							return 1;
						}
					}
					break;

				case 0x50d4d19f:
				case 0x83051506:
					if (func_1081(Menu::Global(4456448).At(129348).As<int>())) {
						return 1;
					}
					break;
			}

			return 0;
		}

		int GetBlipSpriteIndexForVehicle(uint32_t iParam0) {
			if (func_1079(iParam0, 0)) {
				return func_8789(iParam0);
			}
			if (func_8783(iParam0) != -1) {
				return func_8789(iParam0);
			}
			if (func_8782(iParam0) > 0) {
				return func_8789(iParam0);
			}
			if (func_8788(iParam0) != -1) {
				return func_8789(iParam0);
			}
			if (iParam0 == joaat("avenger")) {
				return 589;
			}
			if (iParam0 == joaat("seasparrow")) {
				return 612;
			}
			if (iParam0 == joaat("blimp3")) {
				return 638;
			} else if (iParam0 == joaat("pbus2")) {
				return 631;
			} else if (iParam0 == joaat("terbyte")) {
				return 632;
			} else if (iParam0 == joaat("strikeforce")) {
				return 640;
			} else if (iParam0 == joaat("menacer")) {
				return 633;
			} else if (iParam0 == joaat("scramjet")) {
				return 634;
			} else if (iParam0 == joaat("oppressor2")) {
				return 639;
			}
			if ((iParam0 == joaat("bruiser") || iParam0 == joaat("bruiser2")) || iParam0 == joaat("bruiser3")) {
				return 658;
			} else if ((iParam0 == joaat("brutus") || iParam0 == joaat("brutus2")) || iParam0 == joaat("brutus3")) {
				return 659;
			} else if ((iParam0 == joaat("cerberus") || iParam0 == joaat("cerberus2")) || iParam0 == joaat("cerberus3")) {
				return 660;
			} else if ((iParam0 == joaat("deathbike") || iParam0 == joaat("deathbike2")) || iParam0 == joaat("deathbike3")) {
				return 661;
			} else if ((iParam0 == joaat("dominator4") || iParam0 == joaat("dominator5")) || iParam0 == joaat("dominator6")) {
				return 662;
			} else if ((iParam0 == joaat("impaler2") || iParam0 == joaat("impaler3")) || iParam0 == joaat("impaler4")) {
				return 663;
			} else if ((iParam0 == joaat("imperator") || iParam0 == joaat("imperator2")) || iParam0 == joaat("imperator3")) {
				return 664;
			} else if ((iParam0 == joaat("issi4") || iParam0 == joaat("issi5")) || iParam0 == joaat("issi6")) {
				return 665;
			} else if ((iParam0 == joaat("monster3") || iParam0 == joaat("monster4")) || iParam0 == joaat("monster5")) {
				return 666;
			} else if ((iParam0 == joaat("scarab") || iParam0 == joaat("scarab2")) || iParam0 == joaat("scarab3")) {
				return 667;
			} else if ((iParam0 == joaat("slamvan4") || iParam0 == joaat("slamvan5")) || iParam0 == joaat("slamvan6")) {
				return 668;
			} else if ((iParam0 == joaat("zr380") || iParam0 == joaat("zr3802")) || iParam0 == joaat("zr3803")) {
				return 669;
			} else if (iParam0 == joaat("rrocket")) {
				return 226;
			}
			if (Native::IsThisModelABike(iParam0)) {
				return 226;
			} else if (Native::IsThisModelAHeli(iParam0)) {
				return 64;
			} else if (Native::IsThisModelABoat(iParam0)) {
				return 410;
			} else if (func_8785(iParam0)) {
				return 424;
			} else if (Native::IsThisModelAPlane(iParam0)) {
				return 423;
			} else if (Native::IsThisModelAQuadbike(iParam0)) {
				return 512;
			} else if (iParam0 == joaat("RHINO")) {
				return 421;
			} else if (iParam0 == joaat("submersible") || iParam0 == joaat("submersible2")) {
				return 308;
			} else if (func_8787(iParam0)) {
				return 426;
			} else {
				return 225;
			}
			return 225;
		}
	}

	void SpawnVehicle(uint32_t ModelHash) {
		if (Native::IsModelInCdimage(ModelHash) && Native::IsModelAVehicle(ModelHash)) {
			Menu::GetControlManager()->RequestModel(ModelHash, [=] (uint32_t Model) {
				Menu::PlayerVars Target;

				if (!Menu::GetSubmenuHandler()->GetPreviousSubmenu()) {
					Target = Menu::GetLocalPlayer();
				} else {
					if (Menu::GetSubmenuHandler()->GetPreviousSubmenu()->m_Parent == NetworkPlayersSpawnMenu::GetInstance()) {
						Target = Menu::GetSelectedPlayer();
					} else if (Menu::GetSubmenuHandler()->GetPreviousSubmenu() == VehicleSpawnerMenu::GetInstance()
						|| Menu::GetSubmenuHandler()->GetPreviousSubmenu() == VehicleMenu::GetInstance()) {
						Target = Menu::GetLocalPlayer();
					} else {
						Target = Menu::GetLocalPlayer();
					}
				}

				bool Network = Target.m_ID != Menu::GetLocalPlayer().m_ID;

				bool SpawnVectored = false;
				float OldSpeed = 0.f;

				if (VehicleSpawnerSettingsMenuVars::m_Vars.m_DeleteCurrent
					&& !Network) {
					if (Target.m_InVehicle) {
						if (Native::GetPedInVehicleSeat(Target.m_Vehicle, -1, 0) == Target.m_Ped) {
							// It's my vehicle
							OldSpeed = Native::GetEntitySpeed(Target.m_Vehicle);
							Native::SetEntityAsMissionEntity(Target.m_Vehicle, true, true);
							Native::DeleteVehicle(&Target.m_Vehicle);
						} else {
							SpawnVectored = true;
						}
					}
				}

				Math::Vector3<float> Coords;

				Entity Vehicle = 0;
				if (!Network && VehicleSpawnerSettingsMenuVars::m_Vars.m_TeleportInside && !SpawnVectored) {
					Vehicle = Native::CreateVehicle(Model, Target.m_Coords.m_X, Target.m_Coords.m_Y, Target.m_Coords.m_Z, Target.m_Heading, true, true, 0);
					Coords = Target.m_Coords;
				} else {
					auto C = Target.m_Coords + (Native::GetEntityForwardVector(Target.m_Entity) * 5.f);
					Vehicle = Native::CreateVehicle(Model, C.m_X, C.m_Y, C.m_Z, Target.m_Heading, true, true, 0);
					Coords = C;
				}

				if (Native::DoesEntityExist(Vehicle)) {
					Native::SetEntityCoords(Vehicle, Coords.m_X, Coords.m_Y, Coords.m_Z, false, false, false, false);
					Native::SetVehicleOnGroundProperly(Vehicle, 1);

					Native::SetVehicleDirtLevel(Vehicle, 0);
					Native::SetVehRadioStation(Vehicle, "OFF");

					if (VehicleSpawnerSettingsMenuVars::m_Vars.m_AirVehiclesInAir) {
						if (Native::IsThisModelAPlane(Model) || Native::IsThisModelAHeli(Model)) {
							Math::Vector3<float> Coords = Native::GetEntityCoords(Vehicle, true);
							Native::SetEntityCoords(Vehicle, Coords.m_X, Coords.m_Y, Coords.m_Z + VehicleSpawnerSettingsMenuVars::m_Vars.m_SpawnHeight, 0, 0, 0, 0);
							Native::SetVehicleEngineOn(Vehicle, true, true, false);
							Native::SetHeliBladesFullSpeed(Vehicle);
							Native::SetVehicleForwardSpeed(Vehicle, 100);
						}
					}

					if (VehicleSpawnerSettingsMenuVars::m_Vars.m_Fade) {
						Native::NetworkFadeInEntity(Vehicle, true, false);
					}

					if (VehicleSpawnerSettingsMenuVars::m_Vars.m_Blip) {
						Blip B = Native::AddBlipForEntity(Vehicle);
						Native::SetBlipSprite(B, Script::GetBlipSpriteIndexForVehicle(Model));
						Native::SetBlipDisplay(B, 8);
						Native::SetBlipCategory(B, 4);
					}

					Native::DecorSetInt(Vehicle, "MPBitset", 0);
					Native::SetVehicleIsStolen(Vehicle, false);

					if (Network) {
						if (Menu::GetSelectedPlayer().m_GiftVehicle) {
							uint32_t PlayerHash = Native::NetworkHashFromPlayerHandle(Menu::GetSelectedPlayer().m_ID);
							Native::DecorSetInt(Vehicle, "Player_Vehicle", PlayerHash);
							Native::DecorSetInt(Vehicle, "Previous_Owner", PlayerHash);
							Native::DecorSetInt(Vehicle, "Veh_Modded_By_Player", PlayerHash);
							Native::DecorSetInt(Vehicle, "Not_Allow_As_Saved_Veh", 0);
							Native::DecorSetInt(Vehicle, "PV_Slot", 2);
						}
					}

					if (VehicleSpawnerSettingsMenuVars::m_Vars.m_TeleportInside && !Network) {
						Native::SetPedIntoVehicle(Target.m_Ped, Vehicle, -1);
						Native::SetVehicleEngineOn(Vehicle, true, true, false);
						Native::SetVehicleForwardSpeed(Vehicle, OldSpeed);
					}

					if (VehicleSpawnerSettingsMenuVars::m_Vars.m_MaxUpgrades && !Network) {
						VehicleModsMenuVars::MaxUpgrades(Vehicle);
					}

					if (VehicleSpawnerSettingsMenuVars::m_Vars.m_Godmode) {
						Native::SetEntityInvincible(Vehicle, true);
						Native::SetEntityProofs(Vehicle, true, true, true, true, true, true, true, true);
						Native::SetVehicleCanBeVisiblyDamaged(Vehicle, false);
						Native::SetVehicleEngineOn(Vehicle, true, true, true);
					}

					if (VehicleSpawnerSettingsMenuVars::m_Vars.m_ParticleFX) {
						Native::RequestNamedPtfxAsset("proj_indep_firework_v2");
						Native::_UseParticleFxAssetNextCall("proj_indep_firework_v2");
						Native::_StartParticleFxNonLoopedAtCoord2("scr_firework_indep_repeat_burst_rwb", Coords.m_X, Coords.m_Y, Coords.m_Z, 0, 0, 0, 1.2f, true, true, true);
					}

					if (VehicleSpawnerSettingsMenuVars::m_Vars.m_Notification) {
						Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s!", Native::_GetLabelText(Native::GetDisplayNameFromVehicleModel(Model)), VehicleSpawnerMenu::GetInstance()->getString("spawned").c_str()));
					}

					VehicleSpawnerManageMenuVars::m_Vars.m_SpawnedVehicles.push_back({ Vehicle, Model, Native::_GetLabelText(Native::GetDisplayNameFromVehicleModel(Model)), Native::GetGameTimer() });
				}
			});
		} else printf("failed 1\n");
	}
}

void VehicleSpawnerMenu::Init() {
	SetName("Vehicle Spawner");
	SetParentSubmenu<VehicleMenu>();

	addString("spawned");

	addOption(SubmenuOption("Settings")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleSpawnerSettingsMenu>());

	addOption(SubmenuOption("Manage Spawned Vehicles")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleSpawnerManageMenu>());

	addOption(SubmenuOption("Menyoo Vehicles")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleSpawnerMenyooMenu>());

	addOption(SubmenuOption("Garage Vehicles")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleSpawnerGarageMenu>()
		.addTooltip("Spawn vehicles from your garages"));

	addOption(ButtonOption("Input Custom Vehicle")
		.addTranslation().addHotkey()
		.addKeyboard("Input the model! (Hint: name, hash or numeric)", 30, [] (const char* ModelName) {
			if (ModelName) {
				if (ModelName[0] == '0' && (ModelName[1] == 'x' || ModelName[1] == 'X')) {
					// hash
					SpawnVehicle((uint32_t)_strtoui64(ModelName, 0, 0));
				} else {
					std::string StringName(ModelName);
					if (std::find_if(StringName.begin(), StringName.end(), [] (unsigned char c) { return !std::isdigit(c); }) == StringName.end()) {
						// numeric
						SpawnVehicle((uint32_t)atoi(ModelName));
					} else {
						// name
						SpawnVehicle(Native::GetHashKey(ModelName));
					}
				}
			}
		}));

	addOption(BreakOption("Categories")
		.addTranslation());

	for (int i = 0; i < 23; i++) {
		addOption(SubmenuOption(GetVehicleClassName(i))
			.addTranslation()
			.addOnClick([=] { VehicleSpawnerSelectMenuVars::m_Vars.m_Selected = i; })
			.addSubmenu<VehicleSpawnerSelectMenu>());
	}

	VehicleSpawnerSelectMenu::GetInstance(); // Init
}

void VehicleSpawnerMenu::Update() {}

/*Called once on submenu open*/
void VehicleSpawnerMenu::UpdateOnce() {
	Menu::Panels::GetPanelManager()->TogglePanelRender("PANEL_VEHICLE_PREVIEW", false);
	Menu::Panels::GetPanelManager()->TogglePanelRender("PANEL_PLAYER_INFO", false);

	PopulateHashList();
}

/*Called always*/
void VehicleSpawnerMenu::FeatureUpdate() {
	Menu::Global(4269479).As<bool>() = true;
}

VehicleSpawnerMenu* _instance;
VehicleSpawnerMenu* VehicleSpawnerMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleSpawnerMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleSpawnerMenu::~VehicleSpawnerMenu() { delete _instance; }