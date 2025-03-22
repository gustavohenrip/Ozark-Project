#include "player_manager.hpp"
#include "rage/invoker/natives.hpp"
#include "rage/engine.hpp"
#include "menu/base/submenus/main/network/players.hpp"
#include "menu/base/submenus/main/network/ozark_detections.hpp"
#include "rage/session/session.hpp"
#include "menu/base/submenus/main/network/all/all_players.hpp"
#include "menu/base/submenus/main/settings/video_settings.hpp"
#include "utils/va.hpp"
#include "menu/base/util/timers.hpp"

//#define OZARK_HIDDEN_USER 1
//#define OZARK_USER 2
//#define OZARK_STAFF 4
//#define OZARK_DONT_HURT_ME 8

namespace menu {
	bool isFriend(int player) {
		int handle[13];
		native::networkHandleFromPlayer(player, &handle[0], 13);
		if (native::networkIsHandleValid(&handle[0], 13)) {
			if (native::networkIsFriend(&handle[0])) {
				return true;
			}
		}

		return false;
	}

	bool isOzarkDevUser(int index) {
		int bit = native::decorGetInt(native::getPlayerPed(index), networkOzarkDetectsMenuVars::vars.ozarkDetects.c_str());
		return (bit & OZARK_USER_DEVELOPER);
	}

	bool isOzarkUser(int index) {
		int bit = native::decorGetInt(native::getPlayerPed(index), networkOzarkDetectsMenuVars::vars.ozarkDetects.c_str());

#ifdef  DEV
		return (bit & OZARK_HIDDEN_USER);
#endif

		return (bit & OZARK_USER);
	}

	bool isOzarkHiddenUser(int index) {
		int bit = native::decorGetInt(native::getPlayerPed(index), networkOzarkDetectsMenuVars::vars.ozarkDetects.c_str());
		return (bit & OZARK_HIDDEN_USER);
	}

	bool isOzarkPassive(int index) {
		int bit = native::decorGetInt(native::getPlayerPed(index), networkOzarkDetectsMenuVars::vars.ozarkDetects.c_str());

#ifdef  DEV
		return false;
#endif

		return (bit & OZARK_DONT_HURT_ME);
	}

	bool hasModdedModel(int index) {
		Hash model = native::getEntityModel(index);
		if (model == 0) return false;
		return model != 0xF5C1611E && model != 0xA7AF20C0;
	}

	const char* getName(int index) {
		if (index == menu::getLocalPlayer().id && videoSettingsMenuVars::vars.hideMe) {
			return "~m~Me";
		}

		return native::getPlayerName(index);
	}

	void playerManager::updateNotes(int id) {
		auto& player = getPlayer(id);
		if (player.id != native::playerId()) {
			if (player.connected && native::networkIsInSession()) {
				player.notes.clear();

				auto net = rage::engine::getNetGamePlayerFromIndex(player.id);
				if (net) {
					auto info = net->playerInfo;
					if (info) {
						if (info->netInfo.rockstarId != info->netInfo.rockstarId2) {
							player.notes = std::string(utils::va::va("%sReal RID: %i", player.notes.empty() ? "" : (player.notes + ". ").c_str(), info->netInfo.rockstarId2));
						}

						if (global::vars::g_rockstarAccounts.find(info->netInfo.rockstarId2) != global::vars::g_rockstarAccounts.end()) {
							auto rinfo = global::vars::g_rockstarAccounts[info->netInfo.rockstarId2];
							if (!rinfo.name.empty()) {
								if (strcmp(rinfo.name.c_str(), info->netInfo.name)) {
									player.notes = std::string(utils::va::va("%sReal name: %s", player.notes.empty() ? "" : (player.notes + ". ").c_str(), rinfo.name.c_str()));
								}
							}
						}

						if (global::vars::g_relayUsage.find(info->netInfo.rockstarId2) != global::vars::g_relayUsage.end()) {
							uint32_t ip = *(uint32_t*)&info->netInfo.externalIp;
							if (global::vars::g_relayUsage[info->netInfo.rockstarId2] != ip) {
								ip = global::vars::g_relayUsage[info->netInfo.rockstarId2];
								player.notes = std::string(utils::va::va("%sReal IP: %i.%i.%i.%i", player.notes.empty() ? "" : (player.notes + ". ").c_str(), ((ip & 0xFF000000) >> 24), ((ip & 0xFF0000) >> 16), ((ip & 0xFF00) >> 8), (ip & 0xFF)));
							}
						}
					}
				}

				if (hasModdedModel(player.ped)) {
					player.notes = std::string(utils::va::va("%sModded model", player.notes.empty() ? "" : (player.notes + ". ").c_str()));
				}
			}
		}
	}

