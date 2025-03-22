#include "network_protection_event_limits.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_protection.hpp"
#include "global/arrays.hpp"

using namespace NetworkProtectionEventLimitsMenuVars;

namespace NetworkProtectionEventLimitsMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> BlockMessages[] = {
		{ TranslationString("Spoofed", true), 0, },
		{ TranslationString("All", true), 1 }
	};

}

void NetworkProtectionEventLimitsMenu::Init() {
	SetName("Event Limits");
	SetParentSubmenu<NetworkProtectionMenu>();

	addOption(ScrollOption<int>(SCROLL, "Text Messages")
		.addTranslation()
		.addScroll(m_Vars.m_TextMessageDelay, 0, NUMOF(Global::Arrays::g_Timers), Global::Arrays::g_Timers));

	addOption(ScrollOption<int>(SCROLL, "Global Chat Messages")
		.addTranslation()
		.addScroll(m_Vars.m_GlobalChatMessageDelay, 0, NUMOF(Global::Arrays::g_Timers), Global::Arrays::g_Timers));

	addOption(ScrollOption<int>(TOGGLE, "Block Text Messages")
		.addTranslation()
		.addToggle(m_Vars.m_BlockSpoofedTextMessages)
		.addScroll(m_Vars.m_BlockSpoofedTextMessagesID, 0, NUMOF(BlockMessages), BlockMessages).canBeSaved(m_ParentNameStack));

	//addOption(ScrollOption<int>(TOGGLE, "Block Chat Messages")
	//	.addTranslation()
	//	.addToggle(m_Vars.m_BlockSpoofedChatMessages)
	//	.addScroll(m_Vars.m_BlockSpoofedChatMessagesID, 0, NUMOF(BlockMessages), BlockMessages));
}

void NetworkProtectionEventLimitsMenu::Update() {}

/*Called once on submenu open*/
void NetworkProtectionEventLimitsMenu::UpdateOnce() {}

/*Called always*/
void NetworkProtectionEventLimitsMenu::FeatureUpdate() {}

NetworkProtectionEventLimitsMenu* _instance;
NetworkProtectionEventLimitsMenu* NetworkProtectionEventLimitsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkProtectionEventLimitsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkProtectionEventLimitsMenu::~NetworkProtectionEventLimitsMenu() { delete _instance; }