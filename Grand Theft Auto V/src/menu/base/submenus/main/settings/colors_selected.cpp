#include "colors_selected.hpp"
#include "menu/base/submenu_handler.hpp"
#include "colors.hpp"

using namespace ColorSelectedMenuVars;

namespace ColorSelectedMenuVars {
	Vars_ m_Vars;

	ScrollStruct<Color> Presets[] = {
		{ TranslationString("Ozark Blue", true), Color(0x0095ffff) },
		{ TranslationString("Emerald", true), Color(0, 155, 119, 200) },
		{ TranslationString("Tangerine Tango", true), Color(221, 65, 36, 200) },
		{ TranslationString("Honeysucle", true), Color(214, 80, 118, 200) },
		{ TranslationString("Turquoise", true), Color(68, 184, 172 , 200) },
		{ TranslationString("Mimosa", true), Color(239, 192, 80 , 200) },
		{ TranslationString("Blue Izis", true), Color(91, 94, 166, 200) },
		{ TranslationString("Chili Pepper", true), Color(155, 35, 53, 200) },
		{ TranslationString("Sand Dollar", true), Color(223, 207, 190, 200) },
		{ TranslationString("Blue Turquoise", true), Color(85, 180, 176, 200) },
		{ TranslationString("Tigerlily", true), Color(225, 93, 68, 200) },
		{ TranslationString("Aqua Sky", true), Color(127, 205, 205, 200) },
		{ TranslationString("True Red", true), Color(188, 36, 60, 200) },
		{ TranslationString("Fuchsia Rose", true), Color(195, 68, 122, 200) },
		{ TranslationString("Cerulean Blue", true), Color(152, 180, 212, 200) },
		{ TranslationString("Galaxy Blue", true), Color(0x2A, 0x4B, 0x7C, 200) },
		{ TranslationString("Dark Cheddar", true), Color(0xE0, 0x81, 0x19, 200) },
		{ TranslationString("Orange Tiger", true), Color(0xF9, 0x67, 0x14, 200) },
		{ TranslationString("Turmeric", true), Color(0xFE, 0x84, 0x0E, 200) },
		{ TranslationString("Pink Peacock", true), Color(0xC6, 0x21, 0x68, 200) },
		{ TranslationString("Aspen Gold", true), Color(0xFF, 0xD6, 0x62, 200) },
		{ TranslationString("Eclipse", true), Color(0x34, 0x31, 0x48, 200) },
		{ TranslationString("Valiant Poppy", true), Color(0xBD, 0x3D, 0x3A, 200) },
		{ TranslationString("Nebulas Blue", true), Color(0x3F, 0x69, 0xAA, 200) },
		{ TranslationString("Ceylon Yellow", true), Color(0xD5, 0xAE, 0x41, 200) },
		{ TranslationString("Russet Orange", true), Color(0xE4, 0x7A, 0x2E, 200) },
		{ TranslationString("Quetzal Green", true), Color(0x00, 0x6E, 0x6D, 200) },
		{ TranslationString("Cherry Tomato", true), Color(0xE9, 0x4B, 0x3C, 200) },
		{ TranslationString("Meadowlark", true), Color(0xEC, 0xDB, 0x54, 200) },
		{ TranslationString("Baby Blue", true), Color(0x6F, 0x9F, 0xD8, 200) },
		{ TranslationString("Arcadia", true), Color(0x00, 0xA5, 0x91, 200) },
		{ TranslationString("Ultra Violet", true), Color(0x6B, 0x5B, 0x95, 200) },
		{ TranslationString("Lime Punch", true), Color(0xBF, 0xD6, 0x41, 200) },
		{ TranslationString("Harbor Mist", true), Color(0xB4, 0xB7, 0xBA, 200) },
		{ TranslationString("Grenadine", true), Color(0xDC, 0x4C, 0x46, 200) },
		{ TranslationString("Navy Peony", true), Color(0x22, 0x3A, 0x5E, 200) },
		{ TranslationString("Marina", true), Color(0x4F, 0x84, 0xC4, 200) },
		{ TranslationString("Autumn Maple", true), Color(0xD2, 0x69, 0x1E, 200) },
		{ TranslationString("Niagara", true), Color(0x57, 0x8C, 0xA9, 200) },
		{ TranslationString("Primrose Yellow", true), Color(0xF6, 0xD1, 0x55, 200) },
		{ TranslationString("Flame", true), Color(0xF2, 0x55, 0x2C, 200) },
		{ TranslationString("Lapis Blue", true), Color(0x00, 0x4B, 0x8D, 200) },
		{ TranslationString("Island Paradise", true), Color(0x95, 0xDE, 0xE3, 200) },
		{ TranslationString("Pink Yarrow", true), Color(0xCE, 0x31, 0x75, 200) },
		{ TranslationString("Riverside", true), Color(0x4C, 0x6A, 0x92, 200) },
		{ TranslationString("Aurora Red", true), Color(0xB9, 0x3A, 0x32, 200) },
		{ TranslationString("Serenity", true), Color(0x91, 0xA8, 0xD0, 200) },
		{ TranslationString("Green Flash", true), Color(0x79, 0xC7, 0x53, 200) },
		{ TranslationString("Buttercup", true), Color(0xFA, 0xE0, 0x3C, 200) },
		{ TranslationString("Limpet Shell", true), Color(0x98, 0xDD, 0xDE, 200) }
	};
}

