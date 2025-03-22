#pragma once
#include "menu/base/submenu.hpp"

class AnimationsMenu : public Menu::Submenu {
public:
	static AnimationsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	AnimationsMenu()
		: Menu::Submenu() {}

	~AnimationsMenu();
};

namespace AnimationsMenuVars {
	struct Vars_ {
		bool m_Controllable;
		bool m_Contort;

		int m_Sexual;
		int m_Animals;
		int m_Actions;
		int m_Dance;
		int m_Misc;
	};

	extern Vars_ m_Vars;
}