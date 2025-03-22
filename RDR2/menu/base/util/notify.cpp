#include "notify.hpp"
#include "utils/log.hpp"
#include "global/variables.hpp"
#include "utils/memory/memory.hpp"
#include "utils/caller.hpp"

namespace menu {
	void notify::bottom(std::string what) {
		native::setCachedObjective(native::createVarString(10, "LITERAL_STRING", what.c_str()));
		native::printCachedObjective();
		native::clearCachedObjective();
		customInfo("Notify", what.c_str());
	}

	void notify::right(std::string text) {
		if (!text.empty()) {
			const char* yeet = native::createVarString(10, "LITERAL_STRING", text.c_str());
			int32_t rcx[10] = { -2, 0, 0, 0 };
			uint64_t rdx[10] = { 0x0, (uint64_t)yeet };
			_i<int>(0xB2920B9760F0F36B, &rcx, &rdx, 1);
			customInfo("Notify", text.c_str());
		}
	}

	notify* getNotify() {
		static notify instance;
		return &instance;
	}
}