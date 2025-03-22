#include "network_protection_game_events.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_protection.hpp"
#include "network_protection_event_handler.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/util/helpers.hpp"

using namespace NetworkProtectionGameEventsMenuVars;

namespace NetworkProtectionGameEventsMenuVars {
	Vars_ m_Vars;
}

void NetworkProtectionGameEventsMenu::Init() {
	SetName("Game Events");
	SetParentSubmenu<NetworkProtectionMenu>();

	std::pair<std::string, Menu::Structs::ProtectionEventContext*> Items[] = {
		{ "Script Host Drop Kick", &m_Vars.m_ScriptHostDropKick },
		{ "Entity Owner Migration", &m_Vars.m_SpawnedEntityOwnerMigration },
		{ "Player Model Update", &m_Vars.m_PlayerModelUpdate },
		{ "Pickup Processing", &m_Vars.m_PickupProcessing },
		{ "Clone", &m_Vars.m_Clone },
		{ "Player Attachment", &m_Vars.m_PlayerAttachment }
	};

	for (auto& Item : Items) {
		addOption(SubmenuOption(Item.first)
			.addTranslation()
			.addSubmenu<NetworkProtectionEventHandlerMenu>()
			.addOnClick([=] {
				NetworkProtectionEventHandlerMenu::GetInstance()->SetName(Item.first);
				NetworkProtectionEventHandlerMenu::GetInstance()->SetParentSubmenu<NetworkProtectionGameEventsMenu>();
				NetworkProtectionEventHandlerMenuVars::m_Vars.m_Context = Item.second;
			})
			.addTooltip(Item.second->m_Tooltip));

		Item.second->m_Block = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Block", Item.second->m_Block, { Item.first });
		Item.second->m_Redirect = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Redirect", Item.second->m_Redirect, { Item.first });
		Item.second->m_Notify = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Notify", Item.second->m_Notify, { Item.first });
	}

	addOption(ToggleOption("Cage")
		.addTranslation()
		.addToggle(m_Vars.m_Cage).canBeSaved(m_ParentNameStack)
		.addTooltip("Prevents cages spawning"));
}

void NetworkProtectionGameEventsMenu::Update() {}

/*Called once on submenu open*/
void NetworkProtectionGameEventsMenu::UpdateOnce() {}

/*Called always*/
void NetworkProtectionGameEventsMenu::FeatureUpdate() {}

NetworkProtectionGameEventsMenu* _instance;
NetworkProtectionGameEventsMenu* NetworkProtectionGameEventsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkProtectionGameEventsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkProtectionGameEventsMenu::~NetworkProtectionGameEventsMenu() { delete _instance; }