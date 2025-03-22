#include "network_players_trolling.hpp"
#include "menu/base/submenu_handler.hpp"
#include "network_players_selected.hpp"
#include "network_players_abusive.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/util/helpers.hpp"
#include "utils/va.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "menu/base/util/timers.hpp"
#include "trolling/network_trolling_sounds.hpp"
#include "menu/base/submenus/main/network/network_session.hpp"

using namespace NetworkPlayersTrollingMenuVars;

namespace NetworkPlayersTrollingMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> Cage[] = {
		{ TranslationString("Trolly", true), 0 },
		{ TranslationString("Fence", true), 1 }
	};

	ScrollStruct<int> Messages[] = {
		{ TranslationString("Spectating You", true), 0 },
		{ TranslationString("Stopped Spectating You", true), 1 },
		{ TranslationString("Now Spectating", true), 2 },
		{ TranslationString("$ Banked", true), 3 },
		{ TranslationString("$ Stolen", true), 4 },
		{ TranslationString("$ Removed", true), 5 },
		{ TranslationString("ATM Stolen", true), 6 },
		{ TranslationString("Equipped Minigun", true), 7 },
		{ TranslationString("Killed Gang Member", true), 8 },
		{ TranslationString("Registered as President", true), 9 },
		{ TranslationString("Registered as CEO", true), 10 },
		{ TranslationString("Registered as VIP", true), 11 },
		{ TranslationString("Going Psycho", true), 12 },
		{ TranslationString("VIP Dismissed", true), 13 }
	};

	ScrollStruct<int> Clone[] = {
		{ TranslationString("Ambient", true), 0 },
		{ TranslationString("Hostile", true), 1 }
	};

	ScrollStruct<uint32_t> WeaponGroups[] = {
		{ TranslationString("Melee", true), 0xD49321D4 },
		{ TranslationString("Pistol", true), 0x18D5FA97 },
		{ TranslationString("SMG", true), 0xC6E9A5C5 },
		{ TranslationString("Assault Rifle", true), 0x39D5C192 },
		{ TranslationString("MG", true), 0x451B04BC },
		{ TranslationString("Shotgun", true), 0x33431399 },
		{ TranslationString("Sniper", true), 0xB7BBD827 },
		{ TranslationString("Heavy", true), 0xA27A4F9F },
		{ TranslationString("Throwables", true), 0x5C4C5883 },
		{ TranslationString("Spillable", true), 0x5F1BE07C },
		{ TranslationString("Stun", true), 0x29268262 },
		{ TranslationString("Extinguisher", true), 0xFDBF656C }
	};

	ScrollStruct<ExplosionTypes> Jet[] = {
		{ TranslationString("Steam", true), EXPLOSION_DIR_STEAM },
		{ TranslationString("Flame", true), EXPLOSION_DIR_FLAME },
		{ TranslationString("Water Hydrant", true), EXPLOSION_DIR_WATER_HYDRANT }
	};

	ScrollStruct<uint32_t> RamVehicles[] = {
		{ TranslationString("", false), 0x5A82F9AE },
		{ TranslationString("", false), 0x5BA0FF1E },
		{ TranslationString("", false), 0x18619B7E },
		{ TranslationString("", false), 0x322CF98F },
		{ TranslationString("", false), 0xB2E046FB },
		{ TranslationString("", false), 0xE83C17 },
		{ TranslationString("", false), 0xA29F78B0 },
		{ TranslationString("", false), 0x42BC5E19 },
		{ TranslationString("", false), 0x1FD824AF },
		{ TranslationString("", false), 0x8B213907 },
		{ TranslationString("", false), 0x2C1FEA99 },
		{ TranslationString("", false), 0xEA6A047F },
		{ TranslationString("", false), 0x432AA566 },
		{ TranslationString("", false), 0xEB298297 },
		{ TranslationString("", false), 0xB44F0582 },
		{ TranslationString("", false), 0x9472CD24 },
		{ TranslationString("", false), 0x23CA25F2 },
		{ TranslationString("", false), 0x619C1B82 },
		{ TranslationString("", false), 0xB472D2B5 },
		{ TranslationString("", false), 0xC2974024 },
		{ TranslationString("", false), 0xd577c962 },
		{ TranslationString("", false), 0x2ea68690 },
		{ TranslationString("", false), 0xb779a091 },
	};

	void CagePlayer(Menu::PlayerVars Player) {
		if (Player.m_CageType == 1) {
			Menu::GetControlManager()->RequestModel(0x466EB7B8, [=] (uint32_t Model) {
				Math::Vector3<float> MinDimensions;
				Math::Vector3<float> MaxDimensions;

				Native::GetModelDimensions(Model, &MinDimensions, &MaxDimensions);
				Math::Vector3<float> Diameter = MaxDimensions - MinDimensions;

				Math::Vector3<float> SpawnCoords = Native::GetOffsetFromEntityInWorldCoords(Player.m_Ped, -1.5f, -1.5f, 0.f);
				Native::GetGroundZFor3DCoord(SpawnCoords.m_X, SpawnCoords.m_Y, SpawnCoords.m_Z, &SpawnCoords.m_Z, false);

				Object FirstObject = Native::CreateObject(Model, SpawnCoords.m_X, SpawnCoords.m_Y, SpawnCoords.m_Z, true, true, false);
				if (Native::DoesEntityExist(FirstObject)) {
					float Rotation = Player.m_Heading;
					Native::SetEntityRotation(FirstObject, 0.f, 0.f, Rotation, 2, false);
					Native::FreezeEntityPosition(FirstObject, true);

					Object NextObject = FirstObject;
					for (int i = 0; i < 3; i++) {
						Rotation += 90.f;

						Math::Vector3<float> Next = Native::GetOffsetFromEntityInWorldCoords(NextObject, Diameter.m_X, 0.f, 0.f);
						NextObject = Native::CreateObject(Model, Next.m_X, Next.m_Y, Next.m_Z, true, true, false);
						if (Native::DoesEntityExist(NextObject)) {
							Native::SetEntityRotation(NextObject, 0.f, 0.f, Rotation, 2, false);
							Native::FreezeEntityPosition(NextObject, true);
						}
					}
				}
			});
		} else {
			Menu::GetControlManager()->RequestModel(0x7B059043, [=] (uint32_t Model) {
				Object Cage = Native::CreateObject(Model, Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z - 1, true, true, false);
				Native::FreezeEntityPosition(Cage, true);

				Cage = Native::CreateObject(Model, Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z + 1, true, true, false);
				Native::FreezeEntityPosition(Cage, true);
				Native::SetEntityRotation(Cage, 0.f, 180.f, 90.f, 0, false);
			});
		}
	}

	void SendMessagePlayer(Menu::PlayerVars Player) {
		std::vector<int> Event;

		switch (Player.m_Messages) {
			case 0:
				Event = {
					1975453628,
					Player.m_ID,
					-941009932,
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					Player.m_ID
				};
				Menu::Helpers::TriggerScriptEvent(Event, 14, Player.m_ID);
				break;

			case 1:
				Event = {
					1975453628,
					Player.m_ID,
					-1412451274,
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					Player.m_ID
				};
				Menu::Helpers::TriggerScriptEvent(Event, 14, Player.m_ID);
				break;

			case 2:
				Event = {
					1975453628,
					Player.m_ID,
					-2129516738,
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					Player.m_ID
				};
				Menu::Helpers::TriggerScriptEvent(Event, 14, Player.m_ID);
				break;

			case 3:
				Event = {
					1975453628,
					Player.m_ID,
					-141590900,
					std::numeric_limits<int>::max(),
					0,
					0,
					0,
					0,
					0,
					0,
					Player.m_ID
				};
				Menu::Helpers::TriggerScriptEvent(Event, 14, Player.m_ID);
				break;

			case 4:
				Event = {
					1975453628,
					Player.m_ID,
					309201771,
					std::numeric_limits<int>::max(),
					0,
					0,
					0,
					0,
					0,
					0,
					Player.m_ID
				};
				Menu::Helpers::TriggerScriptEvent(Event, 14, Player.m_ID);
				break;

			case 5:
				Event = {
					1975453628,
					Player.m_ID,
					1018143336,
					std::numeric_limits<int>::max(),
					0,
					0,
					0,
					0,
					0,
					0,
					Player.m_ID
				};
				Menu::Helpers::TriggerScriptEvent(Event, 14, Player.m_ID);
				break;

			case 6:
				Event = {
					1975453628,
					Player.m_ID,
					1658299413,
					std::numeric_limits<int>::max(),
					0,
					0,
					0,
					0,
					0,
					0,
					Player.m_ID
				};
				Menu::Helpers::TriggerScriptEvent(Event, 14, Player.m_ID);
				break;

			case 7:
				Event = {
					1975453628,
					Player.m_ID,
					-1607682153,
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					Player.m_ID
				};
				Menu::Helpers::TriggerScriptEvent(Event, 14, Player.m_ID);
				break;

			case 8:
				Event = {
					1975453628,
					Player.m_ID,
					244054042,
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					Player.m_ID
				};
				Menu::Helpers::TriggerScriptEvent(Event, 14, Player.m_ID);
				break;

			case 9:
				Event = {
					1975453628,
					Player.m_ID,
					-727278527,
					1,
					0,
					0,
					0,
					0,
					0,
					0,
					Player.m_ID
				};
				Menu::Helpers::TriggerScriptEvent(Event, 14, Player.m_ID);
				break;

			case 10:
				Event = {
					1975453628,
					Player.m_ID,
					-727278527,
					100,
					0,
					0,
					0,
					0,
					0,
					0,
					Player.m_ID
				};
				Menu::Helpers::TriggerScriptEvent(Event, 14, Player.m_ID);
				break;

			case 11:
				Event = {
					1975453628,
					Player.m_ID,
					-727278527,
					std::numeric_limits<int>::max(),
					0,
					0,
					0,
					0,
					0,
					0,
					Player.m_ID
				};
				Menu::Helpers::TriggerScriptEvent(Event, 14, Player.m_ID);
				break;

			case 12:
				Event = {
					1975453628,
					Player.m_ID,
					-1052312640,
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					Player.m_ID
				};
				Menu::Helpers::TriggerScriptEvent(Event, 14, Player.m_ID);
				break;

			case 13:
				Event = {
					1975453628,
					Player.m_ID,
					-326501313,
					0,
					0,
					0,
					0,
					0,
					0,
					0,
					Player.m_ID
				};
				Menu::Helpers::TriggerScriptEvent(Event, 14, Player.m_ID);
				break;
		}
	}

	void ClonePlayer(Menu::PlayerVars Player) {
		Ped Cloned = Native::ClonePed(Player.m_Ped, Player.m_Heading, true, true);
		if (Player.m_CloneType == 1 && Native::DoesEntityExist(Cloned)) {
			Native::GiveWeaponToPed(Cloned, 0x7FD62962, 9999, true, true);
			Native::TaskCombatPed(Cloned, Player.m_Ped, 0, 16);
			Native::SetPedKeepTask(Cloned, true);
		}
	}

	void RemoveWeaponGroupPlayer(Menu::PlayerVars Player, uint32_t Group) {
		std::vector<uint32_t> Hashes;
		
		uint64_t Table = *(uint64_t*)Global::Vars::g_WeaponInfoList;
		if (Table) {
			for (int16_t i = 0; i < (*(int16_t*)Global::Vars::g_WeaponInfoListCount) - 1; i++) {
				Rage::Types::Weapon* Ptr = (Rage::Types::Weapon*) * (uint64_t*)(Table + (i * 8));
				if (Ptr) {
					if (Ptr->m_GroupHash == Group) {
						Hashes.push_back(Ptr->m_NameHash);
					}
				}
			}
		}

		for (auto& Hash : Hashes) {
			Native::RemoveWeaponFromPed(Player.m_Ped, Hash);
		}

		Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %i %s %s", NetworkPlayersTrollingMenu::GetInstance()->getString("Removed").c_str(), Hashes.size(), NetworkPlayersTrollingMenu::GetInstance()->getString("weapons from").c_str(), Player.m_Name));
	}

	void HostilePedsPlayer(Menu::PlayerVars Player) {
		Menu::Pools::GetPedPool()->Run([=] (Entity Ped) {
			Native::GiveWeaponToPed(Ped, 0x7FD62962, 9999, true, true);
			Native::TaskCombatPed(Ped, Player.m_Ped, 0, 16);
			Native::SetPedKeepTask(Ped, true);
		}, true);
	}

	void HostileTrafficPlayer(Menu::PlayerVars Player) {
		Menu::Pools::GetVehiclePool()->Run([=] (Entity Vehicle) {
			Ped Ped = Native::GetPedInVehicleSeat(Vehicle, -1, 1);
			if (Native::DoesEntityExist(Ped)) {
				Native::TaskVehicleDriveToCoord(Ped, Vehicle, Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, 100, 1, Native::GetEntityModel(Vehicle), 16777216, 4, -1);
			}
		}, true);
	}

	void FakeMoneyPlayer(Menu::PlayerVars Player) {
		Native::CreateAmbientPickup(0x1E9A99F8, Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, 0, 0, 0x113FD533, 0, 1);
	}

	void JetPlayer(Menu::PlayerVars Player) {
		Native::AddExplosion(Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, Jet[Player.m_JetType].m_Result, 1.f, true, false, 0.f, false);
	}

	void RamPlayer(Menu::PlayerVars Player) {
		Menu::GetControlManager()->RequestModel(RamVehicles[Player.m_RamType].m_Result, [=] (uint32_t Model) {
			Vehicle Vehicle = Native::CreateVehicle(Model, Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, 0.f, true, false, 1);
			if (Native::DoesEntityExist(Vehicle)) {
				Native::SetVehicleOutOfControl(Vehicle, true, true);
				Native::SetEntityHeading(Vehicle, Player.m_Heading - 90.f);
				Native::SetVehicleForwardSpeed(Vehicle, 100.f);
				Native::SetModelAsNoLongerNeeded(Model);
			}
		});
	}
}

