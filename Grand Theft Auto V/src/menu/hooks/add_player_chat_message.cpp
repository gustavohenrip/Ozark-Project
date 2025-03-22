#include "hooks.hpp"
#include "menu/base/base.hpp"
#include "rage/engine.hpp"
#include "menu/base/submenus/main/network/players/network_players_chat_commands.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_event_limits.hpp"
#include "menu/base/submenus/main/misc/misc_disables.hpp"
#include "menu/base/util/player_manager.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/va.hpp"
#include "global/arrays.hpp"
#include "menu/base/submenus/main.hpp"
#include "utils/caller.hpp"
#include "utils/memory/pattern.hpp"
#include "utils/memory/memory.hpp"
#include "utils/log.hpp"
#include "utils/fiber_pool.hpp"
#include "utils/fibers.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_notifications.hpp"

Rage::Network::NetGamePlayer* CachedTextMsgSender = nullptr;
Rage::Network::NetGamePlayer* CachedChatMsgSender = nullptr;

TranslationString T_SpoofingChatTo("Spoofing Chat to", true, true);
TranslationString T_SpoofingTextTo("Spoofing Text Message to", true, true);

bool Menu::Hooks::ReadTextMessageBitbufferHook(Rage::Network::TextMessageNetMsg* Identifier, uint64_t Rdx) {
	if (CachedTextMsgSender) {
		int Token = 0;
		int Counter = 0;

		Global::Vars::g_BypassProfanityChecks = true;
		if (Native::ScProfanityCheckString(Identifier->m_Message, &Token)) {
			while (Native::ScProfanityGetCheckIsPending(Token)) {
				Counter++;
				if (Counter > 20) break;
				Utils::WaitFiber(100);
			}

			// DIRTY DIRTY DIRTY
			if (Native::ScProfanityGetStringStatus(Token) == 1) {
				Menu::GetPlayer(CachedTextMsgSender->m_ID).m_ModderFlags[Menu::REASON_PROFANITY] = true;
				Menu::GetPlayer(CachedTextMsgSender->m_ID).m_IsModder = true;
			}
		}
	}

	auto Return = OriginalReadTextMessageBitbuffer(Identifier, Rdx);
	Rage::Network::NetGamePlayer* NetPlayer = nullptr;

	if (NetworkProtectionEventLimitsMenuVars::m_Vars.m_BlockSpoofedTextMessages) {
		if (NetworkProtectionEventLimitsMenuVars::m_Vars.m_BlockSpoofedTextMessagesID == 1) {
			return false;
		}
	}

	Menu::GetPlayerManager()->Foreach([&](Menu::PlayerVars Player) {
		if (Player.m_Connected && Player.m_NetGamePlayer) {
			if (Player.m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID == Identifier->m_Sender.m_RockstarID) {
				NetPlayer = Player.m_NetGamePlayer;
			}
		}
	}, true, true);

	if (CachedTextMsgSender) {
		if (Identifier->m_Sender.m_RockstarID != CachedTextMsgSender->m_PlayerInfo->m_Identifier.m_RockstarID) {
			if (NetworkProtectionNotificationsMenuVars::m_Vars.m_SpoofedText) {
				if (NetPlayer) {
					Menu::GetNotify()->NotifyStackedProtectionEvent(CachedTextMsgSender->m_PlayerInfo->m_Name, Utils::VA::VA("%s %s", TRANSLATE(T_SpoofingTextTo), NetPlayer->m_PlayerInfo->m_Name), false, true);
				}
			}

			Menu::GetPlayer(CachedTextMsgSender->m_ID).m_ModderFlags[Menu::REASON_SPOOFED_TEXT] = true;
			Menu::GetPlayer(CachedTextMsgSender->m_ID).m_IsModder = true;

			if (NetworkProtectionEventLimitsMenuVars::m_Vars.m_BlockSpoofedTextMessages) {
				if (NetworkProtectionEventLimitsMenuVars::m_Vars.m_BlockSpoofedTextMessagesID == 0) {
					return false;
				}
			}
		}
	}

	if (CachedTextMsgSender) {
		static std::unordered_map<int, uint32_t> Limits;

		if ((GetTickCount() - Limits[CachedTextMsgSender->m_ID]) < (uint32_t)Global::Arrays::g_Timers[NetworkProtectionEventLimitsMenuVars::m_Vars.m_TextMessageDelay].m_Result) {
			return false;
		}

		Limits[CachedTextMsgSender->m_ID] = GetTickCount();
	}

	return Return;
}

