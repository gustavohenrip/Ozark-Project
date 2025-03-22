#pragma once
#include "menu/base/submenu.hpp"

class VehicleSpawnerSettingsMenu : public Menu::Submenu {
public:
	static VehicleSpawnerSettingsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleSpawnerSettingsMenu()
		: Menu::Submenu() {}

	~VehicleSpawnerSettingsMenu();
};

namespace VehicleSpawnerSettingsMenuVars {
	struct Vars_ {
		bool m_Notification;
		bool m_Blip;
		bool m_ParticleFX;
		bool m_MaxUpgrades;
		bool m_TeleportInside;
		bool m_Godmode;
		bool m_Fade;
		bool m_AirVehiclesInAir;
		bool m_DeleteCurrent;

		float m_SpawnHeight = 80.f;
	};

	extern Vars_ m_Vars;
}