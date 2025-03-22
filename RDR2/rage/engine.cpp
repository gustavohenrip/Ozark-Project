#include "engine.hpp"
#include "utils/memory/pattern.hpp"
#include "utils/math.hpp"
#include "invoker/natives.hpp"

namespace rage::engine {
	void sendSessionInfoRequestOld(presenceInfo* info, int count) {
		caller::call<int>(global::vars::g_sendSessionInfoRequest, info, count);
	}

	const char* getGameInfoString(const char* identifier) {
		if (global::vars::g_getGameInfoItem && global::vars::g_gameInfoItemTable) {
			int nameItem = caller::call<int>(global::vars::g_getGameInfoItem, identifier, global::vars::g_gameInfoItemTable + 0x20, *(uint32_t*)(global::vars::g_gameInfoItemTable + 0x5930));
			const char* name = (char*)(((global::vars::g_gameInfoItemTable + 0x20) + (nameItem * 0x1C8)) + 0x40);
			if (name && *name != 0xFF) {
				return name;
			}
		}

		return NULL;
	}

	int getGameInfoInt(const char* identifier) {
		if (global::vars::g_getGameInfoItem && global::vars::g_gameInfoItemTable) {
			int nameItem = caller::call<int>(global::vars::g_getGameInfoItem, identifier, global::vars::g_gameInfoItemTable + 0x20, *(uint32_t*)(global::vars::g_gameInfoItemTable + 0x5930));
			return *(int*)(((global::vars::g_gameInfoItemTable + 0x20) + (nameItem * 0x1C8)) + 0x18);
		}

		return 0;
	}

	uint64_t getGameInfoUInt64(const char* identifier) {
		if (global::vars::g_getGameInfoItem && global::vars::g_gameInfoItemTable) {
			int nameItem = caller::call<int>(global::vars::g_getGameInfoItem, identifier, global::vars::g_gameInfoItemTable + 0x20, *(uint32_t*)(global::vars::g_gameInfoItemTable + 0x5930));
			return *(uint64_t*)(((global::vars::g_gameInfoItemTable + 0x20) + (nameItem * 0x1C8)) + 0x18);
		}

		return 0;
	}

	uint64_t getGameInfoAddress(const char* identifier) {
		if (global::vars::g_getGameInfoItem && global::vars::g_gameInfoItemTable) {
			int nameItem = caller::call<int>(global::vars::g_getGameInfoItem, identifier, global::vars::g_gameInfoItemTable + 0x20, *(uint32_t*)(global::vars::g_gameInfoItemTable + 0x5930));

			//return (((global::vars::g_gameInfoItemTable + 0x20) + (nameItem * 0x148)));
			return (((global::vars::g_gameInfoItemTable + 0x20) + (nameItem * 0x1C8)));
			//0x1C8
		}

		return 0;
	}

	uint64_t getWeaponInfo(Hash hash) {
		Hash h = hash;
		return caller::call<uint64_t>(global::vars::g_getWeaponInfo, &h);
	}

	network::netGamePlayer* getHostNetGamePlayer() {
		return caller::call<network::netGamePlayer*>(global::vars::g_getHostNetGamePlayer);
	}

	bool changeNetObjOwner(uint64_t netObj, network::netGamePlayer* target) {
		static int idkwhatimdoing = 0;
		return caller::call<bool>(global::vars::g_changeNetObjOwner, *(uint64_t*)global::vars::g_netObjMgr, netObj, target, 0, 1);
	}

	network::netGamePlayer* getNetGamePlayerFromIndex(int index) {
		return caller::call<network::netGamePlayer*>(global::vars::g_getNetGamePlayerFromIndex, index);
	}

	uint64_t getEntityAddress(Entity entity) {
		return caller::call<uint64_t>(global::vars::g_getEntityAddress, entity);
	}

	Entity getEntityHandleFromAddress(uint64_t address) {
		return caller::call<Entity>(global::vars::g_getEntityHandleFromAddress, address, 0);
	}

	uint64_t getListPoolElementFromIndex(uint64_t pool, int index) {
		return caller::call<uint64_t>(global::vars::g_getListPoolElementFromIndex, pool, index);
	}

	uint64_t migrateScriptHost(uint64_t scriptNetComp, network::netGamePlayer* target) {
		return caller::call<uint64_t>(global::vars::g_migrateScriptHost, scriptNetComp, target, 0, true);
	}

