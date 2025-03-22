#include "hooks.hpp"
#include "menu/base/submenus/main/misc.hpp"

bool menu::hooks::scProfanityCheckStringHook(const char* str, int* token) {
	return originalScProfanityCheckString(miscMenuVars::vars.bypassProfanityFilter ? "OK" : str, token);
}