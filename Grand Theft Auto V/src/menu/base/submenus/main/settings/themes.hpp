#pragma once
#include "menu/base/submenu.hpp"

class ThemesMenu : public Menu::Submenu {
public:
	static ThemesMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ThemesMenu()
		: Menu::Submenu() {}

	~ThemesMenu();
};

namespace ThemesMenuVars {
	struct Vars_ {
		std::vector<std::pair<std::string, struct stat>> m_CachedThemes;
	};

	extern Vars_ m_Vars;
}