#pragma once
#include "menu/base/submenu.hpp"

class NetworkPlayersEntityBlockingMenu : public Menu::Submenu {
public:
	static NetworkPlayersEntityBlockingMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkPlayersEntityBlockingMenu()
		: Menu::Submenu() {}

	~NetworkPlayersEntityBlockingMenu();
};

namespace NetworkPlayersEntityBlockingMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}