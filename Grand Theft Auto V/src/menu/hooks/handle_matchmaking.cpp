#include "hooks.hpp"
#include "utils/caller.hpp"
#include "utils/json.hpp"
#include "rage/engine.hpp"
#include "menu/base/submenus/main/network/browser/network_browser.hpp"
#include "menu/base/submenu_handler.hpp"
#include "utils/log.hpp"

bool Menu::Hooks::ReadMatchmakingResponseHook(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9) {
	if (Rcx && *(uint32_t*)(Rcx + 0xE00) > 15) {
		uint64_t XmlContext = Rage::Engine::ReadXmlString(R8, "Results", 0, 1);
		if (XmlContext) {
			Rage::Types::XmlNode* Node = *(Rage::Types::XmlNode**)(XmlContext + 0x30);

			while (Node) {
				if (Node->m_Name) {
					if (!_stricmp(Node->m_Name, "R")) {
						const char* AttributeString = Rage::Engine::ReadXmlAttributeString(Node, "Data", 0);
						if (AttributeString) {
							if (*AttributeString) {
								Rage::Network::GsSession Session;
								if (Rage::Engine::GetSessionInfoFromGs(&Session, AttributeString, 0, 0)) {
									if (std::find_if(Global::Vars::g_Sessions.begin(), Global::Vars::g_Sessions.end(), [=] (SessionBrowser& Element) { return Element.m_RockstarID == Session.m_RockstarID; }) == Global::Vars::g_Sessions.end()) {
										Global::Vars::g_Sessions.push_back({ Session.m_RockstarID, Session.m_PeerToken, "" });
									}
								}
							}
						}
					}
				}

				Node = Node->m_Next;
			}
		}

		return true;
	}

	return OriginalReadMatchmakingResponse(Rcx, Rdx, R8, R9);
}

bool Menu::Hooks::WriteMatchmakingRequestHook(uint64_t Rcx, uint32_t RsIndex, uint32_t AvailableSlots, uint64_t FilterInfo, uint32_t MaxResults, uint64_t Stack_20, uint64_t Stack_18) {
	if (NetworkBrowserMenuVars::m_Vars.m_Populate) {
		if (Menu::GetSubmenuHandler()->GetCurrentSubmenu() == NetworkBrowserMenu::GetInstance()) {
			return OriginalWriteMatchmakingRequest(Rcx, RsIndex, 1, FilterInfo, 1600, Stack_20, Stack_18);
		}
	}
	
	return OriginalWriteMatchmakingRequest(Rcx, RsIndex, AvailableSlots, FilterInfo, MaxResults, Stack_20, Stack_18);
}

bool Menu::Hooks::WriteEncryptedStringToRequestHook(uint64_t Rcx, const char* Name, char* Value, bool Encrypted, uint64_t Stack_3C8, uint64_t Stack_3C0) {
	if (Name && NetworkBrowserMenuVars::m_Vars.m_Populate) {
		if (Menu::GetSubmenuHandler()->GetCurrentSubmenu() == NetworkBrowserMenu::GetInstance()) {
			if (!strcmp(Name, "filterName")) {
				Value = "Group";
			}

			if (!strcmp(Name, "filterParamsJson")) {
				nlohmann::json Json;
				Json["GAME_MODE"] = 0;
				Json["MMATTR_DISCRIMINATOR"] = nlohmann::json::parse(Value)["MMATTR_DISCRIMINATOR"];
				Json["MMATTR_MM_GROUP_2"] = 30;
				Json["MMATTR_REGION"] = rand() % 8;

				strcpy(Value, Json.dump().c_str());
			}
		}
	}

	return OriginalWriteEncryptedStringToRequest(Rcx, Name, Value, Encrypted, Stack_3C8, Stack_3C0);
}