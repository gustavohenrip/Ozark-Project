#include "network_players_selected.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_players.hpp"
#include "network_players_friendly.hpp"
#include "network_players_abusive.hpp"
#include "network_players_trolling.hpp"
#include "network_players_chat_commands.hpp"
#include "network_players_teleport.hpp"
#include "network_players_text.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/submenus/main/player/wardrobe.hpp"
#include "utils/caller.hpp"
#include "utils/va.hpp"
#include "rage/engine.hpp"
#include "global/auth_vars.hpp"
#include "rage/entity/entity.hpp"
#include "menu/base/util/helpers.hpp"
#include "menu/base/util/global.hpp"
#include "menu/base/submenus/main/helpers/esp.hpp"
#include "menu/base/submenus/main/network/network_overseer.hpp"
#include "menu/base/submenus/main/vehicle/vehicle_spawner.hpp"
#include "menu/base/submenus/main/npc/ped_spawner.hpp"
#include "network_players_spawn.hpp"
#include "../../network.hpp"
#include "utils/memory/pattern.hpp"
#include "utils/memory/memory.hpp"
#include "network_players_entity_blocking.hpp"
#include "network_players_removal.hpp"
#include "../spoofing/network_spoofing_profiles.hpp"
#include "menu/base/util/timers.hpp"

using namespace NetworkPlayersSelectedMenuVars;

namespace NetworkPlayersSelectedMenuVars {
	Vars_ m_Vars;

	void KickPlayer_() {
		auto NetworkBaseConfig = *(uint64_t*)(uint64_t)Global::Vars::g_NetworkBaseConfig;
		if (NetworkBaseConfig) {
			auto networkBaseConfig0x20Key = *(uint64_t*)(NetworkBaseConfig + (Global::Vars::g_Steam ? 0x70 : 0x20));
			if (networkBaseConfig0x20Key) {
				uint64_t UnkArg = *(uint64_t*)((networkBaseConfig0x20Key + 0x80) + 0x200);
				if (IsValidPtr(Menu::GetSelectedPlayer().m_NetGamePlayer)
					 && IsValidPtr(Menu::GetSelectedPlayer().m_NetGamePlayer->m_PlayerInfo)) {
					uint64_t HostPeer = Caller::Call<uint64_t>(Global::Vars::g_GetPeerFromNetGamePlayer, (uint64_t)Rage::Engine::GetHostNetGamePlayer());
					if (HostPeer) {
						int32_t* HostBaseConfig = Caller::Call<int32_t*>(Global::Vars::g_GetNetworkConfigFromPeer, networkBaseConfig0x20Key, HostPeer);
						if (HostBaseConfig) {
							Rage::Network::NetMsgIdentifier Target;
							memset(&Target, 0, sizeof(Target));

							Target.m_RockstarID = Menu::GetSelectedPlayer().m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID; // recipient
							Target.m_Type = 3;

							if (Menu::GetLocalPlayer().m_NetGamePlayer == Rage::Engine::GetHostNetGamePlayer()) {
								Caller::Call<bool>(Global::Vars::g_TaskLeaveGamer, networkBaseConfig0x20Key, HostBaseConfig, Target, 1, 0, 0, -1, 0); // host desync
							}
						}
					}
				}
			}
		}
	}

	void PrintDecors(Menu::PlayerVars& Player) {
		return;
		if (Player.m_PedPointer) {
			uint64_t DecoratorExtension = Caller::Call<uint64_t>(Global::Vars::g_GameAddress + 0x16446F4, (uint64_t)Player.m_PedPointer + 0x10, *(uint32_t*)(Global::Vars::g_GameAddress + 0x2E7BFEC));
			if (DecoratorExtension) {
				uint64_t Decorator = *(uint64_t*)(DecoratorExtension + 0x8);
				if (Decorator) {
					for (; Decorator; Decorator = *(uint64_t*)(Decorator + 8)) {
						if (*(uint32_t*)(Decorator + 0x14) == 0xE8E2F63) {

						}

						int Value = 0;
						Caller::Call<int>(Global::Vars::g_GameAddress + 0x1642854, Decorator, &Value);
						printf("\t[INT] 0x%X: %i\n", *(uint32_t*)(Decorator + 0x14), Value);
					}
				}
			}
		}
	}
}

