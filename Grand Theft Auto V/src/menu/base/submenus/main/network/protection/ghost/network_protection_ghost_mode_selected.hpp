#pragma once
#include "menu/base/submenu.hpp"
#include "../network_protection_ghost_mode.hpp"

class NetworkProtectionGhostModeSelectedMenu : public Menu::Submenu {
public:
	static NetworkProtectionGhostModeSelectedMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkProtectionGhostModeSelectedMenu()
		: Menu::Submenu() {}

	~NetworkProtectionGhostModeSelectedMenu();
};

namespace NetworkProtectionGhostModeSelectedMenuVars {
	struct Vars_ {
		bool m_Populated;
		NetworkProtectionGhostModeMenuVars::Ghost* m_Selected;
	};

	extern Vars_ m_Vars;
}