#include "esp_settings_edit.hpp"
#include "menu/base/submenu_handler.hpp"
#include "esp_settings.hpp"
#include "esp.hpp"

using namespace ESPSettingsEditMenuVars;

namespace ESPSettingsEditMenuVars {
	Vars_ m_Vars;

	void HandleOnClick() {
		if (ESPMenuVars::m_Vars.m_Player) {
			Menu::GetPlayerManager()->Foreach([] (Menu::PlayerVars& Player) {
				Player.m_ESP.m_NameScale = *m_Vars.m_Scale;
			});
		}
	}
}

void ESPSettingsEditMenu::Init() {
	SetName("Edit");
	SetParentSubmenu<ESPSettingsMenu>();
}

void ESPSettingsEditMenu::Update() {}

/*Called once on submenu open*/
void ESPSettingsEditMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	if (m_Vars.m_Scale) {
		addOption(NumberOption<float>(SCROLL, "Scale")
			.addNumber(*m_Vars.m_Scale, "%.2f", 0.01f).addMin(0.f).addMax(4.f)
			.addOnClick(HandleOnClick));
	}

	addOption(NumberOption<int>(SCROLL, "[R]ed")
		.addNumber(m_Vars.m_Color->R, "%i", 1).addMin(0).addMax(255).canLoop().setScrollSpeed(5));

	addOption(NumberOption<int>(SCROLL, "[G]reen")
		.addNumber(m_Vars.m_Color->G, "%i", 1).addMin(0).addMax(255).canLoop().setScrollSpeed(5));

	addOption(NumberOption<int>(SCROLL, "[B]lue")
		.addNumber(m_Vars.m_Color->B, "%i", 1).addMin(0).addMax(255).canLoop().setScrollSpeed(5));

	addOption(ToggleOption("Rainbow")
		.addToggle(*m_Vars.m_RainbowToggle)
		.addOnClick([] {
			m_Vars.m_Rainbow->Toggle(*m_Vars.m_RainbowToggle);
			if (*m_Vars.m_RainbowToggle) {
				m_Vars.m_Rainbow->Add(m_Vars.m_Color);
			} else {
				m_Vars.m_Rainbow->Remove(m_Vars.m_Color);
			}
		}));

	addOption(BreakOption("Line of Sight"));

	addOption(NumberOption<int>(SCROLL, "[R]ed")
		.addNumber(m_Vars.m_LineOfSightColor->R, "%i", 1).addMin(0).addMax(255).canLoop().setScrollSpeed(5));

	addOption(NumberOption<int>(SCROLL, "[G]reen")
		.addNumber(m_Vars.m_LineOfSightColor->G, "%i", 1).addMin(0).addMax(255).canLoop().setScrollSpeed(5));

	addOption(NumberOption<int>(SCROLL, "[B]lue")
		.addNumber(m_Vars.m_LineOfSightColor->B, "%i", 1).addMin(0).addMax(255).canLoop().setScrollSpeed(5));

	addOption(ToggleOption("Rainbow")
		.addToggle(*m_Vars.m_LineOfSightToggle)
		.addOnClick([] {
			m_Vars.m_LineOfSightRainbow->Toggle(*m_Vars.m_LineOfSightToggle);
			if (*m_Vars.m_LineOfSightToggle) {
				m_Vars.m_LineOfSightRainbow->Add(m_Vars.m_LineOfSightColor);
			} else {
				m_Vars.m_LineOfSightRainbow->Remove(m_Vars.m_LineOfSightColor);
			}
		}));
}

/*Called always*/
void ESPSettingsEditMenu::FeatureUpdate() {}

ESPSettingsEditMenu* _instance;
ESPSettingsEditMenu* ESPSettingsEditMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ESPSettingsEditMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

ESPSettingsEditMenu::~ESPSettingsEditMenu() { delete _instance; }