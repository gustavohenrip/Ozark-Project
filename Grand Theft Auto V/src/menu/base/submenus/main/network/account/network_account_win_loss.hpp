#pragma once
#include "menu/base/submenu.hpp"

class NetworkAccountWinsLossesMenu : public Menu::Submenu {
public:
	static NetworkAccountWinsLossesMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkAccountWinsLossesMenu()
		: Menu::Submenu() {}

	~NetworkAccountWinsLossesMenu();
};

namespace NetworkAccountWinsLossesMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}