	uint8_t joinOrHost(char * sessionID) {
		return caller::call<uint8_t>(global::vars::g_joinOrHostSession, 1, nullptr, sessionID);
	}

	rage::network::modelInfo* getModelInfo(uint64_t model) {
		uint32_t unk = 0xFFFF;
		return caller::call<rage::network::modelInfo*>(global::vars::g_getModelInfo, model, &unk);
	}

	int getFrameCount() {
		return *(int*)global::vars::g_currentFrameCount;
	}

	void setTextWrap(float min, float max) {
		*(float*)global::vars::g_textWrapMin = min;
		*(float*)global::vars::g_textWrapMax = max;
	}

	void setTextFont(int font) {
		*(int*)global::vars::g_textFont = font;
	}

	void setTextJustification(int align) {
		*(int*)global::vars::g_textAlign = align;
	}

	void setTextOutline() {
		*(bool*)global::vars::g_textOutline = true;
	}

	void setTimecycleModifierWithHash(DWORD hash) {
		DWORD h = hash;
		int id = caller::call<int>(global::vars::g_getIndexFromHashList, global::vars::g_visionHashList, &h, "SetScriptModifierId", 0);
		if (id) {
			*(int*)global::vars::g_timecycleIndex = id;
		}
	}

	bool triggerPresence(presenceInfo* data, const char* payload) {
		return caller::call<bool>(global::vars::g_triggerPresence, 0, data, 1, payload, 0, true, 0);
	}

	bool triggerClockEvent(int unk, network::netGamePlayer* target) {
		if (global::vars::g_triggerClockEvent) {
			return caller::call<bool>(global::vars::g_triggerClockEvent, unk, target);
		}
		
		return 0;
	}

	bool triggerWeatherEvent(bool unk, int id, uint32_t table, network::netGamePlayer* target) {
		if (global::vars::g_triggerWeatherEvent) {
			return caller::call<bool>(global::vars::g_triggerWeatherEvent, unk, id, table, target);
		}

		return 0;
	}

	bool worldToScreen(math::vector3<float> world, float * x, float * y) {
		return caller::call<bool>(global::vars::g_worldToScreen, world, x, y);
	}

	void rewardsPickupCrash(int index) {
		int32_t bit = 0;
		bit |= 1 << index;
		caller::call<uint32_t>(global::vars::g_rewardPickupsEvent, &bit, 0x4ACCE289);
		caller::call<uint32_t>(global::vars::g_rewardPickupsEvent, &bit, 0x91ECDA87);
	}

	std::vector<DWORD> getHashListFromHashTable(WORD count, uint64_t table) {
		std::vector<DWORD> ret;

		if (count) {
			if (table) {
				for (WORD i = 0; i < count; i++) {
					uint64_t entries = *(uint64_t*)(table + (8 * i));
					if (entries) {
						DWORD hash = *(DWORD*)(entries + 0x20);
						if (hash) {
							ret.push_back(hash);
						}
					}
				}
			}
		}

		return ret;
	}

	rage::network::pedFactory* getPedFactory() {
		return (rage::network::pedFactory*)decryptPedFactory(global::vars::g_encryptedPedFactory.table1, global::vars::g_encryptedPedFactory.table2, global::vars::g_encryptedPedFactory.addition);
	}

	rage::network::networkPlayerManager* getNetworkPlayerManager() {
		return *(rage::network::networkPlayerManager**)global::vars::g_networkPlayerMgr;
	}

	uint64_t decryptPedFactory(uint64_t table1, uint64_t table2, int expo) {
		uint64_t rcx = *(uint64_t*)table1;
		uint64_t rdx = *(uint64_t*)table2;
		rcx = math::ror8(rcx, 5);
		rcx = math::rol8(rcx, 0x20);
		rdx ^= rcx;
		uint32_t eax = (uint32_t)rcx;
		eax &= 0x1F;
		rdx = math::rol8(rdx, 0x20);
		eax += expo; // 1
		uint8_t cl = (uint8_t)eax;
		rdx = math::rol8(rdx, cl);
		return ~rdx;
	}

