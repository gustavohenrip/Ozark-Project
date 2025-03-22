#pragma once
#include "menu/base/submenu.hpp"

class ThemeSelectedMenu : public Menu::Submenu {
public:
	static ThemeSelectedMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ThemeSelectedMenu()
		: Menu::Submenu() {}

	~ThemeSelectedMenu();
};

namespace ThemeSelectedMenuVars {
	struct Vars_ {
		std::size_t m_Selected;
	};

	extern Vars_ m_Vars;
}