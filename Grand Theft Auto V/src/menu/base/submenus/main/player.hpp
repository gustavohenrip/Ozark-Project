#pragma once
#include "menu/base/submenu.hpp"

class PlayerMenu : public Menu::Submenu {
public:
	static PlayerMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	PlayerMenu()
		: Menu::Submenu() {}

	~PlayerMenu();
};

namespace PlayerMenuVars {
	struct Vars_ {
		bool m_Godmode;
		bool m_DisablePolice;
		bool m_DisableRagdoll;
		bool m_CopsTurnBlindEye;
		bool m_OffTheRadar;
		bool m_RevealHiddenPlayers;
		bool m_ReducedCollision;
		bool m_Invisibility;
		bool m_Superman;
		bool m_Badsport;
		bool m_SwimAnywhere;
		bool m_BreatheFire;

		int m_BreatheFireHandle;
	};

	extern Vars_ m_Vars;
}