#include "hooks.hpp"
#include "menu/base/util/helpers.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/submenus/main/network/network_host_toolkit.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_notifications.hpp"
#include "menu/base/submenus/main/network/toolkit/network_host_toolkit_desync.hpp"
#include "menu/base/submenus/main.hpp"
#include "utils/log.hpp"
#include "rage/engine.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_event_reactions.hpp"

TranslationString T_DesyncKicking("Desync Kicking", true, true);
TranslationString T_AttemptingToDesyncKick("is attempting to desync kick", true, true);

bool Menu::Hooks::HostDesyncPlayerRequestHook(uint64_t Rcx, uint64_t Rdx, Rage::Network::ListKickPeers* R8, uint32_t R9d) {
	// Ensures that if two people are desync kicking people at the same time, it handles it properly

	uint64_t Sender = 0;
	uint64_t Target = 0;

	auto Vit = std::find_if(begin(Global::Vars::g_DesyncsHost), end(Global::Vars::g_DesyncsHost), [=](std::unordered_map<uint64_t, uint64_t>::value_type& Element) {
		return Element.first == R8->m_Peers[0].m_Peer;
	});

	if (Vit != end(Global::Vars::g_DesyncsHost)) {
		Sender = R8->m_Peers[0].m_Peer;
		Target = R8->m_Peers[1].m_Peer;
	} else {
		Vit = std::find_if(begin(Global::Vars::g_DesyncsHost), end(Global::Vars::g_DesyncsHost), [=](std::unordered_map<uint64_t, uint64_t>::value_type& Element) {
			return Element.first == R8->m_Peers[1].m_Peer;
		});

		if (Vit != end(Global::Vars::g_DesyncsHost)) {
			Sender = R8->m_Peers[1].m_Peer;
			Target = R8->m_Peers[0].m_Peer;
		}
	}

	auto& PlayerVars = NetworkHostToolkitDesyncMenuVars::m_Vars;

	if (Sender && Target) {
		Menu::PlayerVars& SenderPlayer = Menu::Helpers::GetPlayerFromPeer(Sender);
		Menu::PlayerVars& TargetPlayer = Menu::Helpers::GetPlayerFromPeer(Target);

		if (SenderPlayer.m_Connected && TargetPlayer.m_Connected) {
			if (PlayerVars.m_Notify) {
				auto V = std::find_if(begin(PlayerVars.m_Desyncs), end(PlayerVars.m_Desyncs), [=](std::pair<uint64_t, uint64_t>& Element) {
					return Element.first == Sender && Element.second == Target;
				});

				if (V == end(PlayerVars.m_Desyncs)) {
					PlayerVars.m_Desyncs.push_back({ Sender, Target });
					Menu::GetNotify()->NotifyStackedProtectionEvent(SenderPlayer.m_Name, std::string(TRANSLATE(T_DesyncKicking)) + " " + std::string(TargetPlayer.m_Name), PlayerVars.m_Redirect, PlayerVars.m_Block);

					if (!SenderPlayer.m_ModderFlags[REASON_QUANTUM_KICK]) {
						NetworkProtectionEventReactionsMenuVars::ProcessEvent(NetworkProtectionEventReactionsMenuVars::EVENT_QUANTUM_KICK, SenderPlayer.m_ID);
					}

					SenderPlayer.m_ModderFlags[REASON_QUANTUM_KICK] = true;
					SenderPlayer.m_IsModder = true;
				}
			}

			if (PlayerVars.m_Redirect) {
				if (IsValidPtr(SenderPlayer.m_NetGamePlayer)) {
					if (IsValidPtr(SenderPlayer.m_NetGamePlayer->m_PlayerInfo)) {
						R8->m_Peers[1].m_Peer = SenderPlayer.m_NetGamePlayer->m_PlayerInfo->m_HostToken;
						return OriginalHostDesyncPlayerRequest(Rcx, Rdx, R8, R9d);
					}
				}
			}
		}
	}

	// Checking for redirects too because this will only get here if it couldn't resolve a sender
	if (PlayerVars.m_Block || PlayerVars.m_Redirect) {
		R9d = 0;
	}

	return OriginalHostDesyncPlayerRequest(Rcx, Rdx, R8, R9d);
}

