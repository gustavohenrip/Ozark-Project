#include "hooks.hpp"
#include "menu/base/submenus/main/network/spoofing/crew/network_spoofing_crew_name.hpp"
#include "menu/base/submenus/main/network/spoofing/crew/network_spoofing_crew_tag.hpp"
#include "menu/base/submenus/main/network/spoofing/crew/network_spoofing_crew_motto.hpp"
#include "menu/base/submenus/main/network/spoofing/crew/network_spoofing_crew_rank.hpp"
#include "menu/base/submenus/main/network/spoofing/crew/network_spoofing_crew_icon.hpp"
#include "menu/base/util/player_manager.hpp"

void Menu::Hooks::PlayerGamerDataNodeHook(Rage::Network::PlayerGamerDataNode* Data, Rage::BitBufferSyncInstance* Buffer) {
	if (Buffer->m_Type == 2) {
		Rage::Network::SyncTree* PlayerSyncTree = *(Rage::Network::SyncTree**)((uint64_t)Data + 0x18);
		if (PlayerSyncTree) {
			Rage::Network::NetObj* Object = PlayerSyncTree->m_Object;
			if (Object) {
				if (Object->m_Owner == Menu::GetLocalPlayer().m_ID) {
					if (!NetworkSpoofingCrewNameMenuVars::m_Vars.m_SpoofedName.empty()) {
						strcpy(Data->m_CrewName, NetworkSpoofingCrewNameMenuVars::m_Vars.m_SpoofedName.c_str());
					}

					if (!NetworkSpoofingCrewTagMenuVars::m_Vars.m_SpoofedName.empty()) {
						strcpy(Data->m_CrewAbbr, NetworkSpoofingCrewTagMenuVars::m_Vars.m_SpoofedName.c_str());
					}

					if (!NetworkSpoofingCrewMottoMenuVars::m_Vars.m_SpoofedName.empty()) {
						strcpy(Data->m_CrewMotto, NetworkSpoofingCrewMottoMenuVars::m_Vars.m_SpoofedName.c_str());
					}

					if (!NetworkSpoofingCrewRankMenuVars::m_Vars.m_SpoofedName.empty()) {
						strcpy(Data->m_CrewRank, NetworkSpoofingCrewRankMenuVars::m_Vars.m_SpoofedName.c_str());
					}

					if (NetworkSpoofingCrewIconMenuVars::m_Vars.m_SpoofedIcon != -1) {
						Data->m_CrewID = NetworkSpoofingCrewIconMenuVars::m_Vars.m_SpoofedIcon;
					}
				}
			}
		}
	}

	OriginalPlayerGamerDataNode(Data, Buffer);
}