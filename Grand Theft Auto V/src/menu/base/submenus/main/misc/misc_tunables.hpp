#pragma once
#include "menu/base/submenu.hpp"

class MiscTunablesMenu : public Menu::Submenu {
public:
	static MiscTunablesMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	MiscTunablesMenu()
		: Menu::Submenu() {}

	~MiscTunablesMenu();
};

namespace MiscTunablesMenuVars {
	struct Vars_ {
		bool m_DisableIdleKick;
		bool m_XmasDLC;
		bool m_ValentinesDLC;
		bool m_IndependenceDLC;
		bool m_FreeShopping;
		bool m_UnlimitedFireworks;
		bool m_DisableUtilityFees;
		bool m_DisableSCTV;
	};

	extern Vars_ m_Vars;
}