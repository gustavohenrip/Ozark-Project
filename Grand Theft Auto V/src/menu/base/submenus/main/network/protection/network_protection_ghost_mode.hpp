#pragma once
#include "menu/base/submenu.hpp"

class NetworkProtectionGhostModeMenu : public Menu::Submenu {
public:
	static NetworkProtectionGhostModeMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkProtectionGhostModeMenu()
		: Menu::Submenu() {}

	~NetworkProtectionGhostModeMenu();
};

namespace NetworkProtectionGhostModeMenuVars {
	struct Ghost {
		bool m_Enabled = false;
		bool m_DisableForSession = false;
		eNetObjectTypes m_Type;
		std::string m_Name;
	};

	struct Vars_ {
		Ghost m_Ghosts[14];
	};

	extern Vars_ m_Vars;
}