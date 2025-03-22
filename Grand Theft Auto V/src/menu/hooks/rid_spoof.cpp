#include "hooks.hpp"
#include "menu/base/submenus/main/network/spoofing/network_spoofing_rid.hpp"

bool Menu::Hooks::WriteNetMsgIdentifierHook(uint64_t Rcx, uint64_t Rdx, int R8d) {
	if (NetworkSpoofingRockstarIDMenuVars::m_Vars.m_SpoofingRockstarID) {
		if (Rdx == Global::Vars::g_RsInfo->m_RockstarID) {
			Rdx = NetworkSpoofingRockstarIDMenuVars::m_Vars.m_SpoofingRockstarID;
		}
	}

	return OriginalWriteNetMsgIdentifier(Rcx, Rdx, R8d);
}

bool Menu::Hooks::SyncRockstarIDHook(uint64_t Rcx, uint64_t Rdx, uint32_t R8d, uint64_t R9) {
	if (NetworkSpoofingRockstarIDMenuVars::m_Vars.m_SpoofingRockstarID) {
		if (*(uint64_t*)(Rcx + 0x38) == Global::Vars::g_RsInfo->m_RockstarID) {
			*(uint64_t*)(Rcx + 0x38) = NetworkSpoofingRockstarIDMenuVars::m_Vars.m_SpoofingRockstarID;
		}
	}

	return OriginalSyncRockstarID(Rcx, Rdx, R8d, R9);
}

bool Menu::Hooks::WriteUnsignedLongLongToBitBufferHook(uint64_t Rcx, uint64_t Rdx, uint64_t R8) {
	if (NetworkSpoofingRockstarIDMenuVars::m_Vars.m_SpoofingRockstarID) {
		if (Rdx == Global::Vars::g_RsInfo->m_RockstarID) {
			return OriginalWriteUnsignedLongLongToBitBuffer(Rcx, NetworkSpoofingRockstarIDMenuVars::m_Vars.m_SpoofingRockstarID, R8);
		}
	}

	return OriginalWriteUnsignedLongLongToBitBuffer(Rcx, Rdx, R8);
}