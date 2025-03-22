#include "model_change.hpp"
#include "menu/base/submenu_handler.hpp"
#include "model.hpp"

using namespace modelChangeMenuVars;

namespace modelChangeMenuVars {
	vars_ vars;
}

void modelChangeMenu::init() {
	setName("Model");
	setParentSubmenu<modelMenu>();
}

/*Called while in submenu*/
void modelChangeMenu::update() {}

/*Called once on submenu open*/
void modelChangeMenu::updateOnce() {
	clearOptionsOffset(0);

	if (vars.list) {
		for (int i = 0; i < vars.size; i++) {
			addOption(buttonOption(vars.list[i].name)	
				.addOnClick([=] { modelMenuVars::changeModel(vars.list[i].result); }));
		}
	} else {
		addOption(buttonOption("An error occured populating list :(")
			.addTranslation());
	}
}

/*Called always*/
void modelChangeMenu::featureUpdate() {}

modelChangeMenu* _instance;
modelChangeMenu* modelChangeMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new modelChangeMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

modelChangeMenu::~modelChangeMenu() { delete _instance; }