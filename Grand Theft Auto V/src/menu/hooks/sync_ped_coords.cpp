#include "hooks.hpp"
#include "menu/base/util/player_manager.hpp"
#include "menu/base/submenus/main/network/network_spoofing.hpp"
#include "menu/base/util/timers.hpp"

Math::Vector4<float>* Menu::Hooks::SyncPedCoordsHook(Rage::Network::NetObj* Object, Math::Vector4<float>* Out) {
	auto Return = OrignalSyncPedCoords(Object, Out);

	if (NetworkSpoofingMenuVars::m_Vars.m_Position) {
		switch (NetworkSpoofingMenuVars::m_Vars.m_PositionVal) {
			case 0: { // Cycle Players
				static int Current = 0;
				if (Current == 32) Current = 0;

				static int Timer = 0;
				Menu::Timers::RunTimedFunction(&Timer, 1000, [=] {
					if (Menu::GetPlayer(Current).m_Exists) {
						Return->m_X = Menu::GetPlayer(Current).m_Coords.m_X;
						Return->m_Y = Menu::GetPlayer(Current).m_Coords.m_Y;
						Return->m_Z = Menu::GetPlayer(Current).m_Coords.m_Z;
					}
				});

				Current++;

				break;
			}

			case 1: { // Los Santos Airport
				Return->m_X = -1052.392f;
				Return->m_Y = -2933.508f;
				Return->m_Z = 13.951f;
				break;
			}

			case 2: { // In Place
				Return->m_X = NetworkSpoofingMenuVars::m_Vars.m_CachedPosition.m_X;
				Return->m_Y = NetworkSpoofingMenuVars::m_Vars.m_CachedPosition.m_Y;
				Return->m_Z = NetworkSpoofingMenuVars::m_Vars.m_CachedPosition.m_Z;
				break;
			}
		}
	}

	return Return;
}