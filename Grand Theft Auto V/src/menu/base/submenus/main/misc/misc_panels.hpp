#pragma once
#include "menu/base/submenu.hpp"

class MiscPanelsMenu : public Menu::Submenu {
public:
	static MiscPanelsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	MiscPanelsMenu()
		: Menu::Submenu() {}

	~MiscPanelsMenu();
};

namespace MiscPanelsMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}