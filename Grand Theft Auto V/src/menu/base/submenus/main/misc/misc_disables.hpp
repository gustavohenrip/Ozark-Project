#pragma once
#include "menu/base/submenu.hpp"

class MiscDisablesMenu : public Menu::Submenu {
public:
	static MiscDisablesMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	MiscDisablesMenu()
		: Menu::Submenu() {}

	~MiscDisablesMenu();
};

namespace MiscDisablesMenuVars {
	struct Vars_ {
		bool m_DisablePhone;
		bool m_DisableCalls;
		bool m_DisableStuntJumps;
		bool m_DisableCinematicCamera;
		bool m_DisableProfanityFilter;
		bool m_DisableMinimap;
		bool m_DisableHUD;
		bool m_DisableChatOnOpen;
		bool m_DisableRestrictedAreas;
		bool m_DisableNotifications;
		bool m_DisableLoadingPrompts;
		bool m_DisableCutscenes;
		bool m_DisableBoundryLimit;
	};

	extern Vars_ m_Vars;
}
