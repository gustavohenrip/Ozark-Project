#include "../hooks.hpp"
#include "utils/log.hpp"

const char* menu::hooks::constructRequestUrlHook(uint64_t thisPtr, char* buffer, int size) {
	auto ret = originalConstructRequestUrl(thisPtr, buffer, size);
	if (ret) {
		for (auto blockedApi : { "Telemetry", "Bonus" }) {
			if (std::string(ret).find(blockedApi) != std::string::npos) {
				customDev("AC", "[%llx] Blocked api %s", 0x140000000 + ((uint64_t)_ReturnAddress() - (uint64_t)GetModuleHandleA(0)), ret);

				memset(buffer, 0, size);
				sprintf(buffer, "https://pornhub.com");
				return buffer;
			}
		}
	}

	return ret;
}