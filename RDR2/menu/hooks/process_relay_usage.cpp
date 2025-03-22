#include "hooks.hpp"
#include "utils/thread_pool.hpp"
#include "utils/caller.hpp"
#include "src/menu/base/util/fiber_manager.hpp"
#include "src/menu/base/util/fiber_pool.hpp"
#include "menu/base/util/player_manager.hpp"

//bool isRidInSession(uint64_t rid, const char* name) {
//	bool result = false;
//	menu::getPlayerManager()->foreach([&](menu::playerVars& player) {
//		if (player.netGamePlayer) {
//			auto info = player.netGamePlayer->playerInfo;
//			if (info) {
//				if ((info->netInfo.rockstarId == rid || info->netInfo.rockstarId2 == rid) && std::string(name) == std::string(player.name)) {
//					result = true;
//				}
//			}
//		}
//		}, true);
//	return result;
//}

bool menu::hooks::processRelayUsageHook(uint64_t rcx, rage::network::relayUser* relay, rage::network::relayNetwork* network) {
	auto result = originalProcessRelayUsage(rcx, relay, network);
	//menu::util::getFiberPool()->run([=] {
	//	if (relay && network) {
	//		uint64_t timeout = GetTickCount64() + 30000;
	//		while (!isRidInSession(relay->rid, relay->name) && timeout > GetTickCount64()) {
	//			menu::util::waitFiber(500);
	//		}
	//		global::vars::g_relayUsage[relay->rid] = network->ip;
	//	}
	//});
	return result;
}