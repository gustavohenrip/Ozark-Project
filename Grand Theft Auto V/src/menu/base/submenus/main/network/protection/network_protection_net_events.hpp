#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/submenus/main/helpers/structs.hpp"

class NetworkProtectionNetEventsMenu : public Menu::Submenu {
public:
	static NetworkProtectionNetEventsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkProtectionNetEventsMenu()
		: Menu::Submenu() {}

	~NetworkProtectionNetEventsMenu();
};

namespace NetworkProtectionNetEventsMenuVars {
	struct Vars_ {
		Menu::Structs::ProtectionEventContext m_GiveWeapon = { true, false, true, true, true, true };
		Menu::Structs::ProtectionEventContext m_RemoveWeapon = { true, false, true, true, true, true };
		Menu::Structs::ProtectionEventContext m_RemoveAllWeapons = { true, false, true, true, true, true };
		Menu::Structs::ProtectionEventContext m_Freeze = { true, false, true, true, true, true };
		Menu::Structs::ProtectionEventContext m_Explosion = { true, false, true, true, true, true };
		Menu::Structs::ProtectionEventContext m_Fire = { true, false, true, true, true, true };
		Menu::Structs::ProtectionEventContext m_PTFX = { true, false, true , true, true, true };
		Menu::Structs::ProtectionEventContext m_ScriptEvents = { true, false, true, true, true, true };
		Menu::Structs::ProtectionEventContext m_IncrementStat = { true, false, true, true, false, true };
		Menu::Structs::ProtectionEventContext m_Weather = { true, false, true, true, false, true };
		Menu::Structs::ProtectionEventContext m_Clock = { true, false, true, true, false, true };
		Menu::Structs::ProtectionEventContext m_Sound = { true, false, true, true, false, true };
		Menu::Structs::ProtectionEventContext m_Control = { true, false, true, true, false, false };

		bool m_Blame = true;
	};

	extern Vars_ m_Vars;
}