#include "network_text_preset.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_players_text.hpp"

using namespace NetworkTextPresetMenuVars;

namespace NetworkTextPresetMenuVars {
	Vars_ m_Vars;
}

void NetworkTextPresetMenu::Init() {
	SetName("Presets");
	SetParentSubmenu<NetworkPlayersTextMenu>();

	addOption(ButtonOption("IP Address")
		.addTranslation()
		.addOnClick([] {
			uint32_t IP = Menu::GetSelectedPlayer().m_NetGamePlayer->m_PlayerInfo->m_ExternalIP;

			char Buffer[16];
			sprintf(Buffer, "%i.%i.%i.%i", ((IP & 0xFF000000) >> 24), ((IP & 0xFF0000) >> 16), ((IP & 0xFF00) >> 8), (IP & 0xFF));
			Menu::GetSelectedPlayer().m_TextMessageMsg = std::string(Buffer);
			Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		}).addRequirement([] { return Global::Vars::g_NetworkMenuType == SELECTED_PLAYER; }));

	addOption(ButtonOption("Geo IP")
		.addTranslation()
		.addOnClick([] {
			uint32_t IP = Menu::GetSelectedPlayer().m_NetGamePlayer->m_PlayerInfo->m_ExternalIP;

			auto Vit = ::Global::Vars::g_Geo.find(IP);
			if (Vit != ::Global::Vars::g_Geo.end()) {
				Menu::GetSelectedPlayer().m_TextMessageMsg = Vit->second.m_City + ", " + Vit->second.m_RegionName;
				Menu::GetSubmenuHandler()->SetSubmenuPrevious();
			}

			Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		}).addRequirement([] { return Global::Vars::g_NetworkMenuType == SELECTED_PLAYER; }));

	addOption(ButtonOption("Advertisement")
		.addTranslation()
		.addOnClick([] {
			Menu::GetSelectedPlayer().m_TextMessageMsg = "Purchase ~f~Ozark~w~ at ozark.gg";
			Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		}));

	addOption(ButtonOption("Shit Cheat")
		.addTranslation()
		.addOnClick([] {
			Menu::GetSelectedPlayer().m_TextMessageMsg = "Cool cheats. Sike.";
			Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		}));

	addOption(ButtonOption("I boke up wif my X gurl")
		.addTranslation()
		.addOnClick([] {
			Menu::GetSelectedPlayer().m_TextMessageMsg = "I boke up wif my X gurl, heres her numba. SIKE. Dats the wrong nuMBA!!";
			Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		}));

	addOption(ButtonOption("Cool Stream")
		.addTranslation()
		.addOnClick([] {
			Menu::GetSelectedPlayer().m_TextMessageMsg = "Cool stream, you're killing it";
			Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		}));

	addOption(ButtonOption("Ozark #1")
		.addTranslation()
		.addOnClick([] {
			Menu::GetSelectedPlayer().m_TextMessageMsg = "~f~Ozark #1";
			Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		}));

	addOption(ButtonOption("Enjoy the Crash")
		.addTranslation()
		.addOnClick([] {
			Menu::GetSelectedPlayer().m_TextMessageMsg = "Enjoy the crash retard :)";
			Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		}));

	addOption(ButtonOption("Big Brain Time")
		.addTranslation()
		.addOnClick([] {
			Menu::GetSelectedPlayer().m_TextMessageMsg = "It's big brain time, kid";
			Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		}));
}

void NetworkTextPresetMenu::Update() {}

/*Called once on submenu open*/
void NetworkTextPresetMenu::UpdateOnce() {}

/*Called always*/
void NetworkTextPresetMenu::FeatureUpdate() {}

NetworkTextPresetMenu* _instance;
NetworkTextPresetMenu* NetworkTextPresetMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkTextPresetMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkTextPresetMenu::~NetworkTextPresetMenu() { delete _instance; }