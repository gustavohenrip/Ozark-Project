#include "hooks.hpp"
#include "utils/fiber_pool.hpp"
#include "rage/engine.hpp"
#include "rage/invoker/natives.hpp"
#include "menu/base/submenus/main/network/network_protection.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_entity_events.hpp"
#include "menu/base/util/player_manager.hpp"
#include "utils/log.hpp"

void Menu::Hooks::PhysicalAttachDataNodeHook(uint64_t Rcx, uint64_t Rdx) {
	if (Rcx) {
		if (*(bool*)(Rcx + 0xC0)) {
			if (IsValidPtr(Menu::GetLocalPlayer().m_PedPointer)) {
				if (IsValidPtr(Menu::GetLocalPlayer().m_PedPointer->GetNetObj())) {
					if (Menu::GetLocalPlayer().m_PedPointer->GetNetObj()->m_NetworkID == *(uint16_t*)(Rcx + 0xC2)) {
						if (NetworkProtectionEntityEventsMenuVars::m_Vars.m_Attachments) {
							LOG_WARN("Blocked physical attachment on self");
							return;
						}
					}
				}
			}

			if (Menu::GetLocalPlayer().m_InVehicle) {
				uint64_t Address = Rage::Engine::GetEntityAddress(Menu::GetLocalPlayer().m_Vehicle);
				if (Address) {
					Rage::Network::NetObj* Object = *(Rage::Network::NetObj**)(Address + 0xD0);
					if (Object) {
						if (Object->m_NetworkID == *(uint16_t*)(Rcx + 0xC2)) {
							if (NetworkProtectionEntityEventsMenuVars::m_Vars.m_Attachments) {
								LOG_WARN("Blocked physical attachment on vehicle");
								return;
							}
						}
					}
				}
			}
		}
	}

	OriginalPhysicalAttachDataNode(Rcx, Rdx);
}

void Menu::Hooks::PedAttachDataNodeHook(uint64_t Rcx, uint64_t Rdx) {
	if (Rcx) {
		if (IsValidPtr(Menu::GetLocalPlayer().m_PedPointer)) {
			if (IsValidPtr(Menu::GetLocalPlayer().m_PedPointer->GetNetObj())) {
				if (Menu::GetLocalPlayer().m_PedPointer->GetNetObj()->m_NetworkID == *(uint16_t*)(Rcx + 0xC2)) {
					if (NetworkProtectionEntityEventsMenuVars::m_Vars.m_Attachments) {
						LOG_WARN("Blocked ped attachment");
						return;
					}
				}
			}
		}
	}

	OriginalPedAttachDataNode(Rcx, Rdx);
}