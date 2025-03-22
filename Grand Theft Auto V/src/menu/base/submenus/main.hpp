#pragma once
#include "menu/base/submenu.hpp"

class MainMenu : public Menu::Submenu {
public:
	static MainMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	MainMenu()
		: Menu::Submenu() {}

	~MainMenu();
};

namespace MainMenuVars {
	struct Vars_ {
		bool m_HasInitializedScaleform = false;
		int m_ScaleformHandle = 0;
		int m_CurrentIndex = -1;
		DWORD m_CachedTime = 0;
		bool m_ShownWelcome = false;
	};

	extern Vars_ m_Vars;
}