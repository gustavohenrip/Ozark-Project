#include "hooks.hpp"
#include "utils/caller.hpp"
#include "global/variables.hpp"
#include "menu/base/util/player_manager.hpp"
#include "menu/base/submenus/main/network/spoofing/network_spoofing_vehicle_rotation.hpp"
#include "rage/engine.hpp"

struct EntityOrientation {
	char _0x0000[0xC0];
	Math::Matrix m_TransformedMatrix;
};

void Menu::Hooks::SyncEntityMatrixHook(uint64_t Rcx, uint64_t Rdx) {
	EntityOrientation* Transform = (EntityOrientation*)Rdx;

	if (Transform) {
		auto& Vars = NetworkSpoofingVehicleRotationMenuVars::m_Vars;
		if (Vars.m_Spinbot || Vars.m_Static || Vars.m_Jitter) {
			if (Menu::GetLocalPlayer().m_InVehicle) {
				if (Global::Vars::g_PedFactory) {
					if (Global::Vars::g_PedFactory->m_LocalPed) {
						Rage::Types::Vehicle* Vehicle = (Rage::Types::Vehicle*)(*(uint64_t*)((Rcx - 200) + 0x50));

						if (Vehicle) {
							if (Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle == Vehicle) {
								memcpy(&Transform->m_TransformedMatrix, Vehicle->GetMatrix(), sizeof(Math::Matrix));
								Rage::Engine::HandleRotationValuesFromOrder(&Transform->m_TransformedMatrix, &Vars.m_HookValue, 2);
								return;
							}
						}
					}
				}
			}
		}
	}

	OriginalSyncEntityMatrix(Rcx, Rdx);
}