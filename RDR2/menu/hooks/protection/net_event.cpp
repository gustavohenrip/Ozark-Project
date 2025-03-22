#include "../hooks.hpp"
#include "menu/base/submenus/main/network/protection/protection.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/util/player_manager.hpp"
#include "menu/base/submenus/main/network/protection/event_reactions.hpp"
#include "menu/base/submenus/main/network/nasty/crash.hpp"
#include "menu/base/submenus/main/network/nasty/nasty.hpp"
#include "menu/base/submenus/main/network/overseer.hpp"
#include "utils/va.hpp"
#include "utils/log.hpp"
#include "utils/memory/pattern.hpp"
#include "rage/engine.hpp"

bool shouldRunSmartProtectionBlock(rage::network::netGamePlayer* sender) {
	//if (sender) {
	//	if (networkProtectionMenuVars::vars.smartProtection) {
	//		menu::playerVars player = menu::getPlayer(sender->playerID);
	//		int includes = networkProtectionMenuVars::vars.smartProtectionIncludes;

	//		if (((player.modder || player.isOzarkUser) && includes == 2)
	//			|| (includes == 1 && player.isOzarkUser)
	//			|| (player.modder && !player.isOzarkUser && includes == 0)) {
	//			return true;
	//		}
	//	}
	//}

	return false;
}

bool menu::hooks::requestControlEventHook(uint64_t netEvent, rage::network::netGamePlayer* sender, uint64_t r8) {
	if (networkProtectionMenuVars::vars.giveControl || shouldRunSmartProtectionBlock(sender)) {
		return true;
	}

	return originalRequestControlEvent(netEvent, sender, r8);
}

bool menu::hooks::gameClockEventHook(uint64_t netEvent, rage::network::netGamePlayer* sender, uint64_t r8) {
	if (networkProtectionMenuVars::vars.clock || shouldRunSmartProtectionBlock(sender)) {
		return true;
	}

	return originalGameClockEvent(netEvent, sender, r8);
}

bool menu::hooks::gameWeatherEventHook(uint64_t netEvent, rage::network::netGamePlayer* sender, uint64_t r8) {
	if (networkProtectionMenuVars::vars.weather || shouldRunSmartProtectionBlock(sender)) {
		return true;
	}

	return originalGameWeatherEvent(netEvent, sender, r8);
}

bool menu::hooks::fireEventHook(uint64_t netEvent, rage::network::netGamePlayer* sender, uint64_t r8) {
	if (networkProtectionMenuVars::vars.fire || shouldRunSmartProtectionBlock(sender)) {
		return true;
	}

	return originalFireEvent(netEvent, sender, r8);
}

bool menu::hooks::explosionEventHook(uint64_t netEvent, rage::network::netGamePlayer* sender, uint64_t r8) {
	if (networkProtectionMenuVars::vars.explosion || shouldRunSmartProtectionBlock(sender)) {
		return true;
	}

	return originalExplosionEvent(netEvent, sender, r8);
}

bool menu::hooks::networkClearPedTasksEventHook(uint64_t netEvent, rage::network::netGamePlayer* sender, uint64_t r8) {
	if (sender) {
		if (networkProtectionMenuVars::vars.paralyse || shouldRunSmartProtectionBlock(sender)) {
			if (networkProtectionMenuVars::vars.notifyAttacks) {
				getNotify()->right(utils::va::va("~e~%s~q~ tried to paralyse you", sender->playerInfo->netInfo.name));
			}

			return true;
		}
	}

	return originalNetworkClearPedTasksEvent(netEvent, sender, r8);
}

bool menu::hooks::networkIncrementStatEventHook(uint64_t netEvent, rage::network::netGamePlayer* sender, uint64_t r8) {
	if (sender) {
		if (networkEventReactionsMenuVars::vars.onReport.addToOverseer) {
			if (networkOverseerMenuVars::save(sender->playerInfo->netInfo.name, sender->playerInfo->netInfo.rockstarId2, "Reported you")) {
				menu::getNotify()->right(utils::va::va("Adding ~e~%s~q~ to the overseer for reporting you", sender->playerInfo->netInfo.name));
			}
		}

		if (networkEventReactionsMenuVars::vars.onReport.crash) {
			menu::getNotify()->right(utils::va::va("Crashing ~e~%s~q~ for reporting you", sender->playerInfo->netInfo.name));
			networkCrashMenuVars::v3(sender->playerID);
		}

		if (networkEventReactionsMenuVars::vars.onReport.explode) {
			menu::getNotify()->right(utils::va::va("Exploding ~e~%s~q~ for reporting you", sender->playerInfo->netInfo.name));
			networkNastyMenuVars::explode(menu::getPlayer(sender->playerID));
		}

		if (networkProtectionMenuVars::vars.stats || shouldRunSmartProtectionBlock(sender)) {
			getNotify()->right(utils::va::va("~e~%s~q~ tried to report you", sender->playerInfo->netInfo.name));
			return true;
		}

		return true;
	} else
		return true;
}

