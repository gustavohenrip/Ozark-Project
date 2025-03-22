#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/submenus/main/helpers/structs.hpp"

class NetworkProtectionGameEventsMenu : public Menu::Submenu {
public:
	static NetworkProtectionGameEventsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkProtectionGameEventsMenu()
		: Menu::Submenu() {}

	~NetworkProtectionGameEventsMenu();
};

namespace NetworkProtectionGameEventsMenuVars {
	struct Vars_ {
		Menu::Structs::ProtectionEventContext m_ScriptHostDropKick = { true, false, true, true, false, true, "Prevents modders kicking you with script host" };
		Menu::Structs::ProtectionEventContext m_SpawnedEntityOwnerMigration = { true, false, true, true, false, true, "Prevents modders taking ownership of your spawned entities" };
		Menu::Structs::ProtectionEventContext m_PlayerModelUpdate = { true, false, true, true, false, true, "Prevents model change updates" };
		Menu::Structs::ProtectionEventContext m_PickupProcessing = { true, false, true, true, false, true, "Prevents transactions processing for pickups" };
		Menu::Structs::ProtectionEventContext m_Clone = { true, false, true, true, true, true, "Prevents players cloning you" };
		Menu::Structs::ProtectionEventContext m_PlayerAttachment = { true, false, true, true, false, true, "Prevents players attaching to you" };
	
		bool m_Cage = true;
	};

	extern Vars_ m_Vars;
}