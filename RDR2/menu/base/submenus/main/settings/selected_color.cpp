#include "selected_color.hpp"
#include "menu/base/submenu_handler.hpp"
#include "menu_color.hpp"

using namespace selectedColorMenuVars;

namespace selectedColorMenuVars {
	vars_ vars;
	
	scrollStruct<color> presets[] = {
		{ "Ozark Blue", color(74, 127, 184, 200) },
		{ "Emerald", color(0, 155, 119, 200) },
		{ "Tangerine Tango", color(221, 65, 36, 200) },
		{ "Honeysucle", color(214, 80, 118, 200) },
		{ "Turquoise", color(68, 184, 172 , 200) },
		{ "Mimosa", color(239, 192, 80 , 200) },
		{ "Blue Izis", color(91, 94, 166, 200) },
		{ "Chili Pepper", color(155, 35, 53, 200) },
		{ "Sand Dollar", color(223, 207, 190, 200) },
		{ "Blue Turquoise", color(85, 180, 176, 200) },
		{ "Tigerlily", color(225, 93, 68, 200) },
		{ "Aqua Sky", color(127, 205, 205, 200) },
		{ "True Red", color(188, 36, 60, 200) },
		{ "Fuchsia Rose", color(195, 68, 122, 200) },
		{ "Cerulean Blue", color(152, 180, 212, 200) },
		{ "Galaxy Blue", color(0x2A, 0x4B, 0x7C, 200) },
		{ "Dark Cheddar", color(0xE0, 0x81, 0x19, 200) },
		{ "Orange Tiger", color(0xF9, 0x67, 0x14, 200) },
		{ "Turmeric", color(0xFE, 0x84, 0x0E, 200) },
		{ "Pink Peacock", color(0xC6, 0x21, 0x68, 200) },
		{ "Aspen Gold", color(0xFF, 0xD6, 0x62, 200) },
		{ "Eclipse", color(0x34, 0x31, 0x48, 200) },
		{ "Valiant Poppy", color(0xBD, 0x3D, 0x3A, 200) },
		{ "Nebulas Blue", color(0x3F, 0x69, 0xAA, 200) },
		{ "Ceylon Yellow", color(0xD5, 0xAE, 0x41, 200) },
		{ "Russet Orange", color(0xE4, 0x7A, 0x2E, 200) },
		{ "Quetzal Green", color(0x00, 0x6E, 0x6D, 200) },
		{ "Cherry Tomato", color(0xE9, 0x4B, 0x3C, 200) },
		{ "Meadowlark", color(0xEC, 0xDB, 0x54, 200) },
		{ "Baby Blue", color(0x6F, 0x9F, 0xD8, 200) },
		{ "Arcadia", color(0x00, 0xA5, 0x91, 200) },
		{ "Ultra Violet", color(0x6B, 0x5B, 0x95, 200) },
		{ "Lime Punch", color(0xBF, 0xD6, 0x41, 200) },
		{ "Harbor Mist", color(0xB4, 0xB7, 0xBA, 200) },
		{ "Grenadine", color(0xDC, 0x4C, 0x46, 200) },
		{ "Navy Peony", color(0x22, 0x3A, 0x5E, 200) },
		{ "Marina", color(0x4F, 0x84, 0xC4, 200) },
		{ "Autumn Maple", color(0xD2, 0x69, 0x1E, 200) },
		{ "Niagara", color(0x57, 0x8C, 0xA9, 200) },
		{ "Primrose Yellow", color(0xF6, 0xD1, 0x55, 200) },
		{ "Flame", color(0xF2, 0x55, 0x2C, 200) },
		{ "Lapis Blue", color(0x00, 0x4B, 0x8D, 200) },
		{ "Island Paradise", color(0x95, 0xDE, 0xE3, 200) },
		{ "Pink Yarrow", color(0xCE, 0x31, 0x75, 200) },
		{ "Riverside", color(0x4C, 0x6A, 0x92, 200) },
		{ "Aurora Red", color(0xB9, 0x3A, 0x32, 200) },
		{ "Serenity", color(0x91, 0xA8, 0xD0, 200) },
		{ "Green Flash", color(0x79, 0xC7, 0x53, 200) },
		{ "Buttercup", color(0xFA, 0xE0, 0x3C, 200) },
		{ "Limpet Shell", color(0x98, 0xDD, 0xDE, 200) },
	};
}

void selectedColorMenu::init() {
	setName("Color");
	setParentSubmenu<colorMenu>();

	addOption(numberOption<int>(SCROLL, "[R] Red")
		.addTranslation()
		.addNumber(vars.tmp.r, "%i", 1).addMin(0).addMax(255).setScrollSpeed(2).canLoop()
		.addOnUpdate([] (numberOption<int>* option) { option->addNumber(vars.currentColor->r, "%i", 1).addMin(0).addMax(255).setScrollSpeed(2).canLoop(); })
		.addOnHover([] { menu::getRenderer()->previewColor(*vars.currentColor); }));

	addOption(numberOption<int>(SCROLL, "[G] Green")
		.addTranslation()
		.addNumber(vars.tmp.g, "%i", 1).addMin(0).addMax(255).setScrollSpeed(2).canLoop()
		.addOnUpdate([] (numberOption<int>* option) { option->addNumber(vars.currentColor->g, "%i", 1).addMin(0).addMax(255).setScrollSpeed(2).canLoop(); })
		.addOnHover([] { menu::getRenderer()->previewColor(*vars.currentColor); }));

	addOption(numberOption<int>(SCROLL, "[B] Blue")
		.addTranslation()
		.addNumber(vars.tmp.b, "%i", 1).addMin(0).addMax(255).setScrollSpeed(2).canLoop()
		.addOnUpdate([] (numberOption<int>* option) { option->addNumber(vars.currentColor->b, "%i", 1).addMin(0).addMax(255).setScrollSpeed(2).canLoop(); })
		.addOnHover([] { menu::getRenderer()->previewColor(*vars.currentColor); }));

	addOption(numberOption<int>(SCROLL, "[A] Alpha")
		.addTranslation()
		.addNumber(vars.tmp.a, "%i", 1).addMin(0).addMax(255).setScrollSpeed(2).canLoop()
		.addOnUpdate([] (numberOption<int>* option) { option->addNumber(vars.currentColor->a, "%i", 1).addMin(0).addMax(255).setScrollSpeed(2).canLoop(); })
		.addOnHover([] { menu::getRenderer()->previewColor(*vars.currentColor); }));

	addOption(breakOption("Presets")
		.addTranslation());
}

/*Called while in submenu*/
void selectedColorMenu::update() {}

/*Called once on submenu open*/
void selectedColorMenu::updateOnce() {
	setName(vars.colorName);
	clearOptionsOffset(5);

	for (std::size_t i = 0; i < NUMOF(presets); i++) {
		addOption(buttonOption(presets[i].name)
			.addOnClick([=] { vars.currentColor->r = presets[i].result.r; vars.currentColor->g = presets[i].result.g; vars.currentColor->b = presets[i].result.b;})
			.addOnHover([=] { menu::getRenderer()->previewColor(presets[i].result); }));
	}
}

/*Called always*/
void selectedColorMenu::featureUpdate() {}

selectedColorMenu* _instance;
selectedColorMenu* selectedColorMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new selectedColorMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

selectedColorMenu::~selectedColorMenu() { delete _instance; }