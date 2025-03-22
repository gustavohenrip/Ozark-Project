#include "colors.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../settings.hpp"
#include "colors_selected.hpp"

using namespace ColorsMenuVars;

namespace ColorsMenuVars {
	Vars_ m_Vars;

	ScrollStruct<Color*> Colors[] = {
		{ TranslationString("Title", true), &Global::UIVars::g_TitleTextColor },
		{ TranslationString("Subtitle", true), &Global::UIVars::g_MenuHeaderTextColor },
		{ TranslationString("Open Tooltip", true), &Global::UIVars::g_OpenToolTipColor },
		{ TranslationString("Break", true), &Global::UIVars::g_BreakTextColor },
		{ TranslationString("Main", true), &Global::UIVars::g_MainTextColor },
		{ TranslationString("Selected", true), &Global::UIVars::g_SelectedTextColor },
		{ TranslationString("Toggle - On", true), &Global::UIVars::g_ToggleOnColor },
		{ TranslationString("Toggle - Off", true), &Global::UIVars::g_ToggleOffColor },
		{ TranslationString("Header - Subtitle", true), &Global::UIVars::g_MenuHeaderColor },
		{ TranslationString("Footer", true), &Global::UIVars::g_MenuFooterColor },
		{ TranslationString("Scroller", true), &Global::UIVars::g_ScrollerColor },
		{ TranslationString("Background", true), &Global::UIVars::g_BackgroundColor },
		{ TranslationString("Header", true), &Global::UIVars::g_TitleHeaderColor },
		{ TranslationString("Notification - Success", true), &Global::UIVars::g_NotificationSuccess },
		{ TranslationString("Notification - Error", true), &Global::UIVars::g_NotificationError },
	};
}

void ColorsMenu::Init() {
	SetName("Colors");
	SetParentSubmenu<SettingsMenu>();

	for (int i = 0; i < NUMOF(Colors); i++) {
		addOption(SubmenuOption(Colors[i].m_Name.Get())
			.addTranslation()
			.addSubmenu<ColorSelectedMenu>()
			.addOnClick([=] {
				ColorSelectedMenuVars::m_Vars.m_CurrentColor = Colors[i].m_Result;
				ColorSelectedMenu::GetInstance()->SetName(Colors[i].m_Name.Get(), false);
			}));
	}
}

void ColorsMenu::Update() {}

/*Called once on submenu open*/
void ColorsMenu::UpdateOnce() {}

/*Called always*/
void ColorsMenu::FeatureUpdate() {}

ColorsMenu* _instance;
ColorsMenu* ColorsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ColorsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

ColorsMenu::~ColorsMenu() { delete _instance; }