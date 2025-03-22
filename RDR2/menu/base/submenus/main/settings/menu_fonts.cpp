#include "menu_fonts.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../settings.hpp"

using namespace fontMenuVars;

namespace fontMenuVars {
	vars_ vars;
}

void fontMenu::init() {
	setName("Fonts");
	setParentSubmenu<settingsMenu>();

	addOption(numberOption<int>(SCROLL, "Title Font")
		.addTranslation()
	.addNumber(global::uivars::titleFont, "%i", 1).addMin(0).addMax(7));

	addOption(numberOption<int>(SCROLL, "Header Font")
		.addTranslation()
		.addNumber(global::uivars::headerFont, "%i", 1).addMin(0).addMax(7));

	addOption(numberOption<int>(SCROLL, "Option Font")
		.addTranslation()
		.addNumber(global::uivars::optionFont, "%i", 1).addMin(0).addMax(7));

	addOption(numberOption<int>(SCROLL, "Break Font")
		.addTranslation()
		.addNumber(global::uivars::breakFont, "%i", 1).addMin(0).addMax(7));
}

/*Called while in submenu*/
void fontMenu::update() {}

/*Called once on submenu open*/
void fontMenu::updateOnce() {}

/*Called always*/
void fontMenu::featureUpdate() {}

fontMenu* _instance;
fontMenu* fontMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new fontMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

fontMenu::~fontMenu() { delete _instance; }