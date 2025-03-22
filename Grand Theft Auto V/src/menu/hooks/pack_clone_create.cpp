#include "hooks.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_ghost_mode.hpp"
#include "menu/base/submenus/main/network/network_protection.hpp"
#include "menu/base/util/player_manager.hpp"
#include "menu/base/submenus/main/network/players/network_players_removal.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/helpers.hpp"
#include "utils/log.hpp"

void Menu::Hooks::PackCloneCreateHook(uint64_t This, Rage::Network::NetObj* Object, Rage::Network::NetGamePlayer* Player, uint64_t SyncData) {
	if (Object) {
		if (Object->m_Type >= NetObjectAutomobile && Object->m_Type < INVALID) {
			if (NetworkProtectionGhostModeMenuVars::m_Vars.m_Ghosts[Object->m_Type].m_Enabled) {
				if (Object->m_Type == NetObjectPlayer) return;
				if (NetworkProtectionGhostModeMenuVars::m_Vars.m_Ghosts[Object->m_Type].m_DisableForSession) return;
			}

			if (Player->m_ID >= 0 && Player->m_ID < 34) {
				if (Menu::GetPlayer(Player->m_ID).m_GhostDisable[Object->m_Type]) {
					return;
				}

				if (Object->m_Type == NetObjectPickup) {
					std::vector<int> DroppingStealth;
					Menu::GetPlayerManager()->Foreach([&] (Menu::PlayerVars& Player) {
						if (Player.m_DropToggle && Player.m_DropStealth) {
							DroppingStealth.push_back(Player.m_ID);
						}
					}, true);

					bool Bad = false;
					if (DroppingStealth.size()) {
						for (int ID : DroppingStealth) {
							if (ID != Player->m_ID) {
								Bad = true;
								break;
							}
						}
					}

					if (Bad) return;
				}

				auto& Vars = NetworkPlayersRemovalMenuVars::m_Vars;

				if (Vars.m_TargetedCrash) {
					if (Vars.m_BrokenEntities.find(Object) != Vars.m_BrokenEntities.end()) {
						auto P = Menu::Helpers::GetPlayerFromRID(Vars.m_BrokenEntities[Object]);
						if (P.m_Connected && P.m_NetGamePlayer) {
							LOG_DEV("Packing clone create crash");
							return OriginalPackCloneCreate(This, Object, P.m_NetGamePlayer, SyncData);
						}

						return;
					}
				}
			}
		}
	}

	OriginalPackCloneCreate(This, Object, Player, SyncData);
}