	uint64_t decryptPedPool(uint64_t table1, uint64_t table2, int expo) {
		uint64_t rcx = *(uint64_t*)table1;
		uint64_t rdx = *(uint64_t*)table2;
		rcx = math::rol8(rcx, 0x20);
		rcx = math::ror8(rcx, 2);
		rdx ^= rcx;
		uint32_t eax = (uint32_t)rcx;
		eax &= 0x1F;
		eax += expo;
		uint8_t cl = (uint8_t)eax;
		rdx = math::rol8(rdx, cl);
		rdx = math::rol8(rdx, 0x20);
		return ~rdx;
	}

	uint64_t decryptVehiclePool(uint64_t table1, uint64_t table2, int expo) {
		uint64_t rcx = *(uint64_t*)table1;
		uint64_t rdx = *(uint64_t*)table2;
		rcx = math::ror8(rcx, 1);
		rcx = math::rol8(rcx, 0x20);
		rdx ^= rcx;
		uint32_t eax = (uint32_t)rcx;
		eax &= 0x1F;
		rdx = math::rol8(rdx, 0x20);
		eax += expo;
		uint8_t cl = (uint8_t)eax;
		rdx = math::rol8(rdx, cl);
		return ~rdx;
	}

	uint64_t decryptPickupPool(uint64_t table1, uint64_t table2, int expo) {
		uint64_t rcx = *(uint64_t*)table1;
		uint64_t rdx = *(uint64_t*)table2;
		rcx = math::rol8(rcx, 0x20);
		rcx = math::ror8(rcx, 3);
		rdx ^= rcx;
		uint32_t eax = (uint32_t)rcx;
		eax &= 0x1F;
		eax += expo;
		uint8_t cl = (uint8_t)eax;
		rdx = math::rol8(rdx, cl);
		rdx = math::rol8(rdx, 0x20);
		return ~rdx;
	}

	uint64_t decryptObjectPool(uint64_t table1, uint64_t table2, int expo) {
		uint64_t rcx = *(uint64_t*)table1;
		uint64_t rdx = *(uint64_t*)table2;
		rcx = math::ror8(rcx, 3);
		rcx = math::rol8(rcx, 0x20);
		rdx ^= rcx;
		uint32_t eax = (uint32_t)rcx;
		eax &= 0x1F;
		rdx = math::rol8(rdx, 0x20);
		eax += expo;
		uint8_t cl = (uint8_t)eax;
		rdx = math::rol8(rdx, cl);
		return ~rdx;
	}

	uint64_t decryptEntityPool(uint64_t table1, uint64_t table2, int expo) {
		uint64_t rcx = *(uint64_t*)table1;
		uint64_t rdx = *(uint64_t*)table2;
		rcx = math::rol8(rcx, 0x20);
		rcx = math::ror8(rcx, 2);
		rdx ^= rcx;
		uint32_t eax = (uint32_t)rcx;
		eax &= 0x1F;
		eax += expo;
		uint8_t cl = (uint8_t)eax;
		rdx = math::rol8(rdx, cl);
		rdx = math::rol8(rdx, 0x20);
		return ~rdx;
	}

	uint64_t decryptNetworkQueuePool(uint64_t table1, uint64_t table2, int expo) {
		uint64_t rcx = *(uint64_t*)table1;
		uint64_t rdx = *(uint64_t*)table2;
		rcx = math::rol8(rcx, 0x20);
		rcx = math::ror8(rcx, 4);
		rdx ^= rcx;
		uint32_t eax = (uint32_t)rcx;
		eax &= 0x1F;
		eax += expo;
		uint8_t cl = (uint8_t)eax;
		rdx = math::rol8(rdx, cl);
		rdx = math::rol8(rdx, 0x20);
		return ~rdx;
	}

	uint64_t decryptAttachmentPool(uint64_t table1, uint64_t table2, int expo) {
		uint64_t rcx = *(uint64_t*)table1;
		uint64_t rdx = *(uint64_t*)table2;
		rcx = math::ror8(rcx, 4);
		rcx = math::rol8(rcx, 0x20);
		rdx ^= rcx;
		uint32_t eax = (uint32_t)rcx;
		eax &= 0x1F;
		rdx = math::rol8(rdx, 0x20);
		eax += expo;
		uint8_t cl = (uint8_t)eax;
		rdx = math::rol8(rdx, cl);
		return ~rdx;
	}
}