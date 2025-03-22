#include "network_session.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network.hpp"
#include "network_overseer.hpp"
#include "menu/base/util/panels.hpp"
#include "players/network_players_abusive.hpp"
#include "players/network_players_friendly.hpp"
#include "players/network_players_trolling.hpp"
#include "players/network_players_teleport.hpp"
#include "players/network_players_chat_commands.hpp"
#include "players/network_players_text.hpp"
#include "browser/network_browser.hpp"
#include "players/network_players_abusive.hpp"
#include "players/network_players_removal.hpp"

using namespace NetworkSessionMenuVars;

namespace NetworkSessionMenuVars {
	Vars_ m_Vars;
}

void NetworkSessionMenu::Init() {
	SetName("Session");
	SetParentSubmenu<NetworkMenu>();

	addOption(SubmenuOption("Friendly")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkPlayersFriendlyMenu>());

	addOption(SubmenuOption("Teleport")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkPlayersTeleportMenu>());

	addOption(SubmenuOption("Abusive")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkPlayersAbusiveMenu>());

	addOption(SubmenuOption("Trolling")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkPlayersTrollingMenu>());

	addOption(SubmenuOption("Removals")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkPlayersRemovalMenu>());

	addOption(SubmenuOption("ESP")
		.addTranslation()
		.addSubmenu<ESPMenu>()
		.addOnClick([] {
			ESPMenu::GetInstance()->SetParentSubmenu<NetworkSessionMenu>();
			ESPMenuVars::m_Vars.m_ESP = &m_Vars.m_ESP;
			ESPMenuVars::m_Vars.m_Player = true;
		}));

	addOption(SubmenuOption("Chat Commands")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkPlayersChatCommandsMenu>());

	addOption(SubmenuOption("Text Message")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkPlayersTextMenu>());

	addOption(SubmenuOption("Overseer")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkOverseerMenu>());

	addOption(SubmenuOption("Browser")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkBrowserMenu>()
		.addOnClick([] { Menu::Panels::GetPanelManager()->TogglePanelRender("PANEL_SESSION", false); }));
}

void NetworkSessionMenu::Update() {}

/*Called once on submenu open*/
void NetworkSessionMenu::UpdateOnce() {
	Global::Vars::g_NetworkMenuType = ALL_PLAYERS;
	Menu::Panels::GetPanelManager()->TogglePanelRender("PANEL_OVERSEER", false);
	Menu::Panels::GetPanelManager()->TogglePanelRender("PANEL_SESSION", true);
	Menu::Panels::GetPanelManager()->TogglePanelRender("PANEL_SESSION_BROWSER", false);
}

/*Called always*/
void NetworkSessionMenu::FeatureUpdate() {
	if (m_Vars.m_ESP.m_NameLineOfSightRainbowToggle) {
		m_Vars.m_ESP.m_NameLineOfSightRainbow.Run();
	}

	if (m_Vars.m_ESP.m_NameRainbowToggle) {
		m_Vars.m_ESP.m_NameRainbow.Run();
	}

	if (m_Vars.m_ESP.m_Name) {
		Menu::GetPlayerManager()->Foreach([&](Menu::PlayerVars& Player) {
			ESPMenuVars::NameESP(m_Vars.m_ESP, Player.m_Entity, Player.m_Name);
		});
	}

	// Snapline ESP
	if (m_Vars.m_ESP.m_SnaplineLineOfSightRainbowToggle) {
		m_Vars.m_ESP.m_SnaplineLineOfSightRainbow.Run();
	}

	if (m_Vars.m_ESP.m_SnaplineRainbowToggle) {
		m_Vars.m_ESP.m_SnaplineRainbow.Run();
	}

	if (m_Vars.m_ESP.m_Snapline) {
		Menu::GetPlayerManager()->Foreach([&](Menu::PlayerVars& Player) {
			ESPMenuVars::SnaplineESP(m_Vars.m_ESP, Player.m_Entity);
		});
	}

	// 3D ESP
	if (m_Vars.m_ESP.m_3DLineOfSightRainbowToggle) {
		m_Vars.m_ESP.m_3DLineOfSightRainbow.Run();
	}

	if (m_Vars.m_ESP.m_3DRainbowToggle) {
		m_Vars.m_ESP.m_3DRainbow.Run();
	}

	if (m_Vars.m_ESP.m_3D) {
		Menu::GetPlayerManager()->Foreach([&](Menu::PlayerVars& Player) {
			ESPMenuVars::_3DESP(m_Vars.m_ESP, Player.m_Entity);
		});
	}

}

NetworkSessionMenu* _instance;
NetworkSessionMenu* NetworkSessionMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkSessionMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkSessionMenu::~NetworkSessionMenu() { delete _instance; }