#pragma once
#include "menu/base/submenu.hpp"

class FontsMenu : public Menu::Submenu {
public:
	static FontsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	FontsMenu()
		: Menu::Submenu() {}

	~FontsMenu();
};

namespace FontsMenuVars {
	struct Vars_ {
		int m_Fonts[4];
	};

	extern Vars_ m_Vars;
}