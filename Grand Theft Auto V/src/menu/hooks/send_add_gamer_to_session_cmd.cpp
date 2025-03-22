#include "hooks.hpp"
#include "menu/base/submenus/main/network/spoofing/network_spoofing_ip.hpp"
#include "menu/base/submenus/main/network/spoofing/network_spoofing_name.hpp"
#include "menu/base/submenus/main/network/network_spoofing.hpp"
#include "menu/base/util/player_manager.hpp"
#include "utils/log.hpp"
#include "utils/utils.hpp"
#include "rage/engine.hpp"
#include "utils/caller.hpp"

bool Menu::Hooks::SendAddGamerToSessionCmdHook(uint64_t Rcx, Rage::Network::NetworkGamerCmd* Cmd) {
	LOG_DEV("== add ==");
	LOG_DEV("RID: %i", Cmd->m_Identifier.m_RockstarID);
	LOG_DEV("Name: %s", Cmd->m_Name);
	LOG_DEV("Token: %llx", Cmd->m_HostToken);

	if (Cmd && Cmd->m_Identifier.m_RockstarID == Global::Vars::g_RsInfo->m_RockstarID) {
		if (!NetworkSpoofingIPMenuVars::m_Vars.m_SpoofingIP.empty()) {
			if (strcmp(NetworkSpoofingIPMenuVars::m_Vars.m_DefaultIP.c_str(), NetworkSpoofingIPMenuVars::m_Vars.m_SpoofingIP.c_str())) {
				LOG_CUSTOM(ConsoleForeground::CYAN, "Spoof", "Spoofing IP from %s to %s", NetworkSpoofingIPMenuVars::m_Vars.m_DefaultIP.c_str(), NetworkSpoofingIPMenuVars::m_Vars.m_SpoofingIP.c_str());

				auto Split = Utils::Split(NetworkSpoofingIPMenuVars::m_Vars.m_SpoofingIP + ".", ".");
				Cmd->SetIP(3, (uint8_t)atoi(Split[0].c_str()));
				Cmd->SetIP(2, (uint8_t)atoi(Split[1].c_str()));
				Cmd->SetIP(1, (uint8_t)atoi(Split[2].c_str()));
				Cmd->SetIP(0, (uint8_t)atoi(Split[3].c_str()));
			}
		}

		if (!NetworkSpoofingNameMenuVars::m_Vars.m_SpoofedName.empty()) {
			const char* OriginalName = Rage::Engine::SocialClubGetGameInfoString("gtag");
			if (OriginalName) {
				if (strcmp(OriginalName, NetworkSpoofingNameMenuVars::m_Vars.m_SpoofedName.c_str())) {
					LOG_CUSTOM(ConsoleForeground::CYAN, "Spoof", "Spoofing name from %s to %s", OriginalName, NetworkSpoofingNameMenuVars::m_Vars.m_SpoofedName.c_str());
					strncpy(Cmd->m_Name, NetworkSpoofingNameMenuVars::m_Vars.m_SpoofedName.c_str(), 20);

					auto Net = Menu::GetLocalPlayer().m_NetGamePlayer;
					if (Net) {
						if (Net->m_PlayerInfo) {
							strncpy(Net->m_PlayerInfo->m_Name, NetworkSpoofingNameMenuVars::m_Vars.m_SpoofedName.c_str(), 20);
						}
					}
				}
			}
		}
	}

	return OriginalSendAddGamerToSessionCmd(Rcx, Cmd);
}