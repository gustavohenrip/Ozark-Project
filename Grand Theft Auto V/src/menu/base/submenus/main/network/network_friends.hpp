#pragma once
#include "menu/base/submenu.hpp"

class NetworkFriendsMenu : public Menu::Submenu {
public:
	static NetworkFriendsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkFriendsMenu()
		: Menu::Submenu() {}

	~NetworkFriendsMenu();
};

namespace NetworkFriendsMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}