bool menu::hooks::networkPTFXEventHook(uint64_t netEvent, rage::network::netGamePlayer* sender, uint64_t r8) {
	if (networkProtectionMenuVars::vars.particles || shouldRunSmartProtectionBlock(sender)) {
		return true;
	}

	return originalNetworkPTFXEvent(netEvent, sender, r8);
}

bool menu::hooks::reportCashSpawnEventHook(uint64_t netEvent, rage::network::netGamePlayer* sender, uint64_t r8) {
	if (networkEventReactionsMenuVars::vars.onCashSpawn.addToOverseer) {
		if (networkOverseerMenuVars::save(sender->playerInfo->netInfo.name, sender->playerInfo->netInfo.rockstarId2, "Spawned cash")) {
			menu::getNotify()->right(utils::va::va("Adding ~e~%s~q~ to the overseer for spawning cash", sender->playerInfo->netInfo.name));
		}
	}

	if (networkEventReactionsMenuVars::vars.onCashSpawn.crash) {
		menu::getNotify()->right(utils::va::va("Crashing ~e~%s~q~ for spawning cash", sender->playerInfo->netInfo.name));
		networkCrashMenuVars::v3(sender->playerID);
	}

	if (networkEventReactionsMenuVars::vars.onCashSpawn.explode) {
		menu::getNotify()->right(utils::va::va("Exploding ~e~%s~q~ for spawning cash", sender->playerInfo->netInfo.name));
		networkNastyMenuVars::explode(menu::getPlayer(sender->playerID));
	}

	menu::getPlayer(sender->playerID).modder = true;
	return originalReportCashSpawnEvent(netEvent, sender, r8);
}

bool menu::hooks::givePickupRewardsEventHook(uint64_t netEvent, rage::network::netGamePlayer* sender, uint64_t r8) {
	return true;
}

bool menu::hooks::targetPickuprewardsHook(uint64_t rcx, rage::network::netGamePlayer* reciever) {
	auto ret = originalTargetPickupRewardsEvent(rcx, reciever);
	if (ret) {
		printf("[TPRE] name: %s | index: %i | bitset: %X\n", reciever->playerInfo->netInfo.name, reciever->playerID, *(uint32_t*)(rcx + 0x38));
	}
	return ret;
}

uint64_t menu::hooks::sendWeatherEventHook(uint64_t rcx, uint64_t rdx) {
	//0x40
	//0x70
	//static uint64_t writeData = 0;
	//static uint64_t writeData2 = 0;
	//// 48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 33 FF 41 8B F0 F6 41 28 01 48 8B D9 75 30 8B 49 (write data)
	//if (!writeData || writeData2) {
	//	memory::getPattern()->scanProcess({ "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 33 FF 41 8B F0 F6 41 28 01 48 8B D9 75 30 8B 49", 0, "", true }, [&](uint64_t address) {
	//		writeData = address;
	//	});

	//	memory::getPattern()->scanProcess({ "40 53 48 83 EC 20 8A 41 28 33 DB 44 8A DA 4C 8B D1 A8 01 75 54 8B 49 10 41 3B 4A 0C 7D 4B A8 02 75 36 41", 0, "", true }, [&](uint64_t address) {
	//		writeData2 = address;
	//	});
	//}//40 53 48 83 EC 20 8A 41 28 33 DB 44 8A DA 4C 8B D1 A8 01 75 54 8B 49 10 41 3B 4A 0C 7D 4B A8 02 75 36 41

	//while (!writeData || !writeData2) {
	//	Sleep(0);
	//}

	//for (std::size_t i = 0; i < 0x20; i++) {
	//	printf("rdx[%zi] = %llX\n",i, *(uint64_t*)(rdx + (i * 8)));
	//}

	//printf("rcx data: %llx | rcx data2: %llx | data1: %llX | data2: %llX\n", *(uint64_t*)rdx, *(uint64_t*)(rcx + 0x40), *(uint64_t*)(rcx + 0x44), *(uint64_t*)(rcx + 0x48));
	//printf("sending event\n");
	//uint64_t ret = 0;
	//for (std::size_t i = 0; i < 1; i++) {
	//	caller::call<uint64_t>(writeData2, *(uint64_t*)(rdx + 0x18), *(uint64_t*)(rcx + 0x40), 0, 0);
	//	ret = caller::call<uint64_t>(writeData, *(uint64_t*)(rdx + 0x18), 0x11, 6);
	//	ret = caller::call<uint64_t>(writeData, *(uint64_t*)(rdx + 0x18), 0xFF, 9);
	//}
	//printf("done\n");
	//return ret;
//	printf("rdx[0x40] = %llX | rdx[0x70] = %llX\n", *(uint64_t*)rdx + 0x40, *(uint64_t*)rdx + 0x70);
	return originalSendWeatherEvent(rcx, rdx);
}

