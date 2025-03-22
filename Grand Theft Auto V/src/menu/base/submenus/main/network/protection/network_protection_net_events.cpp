#include "network_protection_net_events.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_protection.hpp"
#include "network_protection_event_handler.hpp"

using namespace NetworkProtectionNetEventsMenuVars;

namespace NetworkProtectionNetEventsMenuVars {
	Vars_ m_Vars;
}

void NetworkProtectionNetEventsMenu::Init() {
	SetName("Network Events");
	SetParentSubmenu<NetworkProtectionMenu>();

	std::pair<std::string, Menu::Structs::ProtectionEventContext*> Items[] = {
		{ "Give Weapon", &m_Vars.m_GiveWeapon },
		{ "Remove Weapon", &m_Vars.m_RemoveWeapon },
		{ "Remove All Weapons", &m_Vars.m_RemoveAllWeapons },
		{ "Freeze", &m_Vars.m_Freeze },
		{ "Explosion", &m_Vars.m_Explosion },
		{ "Fire", &m_Vars.m_Fire },
		{ "PTFX", &m_Vars.m_PTFX },
		{ "Script Events", &m_Vars.m_ScriptEvents },
		{ "Increment Stat", &m_Vars.m_IncrementStat },
		{ "Weather", &m_Vars.m_Weather },
		{ "Clock", &m_Vars.m_Clock },
		{ "Sound", &m_Vars.m_Sound },
		{ "Control", &m_Vars.m_Control },
	};

	for (auto& Item : Items) {
		addOption(SubmenuOption(Item.first)
			.addTranslation()
			.addSubmenu<NetworkProtectionEventHandlerMenu>()
			.addOnClick([=] {
				NetworkProtectionEventHandlerMenu::GetInstance()->SetName(Item.first);
				NetworkProtectionEventHandlerMenu::GetInstance()->SetParentSubmenu<NetworkProtectionNetEventsMenu>();
				NetworkProtectionEventHandlerMenuVars::m_Vars.m_Context = Item.second;
			}));

		// Load
		Item.second->m_Block = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Block", Item.second->m_Block, { Item.first });
		Item.second->m_Redirect = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Redirect", Item.second->m_Redirect, { Item.first });
		Item.second->m_Notify = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Notify", Item.second->m_Notify, { Item.first });
	}

	addOption(ToggleOption("Blame Explosion")
		.addTranslation()
		.addToggle(m_Vars.m_Blame).canBeSaved(m_ParentNameStack)
		.addTooltip("Notification for when someone blames someone for an explosion"));
}

void NetworkProtectionNetEventsMenu::Update() {}

/*Called once on submenu open*/
void NetworkProtectionNetEventsMenu::UpdateOnce() {}

/*Called always*/
void NetworkProtectionNetEventsMenu::FeatureUpdate() {}

NetworkProtectionNetEventsMenu* _instance;
NetworkProtectionNetEventsMenu* NetworkProtectionNetEventsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkProtectionNetEventsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkProtectionNetEventsMenu::~NetworkProtectionNetEventsMenu() { delete _instance; }