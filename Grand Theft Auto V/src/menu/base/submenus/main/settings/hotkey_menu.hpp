#pragma once
#include "menu/base/submenu.hpp"

class HotkeysMenu : public Menu::Submenu {
public:
	static HotkeysMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	HotkeysMenu()
		: Menu::Submenu() {}

	~HotkeysMenu();
};

namespace HotkeysMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}