#include "network.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../main.hpp"
#include "network/network_spoofing.hpp"
#include "network/network_players.hpp"
#include "network/network_host_toolkit.hpp"
#include "network/network_protection.hpp"
#include "network/network_presence.hpp"
#include "network/network_recent_players.hpp"
#include "network/network_global_chat.hpp"
#include "network/network_friends.hpp"
#include "network/network_account.hpp"
#include "network/network_overseer.hpp"
#include "network/network_session.hpp"
#include "network/network_casino.hpp"
#include "menu/base/util/panels.hpp"

using namespace NetworkMenuVars;

namespace NetworkMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> Transitions[] = {
		{ TranslationString("Public", true), 0 },
		{ TranslationString("Crew", true), 12 },
		{ TranslationString("Friend", true), 9 },
		{ TranslationString("Leave Online", true), -1 }
	};
}

void NetworkMenu::Init() {
	SetName("Network");
	SetParentSubmenu<MainMenu>();

	addOption(SubmenuOption("Players")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkPlayersMenu>());

	addOption(SubmenuOption("Session")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkSessionMenu>());

	addOption(SubmenuOption("Protection")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkProtectionMenu>());

	addOption(SubmenuOption("Spoofing")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkSpoofingMenu>());

	addOption(SubmenuOption("Account")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkAccountMenu>());

	addOption(SubmenuOption("Presence")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkPresenceMenu>());

	addOption(SubmenuOption("Friends")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkFriendsMenu>());

	addOption(SubmenuOption("Recent Players")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkRecentPlayersMenu>());

	addOption(SubmenuOption("Global Chat")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkGlobalChatMenu>());

	addOption(SubmenuOption("Host Toolkit")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkHostToolkitMenu>());

	addOption(ScrollOption<int>(SCROLLSELECT, "Find New Session")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_Transition, 0, NUMOF(Transitions), Transitions)
		.addOnClick([] { NetworkHostToolkitMenuVars::TransitionToNewLobby(Transitions[m_Vars.m_Transition].m_Result); }));
}

void NetworkMenu::Update() {}

/*Called once on submenu open*/
void NetworkMenu::UpdateOnce() {
	Global::UIVars::g_InsideCasino = false;
	Global::UIVars::g_InsideCasinoLuckyWheel = false;
	Global::UIVars::g_InsideCasinoSlotMachines = false;
	Global::UIVars::g_InsideCasinoBlackjack = false;
	Global::UIVars::g_InsideCasinoBlackjackHigh = false;
	Global::UIVars::g_InsideCasinoPoker = false;
	Global::UIVars::g_InsideCasinoPokerHigh = false;
	Global::UIVars::g_InsideCasinoRoulette = false;
	Global::UIVars::g_InsideCasinoRouletteHigh = false;

	Menu::Panels::GetPanelManager()->TogglePanelRender("PANEL_PLAYER_INFO", false);
	Menu::Panels::GetPanelManager()->TogglePanelRender("PANEL_RECENT_PLAYER", false);
	Menu::Panels::GetPanelManager()->TogglePanelRender("PANEL_FRIEND", false);
	Menu::Panels::GetPanelManager()->TogglePanelRender("PANEL_OVERSEER", false);
	Menu::Panels::GetPanelManager()->TogglePanelRender("PANEL_SESSION", false);
}

/*Called always*/
void NetworkMenu::FeatureUpdate() {}

NetworkMenu* _instance;
NetworkMenu* NetworkMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkMenu::~NetworkMenu() { delete _instance; }