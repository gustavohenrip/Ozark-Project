#pragma once
#include "menu/base/submenu.hpp"
#include "rage/types/network_types.hpp"

class NetworkFriendsSelectedMenu : public Menu::Submenu {
public:
	static NetworkFriendsSelectedMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkFriendsSelectedMenu()
		: Menu::Submenu() {}

	~NetworkFriendsSelectedMenu();
};

namespace NetworkFriendsSelectedMenuVars {
	struct Vars_ {
		int m_Type; // 0 = selected, 1 = all
		Rage::Network::Friends* m_Selected;
	};

	extern Vars_ m_Vars;
}