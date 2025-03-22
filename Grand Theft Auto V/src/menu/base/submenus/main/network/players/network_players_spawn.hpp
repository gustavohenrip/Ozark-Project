#pragma once
#include "menu/base/submenu.hpp"

class NetworkPlayersSpawnMenu : public Menu::Submenu {
public:
	static NetworkPlayersSpawnMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkPlayersSpawnMenu()
		: Menu::Submenu() {}

	~NetworkPlayersSpawnMenu();
};

namespace NetworkPlayersSpawnMenuVars {
	struct Vars_ {
	};

	extern Vars_ m_Vars;
}