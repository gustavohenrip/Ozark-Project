#include "misc_heads_up_display.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../misc.hpp"

using namespace miscHeadsUpDisplayMenuVars;

namespace miscHeadsUpDisplayMenuVars {
	vars_ vars;
}

void miscHeadsUpDisplayMenu::init() {
	setName("Heads up Display");
	setParentSubmenu<miscMenu>();

	addOption(toggleOption("Disable Minimap")
		.addTranslation()
		.addToggle(vars.disableMinimap).canBeSaved(name.getOriginal())
		.addOnClick([] { if (!vars.disableMinimap) native::displayRadar(TRUE); }));

	addOption(toggleOption("Disable HUD")
		.addTranslation()
		.addToggle(vars.disableHUD).canBeSaved(name.getOriginal())
		.addOnClick([] { if (!vars.disableHUD) native::displayHud(TRUE); }));
}

/*Called while in submenu*/
void miscHeadsUpDisplayMenu::update() {}

/*Called once on submenu open*/
void miscHeadsUpDisplayMenu::updateOnce() {}

/*Called always*/
void miscHeadsUpDisplayMenu::featureUpdate() {
	if (vars.disableMinimap) {
		native::displayRadar(FALSE);
	}

	if (vars.disableHUD) {
		native::displayHud(FALSE);
	}
}

miscHeadsUpDisplayMenu* _instance;
miscHeadsUpDisplayMenu* miscHeadsUpDisplayMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new miscHeadsUpDisplayMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

miscHeadsUpDisplayMenu::~miscHeadsUpDisplayMenu() { delete _instance; }