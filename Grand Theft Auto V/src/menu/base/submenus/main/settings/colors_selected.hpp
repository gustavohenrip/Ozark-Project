#pragma once
#include "menu/base/submenu.hpp"

class ColorSelectedMenu : public Menu::Submenu {
public:
	static ColorSelectedMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ColorSelectedMenu()
		: Menu::Submenu() {}

	~ColorSelectedMenu();
};

namespace ColorSelectedMenuVars {
	struct Vars_ {
		Color m_Tmp;
		Color* m_CurrentColor;
	};

	extern Vars_ m_Vars;
}