bool Menu::Hooks::ReadChatMessageBitbufferHook(Rage::Network::GlobalMsgContext* Identifier, uint64_t Rdx) {
	if (CachedChatMsgSender) {
		int ID = CachedChatMsgSender->m_ID;
		int Token = 0;
		int Counter = 0;

		Global::Vars::g_BypassProfanityChecks = true;
		if (Native::ScProfanityCheckString(Identifier->m_Message, &Token)) {
			while (Native::ScProfanityGetCheckIsPending(Token)) {
				Counter++;
				if (Counter > 20) break;
				Utils::WaitFiber(100);
			}

			// DIRTY DIRTY DIRTY
			if (Native::ScProfanityGetStringStatus(Token) == 1) {
				Menu::GetPlayer(ID).m_ModderFlags[Menu::REASON_PROFANITY] = true;
				Menu::GetPlayer(ID).m_IsModder = true;
			}
		}
	}

	if (MiscDisablesMenuVars::m_Vars.m_DisableChatOnOpen) {
		if (Menu::GetBase()->m_Open) {
			Identifier->m_Message[0] = 0;
			Identifier->m_SenderChatToken = 0;
			return false;
		}
	}

	if (NetworkProtectionEventLimitsMenuVars::m_Vars.m_BlockSpoofedChatMessages) {
		if (NetworkProtectionEventLimitsMenuVars::m_Vars.m_BlockSpoofedChatMessagesID == 1) {
			Identifier->m_Message[0] = 0;
			Identifier->m_SenderChatToken = 0;
			return false;
		}
	}

	auto Return = OriginalReadChatMessageBitbuffer(Identifier, Rdx);
	Rage::Network::NetGamePlayer* NetPlayer = nullptr;

	Menu::GetPlayerManager()->Foreach([&](Menu::PlayerVars Player) {
		if (Player.m_Connected && Player.m_NetGamePlayer) {
			if (Player.m_NetGamePlayer->m_PlayerInfo->m_ChatToken == Identifier->m_SenderChatToken) {
				NetPlayer = Player.m_NetGamePlayer;
			}
		}
	}, true, true);

	if (CachedChatMsgSender) {
		if (Identifier->m_SenderChatToken != CachedChatMsgSender->m_PlayerInfo->m_ChatToken) {
			if (NetworkProtectionNotificationsMenuVars::m_Vars.m_SpoofedChat) {
				if (NetPlayer) {
					Menu::GetNotify()->NotifyStackedProtectionEvent(CachedChatMsgSender->m_PlayerInfo->m_Name, Utils::VA::VA("%s %s", TRANSLATE(T_SpoofingChatTo), NetPlayer->m_PlayerInfo->m_Name), false, true);
				}
			}

			Menu::GetPlayer(CachedChatMsgSender->m_ID).m_ModderFlags[Menu::REASON_SPOOFED_CHAT] = true;
			Menu::GetPlayer(CachedChatMsgSender->m_ID).m_IsModder = true;

			if (NetworkProtectionEventLimitsMenuVars::m_Vars.m_BlockSpoofedChatMessages) {
				if (NetworkProtectionEventLimitsMenuVars::m_Vars.m_BlockSpoofedChatMessagesID == 0) {
					Identifier->m_Message[0] = 0;
					Identifier->m_SenderChatToken = 0;
					return false;
				}
			}
		}
	}

	if (CachedChatMsgSender) {
		static std::unordered_map<int, uint32_t> Limits;

		if ((GetTickCount() - Limits[CachedChatMsgSender->m_ID]) < (uint32_t)Global::Arrays::g_Timers[NetworkProtectionEventLimitsMenuVars::m_Vars.m_GlobalChatMessageDelay].m_Result) {
			Identifier->m_Message[0] = 0;
			Identifier->m_SenderChatToken = 0;
			return false;
		}

		Limits[CachedChatMsgSender->m_ID] = GetTickCount();
	}

	if (NetPlayer && NetPlayer->m_PlayerInfo && NetPlayer->m_PlayerInfo->m_Name) {
		std::istringstream Buffer(Identifier->m_Message);
		std::istream_iterator<std::string> Begin(Buffer), End;
		std::vector<std::string> Tokens(Begin, End);

		if (Tokens.size()) {
			if (Tokens.at(0).at(0) == '?' && Tokens.at(0).length() > 1) {
				NetworkPlayersChatCommandsMenuVars::CallCommand((Tokens.at(0).data() + 1), NetPlayer, Tokens);
			}
		}
	}

	return Return;
}

Rage::Network::NetGamePlayer* GetPlayerFromPtr(uint64_t Ptr) {
	Rage::Network::NetGamePlayer* Result = nullptr;

	if (IsValidPtr(Ptr)) {
		uint64_t _0x40Key = *(uint64_t*)(Ptr + 0x48);
		if (IsValidPtr(_0x40Key)) {
			uint32_t IP = *(uint32_t*)(_0x40Key + 0x60);
			if (IP) {
				Menu::GetPlayerManager()->Foreach([&] (Menu::PlayerVars Player) {
					if (Player.m_ID != Menu::GetLocalPlayer().m_ID) {
						if (Player.m_Connected) {
							if (Player.m_NetGamePlayer && Player.m_NetGamePlayer->m_PlayerInfo) {
								if (IP == Player.m_NetGamePlayer->m_PlayerInfo->m_ExternalIP) {
									Result = Player.m_NetGamePlayer;
								}
							}
						}
					}
				}, true, true);
			}
		}
	}

	return Result;
}

uint64_t Menu::Hooks::ReceiveTextMsgHook(uint64_t Rcx, uint64_t Rdx, uint64_t R8) {
	if (R8 && Global::Vars::g_NetworkBaseConfig) {
		CachedTextMsgSender = GetPlayerFromPtr(R8);
	}

	return OriginalReceiveTextMsg(Rcx, Rdx, R8);
}

uint64_t Menu::Hooks::ReceiveChatMsgHook(uint64_t Rcx, uint64_t Rdx, uint64_t R8) {
	if (Rdx && Global::Vars::g_NetworkBaseConfig) {
		CachedChatMsgSender = GetPlayerFromPtr(R8);
	}

	return OriginalReceiveChatMsg(Rcx, Rdx, R8);
}