void ColorSelectedMenu::Init() {
	SetName("Color");
	SetParentSubmenu<ColorsMenu>();

	addOption(NumberOption<int>(SCROLL, "[R]ed")
		.addTranslation()
		.addNumber(m_Vars.m_Tmp.R, "%i", 1).addMin(0).addMax(255).canLoop()
		.addOnUpdate([] (NumberOption<int>* Option) { Option->addNumber(m_Vars.m_CurrentColor->R, "%i", 1); })
		.addOnHover([] { Menu::GetRenderer()->PreviewColor(*m_Vars.m_CurrentColor); }));

	addOption(NumberOption<int>(SCROLL, "[G]reen")
		.addTranslation()
		.addNumber(m_Vars.m_Tmp.G, "%i", 1).addMin(0).addMax(255).canLoop()
		.addOnUpdate([] (NumberOption<int>* Option) { Option->addNumber(m_Vars.m_CurrentColor->G, "%i", 1); })
		.addOnHover([] { Menu::GetRenderer()->PreviewColor(*m_Vars.m_CurrentColor); }));

	addOption(NumberOption<int>(SCROLL, "[B]lue")
		.addTranslation()
		.addNumber(m_Vars.m_Tmp.B, "%i", 1).addMin(0).addMax(255).canLoop()
		.addOnUpdate([] (NumberOption<int>* Option) { Option->addNumber(m_Vars.m_CurrentColor->B, "%i", 1); })
		.addOnHover([] { Menu::GetRenderer()->PreviewColor(*m_Vars.m_CurrentColor); }));

	addOption(NumberOption<int>(SCROLL, "Opacity")
		.addTranslation()
		.addNumber(m_Vars.m_Tmp.A, "%i", 1).addMin(0).addMax(255).canLoop()
		.addOnUpdate([] (NumberOption<int>* Option) { Option->addNumber(m_Vars.m_CurrentColor->A, "%i", 1); })
		.addOnHover([] { Menu::GetRenderer()->PreviewColor(*m_Vars.m_CurrentColor); }));

	addOption(BreakOption("Presets")
		.addTranslation());
}

void ColorSelectedMenu::Update() {}

/*Called once on submenu open*/
void ColorSelectedMenu::UpdateOnce() {
	ClearOptionsOffset(5);

	for (auto& Preset : Presets) {
		addOption(ButtonOption(Preset.m_Name.Get())
			.addTranslation()
			.addOnClick([=] { *m_Vars.m_CurrentColor = Preset.m_Result; })
			.addOnHover([=] { Menu::GetRenderer()->PreviewColor(Preset.m_Result); }));
	}
}

/*Called always*/
void ColorSelectedMenu::FeatureUpdate() {}

ColorSelectedMenu* _instance;
ColorSelectedMenu* ColorSelectedMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ColorSelectedMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

ColorSelectedMenu::~ColorSelectedMenu() { delete _instance; }