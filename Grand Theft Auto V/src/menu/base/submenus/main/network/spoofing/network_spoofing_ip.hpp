#pragma once
#include "menu/base/submenu.hpp"

class NetworkSpoofingIPMenu : public Menu::Submenu {
public:
	static NetworkSpoofingIPMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkSpoofingIPMenu()
		: Menu::Submenu() {}

	~NetworkSpoofingIPMenu();
};

namespace NetworkSpoofingIPMenuVars {
	struct Vars_ {
		char m_PreviewName[50];
		std::string m_SpoofingIP;
		std::string m_TempSpoofingIP;
		std::string m_DefaultIP;
		bool m_ConfigSaveIP;
		std::shared_ptr<ButtonOption> m_ApplyButton;
	};

	void SpoofIP(NetworkSpoofingIPMenu* This, std::string ip, bool def = false);
	extern Vars_ m_Vars;
}