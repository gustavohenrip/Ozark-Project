#include "network_protection_event_handler.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_protection.hpp"

using namespace NetworkProtectionEventHandlerMenuVars;

namespace NetworkProtectionEventHandlerMenuVars {
	Vars_ m_Vars;

	void SaveConfig(NetworkProtectionEventHandlerMenu* pThis) {
		Utils::GetConfig()->WriteBool(pThis->m_ParentNameStack, "Block", m_Vars.m_Context->m_Block);
		Utils::GetConfig()->WriteBool(pThis->m_ParentNameStack, "Redirect", m_Vars.m_Context->m_Redirect);
		Utils::GetConfig()->WriteBool(pThis->m_ParentNameStack, "Notify", m_Vars.m_Context->m_Notify);
	}
}

void NetworkProtectionEventHandlerMenu::Init() {
	SetName("Event Handler");
	SetParentSubmenu<NetworkProtectionMenu>();

	addString("Block Event");
	addString("Redirect Event");
	addString("Notify Event");
}

void NetworkProtectionEventHandlerMenu::Update() {}

/*Called once on submenu open*/
void NetworkProtectionEventHandlerMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	if (m_Vars.m_Context->m_CanBlock) {
		addOption(ToggleOption(getString("Block Event"))
			.addToggle(m_Vars.m_Context->m_Block)
			.addOnClick([this] { SaveConfig(this); }));
	}

	if (m_Vars.m_Context->m_CanRedirect) {
		addOption(ToggleOption(getString("Redirect Event"))
			.addToggle(m_Vars.m_Context->m_Redirect)
			.addOnClick([this] { SaveConfig(this); }));
	}

	if (m_Vars.m_Context->m_CanNotify) {
		addOption(ToggleOption(getString("Notify Event"))
			.addToggle(m_Vars.m_Context->m_Notify)
			.addOnClick([this] { SaveConfig(this); }));
	}
}

/*Called always*/
void NetworkProtectionEventHandlerMenu::FeatureUpdate() {}

NetworkProtectionEventHandlerMenu* _instance;
NetworkProtectionEventHandlerMenu* NetworkProtectionEventHandlerMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkProtectionEventHandlerMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkProtectionEventHandlerMenu::~NetworkProtectionEventHandlerMenu() { delete _instance; }