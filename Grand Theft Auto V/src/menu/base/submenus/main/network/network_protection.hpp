#pragma once
#include "menu/base/submenu.hpp"

class NetworkProtectionMenu : public Menu::Submenu {
public:
	static NetworkProtectionMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkProtectionMenu()
		: Menu::Submenu() {}

	~NetworkProtectionMenu();
};

namespace NetworkProtectionMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}