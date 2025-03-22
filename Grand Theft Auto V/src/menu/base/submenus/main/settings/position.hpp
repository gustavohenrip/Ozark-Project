#pragma once
#include "menu/base/submenu.hpp"

class PositionMenu : public Menu::Submenu {
public:
	static PositionMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	PositionMenu()
		: Menu::Submenu() {}

	~PositionMenu();
};

namespace PositionMenuVars {
	struct Vars_ {
	};

	extern Vars_ m_Vars;
}