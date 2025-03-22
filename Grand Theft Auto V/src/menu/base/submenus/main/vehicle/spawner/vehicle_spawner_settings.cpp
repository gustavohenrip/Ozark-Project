#include "vehicle_spawner_settings.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_spawner.hpp"
#include "../../network/players/network_players_spawn.hpp"

using namespace VehicleSpawnerSettingsMenuVars;

namespace VehicleSpawnerSettingsMenuVars {
	Vars_ m_Vars;
}

void VehicleSpawnerSettingsMenu::Init() {
	SetName("Vehicle Settings");
	SetParentSubmenu<VehicleSpawnerMenu>();

	addOption(ToggleOption("Notification")
		.addTranslation()
		.addToggle(m_Vars.m_Notification).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Blip")
		.addTranslation()
		.addToggle(m_Vars.m_Blip).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Particle FX")
		.addTranslation()
		.addToggle(m_Vars.m_ParticleFX).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Max Upgrades")
		.addTranslation()
		.addToggle(m_Vars.m_MaxUpgrades).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Teleport Inside")
		.addTranslation()
		.addToggle(m_Vars.m_TeleportInside).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Godmode")
		.addTranslation()
		.addToggle(m_Vars.m_Godmode).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Fade")
		.addTranslation()
		.addToggle(m_Vars.m_Fade).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Air Vehicles In Air")
		.addTranslation()
		.addToggle(m_Vars.m_AirVehiclesInAir).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Air Height")
		.addTranslation()
		.addNumber(m_Vars.m_SpawnHeight, "%.0f", 1.f).addMin(0.f).addMax(1000.f).canBeSaved(m_ParentNameStack)
		.addRequirement([] { return m_Vars.m_AirVehiclesInAir; }));

	addOption(ToggleOption("Delete Current")
		.addTranslation()
		.addToggle(m_Vars.m_DeleteCurrent).canBeSaved(m_ParentNameStack));

	addString("Gift to");
}

void VehicleSpawnerSettingsMenu::Update() {}

/*Called once on submenu open*/
void VehicleSpawnerSettingsMenu::UpdateOnce() {
	ClearOptionsOffset(10);

	if (Menu::GetSubmenuHandler()->GetPreviousSubmenu() && Menu::GetSubmenuHandler()->GetPreviousSubmenu()->m_Parent == NetworkPlayersSpawnMenu::GetInstance()) {
		addOption(ToggleOption(getString("Gift to") + " " + Menu::GetSelectedPlayer().m_Name)
			.addToggle(Menu::GetSelectedPlayer().m_GiftVehicle));
	}
}

/*Called always*/
void VehicleSpawnerSettingsMenu::FeatureUpdate() {}

VehicleSpawnerSettingsMenu* _instance;
VehicleSpawnerSettingsMenu* VehicleSpawnerSettingsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleSpawnerSettingsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleSpawnerSettingsMenu::~VehicleSpawnerSettingsMenu() { delete _instance; }