#include "hooks.hpp"
#include "global/auth_vars.hpp"

const char* menu::hooks::appendStringWithResolvedTagHook(uint32_t tag, int unk, const char** string) {
	if (global::vars::g_showingNotifyMessage) {
		if (tag == 0x2393F765) {
			global::vars::g_showingNotifyMessage = false;
			return *string;
		}
	}

	return originalAppendStringWithResolvedTag(tag, unk, string);
}