#include "world_advanced_ui_color.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../world_advanced_ui.hpp"
#include "menu/base/util/instructionals.hpp"
#include "menu/base/submenus/main/helpers/rainbow_config.hpp"

using namespace WorldAdvancedUIColorMenuVars;

namespace WorldAdvancedUIColorMenuVars {
	Vars_ m_Vars;

	Math::Vector3_<float>* Col;
}

void WorldAdvancedUIColorMenu::Init() {
	SetName("Color");
	SetParentSubmenu<WorldAdvancedUIMenu>();

	m_DefaultInstructionals = false;

	addString("[R]ed");
	addString("[G]reen");
	addString("[B]lue");
	addString("Rainbow");
	addString("Save to Config");
	addString("Reset to Default");
}

void WorldAdvancedUIColorMenu::Update() {
	if (Menu::GetBase()->m_CurrentOption == 3) {
		Instructionals::Rainbow();

		if (Menu::Util::GetInput()->IsJustPressed(false, ControlFrontendX)) {
			if (m_Vars.m_Color != nullptr) {
				RainbowConfigMenuVars::m_Vars.m_Rainbow = &m_Vars.m_Color->m_Rainbow;
			} else {
				RainbowConfigMenuVars::m_Vars.m_Rainbow = &m_Vars.m_Color2->m_Rainbow;
			}

			RainbowConfigMenu::GetInstance()->SetParentSubmenu<WorldAdvancedUIColorMenu>();
			Menu::GetSubmenuHandler()->SetSubmenu(RainbowConfigMenu::GetInstance());
		}
	} else Instructionals::Standard();
}

/*Called once on submenu open*/
void WorldAdvancedUIColorMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	if (m_Vars.m_Color != nullptr) {
		Col = m_Vars.m_Color->m_Color;
	} else {
		Col = &m_Vars.m_Color2->m_Color->m_Color;
	}

	addOption(NumberOption<float>(SCROLL, getString("[R]ed"))
		.addNumber(Col->m_X, "%.2f", 0.01f).addMin(-100000.f).addMax(100000.f).canLoop().setScrollSpeed(5)
		.addOnUpdate([=] (NumberOption<float>* Option) { Option->addNumber(Col->m_X, "%.2f", 0.01f); }));

	addOption(NumberOption<float>(SCROLL, getString("[G]reen"))
		.addNumber(Col->m_Y, "%.2f", 0.01f).addMin(-100000.f).addMax(100000.f).canLoop().setScrollSpeed(5)
		.addOnUpdate([=] (NumberOption<float>* Option) { Option->addNumber(Col->m_Y, "%.2f", 0.01f); }));

	addOption(NumberOption<float>(SCROLL, getString("[B]lue"))
		.addNumber(Col->m_Z, "%.2f", 0.01f).addMin(-100000.f).addMax(100000.f).canLoop().setScrollSpeed(5)
		.addOnUpdate([=] (NumberOption<float>* Option) { Option->addNumber(Col->m_Z, "%.2f", 0.01f); }));

	addOption(ToggleOption(getString("Rainbow"))
		.addToggle(m_Vars.m_Color != nullptr ? m_Vars.m_Color->m_RainbowToggle : m_Vars.m_Color2->m_RainbowToggle)
		.addOnUpdate([] (ToggleOption* Option) { Option->addToggle(m_Vars.m_Color != nullptr ? m_Vars.m_Color->m_RainbowToggle : m_Vars.m_Color2->m_RainbowToggle); })
		.addOnClick([] {
			if (m_Vars.m_Color != nullptr) {
				m_Vars.m_Color->m_Rainbow.Toggle(m_Vars.m_Color->m_RainbowToggle);
				if (m_Vars.m_Color->m_RainbowToggle) {
					m_Vars.m_Color->m_Rainbow.Add(&m_Vars.m_Color->m_InternalColor);
				} else {
					m_Vars.m_Color->m_Rainbow.Remove(&m_Vars.m_Color->m_InternalColor);
				}
			} else {
				m_Vars.m_Color2->m_Rainbow.Toggle(m_Vars.m_Color2->m_RainbowToggle);
				if (m_Vars.m_Color2->m_RainbowToggle) {
					m_Vars.m_Color2->m_Rainbow.Add(&m_Vars.m_Color2->m_InternalColor);
				} else {
					m_Vars.m_Color2->m_Rainbow.Remove(&m_Vars.m_Color2->m_InternalColor);
				}
			}
		}));

	addOption(ToggleOption(getString("Save to Config"))
		.addToggle(m_Vars.m_Color != nullptr ? m_Vars.m_Color->m_SaveToConfig : m_Vars.m_Color2->m_SaveToConfig)
		.addOnUpdate([] (ToggleOption* Option) { Option->addToggle(m_Vars.m_Color != nullptr ? m_Vars.m_Color->m_SaveToConfig : m_Vars.m_Color2->m_SaveToConfig); }));

	addOption(ButtonOption(getString("Reset to Default"))
		.addTranslation()
		.addSprite("Ozark", "spinner")
		.addSpriteScale(0.01232f, 0.02184f)
		.addSpriteRotate()
		.addOnClick([=] {
			if (m_Vars.m_Color != nullptr) {
				m_Vars.m_Color->m_RainbowToggle = false;
				m_Vars.m_Color->m_Rainbow.Stop();

				Col->m_X = m_Vars.m_Color->m_DefaultColor.m_X;
				Col->m_Y = m_Vars.m_Color->m_DefaultColor.m_Y;
				Col->m_Z = m_Vars.m_Color->m_DefaultColor.m_Z;
			} else {
				m_Vars.m_Color2->m_RainbowToggle = false;
				m_Vars.m_Color2->m_Rainbow.Stop();

				memcpy(m_Vars.m_Color2, &m_Vars.m_Color2->m_DefaultColor, sizeof(Rage::Types::UIElementColorStruct));
			}
		}));
}

/*Called always*/
void WorldAdvancedUIColorMenu::FeatureUpdate() {}

WorldAdvancedUIColorMenu* _instance;
WorldAdvancedUIColorMenu* WorldAdvancedUIColorMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WorldAdvancedUIColorMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WorldAdvancedUIColorMenu::~WorldAdvancedUIColorMenu() { delete _instance; }