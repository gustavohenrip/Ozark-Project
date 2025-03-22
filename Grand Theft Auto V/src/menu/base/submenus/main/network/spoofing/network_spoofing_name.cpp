#include "network_spoofing_name.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_spoofing.hpp"
#include "rage/engine.hpp"
#include "../players/network_players_text.hpp"
#include "utils/utils.hpp"
#include "menu/base/util/notify.hpp"

using namespace NetworkSpoofingNameMenuVars;

namespace NetworkSpoofingNameMenuVars {
	Vars_ m_Vars;

	ScrollStruct<const char*> TextSettings[] = {
		{ TranslationString("Normal", true), "" },
		{ TranslationString("Bold", true), "~h~" },
		{ TranslationString("Italic", true), "~italic~" },
	};

	void Spoof(NetworkSpoofingNameMenu* This, bool Reset, const char* Name) {
		if (Reset) {
			m_Vars.m_SpoofedName = std::string(m_Vars.m_OriginalName);

			if (Native::NetworkIsInSession()) {
				auto Net = Menu::GetLocalPlayer().m_NetGamePlayer;
				if (Net) {
					if (Net->m_PlayerInfo) {
						strncpy(Net->m_PlayerInfo->m_Name, m_Vars.m_SpoofedName.c_str(), 20);
					}
				}
			}

			if (m_Vars.m_SaveToConfig) {
				Utils::GetConfig()->WriteString(This->m_ParentNameStack, "Name", m_Vars.m_SpoofedName.c_str());
			}

			if (IsValidPtr(Menu::GetLocalPlayer().m_NetGamePlayer)) {
				if (IsValidPtr(Menu::GetLocalPlayer().m_NetGamePlayer->m_PlayerInfo)) {
					strcpy(Menu::GetLocalPlayer().m_NetGamePlayer->m_PlayerInfo->m_Name, m_Vars.m_SpoofedName.c_str());
				}
			}

			uint64_t ChatInfo = *(uint64_t*)Global::Vars::g_ChatInfo;
			if (ChatInfo) {
				strcpy((char*)(ChatInfo + 0x84), m_Vars.m_SpoofedName.c_str());
			}

			return;
		}

		if (Name) {
			char Tmp[32];
			snprintf(Tmp, 32, "%s%s%s", TextSettings[m_Vars.m_Style].m_Result, NetworkPlayersTextMenuVars::TextColors[m_Vars.m_Color].m_Result, Name);
			m_Vars.m_SpoofedName = std::string(Tmp);

			if (Native::NetworkIsInSession()) {
				auto Net = Menu::GetLocalPlayer().m_NetGamePlayer;
				if (Net) {
					if (Net->m_PlayerInfo) {
						strncpy(Net->m_PlayerInfo->m_Name, m_Vars.m_SpoofedName.c_str(), 20);
					}
				}
			}

			Menu::GetNotify()->NotifyStacked(This->getString("Spoofing name to") + " " + m_Vars.m_SpoofedName);

			if (m_Vars.m_SaveToConfig) {
				Utils::GetConfig()->WriteString(This->m_ParentNameStack, "Name", m_Vars.m_SpoofedName.c_str());
			}

			if (IsValidPtr(Menu::GetLocalPlayer().m_NetGamePlayer)) {
				if (IsValidPtr(Menu::GetLocalPlayer().m_NetGamePlayer->m_PlayerInfo)) {
					strcpy(Menu::GetLocalPlayer().m_NetGamePlayer->m_PlayerInfo->m_Name, m_Vars.m_SpoofedName.c_str());
				}
			}

			uint64_t ChatInfo = *(uint64_t*)Global::Vars::g_ChatInfo;
			if (ChatInfo) {
				strcpy((char*)(ChatInfo + 0x84), m_Vars.m_SpoofedName.c_str());
			}
		}
	}
}