void NetworkPlayersSelectedMenu::Init() {
	SetName("", false, false);
	SetParentSubmenu<NetworkPlayersMenu>();

	addOption(SubmenuOption("Friendly")
		.addTranslation()
		.addSubmenu<NetworkPlayersFriendlyMenu>());

	addOption(SubmenuOption("Teleport")
		.addTranslation()
		.addSubmenu<NetworkPlayersTeleportMenu>());

	addOption(SubmenuOption("Abusive")
		.addTranslation()
		.addSubmenu<NetworkPlayersAbusiveMenu>());

	addOption(SubmenuOption("Trolling")
		.addTranslation()
		.addSubmenu<NetworkPlayersTrollingMenu>());

	addOption(SubmenuOption("Removals")
		.addTranslation()
		.addSubmenu<NetworkPlayersRemovalMenu>());

	addOption(SubmenuOption("ESP")
		.addTranslation()
		.addSubmenu<ESPMenu>()
		.addOnClick([] {
			ESPMenu::GetInstance()->SetParentSubmenu<NetworkPlayersSelectedMenu>();
			ESPMenuVars::m_Vars.m_ESP = &Menu::GetSelectedPlayer().m_ESP;
			ESPMenuVars::m_Vars.m_Player = true;
		}));

	addOption(SubmenuOption("Chat Commands")
		.addTranslation()
		.addSubmenu<NetworkPlayersChatCommandsMenu>());

	addOption(SubmenuOption("Text Message")
		.addTranslation()
		.addSubmenu<NetworkPlayersTextMenu>());

	addOption(SubmenuOption("Spawner")
		.addTranslation()
		.addSubmenu<NetworkPlayersSpawnMenu>());

	addOption(SubmenuOption("Entity Blocking")
		.addTranslation()
		.addSubmenu<NetworkPlayersEntityBlockingMenu>());

	addOption(ButtonOption("Dump Spoofing Profile")
		.addTranslation()
		.addOnClick([] {
			if (!Menu::GetSelectedPlayer().m_NetGamePlayer || !Menu::GetSelectedPlayer().m_NetGamePlayer->m_PlayerInfo) return;
			uint32_t IP = Menu::GetSelectedPlayer().m_NetGamePlayer->m_PlayerInfo->m_ExternalIP;

			char Buffer[16];
			sprintf(Buffer, "%i.%i.%i.%i", ((IP & 0xFF000000) >> 24), ((IP & 0xFF0000) >> 16), ((IP & 0xFF00) >> 8), (IP & 0xFF));

			NetworkSpoofingProfilesMenuVars::SpoofProfile Profile;
			Profile.m_Name = Menu::GetSelectedPlayer().m_Name;
			Profile.m_IP = std::string(Buffer);
			Profile.m_RockstarID = Menu::GetSelectedPlayer().m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID;
			Profile.m_FileName = Profile.m_Name;
			Profile.m_Stats = {};

			Profile.m_CrewIcon = Menu::GetSelectedPlayer().m_NetGamePlayer->GetCrewID();
			Profile.m_CrewMotto = "";
			Profile.m_CrewName = Menu::GetSelectedPlayer().m_NetGamePlayer->GetCrewName();
			Profile.m_CrewRank = Menu::GetSelectedPlayer().m_NetGamePlayer->GetCrewRank();
			Profile.m_CrewTag = Menu::GetSelectedPlayer().m_NetGamePlayer->GetCrewTag();

			NetworkSpoofingProfilesMenuVars::SaveProfile(Profile);
		}));

	addOption(ToggleOption("Spectate")
		.addTranslation()
		.addToggle(Menu::GetSelectedPlayer().m_Spectate)
		.addOnUpdate([](ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_Spectate); })
		.addOnClick([] {
			if (Menu::GetSelectedPlayer().m_Spectate) {
				if (Native::NetworkIsInSpectatorMode()) {
					Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
						Native::NetworkSetInSpectatorMode(false, Player.m_Ped);
					});
				}

				Native::NetworkSetInSpectatorMode(true, Menu::GetSelectedPlayer().m_Ped);
			} else {
				Native::NetworkSetInSpectatorMode(false, Menu::GetSelectedPlayer().m_Ped);
			}
		}));

#ifdef DEV
	addOption(ButtonOption("[DEV] Dump Decors")
		.addOnClick([] { PrintDecors(Menu::GetSelectedPlayer()); }));
#endif
}

