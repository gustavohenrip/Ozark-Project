#include "ped_spawner_settings.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../ped_spawner.hpp"

using namespace PedSpawnerSettingsMenuVars;

namespace PedSpawnerSettingsMenuVars {
	Vars_ m_Vars;
}

void PedSpawnerSettingsMenu::Init() {
	SetName("Settings");
	SetParentSubmenu<PedSpawnerMenu>();

	addOption(ToggleOption("Notification")
		.addTranslation()
		.addToggle(m_Vars.m_Notification).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Blip")
		.addTranslation()
		.addToggle(m_Vars.m_Blip).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Particle FX")
		.addTranslation()
		.addToggle(m_Vars.m_ParticleFX).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Fade")
		.addTranslation()
		.addToggle(m_Vars.m_Fade).canBeSaved(m_ParentNameStack));
}

void PedSpawnerSettingsMenu::Update() {}

/*Called once on submenu open*/
void PedSpawnerSettingsMenu::UpdateOnce() {}

/*Called always*/
void PedSpawnerSettingsMenu::FeatureUpdate() {}

PedSpawnerSettingsMenu* _instance;
PedSpawnerSettingsMenu* PedSpawnerSettingsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new PedSpawnerSettingsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

PedSpawnerSettingsMenu::~PedSpawnerSettingsMenu() { delete _instance; }