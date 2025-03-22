#include "search_results.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../main.hpp"

using namespace searchResultsMenuVars;

namespace searchResultsMenuVars {
	vars_ vars;
}

void searchResultsMenu::init() {
	setName("Search Results", true, false);
	setParentSubmenu<mainMenu>();
}

void searchResultsMenu::update() {}

/*Called once on submenu open*/
void searchResultsMenu::updateOnce() {}

/*Called always*/
void searchResultsMenu::featureUpdate() {}

searchResultsMenu* _instance;
searchResultsMenu* searchResultsMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new searchResultsMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

searchResultsMenu::~searchResultsMenu() { delete _instance; }