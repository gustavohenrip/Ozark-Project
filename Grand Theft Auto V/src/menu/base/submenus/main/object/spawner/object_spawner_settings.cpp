#include "object_spawner_settings.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../object_spawner.hpp"

using namespace ObjectSpawnerSettingsMenuVars;

namespace ObjectSpawnerSettingsMenuVars {
	Vars_ m_Vars;
}

void ObjectSpawnerSettingsMenu::Init() {
	SetName("Object Settings");
	SetParentSubmenu<ObjectSpawnerMenu>();

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

void ObjectSpawnerSettingsMenu::Update() {}

/*Called once on submenu open*/
void ObjectSpawnerSettingsMenu::UpdateOnce() {}

/*Called always*/
void ObjectSpawnerSettingsMenu::FeatureUpdate() {}

ObjectSpawnerSettingsMenu* _instance;
ObjectSpawnerSettingsMenu* ObjectSpawnerSettingsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ObjectSpawnerSettingsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

ObjectSpawnerSettingsMenu::~ObjectSpawnerSettingsMenu() { delete _instance; }