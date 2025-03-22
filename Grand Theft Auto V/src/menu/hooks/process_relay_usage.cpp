#include "hooks.hpp"
#include "menu/base/util/player_manager.hpp"
#include "utils/log.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/va.hpp"
#include "menu/base/util/helpers.hpp"
#include "utils/fiber_pool.hpp"
#include "utils/memory/memory.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_notifications.hpp"

TranslationString T_LegitRSDeveloper("is a legitimate R* developer. Be careful!", true, true);

void Menu::Hooks::ProcessRelayUsageHook(uint8_t Cl, uint64_t Rdx, Rage::Network::EventAddedGamer* Gamer, uint32_t R9d) {
	if (((Gamer->m_ExternalIP & 0xFF000000) >> 24) == 138) {
		if (((Gamer->m_ExternalIP & 0xFF0000) >> 16) == 201) {
			if (((Gamer->m_ExternalIP & 0xFF00) >> 8) == 70) {
				return OriginalProcessRelayUsage(Cl, Rdx, Gamer, R9d);
			}
		}
	}

	bool Insert = true;
	for (auto& Relay : Global::Vars::g_RelayUsage) {
		if (!strcmp(Relay.m_Name, Gamer->m_Name) && Relay.m_RockstarID == Gamer->m_RockstarID) {
			Relay.m_ExternalIP = Gamer->m_ExternalIP;
			Insert = false;
			break;
		}
	}

	if (Menu::Helpers::IsRockstarIP(Gamer->m_ExternalIP)) {
		if (NetworkProtectionNotificationsMenuVars::m_Vars.m_RockstarDeveloper) {
			Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s", Gamer->m_Name, TRANSLATE(T_LegitRSDeveloper)), Color::Red());
		}
	}

	if (Insert) {
		if (((Gamer->m_ExternalIP & 0xFF000000) >> 24) != 255 && ((Gamer->m_ExternalIP & 0xFF0000) >> 16) != 255) {
			Global::Vars::g_GeoQueue.push(Gamer->m_ExternalIP);
		}

		RelayUsage Relay;
		Relay.m_ExternalIP = Gamer->m_ExternalIP;
		Relay.m_RockstarID = Gamer->m_RockstarID;
		strcpy(Relay.m_Name, Gamer->m_Name);

		char Buffer[16];
		sprintf(Buffer, "%i.%i.%i.%i"_Protect, ((Gamer->m_ExternalIP & 0xFF000000) >> 24), ((Gamer->m_ExternalIP & 0xFF0000) >> 16), ((Gamer->m_ExternalIP & 0xFF00) >> 8), (Gamer->m_ExternalIP & 0xFF));

		Global::Vars::g_RelayUsage.push_back(Relay);
		LOG_INFO("[%s:%i] IP=%s"_Protect, Gamer->m_Name, Gamer->m_RockstarID, Buffer);
	}

	return OriginalProcessRelayUsage(Cl, Rdx, Gamer, R9d);
}