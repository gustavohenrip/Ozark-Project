#include "selected_translation.hpp"
#include "menu/base/submenu_handler.hpp"
#include "translations.hpp"
#include "utils/config.hpp"
#include "utils/va.hpp"
#include "utils/json.hpp"
#include "menu/base/util/panel_manager.hpp"

using namespace selectedTranslationMenuVars;

namespace selectedTranslationMenuVars {
	vars_ vars;
}

void selectedTranslationMenu::init() {
	setName("Translations");
	setParentSubmenu<translationMenu>();

	addOption(buttonOption("Load")
		.addTranslation()
		.addOnClick([] { translationMenuVars::load(translationMenuVars::vars.cachedTranslations[vars.selectedTranslation].first.c_str()); }));

	addOption(buttonOption("Delete")
		.addTranslation()
		.addOnClick([] {
			remove(utils::va::va("%s%s.json", utils::getConfig()->getTranslationPath(), translationMenuVars::vars.cachedTranslations[vars.selectedTranslation].first.c_str()));
			menu::getSubmenuHandler()->setSubmenuPrevious(true);
		}));

	addOption(buttonOption("Set as Load Default")
		.addTranslation()
		.addOnClick([] {
		utils::getConfig()->writeString("Launch", "Translation", translationMenuVars::vars.cachedTranslations[vars.selectedTranslation].first.c_str());
	}));
}

/*Called while in submenu*/
void selectedTranslationMenu::update() {
	menu::getPanelManager()->panelFile(translationMenuVars::vars.cachedTranslations[vars.selectedTranslation].first, translationMenuVars::vars.cachedTranslations[vars.selectedTranslation].second);
}

/*Called once on submenu open*/
void selectedTranslationMenu::updateOnce() {
	setName(translationMenuVars::vars.cachedTranslations[vars.selectedTranslation].first, false);
}

/*Called always*/
void selectedTranslationMenu::featureUpdate() {}

selectedTranslationMenu* _instance;
selectedTranslationMenu* selectedTranslationMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new selectedTranslationMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

selectedTranslationMenu::~selectedTranslationMenu() { delete _instance; }