#pragma once
#include "menu/base/submenu.hpp"

class NetworkPlayersSelectedMenu : public Menu::Submenu {
public:
	static NetworkPlayersSelectedMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkPlayersSelectedMenu()
		: Menu::Submenu() {}

	~NetworkPlayersSelectedMenu();
};

namespace NetworkPlayersSelectedMenuVars {
	struct Vars_ {
		float m_ESPNameScale;
	};

	extern Vars_ m_Vars;
}