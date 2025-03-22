#include "menu_position.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../settings.hpp"


using namespace positionMenuVars;

namespace positionMenuVars {
	vars_ vars;
	
}

void positionMenu::init() {
	setName("Position");
	setParentSubmenu<settingsMenu>();

	addOption(numberOption<float>(SCROLL, "Position X")
		.addTranslation()
		.addNumber(global::uivars::menuX, "%.02f", .01f).addMin(0.f). addMax(1.f).setScrollSpeed(2).canLoop());

	addOption(numberOption<float>(SCROLL, "Position Y")
		.addTranslation()
		.addNumber(global::uivars::menuY, "%.02f", .01f).addMin(0.f).addMax(1.f).setScrollSpeed(2).canLoop());
}

/*Called while in submenu*/
void positionMenu::update() {}

/*Called once on submenu open*/
void positionMenu::updateOnce() {}

/*Called always*/
void positionMenu::featureUpdate() {}

positionMenu* _instance;
positionMenu* positionMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new positionMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

positionMenu::~positionMenu() { delete _instance; }