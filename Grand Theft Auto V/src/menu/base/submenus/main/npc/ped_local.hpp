#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/submenus/main/helpers/esp.hpp"

class PedLocalMenu : public Menu::Submenu {
public:
	static PedLocalMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	PedLocalMenu()
		: Menu::Submenu() {}

	~PedLocalMenu();
};

namespace PedLocalMenuVars {
	struct Vars_ {
		ESPMenuVars::ESP m_ESP;
	};

	extern Vars_ m_Vars;
}