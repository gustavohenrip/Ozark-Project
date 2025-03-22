#include "network_protection.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network.hpp"
#include "protection/network_protection_game_events.hpp"
#include "protection/network_protection_net_events.hpp"
#include "protection/network_protection_presence_events.hpp"
#include "protection/network_protection_join_events.hpp"
#include "protection/network_protection_event_limits.hpp"
#include "protection/network_protection_entity_events.hpp"
#include "protection/network_protection_notifications.hpp"
#include "protection/network_protection_flags.hpp"
#include "protection/network_protection_event_reactions.hpp"
#include "global/arrays.hpp"

using namespace NetworkProtectionMenuVars;

namespace NetworkProtectionMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> ScriptEventLimits[] = {
		{ TranslationString("Per Event", true), 0 },
		{ TranslationString("All Events", true), 1 }
	};
}

void NetworkProtectionMenu::Init() {
	SetName("Protection");
	SetParentSubmenu<NetworkMenu>();

	// TODO: Fake Lag

	addOption(SubmenuOption("Event Reactions")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkProtectionEventReactionsMenu>());

	addOption(SubmenuOption("Network Events")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkProtectionNetEventsMenu>());

	addOption(SubmenuOption("Game Events")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkProtectionGameEventsMenu>());

	addOption(SubmenuOption("Presence Events")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkProtectionPresenceEventsMenu>());

	addOption(SubmenuOption("Join Events")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkProtectionJoinEventsMenu>()
		.addTooltip("Block Rockstar ID joiners resolving you, or give it another players info instead"));

	addOption(SubmenuOption("Entity Events")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkProtectionEntityEventsMenu>());

	addOption(SubmenuOption("Event Limits")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkProtectionEventLimitsMenu>());

	addOption(SubmenuOption("Flags")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkProtectionFlagsMenu>()
		.addTooltip("Protect against godmode/super jump etc detection"));

	addOption(SubmenuOption("Notifications")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkProtectionNotificationsMenu>()
		.addTooltip("Protections that only have notifies, non customisable"));
}

void NetworkProtectionMenu::Update() {}

/*Called once on submenu open*/
void NetworkProtectionMenu::UpdateOnce() {}

/*Called always*/
void NetworkProtectionMenu::FeatureUpdate() {}

NetworkProtectionMenu* _instance;
NetworkProtectionMenu* NetworkProtectionMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkProtectionMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkProtectionMenu::~NetworkProtectionMenu() { delete _instance; }