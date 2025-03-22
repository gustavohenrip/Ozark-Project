#include "player_manager.hpp"
#include "rage/invoker/natives.hpp"
#include "rage/engine.hpp"
#include "menu/base/submenus/main/network/network_players.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/va.hpp"
#include "menu/base/util/helpers.hpp"
#include "menu/base/util/global.hpp"
#include "menu/base/submenus/main/settings/videos.hpp"
#include "utils/caller.hpp"
#include "menu/base/submenus/main/network/network_recent_players.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_event_reactions.hpp"

namespace Menu {
	TranslationString T_IsModdingInvalidModel("is modding: Invalid Model", true, true);
	TranslationString T_IsModdingSpoofedName("is modding: Spoofed Name", true, true);
	TranslationString T_IsModdingSpoofedPeer("is modding: Spoofed Peer", true, true);
	TranslationString T_IsModdingSpoofedRID("is modding: Spoofed RID", true, true);
	TranslationString T_IsSpoofingTheirIPTo("is spoofing their ip to", true, true);
	TranslationString T_TheirRealIPIs("Their real IP is", true, true);
	TranslationString T_ExplosiveAmmo("Explosive Ammo", true, true);
	TranslationString T_ExplosiveMelee("Explosive Melee", true, true);
	TranslationString T_FireAmmo("Fire Ammo", true, true);
	TranslationString T_SuperJump("Super Jump", true, true);
	TranslationString T_IsModding("is modding", true, true);
	TranslationString T_CRCRequestFlood("CRC Request Flood", true, true);
	TranslationString T_CRCNotReplied("CRC Not Replied", true, true);
	TranslationString T_CRCCompromised("CRC Compromised", true, true);
	TranslationString T_InfoChange1("Info Change 1", true, true);
	TranslationString T_InfoChange4("Info Change 4", true, true);
	TranslationString T_InfoChange5("Info Change 5", true, true);
	TranslationString T_InfoChange6("Info Change 6", true, true);
	TranslationString T_InfoChange7("Info Change 7", true, true);
	TranslationString T_GotReported("got reported", true, true);
	TranslationString T_GotReportedInfoChange2("got reported: Info Change 2", true, true);
	TranslationString T_GotReportedArxan("got reported: Arxan", true, true);

