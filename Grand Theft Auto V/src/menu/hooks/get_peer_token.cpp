#include "hooks.hpp"
#include "menu/base/submenus/main/network/network_spoofing.hpp"

bool Menu::Hooks::GetPeerTokenHook(uint64_t* Out) {
	if (NetworkSpoofingMenuVars::m_Vars.m_PeerToken) {
		if (Out) *Out = Global::Vars::g_SpoofedPeer;
		return true;
	}

	return OriginalGetPeerToken(Out);
}