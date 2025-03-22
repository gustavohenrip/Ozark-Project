#pragma once
#include "menu/base/submenu.hpp"

class ColorsMenu : public Menu::Submenu {
public:
	static ColorsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ColorsMenu()
		: Menu::Submenu() {}

	~ColorsMenu();
};

namespace ColorsMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}