#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/util/player_manager.hpp"

class NetworkPlayersRemovalMenu : public Menu::Submenu {
public:
	static NetworkPlayersRemovalMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkPlayersRemovalMenu()
		: Menu::Submenu() {}

	~NetworkPlayersRemovalMenu();
};

namespace NetworkPlayersRemovalMenuVars {
	struct Vars_ {
		std::unordered_map<Rage::Network::NetObj*, uint64_t> m_BrokenEntities;

		bool m_BreakNodes = false;
		uint32_t m_BreakNodeThreads[2] = { 0, 0 };
		int m_Crash = 0;

		bool m_SessionCrash = false;
		uint32_t m_SessionCrashTimeout = 0;
		int m_SessionCrashID = 0;

		bool m_TargetedCrash = false;
		uint64_t m_TargetedCrashTarget = 0;
		uint32_t m_TargetedCrashTimeout = 0;
		int m_TargetedCrashID = 0;
	};

	void CrashPlayer(Menu::PlayerVars& Player, int ID);
	void ScriptHostKick(Menu::PlayerVars& Player);
	void SessionHostKick(Menu::PlayerVars& Player);
	void BreakFreemodeKick(Menu::PlayerVars& Player);

	extern ScrollStruct<int> Crashes[3];
	extern Vars_ m_Vars;
}