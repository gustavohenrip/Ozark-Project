#include "hooks.hpp"
#include "utils/log.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/va.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_notifications.hpp"
#include "menu/base/util/player_manager.hpp"

TranslationString T_PreventedJoinTimeout("Prevented join timeout from", true, true);

void Menu::Hooks::UpdatePlayerScriptStatusHook(Rage::Script::GameScriptHandlerNetworkComponent* Component, uint64_t Rdx, uint64_t R8) {
	if (Component->m_State == 2) {
#ifdef DEV
		std::string State = "";
		switch (*(uint32_t*)(Rdx + 8)) {
			case 0: State = "REMOVE_BITSET"; break;
			case 1: State = "SCRIPT_ADD_CANDIDATE"; break;
			case 2: State = "SCRIPT_ADDED_CANDIDATE"; break;
			case 3: State = "SCRIPT_RETRY_JOIN"; break;
		}
#endif

		Rage::Network::NetGamePlayer* Player = *(Rage::Network::NetGamePlayer**)(R8 + 0x10);
		if (IsValidPtr(Player)) {
			if (IsValidPtr(Player->m_PlayerInfo)) {
				if (IsValidPtr(Component)) {
					if (IsValidPtr(Component->m_Component)) {
						if (IsValidPtr(Component->m_Component->m_Thread)) {
							LOG_DEV("[%s][%s] State=%s", Player->m_PlayerInfo->m_Name, Component->m_Component->m_Thread->m_Name, State.c_str());

							if (*(uint32_t*)(Rdx + 8) == 3) {
								auto Vit = std::find_if(begin(Global::Vars::g_JoinTimeoutFix), end(Global::Vars::g_JoinTimeoutFix), [=](JoinTimeoutFix& Element) {
									return Element.m_ID == Player->m_ID && Element.m_ScriptHash == Component->m_Component->m_Thread->m_ScriptHash;
								});

								if (Vit == end(Global::Vars::g_JoinTimeoutFix)) {
									Global::Vars::g_JoinTimeoutFix.push_back({ Player->m_ID, Component->m_Component->m_Thread->m_ScriptHash, 1 });
									goto end;
								}

								Vit->m_RetryCount++;
								if (Vit->m_RetryCount >= 5) {
									*(uint32_t*)(Rdx + 8) = 1;

									if (!strcmp(Component->m_Component->m_Thread->m_Name, "freemode")) {
										Menu::GetPlayer(Player->m_ID).m_IsModder = true;
										Menu::GetPlayer(Player->m_ID).m_ModderFlags[Menu::REASON_JOIN_TIMEOUT] = true;

										if (NetworkProtectionNotificationsMenuVars::m_Vars.m_JoinTimeout) {
											Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s", TRANSLATE(T_PreventedJoinTimeout), Player->m_PlayerInfo->m_Name), Global::UIVars::g_NotificationError);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

end:
	OriginalUpdatePlayerScriptStatus(Component, Rdx, R8);
}