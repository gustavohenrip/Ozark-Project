#pragma once
#include "menu/base/submenu.hpp"

class PedSpawnerSettingsMenu : public Menu::Submenu {
public:
	static PedSpawnerSettingsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	PedSpawnerSettingsMenu()
		: Menu::Submenu() {}

	~PedSpawnerSettingsMenu();
};

namespace PedSpawnerSettingsMenuVars {
	struct Vars_ {
		bool m_Notification;
		bool m_Blip;
		bool m_ParticleFX;
		bool m_Fade;
	};

	extern Vars_ m_Vars;
}