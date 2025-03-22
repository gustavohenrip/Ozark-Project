#pragma once
#include "menu/base/submenu.hpp"

class NetworkRecentPlayersMenu : public Menu::Submenu {
public:
	static NetworkRecentPlayersMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkRecentPlayersMenu()
		: Menu::Submenu() {}

	~NetworkRecentPlayersMenu();
};

namespace NetworkRecentPlayersMenuVars {
	struct RecentPlayer {
		char m_Name[0x20];
		char m_IP[16];
		char m_FirstEncountered[0x50];
		char m_LastEncountered[0x50];
		uint64_t m_RockstarID;
		int m_InternalEncountered;
	};

	struct Vars_ {
		std::vector<uint64_t> m_Queue;
		std::vector<std::pair<std::string, RecentPlayer>> m_Players;
		std::vector<std::pair<std::string, RecentPlayer>> m_PermanentPlayers;
		std::vector<std::pair<std::string, RecentPlayer>>* m_SelectedList;
	};

	void Save(std::string Name, std::string IP, uint64_t RockstarID);
	extern Vars_ m_Vars;
}