uint8_t menu::hooks::checkPlayerDataHook(uint64_t rcx, rage::network::netGamePlayer* rdx) {
	//auto ret = originalCheckPlayerData(rcx, rdx);
	if (rdx->playerID == menu::getSelectedPlayer().id) {
		printf("sending event to %s\n", rdx->getNetPlayerInfo()->name);
	}
	return rdx->playerID == menu::getSelectedPlayer().id;
}
//48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC 30 48 8B 02 48 8B DA 48 8B F1 C7 44 24 ? ? ? ? ? 48
uint8_t menu::hooks::writeRespawnPlayerPedEvent(uint64_t rcx, uint8_t* rdx) {
	printf("step -0 uwu\n");
	uint64_t data[20] = { INT64_MAX, INT64_MAX, INT64_MAX,INT64_MAX,INT64_MAX,INT64_MAX,INT64_MAX,INT64_MAX,INT64_MAX,INT64_MAX };
	printf("step 0\n");
	printf("addr = %llX\n", *(uint64_t*)rdx + 0xE8);
	//caller::call<int32_t>(*(uint64_t*)rdx + 0xE8, rdx, rcx + 0x40, 0, 0, 0x13);
	for (auto i = 0; i < 0x20; i++) {
		(*(void(__fastcall**)(uint8_t*, __int64, uint64_t, uint64_t, signed int))(*(uint64_t*)rdx + 0xE8))(rdx, (int64_t)data, 0, 0, 0x13);
	}
	printf("step 1\n");
	uint32_t data1[20] = { UINT32_MAX, UINT32_MAX ,UINT32_MAX ,UINT32_MAX ,UINT32_MAX ,UINT32_MAX ,UINT32_MAX ,UINT32_MAX ,UINT32_MAX ,UINT32_MAX };
	//caller::call<int32_t>(*(uint64_t*)rdx + 0x70, rdx, data1 ,0x20,0,0);
	(*(void(__fastcall**)(uint8_t*, __int64, signed __int64, uint64_t, uint64_t))(*(uint64_t*)rdx + 0x70))(rdx, (int64_t)data1, 0x20, 0, 0);

	printf("step 2\n");
	uint16_t data2[20] = { UINT16_MAX, UINT16_MAX ,UINT16_MAX ,UINT16_MAX ,UINT16_MAX ,UINT16_MAX ,UINT16_MAX ,UINT16_MAX ,UINT16_MAX ,UINT16_MAX };
	for (auto i = 0; i < 3; i++) {
		(*(void(__fastcall**)(uint8_t*, __int64, uint64_t, uint64_t))(*(uint64_t*)rdx + 0xE0))(rdx, (int64_t)data2, 0, 0);// the uint16
	}
	for (auto i = 0; i < 45; i++) {
		(*(void(__fastcall**)(uint8_t*, __int64, uint64_t, uint64_t))(*(uint64_t*)rdx + 0xB8))(rdx, rcx + 0x5C, 0, 0);
		(*(void(__fastcall**)(uint8_t*, __int64, uint64_t, uint64_t))(*(uint64_t*)rdx + 0x40))(rdx, rcx + 0x60, 0, 0);
		(*(void(__fastcall**)(uint8_t*, __int64, uint64_t, uint64_t))(*(uint64_t*)rdx + 0x40))(rdx, rcx + 0x61, 0, 0);
		(*(void(__fastcall**)(uint8_t*, __int64, uint64_t, uint64_t))(*(uint64_t*)rdx + 0x40))(rdx, rcx + 0x63, 0, 0);
		(*(void(__fastcall**)(uint8_t*, __int64, uint64_t, uint64_t))(*(uint64_t*)rdx + 0x40))(rdx, rcx + 0x64, 0, 0);
		(*(void(__fastcall**)(uint8_t*, __int64, uint64_t, uint64_t))(*(uint64_t*)rdx + 0x40))(rdx, rcx + 0x65, 0, 0);
	}
	printf("step 5\n");

	return true;
	//originalWriteRespawnPlayerPedEvent(rcx, rdx);
}

uint64_t menu::hooks::commandEventSend(uint64_t rcx, uint64_t rdx, uint64_t r8) {
	auto ret = originalCommandEventSend(rcx, rdx, r8);
	*(uint32_t*)(ret + 0x50) = 0x99999;
	return ret;
}

uint8_t menu::hooks::cashInventoryAddItem(uint32_t rcx, uint64_t rdx, uint32_t r8, uint64_t r9, int32_t r10) {

	//printf("rcx %X | rdx %llX | r8 %X | r9 %llX | r10 %X\n", rcx, rdx, r8, r9, r10);
	//for (auto i = 0; i < 0x20; i++) {
	//	printf("rdx[%zi] = %X\n", i, *(uint32_t*)(rdx + (i * 4)));
	//}


	return originalCashInventoryAdditem(rcx, rdx, r8, r9, r10);
}