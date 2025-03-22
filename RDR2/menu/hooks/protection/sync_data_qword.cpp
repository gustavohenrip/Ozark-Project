#include "../hooks.hpp"
#include "global/auth_vars.hpp"
#include "utils/log.hpp"

bool menu::hooks::syncDataQwordHook(uint64_t stream, uint64_t data) {
	printf("%llx\n", data);
	return originalSyncDataQword(stream, data);
}