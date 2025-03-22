#include "hooks.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_notifications.hpp"
#include "menu/base/util/notify.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/helpers.hpp"

TranslationString T_HostBlacklisting("Host Blacklisting", true, true);

bool Menu::Hooks::ReadBlacklistMessageHook(uint64_t Rcx, uint64_t Rdx) {
	bool Return = OriginalReadBlacklistMessage(Rcx, Rdx);

	if (NetworkProtectionNotificationsMenuVars::m_Vars.m_HostKicks) {
		uint64_t RockstarID = *(uint64_t*)(Rcx + 8);
		if (RockstarID) {
			Menu::PlayerVars& Player = Menu::Helpers::GetPlayerFromRID(RockstarID);
			if (Player.m_Connected && Player.IsValidName()) {
				Menu::GetNotify()->NotifyStackedProtectionEvent(Rage::Engine::GetHostNetGamePlayer()->m_PlayerInfo->m_Name, std::string(TRANSLATE(T_HostBlacklisting)) + " " + std::string(Player.m_Name), false, false);
			}
		}
	}

	return Return;
}