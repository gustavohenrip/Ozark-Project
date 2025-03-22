#include "hooks.hpp"
#include "utils/json.hpp"
#include "rage/invoker/natives.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_presence_events.hpp"
#include "menu/base/submenus/main/network/network_presence.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/submenus/main.hpp"
#include "utils/log.hpp"
#include "menu/base/submenus/main/network/spoofing/network_spoofing_name.hpp"

TranslationString T_PresenceTextMessage("Presence: Text Message", true, true);
TranslationString T_PresenceCrewMessage("Presence: Crew Message", true, true);
TranslationString T_PresenceGameInvite("Presence: Game Invite", true, true);
TranslationString T_PresenceServerRewards("Presence: Server Rewards", true, true);
TranslationString T_PresenceAdminInvite("Presence: Admin Invite", true, true);
TranslationString T_PresenceStatUpdate("Presence: Stat Update", true, true);

static char g_Presence[0x200];
bool Menu::Hooks::HandlePresenceEventHook(uint64_t Rcx, uint64_t Rdx, const char** Payload, uint64_t R9) {
	LOG_DEV(*Payload);

	try {
		const char* Key = "gm.evt";

		nlohmann::json Json = nlohmann::json::parse(*Payload);
		if (Json[Key].is_null()) {
			if (Json["gta5.game.event"].is_null()) {
				return true;
			}

			Key = "gta5.game.event";
		}

		auto& EventPayload = Json[Key];
		if (EventPayload["e"].is_null() || EventPayload["d"].is_null()) {
			return true;
		}

		auto ProtectVar = NetworkProtectionPresenceEventsMenuVars::m_Vars;

		uint32_t PresenceHash = Native::GetHashKey(EventPayload["e"].get<std::string>().c_str());
		switch (PresenceHash) {
			case PRESENCE_TEXT_MESSAGE: {
				if (ProtectVar.m_TextMessage.m_Notify) {
					Menu::GetNotify()->NotifyStackedProtectionEvent("~c~Unknown~w~", TRANSLATE(T_PresenceTextMessage), false, ProtectVar.m_TextMessage.m_Block);
				}

				if (ProtectVar.m_TextMessage.m_Block) return true;
				break;
			}

			case PRESENCE_CREW_MESSAGE: {
				if (ProtectVar.m_CrewMessage.m_Notify) {
					Menu::GetNotify()->NotifyStackedProtectionEvent("~c~Unknown~w~", TRANSLATE(T_PresenceCrewMessage), false, ProtectVar.m_CrewMessage.m_Block);
				}

				if (ProtectVar.m_CrewMessage.m_Block) return true;
				break;
			}

			case PRESENCE_GAME_INVITE: {
				return true;
				if (EventPayload["d"]["f"].get<int>() == 0x8000 || EventPayload["d"]["f"].get<int>() == 0x8001) {
					if (!EventPayload["d"]["n"].get<std::string>().compare(Global::Vars::g_RsInfo->m_Name)
						|| !EventPayload["d"]["n"].get<std::string>().compare(NetworkSpoofingNameMenuVars::m_Vars.m_SpoofedName)) {
						EventPayload["d"]["f"] = EventPayload["d"]["f"].get<int>() == 0x8000 ? 0xA000 : 0xA010 | 0x4000;
						LOG_DEV(EventPayload.dump().c_str());

						strcpy(g_Presence, Json.dump().c_str());
						*Payload = g_Presence;

						return OriginalHandlePresenceEvent(Rcx, Rdx, Payload, R9);
					}
				}

				/*if (ProtectVar.m_GameInvite.m_Notify) {
					Menu::GetNotify()->NotifyStackedProtectionEvent("~c~Unknown~w~", TRANSLATE(T_PresenceGameInvite), false, ProtectVar.m_GameInvite.m_Block);
				}

				if (ProtectVar.m_GameInvite.m_Block) return true;*/

				break;
			}

			case PRESENCE_GAME_SERVER: {
				if (ProtectVar.m_GameServerRewards.m_Notify) {
					Menu::GetNotify()->NotifyStackedProtectionEvent("~c~Unknown~w~", TRANSLATE(T_PresenceServerRewards), false, ProtectVar.m_GameServerRewards.m_Block);
				}

				if (ProtectVar.m_GameServerRewards.m_Block) return true;
				break;
			}

			case PRESENCE_ADMIN_INVITE: {
				if (ProtectVar.m_AdminInvite.m_Notify) {
					Menu::GetNotify()->NotifyStackedProtectionEvent("~c~Unknown~w~", TRANSLATE(T_PresenceAdminInvite), false, ProtectVar.m_AdminInvite.m_Block);
				}

				if (ProtectVar.m_AdminInvite.m_Block) return true;
				break;
			}

			case PRESENCE_STAT_UPDATE: {
				if (ProtectVar.m_StatUpdate.m_Notify) {
					Menu::GetNotify()->NotifyStackedProtectionEvent("~c~Unknown~w~", TRANSLATE(T_PresenceStatUpdate), false, ProtectVar.m_StatUpdate.m_Block);
				}

				if (ProtectVar.m_StatUpdate.m_Block) return true;
				break;
			}
		}

		return OriginalHandlePresenceEvent(Rcx, Rdx, Payload, R9);
	} catch (...) {
		return true;
	}
}