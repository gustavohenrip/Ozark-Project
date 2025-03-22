#pragma once
#include "menu/base/submenu.hpp"

class ClipsetMenu : public Menu::Submenu {
public:
	static ClipsetMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ClipsetMenu()
		: Menu::Submenu() {}

	~ClipsetMenu();
};

namespace ClipsetMenuVars {
	struct Vars_ {
		int m_Motion;
		int m_Weapon;
	};

	extern Vars_ m_Vars;
}