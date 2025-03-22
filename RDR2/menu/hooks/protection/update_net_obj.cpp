#include "../hooks.hpp"
#include "rage/invoker/natives.hpp"
#include "rage/engine.hpp"
#include "rage/entity/entity.hpp"
#include "menu/base/submenu_handler.hpp"
#include "menu/base/submenus/main/network/protection/protection.hpp"
#include "utils/thread_pool.hpp"
#include "src/menu/base/util/fiber_pool.hpp"
#include "src/menu/base/util/fiber_manager.hpp"
#include "menu/base/util/control_manager.hpp"
#include "utils/memory/pattern.hpp"

void menu::hooks::updateNetObjHook(uint64_t rcx, rage::network::netGamePlayer* netGamePlayer, rage::network::netObject* netObj, uint64_t r9, uint32_t* r10, uint16_t* r11, uint64_t r12, uint8_t r13) {

	if ((rage::network::netObject*)rage::entity::getEntityTask()->getNetObj(global::vars::g_syncCrashEntity) == netObj) {
		if (netGamePlayer) {
			auto info = netGamePlayer->getNetPlayerInfo();
			if (info) {
				if (std::string(info->name) == global::vars::g_syncCrashPlayerName) {
					if (netObj) {
						originalUpdateNetObj(rcx, netGamePlayer, netObj, r9, r10, r11, r12, r13);
					}
				}
				else {
					return;
				}
			}
		}
	}

	static int counter[32];
	if (netObj == (rage::network::netObject*)rage::entity::getEntityTask()->getNetObj(menu::getLocalPlayer().entity)) {
		if (global::vars::g_spoofLocationPed) {
			if (native::doesEntityExist(global::vars::g_spoofLocationPed)) {
				originalUpdateNetObj(rcx, netGamePlayer, (rage::network::netObject*)rage::entity::getEntityTask()->getNetObj(global::vars::g_spoofLocationPed), r9, r10, r11, r12, r13);
				return;
			} else {
				global::vars::g_spoofLocationPed = 0;
			}
		}

		if (networkProtectionMenuVars::vars.fakeLag && netGamePlayer) {
			counter[netGamePlayer->playerID]++;
			if (counter[netGamePlayer->playerID] >= networkProtectionMenuVars::vars.choke) {
				counter[netGamePlayer->playerID] = 0;
			}

			if (counter[netGamePlayer->playerID] != 0) {
				return;
			}
		}
	}

	if (netObj) {
		originalUpdateNetObj(rcx, netGamePlayer, netObj, r9, r10, r11, r12, r13);
	}
}


uint64_t menu::hooks::syncUserMadeObjectsHook(uint64_t tree, uint64_t netObj, rage::network::netGamePlayer* netGamePlayer, uint64_t r9) {

	//if (rage::entity::getEntityTask()->getNetObj(global::vars::g_syncCrashEntity) == netObj) {
	//	if (global::vars::g_syncCrashPlayerName == std::string(netGamePlayer->playerInfo->netInfo.name)) {
	//		printf("sending obj to %s [1]", global::vars::g_syncCrashPlayerName.c_str());
	//		if (menu::getPlayer(netGamePlayer->playerID).connected) {
	//			auto modelInfo = rage::engine::getModelInfo(native::getHashKey("bountywagon01x"));
	//			//if (modelInfo) {
	//			//	*(int32_t*)(modelInfo + 0x3E0) = 1;
	//			//}
	//			auto ret = originalSyncUserMadeObjects(tree, rage::entity::getEntityTask()->getNetObj(global::vars::g_syncCrashEntity), netGamePlayer, r9);
	//			menu::util::getFiberPool()->run([=] {
	//				menu::util::waitFiber(1000);
	//				*(int32_t*)(modelInfo + 0x3E0) = 5;
	//				});
	//			return ret;
	//		}
	//	} else {
	//		return 0;
	//	}
	//}
	//if (netGamePlayer) {
	//	if (global::vars::g_netObjMgr) {
	//		if (netGamePlayer->playerID == menu::getSelectedPlayer().id) {
	//			if (*(uint64_t*)global::vars::g_netObjMgr == tree) {
	//				printf("syncing to %s\n", netGamePlayer->getNetPlayerInfo()->name);
	//				auto ret = originalSyncUserMadeObjects(tree, netObj, netGamePlayer, r9);
	//				printf("return: %llX\n", ret);
	//				return ret;
	//			} else {
	//				printf("LOL THIS IS NOT NETMGR\n");
	//			}
	//		}
	//	}
	//}
	
	return originalSyncUserMadeObjects(tree, netObj, netGamePlayer, r9);
}
 