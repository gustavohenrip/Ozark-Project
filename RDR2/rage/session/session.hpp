#pragma once
#include "utils/math.hpp"
#include "rage/engine.hpp"
#include "utils/singleton.hpp"

namespace rage::network {
	class networkTask {
	public:
		int getSessionHost();
		netPlayerInfo* getNetPlayerInfo(int player);
		int getWeatherIdFromHash(uint64_t table, Hash hash);
		void triggerWeather(Hash weather, int32_t index = -1);
		void setWeather(int index, int val, bool flag = false);
		void forceScriptHost(int index);
		void joinFreemode();
	}; networkTask* getNetworkTask();
}
