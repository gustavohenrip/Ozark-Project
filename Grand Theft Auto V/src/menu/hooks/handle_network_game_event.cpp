#include "hooks.hpp"
#include "utils/caller.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_game_events.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_notifications.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/util/helpers.hpp"
#include "utils/log.hpp"
#include "utils/memory/memory.hpp"
#include "menu/base/util/player_manager.hpp"
#include "utils/va.hpp"
#include "menu/base/submenus/main.hpp"

TranslationString T_JustSpawned("just spawned", true, true);
TranslationString T_PickupProcessing("Pickup Processing", true, true);

struct EventNetworkPlayerSpawn {
	char _0x0000[0x18];
	char m_Name[0x20];
};

struct EventNetworkPlayerCollectedAmbientPickup {
	char _0x0000[0x10];
	int m_Sender;
	char _0x0014[0x4];
	uint32_t m_PickupHash;
	char _0x001C[0x4];
	int m_Value;
	char _0x0024[0xC];
	uint32_t m_Model;
};

uint64_t Menu::Hooks::HandleNetworkGameEventHook(uint64_t This) {
	uint64_t Table = *(uint64_t*)This;
	uint32_t ID = Caller::Call<uint32_t>(*(uint64_t*)(Table + 0x18));

	switch (ID) {
		case 164: { // CEventNetworkPlayerSpawn
			EventNetworkPlayerSpawn* Event = (EventNetworkPlayerSpawn*)This;

			if (NetworkProtectionNotificationsMenuVars::m_Vars.m_PlayerSpawn) {
				Menu::GetNotify()->NotifyStacked(Menu::Helpers::CleanPlayerName(Event->m_Name) + " " + TRANSLATE(T_JustSpawned), Global::UIVars::g_NotificationSuccess);
			}

			break;
		}

		case 166: { // CEventNetworkPlayerCollectedAmbientPickup
			EventNetworkPlayerCollectedAmbientPickup* Event = (EventNetworkPlayerCollectedAmbientPickup*)This;

			if (NetworkProtectionGameEventsMenuVars::m_Vars.m_PickupProcessing.m_Notify) {
				Menu::GetNotify()->NotifyStackedProtectionEvent("~c~Unknown~w~", TRANSLATE(T_PickupProcessing), false, NetworkProtectionGameEventsMenuVars::m_Vars.m_PickupProcessing.m_Block);
			}

			if (NetworkProtectionGameEventsMenuVars::m_Vars.m_PickupProcessing.m_Block) {
				Event->m_Value = 0;
				Event->m_PickupHash = 0;
				Event->m_Model = 0;
			}
			
			break;
		}
	}

	return OriginalHandleNetworkGameEvent(This);
}