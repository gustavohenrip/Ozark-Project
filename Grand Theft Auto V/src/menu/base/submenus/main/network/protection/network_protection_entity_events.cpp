#include "network_protection_entity_events.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_protection.hpp"
#include "network_protection_entity_limits.hpp"
#include "network_protection_entity_blocking.hpp"
#include "network_protection_ghost_mode.hpp"

using namespace NetworkProtectionEntityEventsMenuVars;

namespace NetworkProtectionEntityEventsMenuVars {
	Vars_ m_Vars;
}

void NetworkProtectionEntityEventsMenu::Init() {
	SetName("Entity Events");
	SetParentSubmenu<NetworkProtectionMenu>();

	addOption(SubmenuOption("Entity Limits")
		.addTranslation()
		.addSubmenu<NetworkProtectionEntityLimitsMenu>());

	addOption(SubmenuOption("Entity Blocking")
		.addTranslation()
		.addSubmenu<NetworkProtectionEntityBlockingMenu>());

	addOption(SubmenuOption("Ghost Mode")
		.addTranslation()
		.addSubmenu<NetworkProtectionGhostModeMenu>()
		.addTooltip("Stops creating entities you spawn on target games"));

	addOption(ToggleOption("Attachment Event")
		.addTranslation()
		.addToggle(m_Vars.m_Attachments).canBeSaved(m_ParentNameStack)
		.addTooltip("Block attachments"));

	addOption(ToggleOption("Disable Modder Sync")
		.addTranslation()
		.addToggle(m_Vars.m_DisableModderSync).canBeSaved(m_ParentNameStack)
		.addTooltip("Automatically stop syncing entities from modders in the session"));

	addOption(ToggleOption("Disable Modder Create")
		.addTranslation()
		.addToggle(m_Vars.m_DisableModderCreate).canBeSaved(m_ParentNameStack)
		.addTooltip("Automatically stop creating entities from modders in the session"));
}

void NetworkProtectionEntityEventsMenu::Update() {}

/*Called once on submenu open*/
void NetworkProtectionEntityEventsMenu::UpdateOnce() {}

/*Called always*/
void NetworkProtectionEntityEventsMenu::FeatureUpdate() {}

NetworkProtectionEntityEventsMenu* _instance;
NetworkProtectionEntityEventsMenu* NetworkProtectionEntityEventsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkProtectionEntityEventsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkProtectionEntityEventsMenu::~NetworkProtectionEntityEventsMenu() { delete _instance; }