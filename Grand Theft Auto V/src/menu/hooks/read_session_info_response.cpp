#include "hooks.hpp"
#include "utils/json.hpp"
#include "menu/base/submenus/main/network/network_presence.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_join_events.hpp"
#include "utils/log.hpp"
#include "rage/engine.hpp"

bool Menu::Hooks::ReadSessionInfoResponseHook(uint64_t Rcx) {
	bool Return = true;

	if (Rcx && *(uint32_t*)(Rcx + 0x2944)) {
		uint32_t i = 0;
		do {
			uint64_t Address = Rcx + 0x2840 + (i * 8);
			if (*(uint64_t*)(Address)) {
				const char* ResponseData = *(const char**)(Address);
				if (ResponseData) {
					try {
						nlohmann::json Json = nlohmann::json::parse(ResponseData);
						if (Json.find("gsinfo") == Json.end()) {
							return false;
						}

						uint32_t RockstarID = std::stoul(Json["_id"].get<std::string>().substr(3));
						std::string GsInfo = Json["gsinfo"].get<std::string>();

						if (RockstarID == NetworkPresenceMenuVars::m_Vars.m_PendingGsInfoRequestID) {
							if (GsInfo.empty()) {
								NetworkPresenceMenuVars::m_Vars.m_PendingTimeout = 0;
							} else {
								NetworkPresenceMenuVars::m_Vars.m_GsInfo = GsInfo;
							}

							Return = false;
						}

						if (RockstarID == NetworkProtectionJoinEventsMenuVars::m_Vars.m_PendingGsInfoRequestID) {
							if (GsInfo.empty()) {
								NetworkProtectionJoinEventsMenuVars::m_Vars.m_PendingTimeout = 0;
							} else {
								NetworkProtectionJoinEventsMenuVars::m_Vars.m_GsInfo = GsInfo;
							}

							Return = false;
						}

						auto Vit = std::find_if(Global::Vars::g_OnlineRockstarIdQueue.begin(), Global::Vars::g_OnlineRockstarIdQueue.end(), [=] (uint64_t value) {
							return value == RockstarID;
						});;

						if (Vit != Global::Vars::g_OnlineRockstarIdQueue.end()) {
							Return = false;

							Global::Vars::g_OnlineRockstarIds[RockstarID] = !Json["gsinfo"].get<std::string>().empty();
							Global::Vars::g_OnlineRockstarIdQueue.erase(Vit);
						}
					} catch (...) {
						return false;
					}
				}
			}

			i++;
		} while (i < *(uint32_t*)(Rcx + 0x2944));
	}

	if (!Return) return false;

	return OriginalReadSessionInfoResponse(Rcx);
}