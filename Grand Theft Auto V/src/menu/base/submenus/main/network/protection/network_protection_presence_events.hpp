#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/submenus/main/helpers/structs.hpp"

class NetworkProtectionPresenceEventsMenu : public Menu::Submenu {
public:
	static NetworkProtectionPresenceEventsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkProtectionPresenceEventsMenu()
		: Menu::Submenu() {}

	~NetworkProtectionPresenceEventsMenu();
};

namespace NetworkProtectionPresenceEventsMenuVars {
	struct Vars_ {
		Menu::Structs::ProtectionEventContext m_TextMessage = { true, false, true, true, false, true };
		Menu::Structs::ProtectionEventContext m_CrewMessage = { true, false, true, true, false, true };
		Menu::Structs::ProtectionEventContext m_GameInvite = { true, false, true, true, false, true };
		Menu::Structs::ProtectionEventContext m_GameServerRewards = { true, false, true, true, false, true };
		Menu::Structs::ProtectionEventContext m_AdminInvite = { true, false, true, true, false, true };
		Menu::Structs::ProtectionEventContext m_StatUpdate = { true, false, true, true, false, true };
	};

	extern Vars_ m_Vars;
}