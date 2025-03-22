#include "world_advanced_ui_cross_multiplier.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../world_advanced_ui.hpp"

using namespace WorldAdvancedUICrossMultiplierMenuVars;

namespace WorldAdvancedUICrossMultiplierMenuVars {
	Vars_ m_Vars;
}

void WorldAdvancedUICrossMultiplierMenu::Init() {
	SetName("UI: Cross Multiplier");
	SetParentSubmenu<WorldAdvancedUIMenu>();

	memcpy(&m_Vars.m_RimLight, &Global::Vars::g_UIVisualSettings->m_RimLight, sizeof(Vars_));

	addOption(NumberOption<float>(SCROLL, "Rim Light")
		.addTranslation()
		.addNumber(Global::Vars::g_UIVisualSettings->m_RimLight, "%.2f", 0.01f).setScrollSpeed(10).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Global Environment Reflection")
		.addTranslation()
		.addNumber(Global::Vars::g_UIVisualSettings->m_GlobalEnvironmentReflection, "%.2f", 0.01f).setScrollSpeed(10).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Gamma")
		.addTranslation()
		.addNumber(Global::Vars::g_UIVisualSettings->m_Gamma, "%.2f", 0.01f).setScrollSpeed(10).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Mid Blur")
		.addTranslation()
		.addNumber(Global::Vars::g_UIVisualSettings->m_MidBlur, "%.2f", 0.01f).setScrollSpeed(10).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Far Blur")
		.addTranslation()
		.addNumber(Global::Vars::g_UIVisualSettings->m_FarBlur, "%.2f", 0.01f).setScrollSpeed(10).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Sky Multiplier")
		.addTranslation()
		.addNumber(Global::Vars::g_UIVisualSettings->m_SkyMultiplier, "%.2f", 0.01f).setScrollSpeed(10).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Desaturation")
		.addTranslation()
		.addNumber(Global::Vars::g_UIVisualSettings->m_Desaturation, "%.2f", 0.01f).setScrollSpeed(10).canBeSaved(m_ParentNameStack));

	addOption(ButtonOption("Reset to Default")
		.addTranslation()
		.addSprite("Ozark", "spinner")
		.addSpriteScale(0.01232f, 0.02184f)
		.addSpriteRotate()
		.addOnClick([this] {
			memcpy(&Global::Vars::g_UIVisualSettings->m_RimLight, &m_Vars.m_RimLight, sizeof(Vars_));

			for (auto& O : m_Options) {
				if (strcmp(O->GetName().GetOriginal().c_str(), "Reset to Default")) {
					O->InvokeSave(m_ParentNameStack);
				}
			}
		}));
}

void WorldAdvancedUICrossMultiplierMenu::Update() {}

/*Called once on submenu open*/
void WorldAdvancedUICrossMultiplierMenu::UpdateOnce() {}

/*Called always*/
void WorldAdvancedUICrossMultiplierMenu::FeatureUpdate() {}

WorldAdvancedUICrossMultiplierMenu* _instance;
WorldAdvancedUICrossMultiplierMenu* WorldAdvancedUICrossMultiplierMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WorldAdvancedUICrossMultiplierMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WorldAdvancedUICrossMultiplierMenu::~WorldAdvancedUICrossMultiplierMenu() { delete _instance; }