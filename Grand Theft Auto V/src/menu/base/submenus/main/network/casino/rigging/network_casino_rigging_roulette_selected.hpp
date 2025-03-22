#pragma once
#include "menu/base/submenu.hpp"

class NetworkCasinoRiggingRouletteSelectedMenu : public Menu::Submenu {
public:
	static NetworkCasinoRiggingRouletteSelectedMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkCasinoRiggingRouletteSelectedMenu()
		: Menu::Submenu() {}

	~NetworkCasinoRiggingRouletteSelectedMenu();
};

namespace NetworkCasinoRiggingRouletteSelectedMenuVars {
	struct Rigging {
		bool m_Enabled;
		int m_Rigged;
	};

	struct Vars_ {
		int m_Selected;
		Rigging m_Tables[6];
	};

	extern Vars_ m_Vars;
}