void NetworkPlayersSelectedMenu::Update() {}

/*Called once on submenu open*/
void NetworkPlayersSelectedMenu::UpdateOnce() {
	Global::Vars::g_NetworkMenuType = SELECTED_PLAYER;
	if (!Menu::GetSelectedPlayer().IsValidName()) return;
	SetName(Menu::GetSelectedPlayer().m_Name);
}

/*Called always*/
void NetworkPlayersSelectedMenu::FeatureUpdate() {
	static int Timer = 0;

	Menu::GetPlayerManager()->Foreach([] (Menu::PlayerVars& Player) {
		// Name ESP
		if (Player.m_ESP.m_NameLineOfSightRainbowToggle) {
			Player.m_ESP.m_NameLineOfSightRainbow.Run();
		}

		if (Player.m_ESP.m_NameRainbowToggle) {
			Player.m_ESP.m_NameRainbow.Run();
		}

		if (Player.m_ESP.m_Name) {
			ESPMenuVars::NameESP(Player.m_ESP, Player.m_Entity, Player.m_Name);
		}

		// Snapline ESP
		if (Player.m_ESP.m_SnaplineLineOfSightRainbowToggle) {
			Player.m_ESP.m_SnaplineLineOfSightRainbow.Run();
		}

		if (Player.m_ESP.m_SnaplineRainbowToggle) {
			Player.m_ESP.m_SnaplineRainbow.Run();
		}

		if (Player.m_ESP.m_Snapline) {
			ESPMenuVars::SnaplineESP(Player.m_ESP, Player.m_Entity);
		}

		// 3D ESP
		if (Player.m_ESP.m_3DLineOfSightRainbowToggle) {
			Player.m_ESP.m_3DLineOfSightRainbow.Run();
		}

		if (Player.m_ESP.m_3DRainbowToggle) {
			Player.m_ESP.m_3DRainbow.Run();
		}

		if (Player.m_ESP.m_3D) {
			ESPMenuVars::_3DESP(Player.m_ESP, Player.m_Entity);
		}

		Menu::Timers::RunTimedFunction(&Timer, 1000, [&] {
			return;
			if (Player.m_PedPointer) {
				uint64_t DecoratorExtension = Caller::Call<uint64_t>(Global::Vars::g_GameAddress + 0x1655180, (uint64_t)Player.m_PedPointer + 0x10, 14);
				if (DecoratorExtension) {
					uint64_t Decorator = *(uint64_t*)(DecoratorExtension + 0x8);
					if (Decorator) {
						for (; Decorator; Decorator = *(uint64_t*)(Decorator + 8)) {
							if (*(uint32_t*)(Decorator + 0x14) == 0xE8E2F63) {
								int Value = 0;
								Caller::Call<int>(Global::Vars::g_GameAddress + 0x1655004, Decorator, &Value);

								Player.m_IsModder = true;
								Player.m_ModderFlags[Menu::REASON_MENU_IMPULSE] = true;
								Player.m_ModderFlags[Menu::REASON_MENU_IMPULSE_VIP] = (Value & 4) != 0;
								Player.m_ModderFlags[Menu::REASON_MENU_IMPULSE_STAFF] = (Value & 8) != 0;
								Player.m_ModderFlags[Menu::REASON_MENU_IMPULSE_BLOCK] = (Value & 32) != 0;
								break;
							}
						}
					}
				}
			}
		});

	});

	Menu::Submenu* CurrentSubmenu = Menu::GetSubmenuHandler()->GetCurrentSubmenu();
	for (; CurrentSubmenu; CurrentSubmenu = CurrentSubmenu->m_Parent) {
		if (CurrentSubmenu == this) {
			if (!Menu::GetSelectedPlayer().m_Connected) {
				if (Menu::GetSubmenuHandler()->GetCurrentSubmenu() != NetworkPlayersMenu::GetInstance()) {
					Menu::GetSubmenuHandler()->SetSubmenu(NetworkPlayersMenu::GetInstance());
					Menu::GetSubmenuHandler()->m_PreviousSubmenu = NetworkMenu::GetInstance();
				}
			}

			break;
		}
	}
}

NetworkPlayersSelectedMenu* _instance;
NetworkPlayersSelectedMenu* NetworkPlayersSelectedMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkPlayersSelectedMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkPlayersSelectedMenu::~NetworkPlayersSelectedMenu() { delete _instance; }
