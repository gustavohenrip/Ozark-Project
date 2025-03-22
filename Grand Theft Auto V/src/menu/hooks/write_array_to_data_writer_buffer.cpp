#include "hooks.hpp"
#include "utils/log.hpp"
#include "menu/base/submenus/main/network/players/network_players_removal.hpp"

bool Menu::Hooks::WriteArrayToDataWriterBufferHook(Rage::BitBufferSyncInstance* Instance, uint32_t* Input, uint32_t BitsToWrite, uint32_t R9d) {
	auto& Vars = NetworkPlayersRemovalMenuVars::m_Vars;

	if (Vars.m_BreakNodes || Vars.m_SessionCrash) {
		if (Vars.m_BreakNodeThreads[0] == GetCurrentThreadId() || Vars.m_BreakNodeThreads[1] == GetCurrentThreadId()) {
			bool FuckUp = false;

			if ((Vars.m_SessionCrash && Vars.m_SessionCrashID == 2) || (Vars.m_TargetedCrash && Vars.m_TargetedCrashID == 2)) {
				FuckUp = true;
			}

			if (FuckUp) {
				return true;
			}
		}
	}

	return OriginalWriteArrayToDataWriterBuffer(Instance, Input, BitsToWrite, R9d);
}