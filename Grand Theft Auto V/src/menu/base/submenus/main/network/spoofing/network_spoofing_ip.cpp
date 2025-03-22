#include "network_spoofing_ip.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_spoofing.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/utils.hpp"
#include "utils/va.hpp"
#include "rage/engine.hpp"

using namespace NetworkSpoofingIPMenuVars;

namespace NetworkSpoofingIPMenuVars {
	Vars_ m_Vars;

	void SpoofIP(NetworkSpoofingIPMenu* This, std::string ip, bool def) {
		if (def) {
			auto Split = Utils::Split(m_Vars.m_DefaultIP + ".", ".");
			if (Split.size() == 4) {
				Utils::GetConfig()->WriteString(This->m_ParentNameStack, "IP", m_Vars.m_DefaultIP.c_str());
			} else {
				Menu::GetNotify()->NotifyStacked(NetworkSpoofingIPMenu::GetInstance()->getString("Invalid default IP"), Global::UIVars::g_NotificationError);
			}

			return;
		}

		auto Split = Utils::Split(ip + ".", ".");
		if (Split.size() == 4) {
			m_Vars.m_SpoofingIP = ip;
			if (m_Vars.m_ConfigSaveIP) {
				Utils::GetConfig()->WriteString(This->m_ParentNameStack, "IP", ip.c_str());
			}

			Menu::GetNotify()->NotifyStacked(NetworkSpoofingIPMenu::GetInstance()->getString("Spoofing IP to") + " " + ip);
		} else {
			Menu::GetNotify()->NotifyStacked(NetworkSpoofingIPMenu::GetInstance()->getString("Invalid IP format (has to be xxx.xxx.xxx.xxx)"), Global::UIVars::g_NotificationError);
		}
	}
}

void NetworkSpoofingIPMenu::Init() {
	SetName("IP Address");
	SetParentSubmenu<NetworkSpoofingMenu>();

	addString("Invalid default IP");
	addString("Spoofing IP to");
	addString("Invalid IP format (has to be xxx.xxx.xxx.xxx)");

	addOption(ButtonOption("Input IP")
		.addTranslation()
		.addKeyboard("Enter IP", 15, [this] (const char* Name) {
			std::string Str = Name + std::string(".");

			auto Split = Utils::Split(Str, ".");
			if (Split.size() == 4) {
				m_Vars.m_TempSpoofingIP = Name;
			} else {
				Menu::GetNotify()->NotifyStacked(getString("Invalid IP format (has to be xxx.xxx.xxx.xxx)"), Global::UIVars::g_NotificationError);
			}
		}));

	addOption(ButtonOption("Reset to Default")
		.addTranslation()
		.addSprite("Ozark", "spinner")
		.addSpriteScale(0.01232f, 0.02184f)
		.addSpriteRotate()
		.addOnClick([this] { SpoofIP(this, m_Vars.m_DefaultIP); }));

	addOption(ToggleOption("Save to Config")
		.addTranslation()
		.addToggle(m_Vars.m_ConfigSaveIP).canBeSaved(m_ParentNameStack)
		.addTooltip("Saves to the config when applied"));

	m_Vars.m_ApplyButton = addOption(ButtonOption("")
		.addOnClick([this] { SpoofIP(this, m_Vars.m_TempSpoofingIP); }));

	if (m_Vars.m_ConfigSaveIP) {
		m_Vars.m_SpoofingIP = m_Vars.m_TempSpoofingIP = Utils::GetConfig()->ReadString(m_ParentNameStack, "IP", m_Vars.m_SpoofingIP.c_str());
		SpoofIP(this, m_Vars.m_SpoofingIP);
	}
}

void NetworkSpoofingIPMenu::Update() {
	snprintf(m_Vars.m_PreviewName, 50, "Apply: %s", m_Vars.m_TempSpoofingIP.c_str());
	m_Vars.m_ApplyButton->setName(m_Vars.m_PreviewName);
}

/*Called once on submenu open*/
void NetworkSpoofingIPMenu::UpdateOnce() {}

/*Called always*/
void NetworkSpoofingIPMenu::FeatureUpdate() {
	if (m_Vars.m_DefaultIP.empty()) {
		if (Menu::GetLocalPlayer().m_NetGamePlayer) {
			if (Menu::GetLocalPlayer().m_NetGamePlayer->m_PlayerInfo) {
				uint8_t* IP = (uint8_t*)&Menu::GetLocalPlayer().m_NetGamePlayer->m_PlayerInfo->m_ExternalIP;
				m_Vars.m_DefaultIP = Utils::VA::VA("%i.%i.%i.%i", IP[3], IP[2], IP[1], IP[0]);

				if (m_Vars.m_TempSpoofingIP.empty()) {
					m_Vars.m_TempSpoofingIP = m_Vars.m_DefaultIP;
				}
			}
		}
	}
}

NetworkSpoofingIPMenu* _instance;
NetworkSpoofingIPMenu* NetworkSpoofingIPMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkSpoofingIPMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkSpoofingIPMenu::~NetworkSpoofingIPMenu() { delete _instance; }