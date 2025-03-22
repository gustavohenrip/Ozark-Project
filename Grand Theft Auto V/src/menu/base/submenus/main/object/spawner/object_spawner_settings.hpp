#pragma once
#include "menu/base/submenu.hpp"

class ObjectSpawnerSettingsMenu : public Menu::Submenu {
public:
	static ObjectSpawnerSettingsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ObjectSpawnerSettingsMenu()
		: Menu::Submenu() {}

	~ObjectSpawnerSettingsMenu();
};

namespace ObjectSpawnerSettingsMenuVars {
	struct Vars_ {
		bool m_Notification;
		bool m_Blip;
		bool m_ParticleFX;
		bool m_Fade;
	};

	extern Vars_ m_Vars;
}