#include "network_global_chat_targets.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_global_chat.hpp"
#include "menu/base/util/timers.hpp"

using namespace NetworkGlobalChatTargetsMenuVars;

namespace NetworkGlobalChatTargetsMenuVars {
	Vars_ m_Vars;
}

void NetworkGlobalChatTargetsMenu::Init() {
	SetName("Targets");
	SetParentSubmenu<NetworkGlobalChatMenu>();

	addOption(ToggleOption("Toggle Targets")
		.addTranslation()
		.addToggle(m_Vars.m_Toggle));
}

void NetworkGlobalChatTargetsMenu::Update() {}

/*Called once on submenu open*/
void NetworkGlobalChatTargetsMenu::UpdateOnce() {
	static int Timer = 0;
	Menu::Timers::RunTimedFunction(&Timer, 250, [this] {
		ClearOptionsOffset(1);

		Menu::GetPlayerManager()->Foreach([this] (Menu::PlayerVars& Player) {
			if (Player.m_Connected) {
				addOption(ToggleOption(Player.m_Name)
					.addOnUpdate([&] (ToggleOption* Option) { Option->addToggle(Player.m_TargetedGlobalChat); }));
			}
		}, false);
	});
}

/*Called always*/
void NetworkGlobalChatTargetsMenu::FeatureUpdate() {}

NetworkGlobalChatTargetsMenu* _instance;
NetworkGlobalChatTargetsMenu* NetworkGlobalChatTargetsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkGlobalChatTargetsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkGlobalChatTargetsMenu::~NetworkGlobalChatTargetsMenu() { delete _instance; }