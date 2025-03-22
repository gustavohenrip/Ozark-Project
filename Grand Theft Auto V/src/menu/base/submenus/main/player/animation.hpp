#pragma once
#include "menu/base/submenu.hpp"

class AnimationMenu : public Menu::Submenu {
public:
	static AnimationMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	AnimationMenu()
		: Menu::Submenu() {}

	~AnimationMenu();
};

namespace AnimationMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}