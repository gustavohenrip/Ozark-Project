#include "esp_settings.hpp"
#include "menu/base/submenu_handler.hpp"
#include "esp.hpp"
#include "esp_settings_edit.hpp"

using namespace ESPSettingsMenuVars;

namespace ESPSettingsMenuVars {
	Vars_ m_Vars;
}

void ESPSettingsMenu::Init() {
	SetName("Settings");
	SetParentSubmenu<ESPMenu>();

	addOption(SubmenuOption("Name")
		.addTranslation()
		.addSubmenu<ESPSettingsEditMenu>()
		.addOnClick([] {
			ESPSettingsEditMenu::GetInstance()->SetName("Name");
			ESPSettingsEditMenuVars::m_Vars.m_Color = &ESPMenuVars::m_Vars.m_ESP->m_NameColor;
			ESPSettingsEditMenuVars::m_Vars.m_LineOfSightColor = &ESPMenuVars::m_Vars.m_ESP->m_NameLineOfSightColor;
			ESPSettingsEditMenuVars::m_Vars.m_Rainbow = &ESPMenuVars::m_Vars.m_ESP->m_NameRainbow;
			ESPSettingsEditMenuVars::m_Vars.m_LineOfSightRainbow = &ESPMenuVars::m_Vars.m_ESP->m_NameLineOfSightRainbow;
			ESPSettingsEditMenuVars::m_Vars.m_RainbowToggle = &ESPMenuVars::m_Vars.m_ESP->m_NameRainbowToggle;
			ESPSettingsEditMenuVars::m_Vars.m_LineOfSightToggle = &ESPMenuVars::m_Vars.m_ESP->m_NameLineOfSightRainbowToggle;
			ESPSettingsEditMenuVars::m_Vars.m_Scale = &ESPMenuVars::m_Vars.m_ESP->m_NameScale;
		}));

	addOption(SubmenuOption("Snapline")
		.addTranslation()
		.addSubmenu<ESPSettingsEditMenu>()
		.addOnClick([] {
			ESPSettingsEditMenu::GetInstance()->SetName("Snapline");
			ESPSettingsEditMenuVars::m_Vars.m_Color = &ESPMenuVars::m_Vars.m_ESP->m_SnaplineColor;
			ESPSettingsEditMenuVars::m_Vars.m_LineOfSightColor = &ESPMenuVars::m_Vars.m_ESP->m_SnaplineLineOfSightColor;
			ESPSettingsEditMenuVars::m_Vars.m_Rainbow = &ESPMenuVars::m_Vars.m_ESP->m_SnaplineRainbow;
			ESPSettingsEditMenuVars::m_Vars.m_LineOfSightRainbow = &ESPMenuVars::m_Vars.m_ESP->m_SnaplineLineOfSightRainbow;
			ESPSettingsEditMenuVars::m_Vars.m_RainbowToggle = &ESPMenuVars::m_Vars.m_ESP->m_SnaplineRainbowToggle;
			ESPSettingsEditMenuVars::m_Vars.m_LineOfSightToggle = &ESPMenuVars::m_Vars.m_ESP->m_SnaplineLineOfSightRainbowToggle;
		}));

	addOption(SubmenuOption("3D")
		.addTranslation()
		.addSubmenu<ESPSettingsEditMenu>()
		.addOnClick([] {
			ESPSettingsEditMenu::GetInstance()->SetName("3D");
			ESPSettingsEditMenuVars::m_Vars.m_Color = &ESPMenuVars::m_Vars.m_ESP->m_3DColor;
			ESPSettingsEditMenuVars::m_Vars.m_LineOfSightColor = &ESPMenuVars::m_Vars.m_ESP->m_3DLineOfSightColor;
			ESPSettingsEditMenuVars::m_Vars.m_Rainbow = &ESPMenuVars::m_Vars.m_ESP->m_3DRainbow;
			ESPSettingsEditMenuVars::m_Vars.m_LineOfSightRainbow = &ESPMenuVars::m_Vars.m_ESP->m_3DLineOfSightRainbow;
			ESPSettingsEditMenuVars::m_Vars.m_RainbowToggle = &ESPMenuVars::m_Vars.m_ESP->m_3DRainbowToggle;
			ESPSettingsEditMenuVars::m_Vars.m_LineOfSightToggle = &ESPMenuVars::m_Vars.m_ESP->m_3DLineOfSightRainbowToggle;
		}));
}

void ESPSettingsMenu::Update() {}

/*Called once on submenu open*/
void ESPSettingsMenu::UpdateOnce() {}

/*Called always*/
void ESPSettingsMenu::FeatureUpdate() {}

ESPSettingsMenu* _instance;
ESPSettingsMenu* ESPSettingsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ESPSettingsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

ESPSettingsMenu::~ESPSettingsMenu() { delete _instance; }