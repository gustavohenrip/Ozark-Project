#pragma once
#include "menu/base/submenu.hpp"

class NetworkRecentPlayersSelectedMenu : public Menu::Submenu {
public:
	static NetworkRecentPlayersSelectedMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkRecentPlayersSelectedMenu()
		: Menu::Submenu() {}

	~NetworkRecentPlayersSelectedMenu();
};

namespace NetworkRecentPlayersSelectedMenuVars {
	struct Vars_ {
		int m_Type; // 1 = recent, 2 = perma
		std::size_t m_SelectedPlayer;
	};

	extern Vars_ m_Vars;
}