#pragma once
#include "menu/base/submenu.hpp"

class NetworkOverseerMenu : public Menu::Submenu {
public:
	static NetworkOverseerMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkOverseerMenu()
		: Menu::Submenu() {}

	~NetworkOverseerMenu();
};

namespace NetworkOverseerMenuVars {
#define OVERSEER_ENABLED (1 << 0)
#define OVERSEER_NOTIFY (1 << 1)
#define OVERSEER_BLOCK_JOIN_HOST (1 << 2)
#define OVERSEER_EXPLODE (1 << 3)
#define OVERSEER_CRASH (1 << 4)
#define OVERSEER_KICK (1 << 5)
#define OVERSEER_CAGE (1 << 6)
#define OVERSEER_KICK_FROM_VEHICLE (1 << 7)
#define OVERSEER_SET_ON_FIRE (1 << 8)
#define OVERSEER_RAGDOLL (1 << 9) 
#define OVERSEER_GLITCH_PHYSICS (1 << 10)
#define OVERSEER_BLACK_SCREEN (1 << 11)
#define OVERSEER_BLAME (1 << 12)
#define OVERSEER_GIVE_WANTED_LEVEL (1 << 13)
#define OVERSEER_SEND_TO_ISLAND (1 << 14)
#define OVERSEER_REMOVE_WEAPONS (1 << 15)
#define OVERSEER_HOSTILE_CLONE (1 << 16)

	struct Overseer {
		std::string m_Name;
		std::string m_Reason;
		uint64_t m_RockstarID;
		int m_Flags;
	};

	struct Vars_ {
		bool m_EnableOverseer;
		std::vector<Overseer> m_Players;
	};

	bool ProcessJoinRequest(uint64_t RID, std::string Name, uint32_t IP);
	void ProcessJoinNotify(uint64_t RID, uint32_t IP, std::string Name);
	void Process(Player ID, int Flag);
	void Save(std::string Name, uint64_t RID, std::string Reason);
	extern Vars_ m_Vars;
}