#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/submenus/main/helpers/esp.hpp"

class NetworkSessionMenu : public Menu::Submenu {
public:
	static NetworkSessionMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkSessionMenu()
		: Menu::Submenu() {}

	~NetworkSessionMenu();
};

namespace NetworkSessionMenuVars {
	struct Vars_ {
		ESPMenuVars::ESP m_ESP;
	};

	extern Vars_ m_Vars;
}