void NetworkSpoofingNameMenu::Init() {
	SetName("Name");
	SetParentSubmenu<NetworkSpoofingMenu>();

	addString("Spoofing name to");

	auto Name = Rage::Engine::SocialClubGetGameInfoString("gtag");
	if (Name) {
		strncpy(m_Vars.m_OriginalName, Name, 20);
		m_Vars.m_SpoofedName = m_Vars.m_OriginalName;
		m_Vars.m_TempName = m_Vars.m_OriginalName;
	}

	addOption(ButtonOption("Input Name")
		.addTranslation()
		.addKeyboard("Enter name", 20, [] (const char* Name) { m_Vars.m_TempName = std::string(Name); }));

	addOption(ButtonOption("Reset to Default")
		.addTranslation()
		.addSprite("Ozark", "spinner")
		.addSpriteScale(0.01232f, 0.02184f)
		.addSpriteRotate()
		.addOnClick([this] { Spoof(this, true); }));

	addOption(ToggleOption("Save to Config")
		.addTranslation()
		.addToggle(m_Vars.m_SaveToConfig).canBeSaved(m_ParentNameStack)
		.addTooltip("Saves to the config when applied"));

	addOption(BreakOption("Text Settings")
		.addTranslation());

	addOption(ScrollOption<const char*>(SCROLL, "Color")
		.addTranslation()
		.addScroll(m_Vars.m_Color, 0, NUMOF(NetworkPlayersTextMenuVars::TextColors), NetworkPlayersTextMenuVars::TextColors).canBeSaved(m_ParentNameStack));

	addOption(ScrollOption<const char*>(SCROLL, "Style")
		.addTranslation()
		.addScroll(m_Vars.m_Style, 0, NUMOF(TextSettings), TextSettings).canBeSaved(m_ParentNameStack));

	m_Vars.m_PreviewButton = addOption(ButtonOption("")
		.addOnClick([this] { Spoof(this, false, m_Vars.m_TempName.c_str()); }));

	if (m_Vars.m_SaveToConfig) {
		m_Vars.m_SpoofedName = Utils::GetConfig()->ReadString(m_ParentNameStack, "Name", m_Vars.m_SpoofedName.c_str());

		for (int i = 1; i < NUMOF(TextSettings); i++) {
			if (strstr(m_Vars.m_SpoofedName.c_str(), TextSettings[i].m_Result)) {
				m_Vars.m_Style = i;
				Utils::ReplaceString(m_Vars.m_SpoofedName, TextSettings[i].m_Result, "");
				break;
			}
		}

		for (int i = 1; i < NUMOF(NetworkPlayersTextMenuVars::TextColors); i++) {
			if (strstr(m_Vars.m_SpoofedName.c_str(), NetworkPlayersTextMenuVars::TextColors[i].m_Result)) {
				m_Vars.m_Color = i;
				Utils::ReplaceString(m_Vars.m_SpoofedName, NetworkPlayersTextMenuVars::TextColors[i].m_Result, "");
				break;
			}
		}

		m_Vars.m_TempName = m_Vars.m_SpoofedName;
		Spoof(this, false, m_Vars.m_TempName.c_str());
	}
}

void NetworkSpoofingNameMenu::Update() {
	snprintf(m_Vars.m_PreviewName, 50, "Apply: %s%s%s", TextSettings[m_Vars.m_Style].m_Result, NetworkPlayersTextMenuVars::TextColors[m_Vars.m_Color].m_Result, m_Vars.m_TempName.c_str());
	m_Vars.m_PreviewButton->setName(m_Vars.m_PreviewName);
}

/*Called once on submenu open*/
void NetworkSpoofingNameMenu::UpdateOnce() {}

/*Called always*/
void NetworkSpoofingNameMenu::FeatureUpdate() {}

NetworkSpoofingNameMenu* _instance;
NetworkSpoofingNameMenu* NetworkSpoofingNameMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkSpoofingNameMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkSpoofingNameMenu::~NetworkSpoofingNameMenu() { delete _instance; }