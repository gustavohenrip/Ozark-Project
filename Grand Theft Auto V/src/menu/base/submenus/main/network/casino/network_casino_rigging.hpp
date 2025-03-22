#pragma once
#include "menu/base/submenu.hpp"

class NetworkCasinoRiggingMenu : public Menu::Submenu {
public:
	static NetworkCasinoRiggingMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkCasinoRiggingMenu()
		: Menu::Submenu() {}

	~NetworkCasinoRiggingMenu();
};

namespace NetworkCasinoRiggingMenuVars {
	struct Vars_ {
		int m_LuckyWheel;
	};

	extern Vars_ m_Vars;
}