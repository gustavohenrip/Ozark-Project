#include "hooks.hpp"
#include "menu/base/submenus/main/network/network_spoofing.hpp"

bool Menu::Hooks::SendJoinRequestHook(Rage::Network::JoinRequest* Rcx, uint64_t Rdx, int R8d, uint32_t* R9d) {
	if (NetworkSpoofingMenuVars::m_Vars.m_RockstarDeveloper) {
		Rcx->m_DeveloperFlags |= 2;
	} else {
		Rcx->m_DeveloperFlags &= ~2; // just in case r* developer perks is enabled
	}

	return OriginalSendJoinRequest(Rcx, Rdx, R8d, R9d);
}