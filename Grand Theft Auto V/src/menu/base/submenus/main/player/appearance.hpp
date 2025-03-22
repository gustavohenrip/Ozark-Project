#pragma once
#include "menu/base/submenu.hpp"

class AppearanceMenu : public Menu::Submenu {
public:
	static AppearanceMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	AppearanceMenu()
		: Menu::Submenu() {}

	~AppearanceMenu();
};

namespace AppearanceMenuVars {
	struct Vars_ {
		bool m_Width;
		bool m_Height;
		bool m_LeanFB;
		bool m_LeanLR;

		float m_WidthVal;
		float m_HeightVal;
		float m_LeanFBVal;
		float m_LeanLRVal;
	};

	extern Vars_ m_Vars;
}