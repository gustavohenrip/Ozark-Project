#include "hooks.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_flags.hpp"

bool(*OriginalWritePlayerGameStateDataNode)(uint64_t, uint64_t);
bool Menu::Hooks::WritePlayerGameStateDataNodeHook(uint64_t Rcx, uint64_t Rdx) {
	bool Return = OriginalWritePlayerGameStateDataNode(Rcx, Rdx);

	Rage::Types::Ped* Ped = *(Rage::Types::Ped**)(Rcx - 0x400);
	if (Ped) {
		if (Ped == Global::Vars::g_PedFactory->m_LocalPed) {
			// *(uint32_t*)(Rdx + 0xC0) = 0xFFFFFFFF; // otr

			if (NetworkProtectionFlagsMenuVars::m_Vars.m_SuperJump) {
				*(bool*)(Rdx + 0x1B7) = false;
			}

			if (NetworkProtectionFlagsMenuVars::m_Vars.m_Godmode) {
				*(bool*)(Rdx + 0xD5) = false;
			}

			if (NetworkProtectionFlagsMenuVars::m_Vars.m_Spectate) {
				*(bool*)(Rdx + 0xC7) = false;
				*(uint16_t*)(Rdx + 0x128) = 0;
			}
		}
	}

	return Return;
}