#include "network_protection_join_events.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_protection.hpp"
#include "utils/va.hpp"
#include "rage/api/social.hpp"
#include "menu/base/util/notify.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/timers.hpp"

using namespace NetworkProtectionJoinEventsMenuVars;

namespace NetworkProtectionJoinEventsMenuVars {
	Vars_ m_Vars;

	void GetPlayerGs() {
		m_Vars.m_GsInfo.clear();
		m_Vars.m_PendingGsInfoRequestID = m_Vars.m_Target;
		m_Vars.m_PendingGsInfoRequest = true;
		m_Vars.m_PendingTimeout = GetTickCount() + 10000;

		Rage::Network::RockstarIdentifier* Identifier = new Rage::Network::RockstarIdentifier();
		Identifier->m_RockstarID = m_Vars.m_Target;
		Identifier->m_Type = 3;

		// call into get info
		Native::ClearGamerTaskData();
		Rage::Engine::SendSessionInfoRequest(Identifier, 1);
		delete Identifier;
	}

	void InputPlayerCallback(const char* Input) {
		std::string StringName(Input);
		if (std::find_if(StringName.begin(), StringName.end(), [] (unsigned char c) { return !std::isdigit(c); }) == StringName.end()) {
			// RID
			m_Vars.m_Target = atoll(StringName.c_str());
			GetPlayerGs();
		} else {
			Rage::API::GetSocialClub()->GetProfileFromName(StringName, [] (bool Success, Rage::API::RockstarAccount Account) {
				if (Success) {
					m_Vars.m_Target = Account.m_RockstarID;
					GetPlayerGs();
				} else {
					Menu::GetNotify()->NotifyStacked(NetworkProtectionJoinEventsMenu::GetInstance()->getString("Failed resolving Rockstar ID"), Global::UIVars::g_NotificationError);
				}
			});
		}
	}
}

void NetworkProtectionJoinEventsMenu::Init() {
	SetName("Join Events");
	SetParentSubmenu<NetworkProtectionMenu>();

	addString("Spoofed To");
	addString("Failed resolving Rockstar ID");
	addString("Player isn't in a valid session, resorting to full join block");
	addString("Failed to resolve players session, they may be offline");

	addOption(ToggleOption("Block Resolves")
		.addTranslation()
		.addToggle(m_Vars.m_BlockResolves)
		.addOnClick([] { m_Vars.m_SpoofResolves = false; }).canBeSaved(m_ParentNameStack)
		.addTooltip("Block joiners from being able to join you"));

	addOption(ToggleOption("Spoof Resolves")
		.addTranslation()
		.addToggle(m_Vars.m_SpoofResolves)
		.addOnClick([] { m_Vars.m_BlockResolves = false; })
		.addTooltip("Spoof to another player so that joiners join them instead"));

	addOption(ButtonOption("Input Player")
		.addTranslation()
		.addKeyboard("Enter players name or rid", 30, InputPlayerCallback)
		.addRequirement([] { return m_Vars.m_SpoofResolves; }));

	m_Vars.m_SpoofedToButton = addOption(ButtonOption("")
		.addTranslation()
		.addRequirement([] { return m_Vars.m_SpoofResolves && m_Vars.m_Target > 0 && !m_Vars.m_GsInfo.empty(); }));
}

void NetworkProtectionJoinEventsMenu::Update() {
	m_Vars.m_SpoofedToButton->setName((getString("Spoofed to") + " " + std::to_string(m_Vars.m_Target)).c_str());
}

/*Called once on submenu open*/
void NetworkProtectionJoinEventsMenu::UpdateOnce() {}

/*Called always*/
void NetworkProtectionJoinEventsMenu::FeatureUpdate() {
	if (m_Vars.m_PendingGsInfoRequest) {
		if (m_Vars.m_PendingTimeout < GetTickCount()) {
			m_Vars.m_PendingGsInfoRequest = false;

			if (m_Vars.m_SpoofResolves) {
				if (Menu::GetBase()->m_CurrentOption >= 2) Menu::GetBase()->m_CurrentOption = 1;
				m_Vars.m_SpoofResolves = false;
				m_Vars.m_BlockResolves = true;
				Menu::GetNotify()->NotifyStacked(getString("Player isn't in a valid session, resorting to full join block"), Global::UIVars::g_NotificationError);
			} else {
				Menu::GetNotify()->NotifyStacked(getString("Failed to resolve players session, they may be offline"), Global::UIVars::g_NotificationError);
			}

			return;
		}

		if (!m_Vars.m_GsInfo.empty()) {
			m_Vars.m_PendingGsInfoRequest = false;
		}
	}

	if (m_Vars.m_SpoofResolves && m_Vars.m_Target > 0 && !m_Vars.m_GsInfo.empty()) {
		static int TIMER = 0;
		Menu::Timers::RunTimedFunction(&TIMER, 30000, [=] {
			GetPlayerGs();
		});
	}
}

NetworkProtectionJoinEventsMenu* _instance;
NetworkProtectionJoinEventsMenu* NetworkProtectionJoinEventsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkProtectionJoinEventsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkProtectionJoinEventsMenu::~NetworkProtectionJoinEventsMenu() { delete _instance; }