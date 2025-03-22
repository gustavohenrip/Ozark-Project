#pragma once
#include "menu/base/submenu.hpp"

class NetworkProtectionEntityEventsMenu : public Menu::Submenu {
public:
	static NetworkProtectionEntityEventsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkProtectionEntityEventsMenu()
		: Menu::Submenu() {}

	~NetworkProtectionEntityEventsMenu();
};

namespace NetworkProtectionEntityEventsMenuVars {
	struct Vars_ {
		bool m_Attachments = true;
		bool m_DisableModderSync = false;
		bool m_DisableModderCreate = false;
	};

	extern Vars_ m_Vars;
}