#pragma once
#include "menu/base/submenu.hpp"

class TeleportDirectionsMenu : public Menu::Submenu {
public:
	static TeleportDirectionsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	TeleportDirectionsMenu()
		: Menu::Submenu() {}

	~TeleportDirectionsMenu();
};

namespace TeleportDirectionsMenuVars {
	struct Vars_ {
		float m_Directions[6] = { 5.f, 5.f, 5.f, 5.f, 5.f, 5.f };
	};

	extern Vars_ m_Vars;
}