#pragma once
#include "menu/base/submenu.hpp"

class NetworkCasinoRiggingRouletteMenu : public Menu::Submenu {
public:
	static NetworkCasinoRiggingRouletteMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkCasinoRiggingRouletteMenu()
		: Menu::Submenu() {}

	~NetworkCasinoRiggingRouletteMenu();
};

namespace NetworkCasinoRiggingRouletteMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}