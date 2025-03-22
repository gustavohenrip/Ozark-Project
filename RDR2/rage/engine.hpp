#pragma once
#include "stdafx.hpp"
#include "rage/types/network_types.hpp"
#include "rage/types/base_types.hpp"
#include "global/variables.hpp"
#include "utils/caller.hpp"
#include "utils/math.hpp"

namespace rage::engine {
	void sendSessionInfoRequestOld(presenceInfo* info, int count);
	bool changeNetObjOwner(uint64_t netObj, network::netGamePlayer* target);
	uint64_t migrateScriptHost(uint64_t scriptNetComp, network::netGamePlayer* target);
	uint8_t joinOrHost(char* sessionID);
	network::netGamePlayer* getHostNetGamePlayer();
	network::netGamePlayer* getNetGamePlayerFromIndex(int index);
	rage::network::pedFactory* getPedFactory();
	rage::network::networkPlayerManager* getNetworkPlayerManager();
	uint64_t getEntityAddress(Entity entity);
	Entity getEntityHandleFromAddress(uint64_t address);
	uint64_t getListPoolElementFromIndex(uint64_t pool, int index);
	uint64_t getWeaponInfo(Hash hash);
	rage::network::modelInfo* getModelInfo(uint64_t model);
	const char* getGameInfoString(const char* identifier);
	int getGameInfoInt(const char* identifier);
	std::vector<DWORD> getHashListFromHashTable(WORD count, uint64_t table);
	uint64_t getGameInfoUInt64(const char* identifier);
	uint64_t getGameInfoAddress(const char* identifier);
	uint64_t decryptPedPool(uint64_t table1, uint64_t table2, int expo);
	uint64_t decryptPedFactory(uint64_t table1, uint64_t table2, int expo);
	uint64_t decryptVehiclePool(uint64_t table1, uint64_t table2, int expo);
	uint64_t decryptPickupPool(uint64_t table1, uint64_t table2, int expo);
	uint64_t decryptObjectPool(uint64_t table1, uint64_t table2, int expo);
	uint64_t decryptEntityPool(uint64_t table1, uint64_t table2, int expo);
	uint64_t decryptNetworkQueuePool(uint64_t table1, uint64_t table2, int expo);
	uint64_t decryptAttachmentPool(uint64_t table1, uint64_t table2, int expo);
	void setTextWrap(float min, float max);
	void setTextFont(int font);
	void setTextJustification(int align);
	void setTextOutline();
	bool triggerPresence(presenceInfo* data, const char* payload);
	bool triggerClockEvent(int unk, network::netGamePlayer* target = 0);
	bool triggerWeatherEvent(bool unk, int id, uint32_t table = 0x10, network::netGamePlayer* target = 0);
	bool worldToScreen(math::vector3<float> world, float* x, float* y);
	void rewardsPickupCrash(int index);
	void setTimecycleModifierWithHash(DWORD hash);
	int getFrameCount();
}