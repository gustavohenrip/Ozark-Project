#pragma once
#include "menu/base/submenu.hpp"

class NetworkBrowserSelectedMenu : public Menu::Submenu {
public:
	static NetworkBrowserSelectedMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkBrowserSelectedMenu()
		: Menu::Submenu() {}

	~NetworkBrowserSelectedMenu();
};

namespace NetworkBrowserSelectedMenuVars {
	struct Vars_ {
		std::size_t m_Selected;
	};

	extern Vars_ m_Vars;
}