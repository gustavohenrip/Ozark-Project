#include "../hooks.hpp"
#include "rage/engine.hpp"
#include "rage/entity/entity.hpp"
#include "menu/base/util/player_manager.hpp"
#include "menu/base/submenus/main/network/protection/protection.hpp"
#include "utils/log.hpp"
#include "menu/base/util/control_manager.hpp"
#include "src/menu/base/util/fiber_manager.hpp"
#include "src/menu/base/util/fiber_pool.hpp"
#include <mutex>
#include "menu/base/util/notify.hpp"
#include "utils/memory/pattern.hpp"

bool menu::hooks::recieveCloneCreateHook(uint64_t this_, rage::network::netGamePlayer* sender, uint16_t objectType, uint16_t objectID, uint16_t objectFlags,
uint64_t unk, uint64_t bitBuffer, unsigned int shit, std::uint32_t timestamp) {

	return originalRecieveCloneCreate(this_, sender, objectType, objectID, objectFlags, unk, bitBuffer, shit, timestamp);
}

uint32_t findModelHash(uint64_t objectInfo) {
	for (int i = 0; i < 500; i++) {
		if (native::isModelValid(*(uint32_t*)(objectInfo + i))) {
			return *(uint32_t*)(objectInfo + i);
		}
	}
	return 0;
}

bool menu::hooks::syncNetworkObject(uint64_t tree, rage::network::netObject* netObj) {


	static std::vector<uint16_t> hashLocations = { 0x130, 0x138, 0x13C, 0x140, 0x24C, 0x134 };


	static std::vector<uint8_t> invalidHandling = {0, 2, 4, 6, 20, 8, 9, 11, 12, 13};
	static std::unordered_map<uint16_t, uint8_t> typeMap;
	typeMap[NET_OBJ_TYPE_DRAFT_VEH] = 0x5;
	typeMap[NET_OBJ_TYPE_BOAT] = 0xE;
	typeMap[NET_OBJ_TYPE_TRAIN] = 0x11;
	typeMap[NET_OBJ_TYPE_AUTOMOBILE] = 0xA; // thanks to sa7an (:

	if (tree) {
		uint64_t objectInfo = *(uint64_t*)(tree + 0xE0);
		if (objectInfo) {


			uint32_t hash = 0;
			std::for_each(hashLocations.begin(), hashLocations.end(), [&](uint16_t& data) {
				if (native::isModelValid(*(uint32_t*)(objectInfo + data))) {
					hash = *(uint32_t*)(objectInfo + data);
				}
			});

			rage::network::modelInfo* modelInfo = (rage::network::modelInfo*)rage::engine::getModelInfo(hash);
			if (modelInfo) {
				if (modelInfo->isModelVehicle()) {

					auto vit = std::find_if(invalidHandling.begin(), invalidHandling.end(), [&](uint8_t& data) {
						return data == modelInfo->vehicleHandlingType;
					});

					if (vit != invalidHandling.end()) {
						return false;
					}

					if (typeMap[netObj->objectType] != modelInfo->vehicleHandlingType) {
						return false;
					}

#ifdef DEV
					static std::unordered_map<uint16_t, std::vector<int32_t>> mapp;
					auto vit2 = std::find_if(mapp[modelInfo->getModelType()].begin(), mapp[modelInfo->getModelType()].end(), [&](int32_t& data) {
						return data == modelInfo->vehicleHandlingType;
						});

					if (vit2 == mapp[modelInfo->getModelType()].end()) {
						mapp[modelInfo->getModelType()].push_back(modelInfo->vehicleHandlingType);

						printf("[map] 0x%X = { ", modelInfo->getModelType());
						std::for_each(mapp[modelInfo->getModelType()].begin(), mapp[modelInfo->getModelType()].end(), [&](int32_t& data) {
							printf("0x%X ", data);
							});
						printf("}; | entity type: 0x%X\n", netObj->objectType);
					}
#endif
				}

				if(netObj->objectType != NET_OBJ_TYPE_PLAYER){
					if (networkProtectionMenuVars::vars.maliciousEntities) {
						if (menu::getPlayer(netObj->ownerID).ped != menu::getLocalPlayer().ped && !netObj->physical) {// check if its not self created and if physical doesnt exist yet
							for (std::size_t i = 0; i < NUMOF(networkProtectionMenuVars::maliciousHashList); i++) {
								if (hash && hash == networkProtectionMenuVars::maliciousHashList[i]) {
									menu::playerVars& player = menu::getPlayer(netObj->ownerID);
									auto vit = std::find_if(networkProtectionMenuVars::vars.maliciousObjQueue.begin(), networkProtectionMenuVars::vars.maliciousObjQueue.end(),
										[=](std::pair<rage::network::netObject*, Player>& data) {
											return data.first == netObj && data.second == player.id;
										});
									if (vit == networkProtectionMenuVars::vars.maliciousObjQueue.end()) {
										networkProtectionMenuVars::vars.maliciousObjQueue.push_back(std::make_pair(netObj, player.id));
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return originalSyncNetworkObject(tree, netObj);
}

uint64_t menu::hooks::deletePedHook(Ped* ped) {
	if (/*native::getHashOfThisScriptName() == 0xDBE6C60 && */native::getEntityModel(*ped) == 0x39c84a35) {
		return 0;
	}

	return originalDeletePed(ped);
}

uint64_t* menu::hooks::doesEntityExist(int32_t ped) {
	if (native::getHashOfThisScriptName() == 0xDBE6C60 && native::getEntityModel(ped) == 0x39c84a35) {//DBE6C60
		printf("kkk model thread: %X\n", _i<int>(0x55525C346BEF6960));
		return 0;
	}
	return originalDoesEntityExist(ped);
}