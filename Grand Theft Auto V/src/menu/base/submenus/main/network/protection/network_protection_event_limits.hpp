#pragma once
#include "menu/base/submenu.hpp"

class NetworkProtectionEventLimitsMenu : public Menu::Submenu {
public:
	static NetworkProtectionEventLimitsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkProtectionEventLimitsMenu()
		: Menu::Submenu() {}

	~NetworkProtectionEventLimitsMenu();
};

namespace NetworkProtectionEventLimitsMenuVars {
	struct Vars_ {
		bool m_BlockSpoofedTextMessages;
		bool m_BlockSpoofedChatMessages;

		int m_BlockSpoofedTextMessagesID;
		int m_BlockSpoofedChatMessagesID;

		int m_TextMessageDelay;
		int m_GlobalChatMessageDelay;
	};

	extern Vars_ m_Vars;
}