#include "fonts.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../settings.hpp"
#include "global/ui_vars.hpp"

using namespace FontsMenuVars;

namespace FontsMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> Fonts[] = {
		{ TranslationString("Chalet London", true), 0, },
		{ TranslationString("House", true), 1, },
		{ TranslationString("Mono Space", true), 2, },
		{ TranslationString("Chalet Comprime", true), 4, },
		{ TranslationString("Pricedown", true), 7 },
	};
}
 
void FontsMenu::Init() {
	SetName("Fonts");
	SetParentSubmenu<SettingsMenu>();

	addOption(ScrollOption<int>(SCROLL, "Title")
		.addTranslation()
		.addScroll(m_Vars.m_Fonts[0], 0, NUMOF(Fonts), Fonts)
		.addOnClick([] {Global::UIVars::g_TitleFont = Fonts[m_Vars.m_Fonts[0]].m_Result; }));

	addOption(ScrollOption<int>(SCROLL, "Header")
		.addTranslation()
		.addScroll(m_Vars.m_Fonts[1], 0, NUMOF(Fonts), Fonts)
		.addOnClick([] {Global::UIVars::g_HeaderFont = Fonts[m_Vars.m_Fonts[1]].m_Result; }));

	addOption(ScrollOption<int>(SCROLL, "Option")
		.addTranslation()
		.addScroll(m_Vars.m_Fonts[2], 0, NUMOF(Fonts), Fonts)
		.addOnClick([] {Global::UIVars::g_OptionFont = Fonts[m_Vars.m_Fonts[2]].m_Result; }));

	addOption(ScrollOption<int>(SCROLL, "Break")
		.addTranslation()
		.addScroll(m_Vars.m_Fonts[3], 0, NUMOF(Fonts), Fonts)
		.addOnClick([] {Global::UIVars::g_BreakFont = Fonts[m_Vars.m_Fonts[3]].m_Result; }));
}

void FontsMenu::Update() {}

/*Called once on submenu open*/
void FontsMenu::UpdateOnce() {
	for (int i = 0; i < NUMOF(Fonts); i++) {
		auto Yes = Fonts[i];

		if (Global::UIVars::g_TitleFont == Yes.m_Result) m_Vars.m_Fonts[0] = i;
		if (Global::UIVars::g_HeaderFont == Yes.m_Result) m_Vars.m_Fonts[1] = i;
		if (Global::UIVars::g_OptionFont == Yes.m_Result) m_Vars.m_Fonts[2] = i;
		if (Global::UIVars::g_BreakFont == Yes.m_Result) m_Vars.m_Fonts[3] = i;
	}
}

/*Called always*/
void FontsMenu::FeatureUpdate() {}

FontsMenu* _instance;
FontsMenu* FontsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new FontsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

FontsMenu::~FontsMenu() { delete _instance; }