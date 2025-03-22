#include "hooks.hpp"
#include "rage/engine.hpp"
#include "rage/invoker/natives.hpp"
#include "rage/types/network_types.hpp"
#include "menu/base/util/player_manager.hpp"
#include "utils/log.hpp"
#include "menu/base/submenus/main/settings/video_settings.hpp"
#include "utils/va.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/submenus/main/network/overseer.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/util/fiber_manager.hpp"
#include "menu/base/util/fiber_pool.hpp"
#include "menu/base/submenus/main/network/all/all_players.hpp"
#include "menu/base/submenus/main/network/recent_players.hpp"
#include "menu/base/submenus/main/network/protection/protection.hpp"

uint64_t menu::hooks::processJoinLeaveHook(uint8_t* cl, rage::network::netGamePlayer* netGamePlayer, uint32_t r8d, uint8_t r9b) {
	if (videoSettingsMenuVars::vars.hideNames) {
		videoSettingsMenuVars::vars.cachedPlayerInfo[netGamePlayer->playerInfo] = netGamePlayer->playerInfo->netInfo.name;
		strcpy(netGamePlayer->playerInfo->netInfo.name, utils::va::va("Player %i", r8d));
	}

	if (netGamePlayer != rage::engine::getNetworkPlayerManager()->localNetGamePlayer) {
		if (menu::getLocalPlayer().connected) {
			if (networkAllPlayersMenuVars::vars.showJoinLeave) {
				menu::getNotify()->bottom(std::string(utils::va::va("~pa~%s~q~ %s %i", netGamePlayer->playerInfo->netInfo.name, r8d == 0xFF ? "just left slot" : "is joining into slot", r8d == 0xFF ? netGamePlayer->playerID : r8d)));
			} else {
				customInfo("Event", "%s %s %i", netGamePlayer->playerInfo->netInfo.name, r8d == 0xFF ? "just left slot" : "is joining into slot", r8d == 0xFF ? netGamePlayer->playerID : r8d);
			}

			if (r8d != 0xFF) {
				if (netGamePlayer->playerInfo->netInfo.getIP()) {
					global::vars::g_geoQueue.push_back(netGamePlayer->playerInfo->netInfo.getIP());
				}

				if (netGamePlayer->playerInfo->netInfo.rockstarId2) {
					global::vars::g_rockstarAccountsQueue.push_back(netGamePlayer->playerInfo->netInfo.rockstarId2);
				}

				networkRecentPlayersMenuVars::save(netGamePlayer->playerInfo->netInfo.name, utils::va::va("%i.%i.%i.%i", netGamePlayer->playerInfo->netInfo.externalIp[3], netGamePlayer->playerInfo->netInfo.externalIp[2], netGamePlayer->playerInfo->netInfo.externalIp[1], netGamePlayer->playerInfo->netInfo.externalIp[0]), netGamePlayer->playerInfo->netInfo.rockstarId2);
			}

			menu::getPlayer(r8d == 0xFF ? netGamePlayer->playerID : r8d) = menu::playerVars();

			if (networkOverseerMenuVars::vars.enableOverseer && r8d != 0xFF) {
				auto vit = std::find_if(networkOverseerMenuVars::vars.players.begin(), networkOverseerMenuVars::vars.players.end(), [=] (const std::pair<std::string, networkOverseerMenuVars::overseer>& elem) -> bool { return elem.first == std::to_string(netGamePlayer->playerInfo->netInfo.rockstarId2); });
				if (vit != networkOverseerMenuVars::vars.players.end()) {
					int flag = vit->second.flags;
					if (flag & OVERSEER_ENABLED) {
						if (flag & OVERSEER_NOTIFY) {
							menu::getNotify()->right(utils::va::va("[Overseer] ~e~%s~q~ is joining", netGamePlayer->playerInfo->netInfo.name));
						}
					}
				}
			}
		}
	}

	return originalProcessJoinLeave(cl, netGamePlayer, r8d, r9b);
}