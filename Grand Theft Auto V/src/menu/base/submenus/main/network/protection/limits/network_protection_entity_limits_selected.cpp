#include "network_protection_entity_limits_selected.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_protection_entity_limits.hpp"
#include "global/arrays.hpp"
#include "utils/va.hpp"

using namespace NetworkProtectionEntityLimitsSelectedMenuVars;

namespace NetworkProtectionEntityLimitsSelectedMenuVars {
	Vars_ m_Vars;
}

void NetworkProtectionEntityLimitsSelectedMenu::Init() {
	SetName("Entity Limits");
	SetParentSubmenu<NetworkProtectionEntityLimitsMenu>();

	addString("Max Count");
	addString("Delay");
	addString("Enabled");
	addString("Comment: ");
	addString("Model: ");
	addString("Delay between processing the spawn of this model");
}

void NetworkProtectionEntityLimitsSelectedMenu::Update() {}

/*Called once on submenu open*/
void NetworkProtectionEntityLimitsSelectedMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	NetworkProtectionEntityLimitsMenuVars::EntityLimit& Limit = NetworkProtectionEntityLimitsMenuVars::m_Vars.m_Limits[m_Vars.m_Selected];

	char Comment[0x100];
	snprintf(Comment, 0x100, Limit.m_Comment);

	addOption(ButtonOption(getString("Model: ") + std::string(Utils::VA::VA("%X", Limit.m_Model))));

	addOption(ButtonOption(getString("Comment: ") + std::string(Comment)));

	addOption(ScrollOption<int>(SCROLL, getString("Delay Per Spawn"))
		.addScroll(Limit.m_Delay, 0, NUMOF(Global::Arrays::g_Timers), Global::Arrays::g_Timers)
		.addTooltip(getString("Delay between processing the spawn of this model")));

	addOption(ToggleOption(getString("Enabled"))
		.addToggle(Limit.m_Enabled));
}

/*Called always*/
void NetworkProtectionEntityLimitsSelectedMenu::FeatureUpdate() {}

NetworkProtectionEntityLimitsSelectedMenu* _instance;
NetworkProtectionEntityLimitsSelectedMenu* NetworkProtectionEntityLimitsSelectedMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkProtectionEntityLimitsSelectedMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkProtectionEntityLimitsSelectedMenu::~NetworkProtectionEntityLimitsSelectedMenu() { delete _instance; }