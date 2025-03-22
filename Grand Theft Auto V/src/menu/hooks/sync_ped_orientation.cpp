#include "hooks.hpp"
#include "menu/base/submenus/main/network/spoofing/network_spoofing_player_rotation.hpp"
#include "menu/base/util/player_manager.hpp"

void Menu::Hooks::SyncPedOrientationHook(uint64_t Rcx, uint64_t Rdx) {
	auto& Vars = NetworkSpoofingPlayerRotationMenuVars::m_Vars;
	if (Vars.m_Spinbot || Vars.m_FaceDirection || Vars.m_Jitter) {
		if (Rcx) {
			if (Global::Vars::g_PedFactory) {
				Rage::Types::Ped* Ped = *(Rage::Types::Ped**)(Rcx - 0x1B0);
				if (Ped == Global::Vars::g_PedFactory->m_LocalPed) {
					if (Vars.m_Spinbot || Vars.m_Jitter) {
						*(float*)(Rdx + 0xC0) = Vars.m_HookValue.m_X;
						*(float*)(Rdx + 0xC4) = Vars.m_HookValue.m_Y;
						return;
					}

					if (Vars.m_FaceDirection) {
						float Val = 0.f;

						switch (Vars.m_FaceDirectionVal) {
							case 0: Val = -90.f; break;
							case 1: Val = 90.f; break;
							case 2: Val = 180.f; break;
						}

						if (Vars.m_Relative) {
							Val += Menu::GetLocalPlayer().m_Heading;
						}

						if (Val > 180.f) Val = -180.f + (Val - 180.f);

						*(float*)(Rdx + 0xC0) = Val * 0.0174532924f;
						*(float*)(Rdx + 0xC4) = Val * 0.0174532924f;
					}

					return;
				}
			}
		}
	}

	OriginalSyncPedOrientation(Rcx, Rdx);
}