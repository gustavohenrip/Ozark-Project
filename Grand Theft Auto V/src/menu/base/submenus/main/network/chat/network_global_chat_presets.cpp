#include "network_global_chat_presets.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_global_chat.hpp"

using namespace NetworkGlobalChatPresetsMenuVars;

namespace NetworkGlobalChatPresetsMenuVars {
	Vars_ m_Vars;

	std::string Presets[] = {
		"Get Ozark @ ozark.gg", "Money drop @ airport", "I love cock", "I'm retarded",
		"I don't get laid", "Fuck you", "Crash me if you can", "Spare some coochie sir",
		"~r~Color Test"
	};

	void Callback(std::string Str) {
		*m_Vars.m_Editing = Str;
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
	}
}

void NetworkGlobalChatPresetsMenu::Init() {
	SetName("Presets");
	SetParentSubmenu<NetworkGlobalChatMenu>();

	for (auto& P : Presets) {
		addOption(ButtonOption(P)
			.addTranslation()
			.addOnClick([=] { Callback(P); }));
	}
}

void NetworkGlobalChatPresetsMenu::Update() {}

/*Called once on submenu open*/
void NetworkGlobalChatPresetsMenu::UpdateOnce() {}

/*Called always*/
void NetworkGlobalChatPresetsMenu::FeatureUpdate() {}

NetworkGlobalChatPresetsMenu* _instance;
NetworkGlobalChatPresetsMenu* NetworkGlobalChatPresetsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkGlobalChatPresetsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkGlobalChatPresetsMenu::~NetworkGlobalChatPresetsMenu() { delete _instance; }