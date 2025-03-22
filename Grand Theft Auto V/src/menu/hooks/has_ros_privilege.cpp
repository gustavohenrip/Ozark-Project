#include "hooks.hpp"
#include "menu/base/submenus/main/misc.hpp"

bool Menu::Hooks::HasRosPrivilegeHook(uint64_t Table, int Type) {
	if (MiscMenuVars::m_Vars.m_RockstarDeveloperPerks) {
		if (Type == 0xC) {
			return true;
		}
	}

	return OriginalHasRosPrivilege(Table, Type);
}