#include "menu/hooks/hooks.hpp"
#include "utils/log.hpp"

bool menu::hooks::deleteNetworkEventHook(uint64_t eventPool, bool deleteNow) {
	auto returnAddress = _ReturnAddress();

	if (returnAddress == (void*)global::vars::g_deleteNetworkEventReportCashSpawn) {
		customDev("AC", "Blocked report cash spawn event");
		return false;
	}

	return originalDeleteNetworkEvent(eventPool, deleteNow);
}