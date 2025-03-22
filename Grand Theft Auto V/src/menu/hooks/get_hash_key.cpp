#include "hooks.hpp"

uint32_t Menu::Hooks::GetHashKeyHook(const char* Text, uint32_t Edx) {
	if (Text && !strcmp(Text, "OZARK_PTFX_PROTECTION")) {
		return Global::Vars::g_PTFXProtectionHash;
	}

	return OriginalGetHashKey(Text, Edx);
}