#pragma once
#include "menu/base/submenu.hpp"

class TeleportPopularMenu : public Menu::Submenu {
public:
	static TeleportPopularMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	TeleportPopularMenu()
		: Menu::Submenu() {}

	~TeleportPopularMenu();
};

namespace TeleportPopularMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}