uint64_t Menu::Hooks::HandleNegotiationMsgHook(uint64_t Rcx, uint64_t Rdx, uint64_t R8) {
	Rage::BitBuffer BitBuffer;
	BitBuffer.Prepare(*(uint64_t*)(R8 + 0x38), *(uint32_t*)(R8 + 0x40));

	int MessageID = 0;
	if (BitBuffer.ReadNetMsgHeader(&MessageID)) {
		if (MessageID == 1) { // rage::reassignNegotiateMsg
			uint32_t SenderIP = *(uint32_t*)(R8 + 0xC);

			uint32_t Mask = 0;
			BitBuffer.ReadUint(&Mask, 8);

			uint32_t PeerCount = 0;
			BitBuffer.ReadUint(&PeerCount, 5);

			// 1 target
			if (PeerCount == 1) {
				// Iterate in case of future code
				uint64_t* Peers = new uint64_t[PeerCount];

				for (uint32_t i = 0; i < PeerCount; i++) {
					BitBuffer.ReadULongLong(&Peers[i], 0x40);
				}

				if (Menu::Helpers::GetPlayerFromPeer(Peers[0]).m_Connected && Menu::Helpers::GetPlayerFromPeer(Peers[0]).IsValidName()) {
					auto Vit = std::find_if(begin(Global::Vars::g_Desyncs), end(Global::Vars::g_Desyncs), [=] (std::unordered_map<uint32_t, Desyncs>::value_type& Element) {
						return Element.first == SenderIP;
					});

					if (Vit == end(Global::Vars::g_Desyncs)) {
						Global::Vars::g_Desyncs[SenderIP] = { Peers[0], GetTickCount(), 1, false };
					} else {
						if (GetTickCount() - Global::Vars::g_Desyncs[SenderIP].m_Last <= 4000) {
							Global::Vars::g_Desyncs[SenderIP].m_Possibility = true;
							Global::Vars::g_Desyncs[SenderIP].m_Count++;
						} else Global::Vars::g_Desyncs[SenderIP].m_Possibility = false;

						Global::Vars::g_Desyncs[SenderIP].m_Last = GetTickCount();

						if (Global::Vars::g_Desyncs[SenderIP].m_Possibility && Global::Vars::g_Desyncs[SenderIP].m_Count >= 10) {
							auto& Player = Menu::Helpers::GetPlayerFromIP(SenderIP);

							// Don't notify if host, it's handled in the kick hook
							if (NetworkProtectionNotificationsMenuVars::m_Vars.m_Desyncs
								&& Menu::GetLocalPlayer().m_NetGamePlayer != Rage::Engine::GetHostNetGamePlayer()) {
								Menu::GetNotify()->NotifyStacked(Menu::Helpers::CleanPlayerName(Player.m_Name) + " " + TRANSLATE(T_AttemptingToDesyncKick) + " " + Menu::Helpers::GetPlayerFromPeer(Peers[0]).m_Name);
							}

							if (!Player.m_ModderFlags[REASON_QUANTUM_KICK]) {
								NetworkProtectionEventReactionsMenuVars::ProcessEvent(NetworkProtectionEventReactionsMenuVars::EVENT_QUANTUM_KICK, Player.m_ID);
							}

							Player.m_ModderFlags[REASON_QUANTUM_KICK] = true;
							Player.m_IsModder = true;
							
							if (Menu::GetLocalPlayer().m_NetGamePlayer == Rage::Engine::GetHostNetGamePlayer()) {
								Global::Vars::g_DesyncsHost[Player.m_NetGamePlayer->m_PlayerInfo->m_HostToken] = Peers[0];
							}

							Global::Vars::g_Desyncs.erase(SenderIP);
						}
					}
				}

				delete[] Peers;
			}
		}
	}

	return OriginalHandleNegotiationMsg(Rcx, Rdx, R8);
}