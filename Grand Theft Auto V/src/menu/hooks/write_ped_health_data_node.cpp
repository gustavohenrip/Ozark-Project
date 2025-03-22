#include "hooks.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_flags.hpp"

void Menu::Hooks::WritePedHealthDataNodeHook(uint64_t Rcx, uint64_t Rdx) {
	OriginalWritePedHealthDataNode(Rcx, Rdx);

	Rage::Types::Ped* Ped = *(Rage::Types::Ped**)(Rcx - 0x1B0);
	if (Ped) {
		if (Ped == Global::Vars::g_PedFactory->m_LocalPed) {
			if (NetworkProtectionFlagsMenuVars::m_Vars.m_Health) {
				if (*(int*)(Rdx + 0xC4) > 328) {
					*(int*)(Rdx + 0xC4) = 328;
				}
			}

			if (NetworkProtectionFlagsMenuVars::m_Vars.m_Armor) {
				if (*(int*)(Rdx + 0xC8) > 50) {
					*(int*)(Rdx + 0xC8) = 50;
				} else {
					if ((*(int*)(Rdx + 0xC8) % 50) != 0) {
						*(int*)(Rdx + 0xC8) = 0;
					}
				}
			}
		}
	}
}