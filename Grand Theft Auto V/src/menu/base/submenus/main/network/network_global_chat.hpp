#pragma once
#include "menu/base/submenu.hpp"

class NetworkGlobalChatMenu : public Menu::Submenu {
public:
	static NetworkGlobalChatMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkGlobalChatMenu()
		: Menu::Submenu() {}

	~NetworkGlobalChatMenu();
};

namespace NetworkGlobalChatMenuVars {
	struct Vars_ {
		bool m_SenderToggle;
		bool m_RandomizeSender;

		Player m_SenderID;
		int m_Sender;
		int m_ListSize;
		int m_MessageCount = 1;
		ScrollStruct<int> m_PlayerList[34];

		std::string m_TempName;
	};

	bool CanSendToPlayer(Player ID);
	Player GetPlayerIDFromIP(uint32_t ExternalIP);
	Player GetPlayerIDFromPlayerKey(int Key);
	uint64_t GetSenderChatToken(uint64_t Default);
	extern Vars_ m_Vars;
}