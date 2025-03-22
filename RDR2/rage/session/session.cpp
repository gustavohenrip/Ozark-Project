#include "session.hpp"
#include "utils/memory/memory.hpp"

namespace rage::network {
	int networkTask::getSessionHost() {
		auto net = engine::getHostNetGamePlayer();
		if (net) {
			return net->playerID;
		}

		return 0;
	}

	netPlayerInfo* networkTask::getNetPlayerInfo(int player) {
		auto net = rage::engine::getNetGamePlayerFromIndex(player);
		if (net) {
			return &net->playerInfo->netInfo;
		}

		return nullptr;
	}

	int networkTask::getWeatherIdFromHash(uint64_t table, Hash hash) {
		if (*(DWORD*)(table) == 0) {
			printf("returned FAILURE\n");
			return 0xFFFFFFFF; 
		}
		int count = *(int*)table;

		table += 0x10;
		for (int i = 0; i <= count; i++) {
			if (*(Hash*)(table) == hash) {
				printf("returned: %i\n", i);
				return i;
			}

			table += 0x1E0;
		}
		printf("returned FAILURE\n");
		return 0xFFFFFFFF;
	}

	void networkTask::triggerWeather(Hash weather, int32_t index) {
		engine::triggerWeatherEvent(1, getWeatherIdFromHash(global::vars::g_weatherTable, weather), 0x4C, index == -1 ? 0 : engine::getNetGamePlayerFromIndex(index));
	}

	void networkTask::setWeather(int index, int val, bool flag) {
		engine::triggerWeatherEvent(1, val, 0x4C, flag ? 0 : engine::getNetGamePlayerFromIndex(index));
	}

	void networkTask::forceScriptHost(int index) {
		int count = 0;
		uint64_t threadList = *(uint64_t*)global::vars::g_scriptThreadList;
		if (threadList) {
			for (std::size_t i = 0; i < *(uint16_t*)global::vars::g_scriptThreadListSize; i++) {
				uint64_t thread = *(uint64_t*)(threadList + (i * 8));
				if (thread) {
					uint64_t scriptNetComp = *(uint64_t*)(thread + 0x06E8);
					if (scriptNetComp) {
						engine::migrateScriptHost(scriptNetComp, engine::getNetGamePlayerFromIndex(index));
						count++;
					}
				}
			}
		}
		printf("Forced host of %i scripts\n", count);
	}

	void networkTask::joinFreemode() {
		static char sessionID[0x30];
		memset(sessionID, 0, sizeof(sessionID));
		engine::joinOrHost(sessionID);
	}

	networkTask* getNetworkTask() {
		static networkTask instance;
		return &instance;
	}
}