#include "network_protection_presence_events.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_protection.hpp"
#include "network_protection_event_handler.hpp"

using namespace NetworkProtectionPresenceEventsMenuVars;

namespace NetworkProtectionPresenceEventsMenuVars {
	Vars_ m_Vars;
}

void NetworkProtectionPresenceEventsMenu::Init() {
	SetName("Presence Events");
	SetParentSubmenu<NetworkProtectionMenu>();

	std::pair<std::string, Menu::Structs::ProtectionEventContext*> Items[] = {
		{ "Text Message", &m_Vars.m_TextMessage },
		{ "Crew Message", &m_Vars.m_CrewMessage },
		// { "Game Invite", &m_Vars.m_GameInvite },
		{ "Game Server Rewards", &m_Vars.m_GameServerRewards },
		{ "Admin Invite", &m_Vars.m_AdminInvite },
		{ "Stat Update", &m_Vars.m_StatUpdate },
	};

	for (auto& Item : Items) {
		addOption(SubmenuOption(Item.first)
			.addTranslation()
			.addSubmenu<NetworkProtectionEventHandlerMenu>()
			.addOnClick([=] {
				NetworkProtectionEventHandlerMenu::GetInstance()->SetName(Item.first);
				NetworkProtectionEventHandlerMenu::GetInstance()->SetParentSubmenu<NetworkProtectionPresenceEventsMenu>();
				NetworkProtectionEventHandlerMenuVars::m_Vars.m_Context = Item.second;
			}));

		// Load
		Item.second->m_Block = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Block", Item.second->m_Block, { Item.first });
		Item.second->m_Redirect = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Redirect", Item.second->m_Redirect, { Item.first });
		Item.second->m_Notify = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Notify", Item.second->m_Notify, { Item.first });
	}
}

void NetworkProtectionPresenceEventsMenu::Update() {}

/*Called once on submenu open*/
void NetworkProtectionPresenceEventsMenu::UpdateOnce() {}

/*Called always*/
void NetworkProtectionPresenceEventsMenu::FeatureUpdate() {}

NetworkProtectionPresenceEventsMenu* _instance;
NetworkProtectionPresenceEventsMenu* NetworkProtectionPresenceEventsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkProtectionPresenceEventsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkProtectionPresenceEventsMenu::~NetworkProtectionPresenceEventsMenu() { delete _instance; }