void NetworkPlayersTrollingMenu::Init() {
	SetName("Trolling");
	SetParentSubmenu<NetworkPlayersSelectedMenu>();

	for (auto& Veh : RamVehicles) {
		Veh.m_Name.Set(Native::_GetLabelText(Native::GetDisplayNameFromVehicleModel(Veh.m_Result)));
	}

	NetworkTrollingSoundsMenu::GetInstance(); // Init

	addString("Removed");
	addString("weapons from");
	addString("Sounds");
	addString("Hostile Peds");
	addString("Hostile Traffic");
	addString("Fake Money Drop");
	addString("Glitch Physics");
	addString("Remove Weapons");
	addString("Taze");
	addString("Ragdoll");
	addString("Kick from Vehicle");
	addString("Give Wanted Level");
	addString("Vehicle Ram");
	addString("Remove Weapon Group");
	addString("Jet");
	addString("Cage");
	addString("Clone");
	addString("Show Message");
}

void NetworkPlayersTrollingMenu::Update() {}

/*Called once on submenu open*/
void NetworkPlayersTrollingMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	if (Global::Vars::g_NetworkMenuType == SELECTED_PLAYER) {
		SetParentSubmenu<NetworkPlayersSelectedMenu>();

		addOption(SubmenuOption(getString("Sounds"))
			.addSubmenu<NetworkTrollingSoundsMenu>());

		addOption(ToggleOption(getString("Hostile Peds"))
			.addToggle(Menu::GetSelectedPlayer().m_HostilePeds)
			.addOnUpdate([](ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_HostilePeds); }));

		addOption(ToggleOption(getString("Hostile Traffic"))
			.addToggle(Menu::GetSelectedPlayer().m_HostileTraffic)
			.addOnUpdate([](ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_HostileTraffic); }));

		addOption(ToggleOption(getString("Fake Money Drop"))
			.addToggle(Menu::GetSelectedPlayer().m_FakeMoneyDrop)
			.addOnUpdate([](ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_FakeMoneyDrop); }));

		addOption(ButtonOption(getString("Glitch Physics"))
			.addOnClick([] { NetworkPlayersAbusiveMenuVars::GlitchPhysics(Menu::GetSelectedPlayer()); }));

		addOption(ButtonOption(getString("Remove Weapons"))
			.addOnClick([] { Native::RemoveAllPedWeapons(Menu::GetSelectedPlayer().m_Ped, true); }));

		addOption(ButtonOption(getString("Taze"))
			.addOnClick([] { NetworkPlayersAbusiveMenuVars::TazePlayer(Menu::GetSelectedPlayer()); })
			.addTooltip("You have to be near them"));

		addOption(ButtonOption(getString("Ragdoll"))
			.addOnClick([] { NetworkPlayersAbusiveMenuVars::RagdollPlayer(Menu::GetSelectedPlayer()); }));

		addOption(ButtonOption(getString("Kick from Vehicle"))
			.addOnClick([] { NetworkPlayersAbusiveMenuVars::KickFromVehiclePlayer(Menu::GetSelectedPlayer()); }));

		addOption(ButtonOption(getString("Give Wanted Level"))
			.addOnClick([] { NetworkPlayersAbusiveMenuVars::GiveWantedLevel(Menu::GetSelectedPlayer(), 5); }));

		addOption(ScrollOption<uint32_t>(SCROLLSELECT, getString("Vehicle Ram"))
			.addScroll(Menu::GetSelectedPlayer().m_RamType, 0, NUMOF(RamVehicles), RamVehicles)
			.addOnClick([] { RamPlayer(Menu::GetSelectedPlayer()); })
			.addOnUpdate([](ScrollOption<uint32_t>* Option) { Option->addScroll(Menu::GetSelectedPlayer().m_RamType, 0, NUMOF(RamVehicles), RamVehicles); }));

		addOption(ScrollOption<uint32_t>(SCROLLSELECT, getString("Remove Weapon Group"))
			.addScroll(Menu::GetSelectedPlayer().m_RemoveWeaponGroupType, 0, NUMOF(WeaponGroups), WeaponGroups)
			.addOnClick([] { RemoveWeaponGroupPlayer(Menu::GetSelectedPlayer(), WeaponGroups[Menu::GetSelectedPlayer().m_RemoveWeaponGroupType].m_Result); })
			.addOnUpdate([](ScrollOption<uint32_t>* Option) { Option->addScroll(Menu::GetSelectedPlayer().m_RemoveWeaponGroupType, 0, NUMOF(WeaponGroups), WeaponGroups); }));

		addOption(ScrollOption<ExplosionTypes>(SCROLLSELECT, getString("Jet"))
			.addScroll(Menu::GetSelectedPlayer().m_JetType, 0, NUMOF(Jet), Jet)
			.addOnClick([] { JetPlayer(Menu::GetSelectedPlayer()); })
			.addOnUpdate([](ScrollOption<ExplosionTypes>* Option) { Option->addScroll(Menu::GetSelectedPlayer().m_JetType, 0, NUMOF(Jet), Jet); }));

		addOption(ScrollOption<int>(SCROLLSELECT, getString("Cage"))
			.addScroll(Menu::GetSelectedPlayer().m_CageType, 0, NUMOF(Cage), Cage)
			.addOnClick([] { CagePlayer(Menu::GetSelectedPlayer()); })
			.addOnUpdate([](ScrollOption<int>* Option) { Option->addScroll(Menu::GetSelectedPlayer().m_CageType, 0, NUMOF(Cage), Cage); }));

		addOption(ScrollOption<int>(SCROLLSELECT, getString("Clone"))
			.addScroll(Menu::GetSelectedPlayer().m_CloneType, 0, NUMOF(Clone), Clone)
			.addOnClick([] { ClonePlayer(Menu::GetSelectedPlayer()); })
			.addOnUpdate([](ScrollOption<int>* Option) { Option->addScroll(Menu::GetSelectedPlayer().m_CloneType, 0, NUMOF(Clone), Clone); }));

		addOption(ScrollOption<int>(SCROLLSELECT, getString("Show Message"))
			.addScroll(Menu::GetSelectedPlayer().m_Messages, 0, NUMOF(Messages), Messages)
			.addOnClick([] { SendMessagePlayer(Menu::GetSelectedPlayer()); })
			.addOnUpdate([](ScrollOption<int>* Option) { Option->addScroll(Menu::GetSelectedPlayer().m_Messages, 0, NUMOF(Messages), Messages); }));
	} else {
		SetParentSubmenu<NetworkSessionMenu>();

		addOption(SubmenuOption(getString("Sounds"))
			.addSubmenu<NetworkTrollingSoundsMenu>());

		addOption(ToggleOption(getString("Hostile Traffic"))
			.addToggle(m_Vars.m_HostileTraffic));

		addOption(ToggleOption(getString("Fake Money Drop"))
			.addToggle(m_Vars.m_FakeMoneyDrop));

		addOption(ButtonOption(getString("Glitch Physics"))
			.addOnClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					NetworkPlayersAbusiveMenuVars::GlitchPhysics(Player);
				});
			}));

		addOption(ButtonOption(getString("Remove Weapons"))
			.addOnClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					Native::RemoveAllPedWeapons(Player.m_Ped, true);
				});
			}));

		addOption(ButtonOption(getString("Taze"))
			.addOnClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					NetworkPlayersAbusiveMenuVars::TazePlayer(Player);
				});
			})
			.addTooltip("You have to be near them"));

		addOption(ButtonOption(getString("Ragdoll"))
			.addOnClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					NetworkPlayersAbusiveMenuVars::RagdollPlayer(Player);
				});
			}));

		addOption(ButtonOption(getString("Kick from Vehicle"))
			.addOnClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					NetworkPlayersAbusiveMenuVars::KickFromVehiclePlayer(Player);
				});
			}));

		addOption(ButtonOption(getString("Give Wanted Level"))
			.addOnClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					NetworkPlayersAbusiveMenuVars::GiveWantedLevel(Player, 5);
				});
			}));

		addOption(ScrollOption<uint32_t>(SCROLLSELECT, getString("Vehicle Ram"))
			.addScroll(m_Vars.m_RamType, 0, NUMOF(RamVehicles), RamVehicles)
			.addOnClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					RamPlayer(Player);
				});
			}));

		addOption(ScrollOption<uint32_t>(SCROLLSELECT, getString("Remove Weapon Group"))
			.addScroll(m_Vars.m_RemoveWeaponGroupType, 0, NUMOF(WeaponGroups), WeaponGroups)
			.addOnClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					RemoveWeaponGroupPlayer(Player, WeaponGroups[m_Vars.m_RemoveWeaponGroupType].m_Result);
				});
			}));

		addOption(ScrollOption<ExplosionTypes>(SCROLLSELECT, getString("Jet"))
			.addScroll(m_Vars.m_JetType, 0, NUMOF(Jet), Jet)
			.addOnClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					JetPlayer(Player);
				});
			}));

		addOption(ScrollOption<int>(SCROLLSELECT, getString("Cage"))
			.addScroll(m_Vars.m_CageType, 0, NUMOF(Cage), Cage)
			.addOnClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					CagePlayer(Player);
				});
			}));

		addOption(ScrollOption<int>(SCROLLSELECT, getString("Clone"))
			.addScroll(m_Vars.m_CloneType, 0, NUMOF(Clone), Clone)
			.addOnClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					ClonePlayer(Player);
				});
			}));

		addOption(ScrollOption<int>(SCROLLSELECT, getString("Show Message"))
			.addScroll(m_Vars.m_Messages, 0, NUMOF(Messages), Messages)
			.addOnClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					SendMessagePlayer(Player);
				});
			}));
	}
}

/*Called always*/
void NetworkPlayersTrollingMenu::FeatureUpdate() {
	Menu::GetPlayerManager()->Foreach([] (Menu::PlayerVars& Player) {
		if (Player.m_HostilePeds) {
			Menu::Timers::RunTimedFunction(&Player.m_HostilePedsTimer, 1000, [=] {
					HostilePedsPlayer(Player); 
			});
		}

		if (Player.m_HostileTraffic || m_Vars.m_HostileTraffic) {
			Menu::Timers::RunTimedFunction(&Player.m_HostileTrafficTimer, 500, [=] {
				HostileTrafficPlayer(Player);
			});
		}

		if (Player.m_FakeMoneyDrop || m_Vars.m_FakeMoneyDrop) {
			Menu::Timers::RunTimedFunction(&Player.m_FakeMoneyDropTimer, 500, [=] {
				FakeMoneyPlayer(Player);
			});
		}
	});
}

NetworkPlayersTrollingMenu* _instance;
NetworkPlayersTrollingMenu* NetworkPlayersTrollingMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkPlayersTrollingMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkPlayersTrollingMenu::~NetworkPlayersTrollingMenu() { delete _instance; }