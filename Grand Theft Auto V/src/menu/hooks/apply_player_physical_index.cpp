#include "hooks.hpp"
#include "menu/base/util/player_manager.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/submenus/main/network/network_recent_players.hpp"
#include "menu/base/submenus/main/misc.hpp"
#include "utils/va.hpp"
#include "menu/base/util/helpers.hpp"
#include "menu/base/submenus/main/settings/videos.hpp"
#include "rage/engine.hpp"
#include "utils/log.hpp"
#include "menu/base/submenus/main/network/network_overseer.hpp"
#include "menu/base/submenus/main.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_entity_limits.hpp"
#include "rage/api/social.hpp"

TranslationString T_Is("is", true, true);
TranslationString T_Leaving("leaving", true, true);
TranslationString T_JoiningInto("joining into", true, true);
TranslationString T_Slot("slot", true, true);
TranslationString T_IsModdingInfoMismatch("is modding: Info Mismatch", true, true);

void Menu::Hooks::ApplyPlayerPhysicalIndexHook(uint64_t Rcx, Rage::Network::NetGamePlayer* NetGamePlayer, uint8_t R8b) {
	auto CleanAllocations = [] {
		LOG_INFO("Clearing memory created from session");

		Global::Vars::g_OnlineRockstarIds.clear();
		Global::Vars::g_OnlineRockstarIdQueue.clear();
		Global::Vars::g_Geo.clear();
		Global::Vars::g_RelayUsage.clear();
		Global::Vars::g_Desyncs.clear();
		Global::Vars::g_DesyncsHost.clear();
		Global::Vars::g_SpoofedDataJoinAttempts.clear();
		Global::Vars::g_LastSyncedPlayerModels.clear();
		Global::Vars::g_JoinTimeoutFix.clear();
		NetworkProtectionEntityLimitsMenuVars::m_Vars.m_SpamDetection.clear();
	};

	if (Menu::GetLocalPlayer().m_Connected && Native::NetworkIsInSession()) {
		if (IsValidPtr(NetGamePlayer) && IsValidPtr(NetGamePlayer->m_PlayerInfo)) {
			if (NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID != Global::Vars::g_RsInfo->m_RockstarID) {
				if (R8b != 0xFF && VideosMenuVars::m_Vars.m_HideNames) {
					if (strcmp(NetGamePlayer->m_PlayerInfo->m_Name, Rage::Engine::SocialClubGetGameInfoString("gtag"))) {
						VideosMenuVars::m_Vars.m_CachedData[NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID] = NetGamePlayer->m_PlayerInfo->m_Name;
						strcpy(NetGamePlayer->m_PlayerInfo->m_Name, Utils::VA::VA("Player_%i", R8b));
					}
				}

				auto Vit = std::find_if(begin(Global::Vars::g_JoinTimeoutFix), end(Global::Vars::g_JoinTimeoutFix), [=](JoinTimeoutFix& Element) {
					return Element.m_ID == (R8b == 0xFF ? NetGamePlayer->m_ID : R8b);
				});

				if (Vit != end(Global::Vars::g_JoinTimeoutFix)) {
					Global::Vars::g_JoinTimeoutFix.erase(Vit);
					LOG_DEV("Removed timeout cache for id %i", R8b == 0xFF ? NetGamePlayer->m_ID : R8b);
				}

				if (MiscMenuVars::m_Vars.m_ShowJoinEvents) {
					Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s %s %s %i", Menu::Helpers::CleanPlayerName(NetGamePlayer->m_PlayerInfo->m_Name).c_str(), TRANSLATE(T_Is), R8b == 0xFF ? TRANSLATE(T_Leaving) : TRANSLATE(T_JoiningInto), TRANSLATE(T_Slot), R8b == 0xFF ? NetGamePlayer->m_ID : R8b), R8b == 0xFF ? Global::UIVars::g_NotificationError : Global::UIVars::g_NotificationSuccess);
				}

				if (R8b != 0xFF) {
					uint32_t IP = NetGamePlayer->m_PlayerInfo->m_ExternalIP;

					char Buffer[16];
					sprintf(Buffer, "%i.%i.%i.%i", ((IP & 0xFF000000) >> 24), ((IP & 0xFF0000) >> 16), ((IP & 0xFF00) >> 8), (IP & 0xFF));

					NetworkRecentPlayersMenuVars::Save(Menu::Helpers::CleanPlayerName(NetGamePlayer->m_PlayerInfo->m_Name), Buffer, NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID);
					NetworkOverseerMenuVars::ProcessJoinNotify(NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID, IP, Menu::Helpers::CleanPlayerName(NetGamePlayer->m_PlayerInfo->m_Name));
				}
			} else CleanAllocations();
		}
	}

	if (R8b == 0xFF && !IsValidPtr(NetGamePlayer)) return OriginalApplyPlayerPhysicalIndex(Rcx, NetGamePlayer, R8b);;

	Menu::GetPlayer(R8b == 0xFF ? NetGamePlayer->m_ID : R8b).Reset(); // Reset player
	OriginalApplyPlayerPhysicalIndex(Rcx, NetGamePlayer, R8b);
}