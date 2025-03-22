#include "hooks.hpp"
#include "rage/engine.hpp"
#include "utils/log.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/submenus/main.hpp"

TranslationString T_Ghost("Ghost", true, true);

bool Menu::Hooks::ReceiveCloneRemoveHook(uint64_t This, Rage::Network::NetGamePlayer* NetGamePlayer, uint64_t R8, uint16_t NetworkID, uint64_t Stack) {
	if (Global::Vars::g_PedFactory && Global::Vars::g_PedFactory->m_LocalPed) {
		Rage::Network::NetObj* Object = Rage::Engine::GetNetworkObjectFromNetworkID(This, NetworkID, true);
		if (Object == Global::Vars::g_PedFactory->m_LocalPed->GetNetObj()) {
			LOG_CUSTOM_DEV("ReceiveCloneRemove", "Trying to delete local player");
			return false;
		}
	}

	Rage::Network::NetObj* Object = Rage::Engine::GetNetworkObjectFromNetworkID(This, NetworkID, true);
	if (Object) {
		if (Object->m_Type == NetObjectPlayer) {
			LOG_INFO("Preventing %s removing due to Ghost", NetGamePlayer->m_PlayerInfo->m_Name);
			Menu::GetNotify()->NotifyStackedProtectionEvent(NetGamePlayer->m_PlayerInfo->m_Name, TRANSLATE(T_Ghost), false, true);
			return false;
		}
	}

	return OriginalReceiveCloneRemove(This, NetGamePlayer, R8, NetworkID, Stack);
}