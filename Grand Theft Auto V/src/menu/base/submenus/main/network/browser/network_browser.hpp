#pragma once
#include "menu/base/submenu.hpp"

class NetworkBrowserMenu : public Menu::Submenu {
public:
	static NetworkBrowserMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkBrowserMenu()
		: Menu::Submenu() {}

	~NetworkBrowserMenu();
};

namespace NetworkBrowserMenuVars {
	struct Vars_ {
		bool m_Populate;
	};

	extern Vars_ m_Vars;
}