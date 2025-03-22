#include "world_advanced_ui_puddles.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../world_advanced_ui.hpp"

using namespace WorldAdvancedUIPuddlesMenuVars;

namespace WorldAdvancedUIPuddlesMenuVars {
	Vars_ m_Vars;
}

void WorldAdvancedUIPuddlesMenu::Init() {
	SetName("Puddles");
	SetParentSubmenu<WorldAdvancedUIMenu>();

	if (!IsValidPtr(Global::Vars::g_UIPuddle) || !IsValidPtr(Global::Vars::g_UIPuddle->m_Ripples)) {
		addOption(ButtonOption("Error"));
		return;
	}

	memcpy(&m_Vars.m_Original, Global::Vars::g_UIPuddle, sizeof(Rage::Types::UIPuddle));
	memcpy(&m_Vars.m_OriginalRipple, Global::Vars::g_UIPuddle->m_Ripples, sizeof(Rage::Types::UIPuddleRipple));

	addOption(NumberOption<float>(SCROLL, "Scale")
		.addTranslation()
		.addNumber(Global::Vars::g_UIPuddle->m_Scale, "%.3f", 0.005f).addMin(-1000.f).addMax(1000.f).setScrollSpeed(0).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Range")
		.addTranslation()
		.addNumber(Global::Vars::g_UIPuddle->m_Range, "%.2f", 0.05f).addMin(-1000.f).addMax(1000.f).setScrollSpeed(0).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Depth")
		.addTranslation()
		.addNumber(Global::Vars::g_UIPuddle->m_Depth, "%.1f", 0.1f).addMin(-1000.f).addMax(1000.f).setScrollSpeed(0).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Depth Test")
		.addTranslation()
		.addNumber(Global::Vars::g_UIPuddle->m_DepthTest, "%.1f", 0.1f).addMin(-1000.f).addMax(1000.f).setScrollSpeed(0).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Reflectivity")
		.addTranslation()
		.addNumber(Global::Vars::g_UIPuddle->m_Reflectivity, "%.1f", 0.1f).addMin(-1000.f).addMax(1000.f).setScrollSpeed(0).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Ripple Min Size")
		.addTranslation()
		.addNumber(Global::Vars::g_UIPuddle->m_Ripples->m_RippleMinSize, "%.3f", 0.005f).addMin(-1000.f).addMax(1000.f).setScrollSpeed(0).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Ripple Max Size")
		.addTranslation()
		.addNumber(Global::Vars::g_UIPuddle->m_Ripples->m_RippleMaxSize, "%.3f", 0.005f).addMin(-1000.f).addMax(1000.f).setScrollSpeed(0).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Ripple Min Duration")
		.addTranslation()
		.addNumber(Global::Vars::g_UIPuddle->m_Ripples->m_RippleMinDuration, "%.1f", 0.1f).addMin(-1000.f).addMax(1000.f).setScrollSpeed(0).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Ripple Max Duration")
		.addTranslation()
		.addNumber(Global::Vars::g_UIPuddle->m_Ripples->m_RippleMaxDuration, "%.1f", 0.1f).addMin(-1000.f).addMax(1000.f).setScrollSpeed(0).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Ripple Speed")
		.addTranslation()
		.addNumber(Global::Vars::g_UIPuddle->m_Ripples->m_RippleSpeed, "%.1f", 0.1f).addMin(-1000.f).addMax(1000.f).setScrollSpeed(0).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Ripple Drop Factor")
		.addTranslation()
		.addNumber(Global::Vars::g_UIPuddle->m_Ripples->m_RippleDropFactor, "%.1f", 0.1f).addMin(-1000.f).addMax(1000.f).setScrollSpeed(0).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Ripple Wind Strength")
		.addTranslation()
		.addNumber(Global::Vars::g_UIPuddle->m_Ripples->m_RippleWindStrength, "%.2f", 0.05f).addMin(-1000.f).addMax(1000.f).setScrollSpeed(0).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Ripple Strength")
		.addTranslation()
		.addNumber(Global::Vars::g_UIPuddle->m_Ripples->m_RippleStrength, "%.1f", 0.1f).addMin(-1000.f).addMax(1000.f).setScrollSpeed(0).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Ripple Player Speed")
		.addTranslation()
		.addNumber(Global::Vars::g_UIPuddle->m_Ripples->m_RipplePlayerSpeed, "%.1f", 0.1f).addMin(-1000.f).addMax(1000.f).setScrollSpeed(0).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Ripple Player Map Size")
		.addTranslation()
		.addNumber(Global::Vars::g_UIPuddle->m_Ripples->m_RipplePlayerMapSize, "%.1f", 0.1f).addMin(-1000.f).addMax(1000.f).setScrollSpeed(0).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Ripple Player Map Size")
		.addTranslation()
		.addNumber(Global::Vars::g_UIPuddle->m_Ripples->m_RipplePlayerMapSize, "%.1f", 0.1f).addMin(-1000.f).addMax(1000.f).setScrollSpeed(0).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Ripple Wind Damping")
		.addTranslation()
		.addNumber(Global::Vars::g_UIPuddle->m_Ripples->m_RippleWindDamping, "%.2f", 0.05f).addMin(-1000.f).addMax(1000.f).setScrollSpeed(0).canBeSaved(m_ParentNameStack));

	addOption(ButtonOption("Reset to Default")
		.addTranslation()
		.addSprite("Ozark", "spinner")
		.addSpriteScale(0.01232f, 0.02184f)
		.addSpriteRotate()
		.addOnClick([this] {
			memcpy(Global::Vars::g_UIPuddle, &m_Vars.m_Original, sizeof(Rage::Types::UIPuddle));
			memcpy(Global::Vars::g_UIPuddle->m_Ripples, &m_Vars.m_OriginalRipple, sizeof(Rage::Types::UIPuddleRipple));

			for (auto& O : m_Options) {
				if (strcmp(O->GetName().GetOriginal().c_str(), "Reset to Default")) {
					O->InvokeSave(m_ParentNameStack);
				}
			}
		}));
}

void WorldAdvancedUIPuddlesMenu::Update() {}

/*Called once on submenu open*/
void WorldAdvancedUIPuddlesMenu::UpdateOnce() {}

/*Called always*/
void WorldAdvancedUIPuddlesMenu::FeatureUpdate() {}

WorldAdvancedUIPuddlesMenu* _instance;
WorldAdvancedUIPuddlesMenu* WorldAdvancedUIPuddlesMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WorldAdvancedUIPuddlesMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WorldAdvancedUIPuddlesMenu::~WorldAdvancedUIPuddlesMenu() { delete _instance; }