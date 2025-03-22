#include "videos.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../settings.hpp"
#include "global/ui_vars.hpp"
#include "utils/va.hpp"

using namespace VideosMenuVars;

namespace VideosMenuVars {
	Vars_ m_Vars;

	void HideNames() {
		Menu::GetPlayerManager()->Foreach([=](Menu::PlayerVars& Player) {
			if (Player.m_ID != Menu::GetLocalPlayer().m_ID) {
				if (Player.m_NetGamePlayer) {
					if (Player.m_NetGamePlayer->m_PlayerInfo) {
						m_Vars.m_CachedData[Player.m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID] = Player.m_NetGamePlayer->m_PlayerInfo->m_Name;
						strcpy(Player.m_NetGamePlayer->m_PlayerInfo->m_Name, Utils::VA::VA("Player_%i", Player.m_NetGamePlayer->m_ID));
					}
				}
			}
		}, true);
	}

	void RestorNames() {
		Menu::GetPlayerManager()->Foreach([=](Menu::PlayerVars& Player) {
			if (Player.m_ID != Menu::GetLocalPlayer().m_ID) {
				if (Player.m_NetGamePlayer) {
					if (Player.m_NetGamePlayer->m_PlayerInfo) {
						strcpy(Player.m_NetGamePlayer->m_PlayerInfo->m_Name, Utils::VA::VA("%s", m_Vars.m_CachedData[Player.m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID].c_str()));
					}
				}
			}
		}, true);
		m_Vars.m_CachedData.clear();
	}

}

void VideosMenu::Init() {
	SetName("Videos");
	SetParentSubmenu<SettingsMenu>();

	addOption(ToggleOption("Hide IPs")
		.addTranslation()
		.addToggle(m_Vars.m_HideIps));

	addOption(ToggleOption("Hide RIDs")
		.addTranslation()
		.addToggle(m_Vars.m_HideRockstarIDs));

	addOption(ToggleOption("Hide Me")
		.addTranslation()
		.addToggle(m_Vars.m_HideMe));

	addOption(ToggleOption("Hide Players")
		.addTranslation()
		.addToggle(m_Vars.m_HideNames)
		.addOnClick([] {
			if (m_Vars.m_HideNames) {
				HideNames();
			}
			else {
				RestorNames();
			}
			
		}));

}

void VideosMenu::Update() {}

/*Called once on submenu open*/
void VideosMenu::UpdateOnce() {}

/*Called always*/
void VideosMenu::FeatureUpdate() {}

VideosMenu* _instance;
VideosMenu* VideosMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VideosMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VideosMenu::~VideosMenu() { delete _instance; }