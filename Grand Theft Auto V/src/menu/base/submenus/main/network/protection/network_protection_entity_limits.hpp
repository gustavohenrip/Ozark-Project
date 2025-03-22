#pragma once
#include "menu/base/submenu.hpp"

class NetworkProtectionEntityLimitsMenu : public Menu::Submenu {
public:
	static NetworkProtectionEntityLimitsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkProtectionEntityLimitsMenu()
		: Menu::Submenu() {}

	~NetworkProtectionEntityLimitsMenu();
};

namespace NetworkProtectionEntityLimitsMenuVars {
	struct EntityLimit {
		uint32_t m_Model;
		int m_Delay = 0;
		char m_Comment[0x100];
		bool m_Enabled = true;
		uint32_t m_LastSpawned = 0;
	};

	struct SpamDetection {
		uint32_t m_Model;
		uint32_t m_LastSpawned = 0;
		int m_Count = 0;
		Player m_Player;

		bool operator==(SpamDetection& Search) {
			return Search.m_Player == m_Player && Search.m_Model == m_Model;
		}
	};
	
	struct Vars_ {
		std::vector<EntityLimit> m_Limits;
		std::vector<SpamDetection> m_SpamDetection;
		bool m_AutoDetect;
	};

	extern Vars_ m_Vars;
}