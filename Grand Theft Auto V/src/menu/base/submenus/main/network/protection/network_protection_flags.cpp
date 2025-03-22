#include "network_protection_flags.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_protection.hpp"

using namespace NetworkProtectionFlagsMenuVars;

namespace NetworkProtectionFlagsMenuVars {
	Vars_ m_Vars;
}

void NetworkProtectionFlagsMenu::Init() {
	SetName("Flags");
	SetParentSubmenu<NetworkProtectionMenu>();

	addOption(ToggleOption("Godmode")
		.addTranslation()
		.addToggle(m_Vars.m_Godmode).canBeSaved(m_ParentNameStack)
		.addTooltip("Disables modders seeing you're in godmode"));

	addOption(ToggleOption("Super Jump")
		.addTranslation()
		.addToggle(m_Vars.m_SuperJump).canBeSaved(m_ParentNameStack)
		.addTooltip("Disables modders seeing you're using super jump"));

	addOption(ToggleOption("Health")
		.addTranslation()
		.addToggle(m_Vars.m_Health).canBeSaved(m_ParentNameStack)
		.addTooltip("Disables modders seeing you have abnormal health"));

	addOption(ToggleOption("Armor")
		.addTranslation()
		.addToggle(m_Vars.m_Armor).canBeSaved(m_ParentNameStack)
		.addTooltip("Disables modders seeing you have abnormal armor"));

	addOption(ToggleOption("Spectate")
		.addTranslation()
		.addToggle(m_Vars.m_Spectate).canBeSaved(m_ParentNameStack)
		.addTooltip("Disables modders seeing you're spectating someone"));
}

void NetworkProtectionFlagsMenu::Update() {}

/*Called once on submenu open*/
void NetworkProtectionFlagsMenu::UpdateOnce() {}

/*Called always*/
void NetworkProtectionFlagsMenu::FeatureUpdate() {}

NetworkProtectionFlagsMenu* _instance;
NetworkProtectionFlagsMenu* NetworkProtectionFlagsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkProtectionFlagsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkProtectionFlagsMenu::~NetworkProtectionFlagsMenu() { delete _instance; }