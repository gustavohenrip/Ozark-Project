#include "hooks.hpp"
#include "menu/base/submenus/main/network/network_spoofing.hpp"

void Menu::Hooks::PedMovementDataNodeHook(uint64_t Rcx, uint64_t Rdx) {
	OriginalPedMovementDataNode(Rcx, Rdx);

	if (NetworkSpoofingMenuVars::m_Vars.m_MoveAnimation) {
		*(bool*)(Rcx + 0xC0) = false; // Animation
		*(bool*)(Rcx + 0xC1) = false; // Animation
	}
}