#include "menu_color.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../settings.hpp"
#include "selected_color.hpp"
#include "global/ui_vars.hpp"

using namespace colorMenuVars;

namespace colorMenuVars {
	vars_ vars;

	scrollStruct<color*> colors[] = {
		{ "Title Text", &global::uivars::titleText },
		{ "Header Text", &global::uivars::menuHeaderText },
		{ "Open Tooltip" ,&global::uivars::openToolTip },
		{ "Break Text", &global::uivars::breakText },
		{ "Option Text" , &global::uivars::mainText },
		{ "Selected text", &global::uivars::selectedText },
		{ "Toggle On", &global::uivars::toggleOn },
		{ "Toggle Off", &global::uivars::toggleOff },
		{ "Submenu Indicator", &global::uivars::submenuIndicator },
		{ "Title Header", &global::uivars::titleHeader },
		{ "Submenu Header", &global::uivars::menuHeader },
		{ "Submenu Footer", &global::uivars::menuFooter },
		{ "Submenu Footer Arrows", &global::uivars::menuFooterArrows },
		{ "Scroller", &global::uivars::scroller },
		{ "Background", &global::uivars::background },
	};
}

void colorMenu::init() {
	setName("Color");
	setParentSubmenu<settingsMenu>();

	for (std::size_t i = 0; i < NUMOF(colors); i++) {
		addOption(submenuOption(colors[i].name)
			.addOnClick([=] { selectedColorMenuVars::vars.currentColor = colors[i].result; selectedColorMenuVars::vars.colorName = colors[i].name; })
			.addSubmenu<selectedColorMenu>());
	}
}

/*Called while in submenu*/
void colorMenu::update() {}

/*Called once on submenu open*/
void colorMenu::updateOnce() {}

/*Called always*/
void colorMenu::featureUpdate() {}

colorMenu* _instance;
colorMenu* colorMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new colorMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

colorMenu::~colorMenu() { delete _instance; }