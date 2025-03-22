#pragma once
#include "menu/base/submenu.hpp"

class NetworkPresenceMenu : public Menu::Submenu {
public:
	static NetworkPresenceMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkPresenceMenu()
		: Menu::Submenu() {}

	~NetworkPresenceMenu();
};

namespace NetworkPresenceMenuVars {
	struct Vars_ {
		bool m_PendingGsInfoRequest;
		uint64_t m_PendingGsInfoRequestID;
		std::string m_GsInfo;
		uint32_t m_PendingTimeout;
		int m_Type;
		int m_JoinThemType;
		int m_JoinMeType;

		uint64_t m_Target = 0;
	};

	void JoinPlayer(uint64_t RockstarID);

	extern ScrollStruct<int> Types[2];
	extern Vars_ m_Vars;
}