#pragma once
#include "menu/base/submenu.hpp"

class NetworkProtectionEntityBlockingMenu : public Menu::Submenu {
public:
	static NetworkProtectionEntityBlockingMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkProtectionEntityBlockingMenu()
		: Menu::Submenu() {}

	~NetworkProtectionEntityBlockingMenu();
};

namespace NetworkProtectionEntityBlockingMenuVars {
	struct Vars_ {
		bool m_Toggled[14];
	};

	extern Vars_ m_Vars;
}