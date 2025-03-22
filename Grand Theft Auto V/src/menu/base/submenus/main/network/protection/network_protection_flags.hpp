#pragma once
#include "menu/base/submenu.hpp"

class NetworkProtectionFlagsMenu : public Menu::Submenu {
public:
	static NetworkProtectionFlagsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkProtectionFlagsMenu()
		: Menu::Submenu() {}

	~NetworkProtectionFlagsMenu();
};

namespace NetworkProtectionFlagsMenuVars {
	struct Vars_ {
		bool m_Godmode = true;
		bool m_SuperJump = true;
		bool m_Health = true;
		bool m_Armor = true;
		bool m_Spectate = true;
	};

	extern Vars_ m_Vars;
}