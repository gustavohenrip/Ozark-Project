#include "network_spoofing_rid.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_spoofing.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/log.hpp"

using namespace NetworkSpoofingRockstarIDMenuVars;

namespace NetworkSpoofingRockstarIDMenuVars {
	Vars_ m_Vars;

	void SpoofRockstarID(NetworkSpoofingRockstarIDMenu* This, uint64_t RockstarID, bool Def) {
		if (Def) {
			m_Vars.m_SpoofingRockstarID = Global::Vars::g_RsInfo->m_RockstarID;

			if (m_Vars.m_ConfigSaveRockstarID) {
				Utils::GetConfig()->WriteUInt64(This->m_ParentNameStack, "Rockstar ID", Global::Vars::g_RsInfo->m_RockstarID);
			}

			if (IsValidPtr(Menu::GetLocalPlayer().m_NetGamePlayer)) {
				if (IsValidPtr(Menu::GetLocalPlayer().m_NetGamePlayer->m_PlayerInfo)) {
					Menu::GetLocalPlayer().m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID = Global::Vars::g_RsInfo->m_RockstarID;
				}
			}

			return;
		}

		m_Vars.m_SpoofingRockstarID = RockstarID;

		Menu::GetNotify()->NotifyStacked(This->getString("Spoofing RID to") + " " + std::to_string(RockstarID));

		if (m_Vars.m_ConfigSaveRockstarID) {
			Utils::GetConfig()->WriteUInt64(This->m_ParentNameStack, "Rockstar ID", RockstarID);
		}

		if (IsValidPtr(Menu::GetLocalPlayer().m_NetGamePlayer)) {
			if (IsValidPtr(Menu::GetLocalPlayer().m_NetGamePlayer->m_PlayerInfo)) {
				Menu::GetLocalPlayer().m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID = RockstarID;
			}
		}
	}
}

void NetworkSpoofingRockstarIDMenu::Init() {
	SetName("Rockstar ID");
	SetParentSubmenu<NetworkSpoofingMenu>();

	addString("Spoofing RID to");
	addString("Setting to 0 crashes others, that's naughty!");

	m_Vars.m_SpoofingRockstarID = Global::Vars::g_RsInfo->m_RockstarID;
	LOG_DEV("RID: %i", m_Vars.m_SpoofingRockstarID);

	addOption(ButtonOption("Input Rockstar ID")
		.addTranslation()
		.addKeyboard("Enter Rockstar ID", 16, [this] (const char* Name) {
			m_Vars.m_TempSpoofingRockstarID = atoll(Name);
			if (m_Vars.m_TempSpoofingRockstarID == 0) {
				Menu::GetNotify()->NotifyStacked(getString("Setting to 0 crashes others, that's naughty!"));
			}
		}));

	addOption(ButtonOption("Reset to Default")
		.addTranslation()
		.addSprite("Ozark", "spinner")
		.addSpriteScale(0.01232f, 0.02184f)
		.addSpriteRotate().addOnClick([this] { SpoofRockstarID(this, 0, true); }));

	addOption(ToggleOption("Save to Config")
		.addTranslation()
		.addToggle(m_Vars.m_ConfigSaveRockstarID).canBeSaved(m_ParentNameStack)
		.addTooltip("Saves to the config when applied"));

	m_Vars.m_ApplyButton = addOption(ButtonOption("")
		.addOnClick([this] { SpoofRockstarID(this, m_Vars.m_TempSpoofingRockstarID); }));

	if (m_Vars.m_ConfigSaveRockstarID) {
		m_Vars.m_SpoofingRockstarID = m_Vars.m_TempSpoofingRockstarID = Utils::GetConfig()->ReadUInt64(m_ParentNameStack, "Rockstar ID", m_Vars.m_SpoofingRockstarID);
		SpoofRockstarID(this, m_Vars.m_SpoofingRockstarID);
	}
}

void NetworkSpoofingRockstarIDMenu::Update() {
	snprintf(m_Vars.m_PreviewName, 50, "Apply: %I64d", m_Vars.m_TempSpoofingRockstarID);
	m_Vars.m_ApplyButton->setName(m_Vars.m_PreviewName);

	/*if (m_Vars.m_TempSpoofingRockstarID == 0) {
		m_Vars.m_TempSpoofingRockstarID = Global::Vars::g_RsInfo->m_RockstarID;
	}

	if (m_Vars.m_SpoofingRockstarID == 0) {
		m_Vars.m_SpoofingRockstarID = Global::Vars::g_RsInfo->m_RockstarID;
	}*/
}

/*Called once on submenu open*/
void NetworkSpoofingRockstarIDMenu::UpdateOnce() {}

/*Called always*/
void NetworkSpoofingRockstarIDMenu::FeatureUpdate() {}

NetworkSpoofingRockstarIDMenu* _instance;
NetworkSpoofingRockstarIDMenu* NetworkSpoofingRockstarIDMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkSpoofingRockstarIDMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkSpoofingRockstarIDMenu::~NetworkSpoofingRockstarIDMenu() { delete _instance; }