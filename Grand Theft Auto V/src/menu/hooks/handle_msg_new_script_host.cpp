#include "hooks.hpp"
#include "utils/log.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_notifications.hpp"
#include "menu/base/util/player_manager.hpp"
#include "menu/base/submenus/main.hpp"
#include "utils/caller.hpp"
#include "utils/va.hpp"
#include "menu/base/util/helpers.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_event_reactions.hpp"

TranslationString T_ScriptHostMigration("Script Host Migration", true, true);
TranslationString T_ScriptHostMigrationModding("is modding: Script Host Migration", true, true);

bool Menu::Hooks::HandleMsgNewScriptHostHook(uint64_t Rcx, uint64_t Rdx) {
	uint8_t Data[0x50];
	memset(Data, 0, 0x50);

	uint64_t ScriptTable = *(uint64_t*)(Rcx);
	*(uint64_t*)&Data[0] = Caller::Call<uint64_t>(*(uint64_t*)(ScriptTable + 0x78), Rcx);

	if (Caller::Call<bool>(Global::Vars::g_ReadMsgNewHost, Data, *(uint64_t*)(Rdx), *(uint32_t*)(Rdx + 0x8), 0)) {
		Rage::Network::NetGamePlayer* NewHost = *(Rage::Network::NetGamePlayer**)(Rdx + 0x10);

		if (IsValidPtr(NewHost)) {
			uint16_t HostToken = *(uint16_t*)&Data[0x8];
			if (HostToken == 0xFFFF) {
				if (!Menu::GetPlayer(NewHost->m_ID).m_ModderFlags[Menu::REASON_SCRIPT_MIGRATION]) {
					NetworkProtectionEventReactionsMenuVars::ProcessEvent(NetworkProtectionEventReactionsMenuVars::EVENT_SCRIPT_MIGRATION, NewHost->m_ID);
					Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s", Menu::Helpers::CleanPlayerName(NewHost->m_PlayerInfo->m_Name).c_str(), TRANSLATE(T_ScriptHostMigrationModding)), ::Global::UIVars::g_NotificationError);
				}

				Menu::GetPlayer(NewHost->m_ID).m_IsModder = true;
				Menu::GetPlayer(NewHost->m_ID).m_ModderFlags[Menu::REASON_SCRIPT_MIGRATION] = true;
			}

			Rage::Script::GameScriptHandler* ScriptHandler = Caller::Call<Rage::Script::GameScriptHandler*>(*(uint64_t*)(ScriptTable + 0x48), Rcx, *(uint64_t*)&Data[0]);
			if (IsValidPtr(ScriptHandler)) {
				if (IsValidPtr(ScriptHandler->m_Thread)) {
					// LOG_INFO("%s is taking host of script %s from %s", NewHost->m_PlayerInfo->m_Name, ScriptHandler->m_Thread->m_Name, Menu::GetPlayer(Native::NetworkGetHostOfScript(ScriptHandler->m_Thread->m_Name, -1, 0)).m_Name);

					if (!strcmp(ScriptHandler->m_Thread->m_Name, "freemode")) {
						if (NetworkProtectionNotificationsMenuVars::m_Vars.m_ScriptHostMigration) {
							if (NewHost->m_ID != Native::NetworkGetHostOfScript(ScriptHandler->m_Thread->m_Name, -1, 0)) {
								Menu::GetNotify()->NotifyStackedProtectionEvent(NewHost->m_PlayerInfo->m_Name, TRANSLATE(T_ScriptHostMigration), false, false);
							}
						}
					}
				}
			}
		}
	}

	return OriginalHandleMsgNewScriptHost(Rcx, Rdx);
}