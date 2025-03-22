#pragma once
#include "menu/base/submenu.hpp"

class NetworkPlayersMenu : public Menu::Submenu {
public:
	static NetworkPlayersMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkPlayersMenu()
		: Menu::Submenu() {}

	~NetworkPlayersMenu();
};

namespace NetworkPlayersMenuVars {
	struct Vars_ {
		int m_SelectedPlayer = 0;
	};

	extern Vars_ m_Vars;
}