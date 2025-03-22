#include "hooks.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_join_events.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/utils.hpp"
#include "rage/engine.hpp"
#include "utils/caller.hpp"
#include "menu/base/util/player_manager.hpp"
#include "menu/base/submenus/main/network/spoofing/network_spoofing_rid.hpp"
#include "menu/base/submenus/main/network/network_spoofing.hpp"
#include "utils/log.hpp"
#include "utils/utils.hpp"

bool Menu::Hooks::GetScItemStringHook(uint64_t This, uint64_t Table, const char* Type, char* Buffer) {
	if (Type && !strcmp(Type, "gsinfo") && Buffer) {
		Global::Vars::g_RealGsInfo = std::string(Buffer);

		auto Vars = NetworkProtectionJoinEventsMenuVars::m_Vars;
		if (Vars.m_SpoofResolves && Vars.m_Target > 0 && !Vars.m_GsInfo.empty()) {
			strcpy(Buffer, Vars.m_GsInfo.c_str());
		}

		if (Vars.m_BlockResolves) {
			memset(Buffer, 0, 64);
		}
	}

	return OriginalGetScItemString(This, Table, Type, Buffer);
}