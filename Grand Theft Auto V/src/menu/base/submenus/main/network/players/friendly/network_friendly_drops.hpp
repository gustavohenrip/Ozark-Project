#pragma once
#include "menu/base/submenu.hpp"

class NetworkFriendlyDropsMenu : public Menu::Submenu {
public:
	static NetworkFriendlyDropsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkFriendlyDropsMenu()
		: Menu::Submenu() {}

	~NetworkFriendlyDropsMenu();
};

namespace NetworkFriendlyDropsMenuVars {
	struct Vars_ {
		bool m_DropToggle;
		int m_DropType;
		int m_DropPickup;
		int m_DropWeapon;
		int m_DropMoneyModel;
		int m_DropMoneyStyle;
		int m_DropMoneyDelay;
		int m_DropMoneyTimer;
	};

	extern Vars_ m_Vars;
}