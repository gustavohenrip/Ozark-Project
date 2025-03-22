#include "hooks.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_game_events.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_notifications.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/util/player_manager.hpp"
#include "utils/log.hpp"
#include "menu/base/submenus/main.hpp"
#include "menu/base/submenus/main/network/network_protection.hpp"

TranslationString T_ModelSyncCrash("Model Sync Crash", true, true);
TranslationString T_ModelUpdate("Model Update", true, true);

bool Menu::Hooks::SyncPlayerModelHook(uint64_t PlayerAppearanceDataNode) {
	auto ProtectVar = NetworkProtectionGameEventsMenuVars::m_Vars.m_PlayerModelUpdate;
	uint32_t Model = *(uint32_t*)(PlayerAppearanceDataNode + 0x410);
	uint64_t PlayerSyncTree = *(uint64_t*)(PlayerAppearanceDataNode + 0x18);

	if (PlayerSyncTree) {
		Rage::Network::NetObj* Object = *(Rage::Network::NetObj**)(PlayerSyncTree + 0x8);
		if (Object) {
			auto& Player = Menu::GetPlayer(Object->m_Owner);
			if (Object->m_Owner >= 0 && Object->m_Owner < 34) {
				if (Player.m_NetworkModelHash == 0) {
					Player.m_NetworkModelHash = Model;
					LOG_DEV("Caching %s model -> %x", Player.m_Name, Model);
				} else {
					if (Player.m_NetworkModelHash) {
						if (Player.m_NetworkModelHash != Model) {
							if (Native::IsPedDeadOrDying(Player.m_Ped, true) || Native::GetEntityHealth(Player.m_Ped) == 0.f) {
								if (NetworkProtectionNotificationsMenuVars::m_Vars.m_Crash) {
									Menu::GetNotify()->NotifyStackedProtectionEvent(Player.m_Name, TRANSLATE(T_ModelSyncCrash), false, true);
								}

								return false;
							} else {
								LOG_DEV("%s is syncing their model as something else (while being alive) - %X", Player.m_Name, Model);
							}
						}
					}
				}

				if (Model != 0x0 && Model != 0x9c9effd8 && Model != 0x705e61f2) {
					if (Player.m_Connected && Player.IsValidName()) {
						if (Global::Vars::g_LastSyncedPlayerModels[Object->m_Owner] != Model) {
							Global::Vars::g_LastSyncedPlayerModels[Object->m_Owner] = Model;

							Player.m_IsModder = true;
							Player.m_ModderFlags[Menu::REASON_INVALID_MODEL] = true;

							if (ProtectVar.m_Notify) {
								Menu::GetNotify()->NotifyStackedProtectionEvent(Player.m_Name, TRANSLATE(T_ModelUpdate), false, ProtectVar.m_Block);
							}
						}
					}
					
					if (ProtectVar.m_Block) {
						*(uint32_t*)(PlayerAppearanceDataNode + 0x410) = Player.m_NetworkModelHash;
					}
				}
			} else LOG_DEV("FAILED 3");
		} else LOG_DEV("FAILED 2");
	} else LOG_DEV("FAILED 1");

	return OriginalSyncPlayerModel(PlayerAppearanceDataNode);
}