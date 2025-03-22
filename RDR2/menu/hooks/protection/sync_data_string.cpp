#include "../hooks.hpp"
#include "menu/base/submenus/main/network/account/name_changer.hpp"
#include "menu/base/submenus/main/network/account/rid_spoofing.hpp"
#include "menu/base/submenus/main/network/account/ip_spoofing.hpp"
#include "rage/engine.hpp"
#include "utils/utils.hpp"
#include "utils/log.hpp"
#include "menu/base/util/player_manager.hpp"
#include "global/auth_vars.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/submenus/main/network/overseer.hpp"
#include "utils/va.hpp"

bool menu::hooks::syncDataStringHook(uint64_t stream, const char* data, int length) {
	/*if (!networkAccountNameChangerMenuVars::vars.spoofedName.empty()) {
		if (data) {
			auto baseName = rage::engine::getGameInfoString("gtag");
			if (baseName) {
				if (!strcmp(baseName, data)) {
					custom(consoleForeground::CYAN, "Spoof", "Spoofing name from %s to %s", baseName, networkAccountNameChangerMenuVars::vars.spoofedName.c_str());

					auto local = rage::engine::getNetworkPlayerManager()->localNetGamePlayer;
					if (local) {
						auto info = local->playerInfo;
						if (info) {
							strncpy(info->netInfo.name, networkAccountNameChangerMenuVars::vars.spoofedName.c_str(), 32);
						}
					}

					return originalSyncDataString(stream, networkAccountNameChangerMenuVars::vars.spoofedName.c_str(), length);
				}
			}
		}
	}*/

	return originalSyncDataString(stream, data, length);
}

bool isRidInSession(uint64_t rid, const char* name) {
	bool result = false;
	menu::getPlayerManager()->foreach([&](menu::playerVars& player) {
		if (player.netGamePlayer) {
			auto info = player.netGamePlayer->playerInfo;
			if (info) {
				if ((info->netInfo.rockstarId == rid || info->netInfo.rockstarId2 == rid) && std::string(name) == std::string(player.name)) {
					result = true;
				}
			}
		}
		}, true);
	return result;
}

bool menu::hooks::syncPlayerInfoMsgHook(uint64_t rcx, rage::network::netMsgPlayer* rdx) {
	static std::vector<uint64_t> blacklistedRid = { 83765581, 19541822, 182093032/*, 148254157*//*kekmehz*/, 176014334 };
	bool block = false;
	if (rdx) {
		auto info = rdx->playerInfoWrapper;
		if (info) {
			auto ip = info->ip;
			auto moreInfo = info->playerInfo;
			if (moreInfo) {
				auto rid = moreInfo->getRid();
				global::vars::g_relayUsage[rid] = ip; // grab real ip
				std::for_each(blacklistedRid.begin(), blacklistedRid.end(), [&](uint64_t& data) { // static blacklists
					if (data == rid) {
						block = true;
					}
				});
				if (networkOverseerMenuVars::vars.blacklist.size()) {
					std::for_each(networkOverseerMenuVars::vars.blacklist.begin(), networkOverseerMenuVars::vars.blacklist.end(), [&](std::pair<std::string, std::string>& data) {
						if (std::to_string(rid) == data.first) {
							char buffer[16];
							sprintf(buffer, "%i.%i.%i.%i", ((ip & 0xFF000000) >> 24), ((ip & 0xFF0000) >> 16), ((ip & 0xFF00) >> 8), (ip & 0xFF));
							menu::getNotify()->right(std::string(utils::va::va("~e~Blacklist~q~ [~pa~%s~q~] [%s]"_Protect, data.second, buffer)));
							block = true;
						}
					});
				}
			}
		}
	}
	if (block) {
		return false;
	}
	return originalSyncPlayerInfoMsg(rcx, rdx);
}

bool menu::hooks::syncPlayerInfoHook(rage::network::netPlayerInfo* rcx, uint64_t rdx, uint32_t r8d, uint64_t r9) {
	if (rcx) {
		if (!networkAccountNameChangerMenuVars::vars.spoofedName.empty()) {
			if (strcmp(rage::engine::getGameInfoString("gtag"_Protect), networkAccountNameChangerMenuVars::vars.spoofedName.c_str())) {
				custom(consoleForeground::CYAN, "Spoof"_Protect, "Spoofing name from %s to %s"_Protect, rage::engine::getGameInfoString("gtag"_Protect), networkAccountNameChangerMenuVars::vars.spoofedName.c_str());
				strncpy(rcx->name, networkAccountNameChangerMenuVars::vars.spoofedName.c_str(), 32);

				auto net = rage::engine::getNetworkPlayerManager()->localNetGamePlayer;
				if (net) {
					if (net->playerInfo) {
						strncpy(net->playerInfo->netInfo.name, networkAccountNameChangerMenuVars::vars.spoofedName.c_str(), 32);
					}
				}
			}
		}

		if (networkRockstarIDSpoofingMenuVars::vars.spoofingRockstarID) {
			if (global::auth::g_rockstarId != networkRockstarIDSpoofingMenuVars::vars.spoofingRockstarID) {
				custom(consoleForeground::CYAN, "Spoof"_Protect, "Spoofing rockstar ID from %I64d to %I64d"_Protect, global::auth::g_rockstarId, networkRockstarIDSpoofingMenuVars::vars.spoofingRockstarID);
				rcx->rockstarId = networkRockstarIDSpoofingMenuVars::vars.spoofingRockstarID;

				auto net = rage::engine::getNetworkPlayerManager()->localNetGamePlayer;
				if (net) {
					if (net->playerInfo) {
						net->playerInfo->netInfo.rockstarId = networkRockstarIDSpoofingMenuVars::vars.spoofingRockstarID;
					}
				}
			}
		}

		if (!networkIPSpoofingMenuVars::vars.spoofingIP.empty()) {
			if (strcmp(networkIPSpoofingMenuVars::vars.defaultIP.c_str(), networkIPSpoofingMenuVars::vars.spoofingIP.c_str())) {
				custom(consoleForeground::CYAN, "Spoof"_Protect, "Spoofing IP from %s to %s"_Protect, networkIPSpoofingMenuVars::vars.defaultIP.c_str(), networkIPSpoofingMenuVars::vars.spoofingIP.c_str());

				auto split = utils::split(networkIPSpoofingMenuVars::vars.spoofingIP + ".", ".");
				rcx->externalIp[3] = (BYTE)atoi(split[0].c_str());
				rcx->externalIp[2] = (BYTE)atoi(split[1].c_str());
				rcx->externalIp[1] = (BYTE)atoi(split[2].c_str());
				rcx->externalIp[0] = (BYTE)atoi(split[3].c_str());

				auto net = rage::engine::getNetworkPlayerManager()->localNetGamePlayer;
				if (net) {
					if (net->playerInfo) {
						net->playerInfo->netInfo.externalIp[3] = (BYTE)atoi(split[0].c_str());
						net->playerInfo->netInfo.externalIp[2] = (BYTE)atoi(split[1].c_str());
						net->playerInfo->netInfo.externalIp[1] = (BYTE)atoi(split[2].c_str());
						net->playerInfo->netInfo.externalIp[0] = (BYTE)atoi(split[3].c_str());
					}
				}
			}
		}
	}

	return originalSyncPlayerInfo(rcx, rdx, r8d, r9);
}