	void PlayerManager::Update() {
		for (int i = 0; i < 34; i++) {
			auto& Player = m_Players[i];

			if (Native::GetPlayerName(i) == nullptr) {
				Player.m_Connected = false;
				continue;
			}

			Player.m_Connected = strcmp(Native::GetPlayerName(i), "**Invalid**");

			Player.m_ID = i;
			Player.m_Ped = Native::GetPlayerPedScriptIndex(i);
			Player.m_Exists = Native::DoesEntityExist(Player.m_Ped);

			Player.m_IsScriptHost = Native::NetworkGetHostOfScript("freemode", -1, 0) == Player.m_ID;

			Player.m_Name = VideosMenuVars::m_Vars.m_HideMe && i == Menu::GetLocalPlayer().m_ID ? "Me" : Native::GetPlayerName(i);
			Native::NetworkHandleFromPlayer(i, &Player.m_NetworkHandle.m_Handle, (sizeof(Player.m_NetworkHandle) / 4));
		
			if (Native::NetworkIsHandleValid(&Player.m_NetworkHandle.m_Handle, (sizeof(Player.m_NetworkHandle) / 4))) {
				Player.m_IsFriend = Native::NetworkIsFriend(&Player.m_NetworkHandle.m_Handle);
			}

			Player.m_PedHeadshot = Menu::Global(1389221).At(Player.m_ID, 5).At(2).As<int>();

			if (i == Native::PlayerId()) {
				if (::Global::Vars::g_PedFactory) {
					if (::Global::Vars::g_PedFactory->m_LocalPed) {
						if (::Global::Vars::g_PedFactory->m_LocalPed->m_PlayerInfo) {
							Player.m_IsAiming = ::Global::Vars::g_PedFactory->m_LocalPed->m_PlayerInfo->IsFreeAiming();
						}
					}
				}
			} else {
				if (Player.m_NetGamePlayer) {
					if (Player.m_NetGamePlayer->m_PlayerInfo) {
						Player.m_IsAiming = Player.m_NetGamePlayer->m_PlayerInfo->IsFreeAiming();
					}
				}
			}

			if (!Player.m_Exists) {
				continue;
			}

			Player.m_Health = Native::GetEntityHealth(Player.m_Ped);
			Player.m_Alive = !Native::IsPedDeadOrDying(Player.m_Ped, true);
			Player.m_InVehicle = Native::IsPedInAnyVehicle(Player.m_Ped, true);
			Player.m_Entity = Player.m_InVehicle ? Native::GetVehiclePedIsIn(Player.m_Ped, false) : Player.m_Ped;
			if (!Player.m_Entity) Player.m_Entity = Player.m_Ped;

			Player.m_Vehicle = Native::GetVehiclePedIsIn(Player.m_Ped, false);
			Player.m_WeaponEntity = Native::GetCurrentPedWeaponEntityIndex(Player.m_Ped);
			Player.m_Coords = Native::GetEntityCoords(Player.m_Entity, false);
			Player.m_IsShooting = Native::IsPedShooting(Player.m_Ped);
			Player.m_Heading = Native::GetEntityHeading(Player.m_Ped);
			Player.m_HasWeaponInHand = (Player.m_WeaponHash && Player.m_WeaponHash != 2725352035);
			Player.m_Rotation = Native::GetEntityRotation(Player.m_Entity, 2);
			Player.m_Model = Native::GetEntityModel(Player.m_Ped);
			Player.m_IsInside = Native::GetInteriorAtCoords(Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z) > 0;
			Player.m_NetGamePlayer = Rage::Engine::GetNetGamePlayerFromIndex(Player.m_ID);
			Player.m_PedPointer = (Rage::Types::Ped*)Rage::Engine::GetEntityAddress(Player.m_Ped);

			Native::GetCurrentPedWeapon(Player.m_Ped, &Player.m_WeaponHash, false);
			if (Player.m_InVehicle) Native::GetCurrentPedVehicleWeapon(Player.m_Ped, &Player.m_VehicleWeaponHash);

			// Modder checks
			if (Player.m_Connected) {
				if (i != Native::PlayerId()) {
					if (Player.m_Model != 0x0 && Player.m_Model != 0x9c9effd8 && Player.m_Model != 0x705e61f2) {
						if (!Player.m_ModderFlags[Menu::REASON_INVALID_MODEL]) {
							Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s", Menu::Helpers::CleanPlayerName(Player.m_Name).c_str(), TRANSLATE(T_IsModdingInvalidModel)), ::Global::UIVars::g_NotificationError);
						}

						Player.m_ModderFlags[Menu::REASON_INVALID_MODEL] = true;
						Player.m_IsModder = true;
					}

					if (!VideosMenuVars::m_Vars.m_HideNames) {
						if (std::string(Player.m_Name).find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.") != std::string::npos || strlen(Player.m_Name) < 6) {
							if (!Player.m_ModderFlags[Menu::REASON_SPOOFED_NAME]) {
								NetworkProtectionEventReactionsMenuVars::ProcessEvent(NetworkProtectionEventReactionsMenuVars::EVENT_SPOOFED_NAME, Player.m_ID);

								Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s", Menu::Helpers::CleanPlayerName(Player.m_Name).c_str(), TRANSLATE(T_IsModdingSpoofedName)), ::Global::UIVars::g_NotificationError);
							}

							Player.m_ModderFlags[Menu::REASON_SPOOFED_NAME] = true;
							Player.m_IsModder = true;
						}
					}

					if (IsValidPtr(Player.m_NetGamePlayer)) {
						if (Player.m_NetGamePlayer->m_IsCheater) {
							Player.m_ModderFlags[Menu::REASON_NET_GAME_PLAYER_CHEATER] = true;
							Player.m_IsModder = true;
						}
						
						if (IsValidPtr(Player.m_NetGamePlayer->m_PlayerInfo)) {
							if (((Player.m_NetGamePlayer->m_PlayerInfo->m_HostToken >> 32) & 0xFFFFFFFF) < 0x1000 || Player.m_NetGamePlayer->m_PlayerInfo->m_HostToken < 0x1000) {
								if (!Player.m_ModderFlags[Menu::REASON_SPOOFED_PEER]) {
									NetworkProtectionEventReactionsMenuVars::ProcessEvent(NetworkProtectionEventReactionsMenuVars::EVENT_SPOOFED_PEER, Player.m_ID);
									Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s", Menu::Helpers::CleanPlayerName(Player.m_Name).c_str(), TRANSLATE(T_IsModdingSpoofedPeer)), ::Global::UIVars::g_NotificationError);
								}

								Player.m_ModderFlags[Menu::REASON_SPOOFED_PEER] = true;
								Player.m_IsModder = true;
							}

							auto Vit = std::find_if(::Global::Vars::g_RelayUsage.begin(), ::Global::Vars::g_RelayUsage.end(), [=] (RelayUsage& Usage) {
								return !strcmp(Usage.m_Name, Player.m_Name)/* && ((Usage.m_RockstarID == Player.m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID) || Usage.m_RockstarID == Player.m_NetGamePlayer->m_PlayerInfo->m_RockstarID)*/;
							});

							if (Vit != ::Global::Vars::g_RelayUsage.end()) {
								if (Player.m_NetGamePlayer->m_PlayerInfo->m_ExternalIP != Vit->m_ExternalIP) {
									if (!Player.m_ModderFlags[Menu::REASON_SPOOFED_IP]) {
										NetworkProtectionEventReactionsMenuVars::ProcessEvent(NetworkProtectionEventReactionsMenuVars::EVENT_SPOOFED_IP, Player.m_ID);

										char Buffer[16];
										sprintf(Buffer, "%i.%i.%i.%i", ((Vit->m_ExternalIP & 0xFF000000) >> 24), ((Vit->m_ExternalIP & 0xFF0000) >> 16), ((Vit->m_ExternalIP & 0xFF00) >> 8), (Vit->m_ExternalIP & 0xFF));

										char SpoofedBuffer[16];
										sprintf(SpoofedBuffer, "%i.%i.%i.%i", ((Player.m_NetGamePlayer->m_PlayerInfo->m_ExternalIP & 0xFF000000) >> 24), ((Player.m_NetGamePlayer->m_PlayerInfo->m_ExternalIP & 0xFF0000) >> 16), ((Player.m_NetGamePlayer->m_PlayerInfo->m_ExternalIP & 0xFF00) >> 8), (Player.m_NetGamePlayer->m_PlayerInfo->m_ExternalIP & 0xFF));

										// Update existing recent player entry with real IP
										NetworkRecentPlayersMenuVars::Save(Menu::Helpers::CleanPlayerName(Player.m_Name), Buffer, Player.m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID);
										Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s %s. %s %s", Menu::Helpers::CleanPlayerName(Player.m_Name).c_str(), TRANSLATE(T_IsSpoofingTheirIPTo), SpoofedBuffer, TRANSLATE(T_TheirRealIPIs), Buffer), ::Global::UIVars::g_NotificationError);
									}

									Player.m_NetGamePlayer->m_PlayerInfo->m_ExternalIP = Vit->m_ExternalIP;
									Player.m_ModderFlags[Menu::REASON_SPOOFED_IP] = true;
									Player.m_IsModder = true;
								}
							}

							// Frame flag checks
							std::pair<int, std::pair<Menu::ePlayerReportReasons, std::string>> FrameFlags[] = {
								{ 0x800, { Menu::REASON_EXPLOSIVE_AMMO, TRANSLATE(T_ExplosiveAmmo) } },
								{ 0x2000, { Menu::REASON_EXPLOSIVE_MELEE, TRANSLATE(T_ExplosiveMelee) } },
								{ 0x1000, { Menu::REASON_FIRE_AMMO, TRANSLATE(T_FireAmmo) } },
								{ 0x4000, { Menu::REASON_SUPER_JUMP, TRANSLATE(T_SuperJump) } }
							};

							for (auto& Flag : FrameFlags) {
								if ((Player.m_NetGamePlayer->m_PlayerInfo->m_FrameFlags & Flag.first) != 0) {
									if (!Player.m_ModderFlags[Flag.second.first]) {
										Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s: %s", Menu::Helpers::CleanPlayerName(Player.m_Name).c_str(), TRANSLATE(T_IsModding), Flag.second.second.c_str()), ::Global::UIVars::g_NotificationError);
									}

									Player.m_ModderFlags[Flag.second.first] = true;
									Player.m_IsModder = true;
								}
							}

							if (Player.m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID < 10000) {
								if (!Player.m_ModderFlags[Menu::REASON_SPOOFED_RID]) {
									NetworkProtectionEventReactionsMenuVars::ProcessEvent(NetworkProtectionEventReactionsMenuVars::EVENT_SPOOFED_RID, Player.m_ID);

									Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s", Menu::Helpers::CleanPlayerName(Player.m_Name).c_str(), TRANSLATE(T_IsModdingSpoofedRID)), ::Global::UIVars::g_NotificationError);
								}

								Player.m_ModderFlags[Menu::REASON_SPOOFED_RID] = true;
								Player.m_IsModder = true;
							}
						}

						static std::pair<std::pair<int, Menu::ePlayerReportReasons>, std::string> Reasons[] = {
							{ { 0, REASON_CRC_FLOOD }, TRANSLATE(T_CRCRequestFlood) },
							{ { 1, REASON_CRC_COMPROMISED }, TRANSLATE(T_CRCCompromised) },
							{ { 2, REASON_CRC_NOT_REPLIED }, TRANSLATE(T_CRCNotReplied) },
							{ { 4, REASON_INFO_CHANGE_1 }, TRANSLATE(T_InfoChange1) },
							{ { 7, REASON_INFO_CHANGE_4 }, TRANSLATE(T_InfoChange4) },
							{ { 8, REASON_INFO_CHANGE_5 }, TRANSLATE(T_InfoChange5) },
							{ { 9, REASON_INFO_CHANGE_6 }, TRANSLATE(T_InfoChange6) },
							{ { 10, REASON_INFO_CHANGE_7 }, TRANSLATE(T_InfoChange7) },
						};

						auto IsReportedForReason = [=] (Rage::Network::NetGamePlayer* Player, int Category, int Reason) {
							if (!IsValidPtr(Player)) return false;
							return Caller::Call<bool>(::Global::Vars::g_IsReportedForReason, Player, Category, Reason);
						};

						for (auto& Reason : Reasons) {
							for (int i = 0; i < 10; i++) {
								if (IsReportedForReason(Player.m_NetGamePlayer, Reason.first.first, i)) {
									if (!Player.m_ModderFlags[Reason.first.second]) {
										Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s: %s", Menu::Helpers::CleanPlayerName(Player.m_Name).c_str(), TRANSLATE(T_GotReported), Reason.second.c_str()), ::Global::UIVars::g_NotificationError);
									}

									Player.m_ModderFlags[Reason.first.second] = true;
									Player.m_IsModder = true;
									break;
								}
							}
						}

						if (IsReportedForReason(Player.m_NetGamePlayer, 5, 0)) {
							if (!Player.m_ModderFlags[Menu::REASON_INFO_CHANGE_2]) {
								Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s", Menu::Helpers::CleanPlayerName(Player.m_Name).c_str(), TRANSLATE(T_GotReportedInfoChange2)), ::Global::UIVars::g_NotificationError);
							}

							Player.m_ModderFlags[Menu::REASON_INFO_CHANGE_2] = true;
							Player.m_IsModder = true;
						}

						if (IsReportedForReason(Player.m_NetGamePlayer, 6, 0)) {
							if (!Player.m_ModderFlags[Menu::REASON_ARXAN]) {
								NetworkProtectionEventReactionsMenuVars::ProcessEvent(NetworkProtectionEventReactionsMenuVars::EVENT_ARXAN, Player.m_ID);
								Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s", Menu::Helpers::CleanPlayerName(Player.m_Name).c_str(), TRANSLATE(T_GotReportedArxan)), ::Global::UIVars::g_NotificationError);
							}

							Player.m_ModderFlags[Menu::REASON_ARXAN] = true;
							Player.m_IsModder = true;
						}
					}
				}
			}
		}
	}

	void PlayerManager::Foreach(std::function<void(PlayerVars&)> Callback, bool BypassExcludes, bool BypassGhost) {
		for (int i = 0; i < 34; i++) {
			if (!BypassGhost) {
				if (!m_Players[i].m_Exists) continue;
			}

			if (m_Players[i].m_Connected) {
				if (!BypassExcludes) {
					// handle excludes
					if (i == GetLocalPlayer().m_ID) continue;
				}

				Callback(m_Players[i]);
			}
		}
	}

	PlayerVars& PlayerManager::GetPlayer(int Index) {
		return m_Players[Index];
	}

	PlayerVars& PlayerManager::GetLocalPlayer() {
		return m_Players[Native::PlayerId()];
	}

	PlayerManager* GetPlayerManager() {
		static PlayerManager Instance;
		return &Instance;
	}

	PlayerVars& GetPlayer(int Index) {
		return GetPlayerManager()->GetPlayer(Index);
	}

	PlayerVars& GetLocalPlayer() {
		return GetPlayerManager()->GetLocalPlayer();
	}

	PlayerVars& GetSelectedPlayer() {
		return GetPlayerManager()->GetPlayer(NetworkPlayersMenuVars::m_Vars.m_SelectedPlayer);
	}
}