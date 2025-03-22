#pragma once
#include "menu/base/submenu.hpp"

class NetworkProtectionNotificationsMenu : public Menu::Submenu {
public:
	static NetworkProtectionNotificationsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkProtectionNotificationsMenu()
		: Menu::Submenu() {}

	~NetworkProtectionNotificationsMenu();
};

namespace NetworkProtectionNotificationsMenuVars {
	struct Vars_ {
		bool m_PlayerSpawn = true;
		bool m_Spectating = true;
		bool m_MoneyDrop = true;
		bool m_Desyncs = true;
		bool m_HostKicks = true;
		bool m_Crash = true;
		bool m_ScriptHostMigration = true;
		bool m_RockstarDeveloper = true;
		bool m_SpoofedText = true;
		bool m_SpoofedChat = true;
		bool m_JoinTimeout = true;

		std::unordered_map<int, std::unordered_map<int, uint32_t>> m_SpectateMap;
		std::unordered_map<uint64_t, uint32_t> m_MoneyDropMap;
	};

	extern Vars_ m_Vars;
}