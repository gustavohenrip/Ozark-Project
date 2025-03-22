#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/util/panels.hpp"

class MiscPanelsParentMenu : public Menu::Submenu {
public:
	static MiscPanelsParentMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	MiscPanelsParentMenu()
		: Menu::Submenu() {}

	~MiscPanelsParentMenu();
};

namespace MiscPanelsParentMenuVars {
	struct Vars_ {
		Menu::Panels::PanelParent* m_Parent;
	};

	extern Vars_ m_Vars;
}