	void playerManager::update() {
		for (int i = 0; i < 32; i++) {
			playerVars& player = players[i];
			player.connected = native::networkIsPlayerActive(i) && native::networkIsPlayerConnected(i);

			player.id = i;
			player.ped = native::getPlayerPedScriptIndex(i);
			player.exists = native::doesEntityExist(player.ped);
			if (!player.exists) continue;

			player.name = getName(i);
			player.coords = native::getEntityCoords(player.ped);
			player.inVehicle = native::isPedInAnyVehicle(player.ped, TRUE);
			player.entity = player.inVehicle ? native::getVehiclePedIsIn(player.ped, FALSE) : native::isPedOnMount(player.ped) ? native::getMount(player.ped) : player.ped;
			player.isFriend = isFriend(player.id);
			player.isScriptHost = player.id == native::networkGetHostOfThisScript();
			player.isSessionHost = player.id == rage::network::getNetworkTask()->getSessionHost();
			player.isOzarkUser = isOzarkUser(player.id);
			player.isOzarkUserDev = isOzarkDevUser(player.id);
			player.isOzarkHiddenUser = isOzarkHiddenUser(player.id);
			player.isOzarkPassive = isOzarkPassive(player.id);
			player.isOnMount = native::isPedOnMount(player.ped);
			player.mount = native::getMount(player.ped);
			player.vehicle = native::getVehiclePedIsIn(player.ped, 0);
			player.netGamePlayer = rage::engine::getNetGamePlayerFromIndex(i);


			if (player.id != native::playerId()) {
				if (player.connected && native::networkIsInSession()) {
					auto net = rage::engine::getNetGamePlayerFromIndex(player.id);
					if (net) {
						auto info = net->playerInfo;
						if (info) {
							if (info->netInfo.rockstarId != info->netInfo.rockstarId2) {
								player.modder = true;
							}

							if (global::vars::g_rockstarAccounts.find(info->netInfo.rockstarId2) != global::vars::g_rockstarAccounts.end()) {
								auto rinfo = global::vars::g_rockstarAccounts[info->netInfo.rockstarId2];
								if (!rinfo.name.empty()) {
									if (strcmp(rinfo.name.c_str(), info->netInfo.name)) {
										player.modder = true;
									}
								}
							}
						}
					}

					if (hasModdedModel(player.ped)) {
						player.modder = true;
					}
				}
			}

			native::getCurrentPedWeapon(player.ped, &player.weaponHash, FALSE, FALSE, FALSE);
		}
	}

	void playerManager::foreach(std::function<void(playerVars&)> callback, bool bypass) {
		if (native::networkIsPlayerConnected(native::playerId())) {
			for (int i = 0; i < 32; i++) {
				if (players[i].exists && players[i].connected) {
					if (!bypass) {
						if (networkAllPlayersMenuVars::vars.excludeFriends && isFriend(i)) continue;
						if (networkAllPlayersMenuVars::vars.otherPlayers && !isFriend(i)) continue;
						if (networkAllPlayersMenuVars::vars.excludeOzarkUsers && players[i].isOzarkUser) continue;
						if (players[i].isOzarkPassive) continue;
						if (networkAllPlayersMenuVars::vars.me && players[i].id == getLocalPlayer().id) continue;
					}
					callback(players[i]);
				}
			}
		}
	}

	playerVars& playerManager::getPlayer(int index) {
		return players[index];
	}

	playerVars& playerManager::getLocalPlayer() {
		return players[native::playerId()];
	}

	playerVars& getPlayer(int index) {
		return getPlayerManager()->getPlayer(index);
	}

	playerVars& getLocalPlayer() {
		return getPlayerManager()->getLocalPlayer();
	}

	playerVars& getSelectedPlayer() {
		return getPlayer(networkPlayersMenuVars::vars.selectedPlayer);
	}

	playerManager* getPlayerManager() {
		static playerManager instance;
		return &instance;
	}
}