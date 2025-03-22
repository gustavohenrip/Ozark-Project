#include "hooks.hpp"
#include "utils/log.hpp"
#include "rage/invoker/natives.hpp"

bool menu::hooks::readSessionInfoResponseHook(uint64_t rcx, int* edx) {
	bool retFalse = false;
	if (*(uint32_t*)(rcx + 0x23C4)) {
		uint32_t i = 0;
		do {
			uint64_t address = rcx + 0x22C0 + (i * 8);
			if (*(uint64_t*)(address)) {
				auto responseData = *(const char**)(address);
				if (responseData) {
					try {
						nlohmann::json json = nlohmann::json::parse(responseData);
						if (json.find("gsinfo") == json.end()) {
							return false;
						}

						uint64_t rid = std::stoull(json["_id"].get<std::string>().substr(3)); // strip "SC "

						auto vit = std::find_if(global::vars::g_onlineRockstarIdQueue.begin(), global::vars::g_onlineRockstarIdQueue.end(), [=] (uint64_t value) {
							return value == rid;
						});;

						if (vit != global::vars::g_onlineRockstarIdQueue.end()) {
							retFalse = true;
							global::vars::g_onlineRockstarIds[rid] = !json["gsinfo"].get<std::string>().empty();
							//dev("%i is %s", rid, global::vars::g_onlineRockstarIds[rid] ? "online" : "offline");
							//printf("response: %s\n", responseData);
							//dev(responseData);
							global::vars::g_onlineRockstarIdQueue.erase(vit);
						}
					} catch (...) {
						return false;
					}
				}
			}

			i++;
		} while (i < *(uint32_t*)(rcx + 0x23C4));

		if (retFalse) return false;
	}

	return originalReadSessionInfoResponse(rcx, edx);
}