#pragma once
#include "menu/base/submenu.hpp"

class NetworkProtectionJoinEventsMenu : public Menu::Submenu {
public:
	static NetworkProtectionJoinEventsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkProtectionJoinEventsMenu()
		: Menu::Submenu() {}

	~NetworkProtectionJoinEventsMenu();
};

namespace NetworkProtectionJoinEventsMenuVars {
	struct Vars_ {
		bool m_BlockResolves;
		bool m_SpoofResolves;
		uint64_t m_Target;
		std::string m_GsInfo;
		uint32_t m_PendingTimeout;
		uint64_t m_PendingGsInfoRequestID;
		bool m_PendingGsInfoRequest;
		std::shared_ptr<ButtonOption> m_SpoofedToButton;
	};

	extern Vars_ m_Vars;
}