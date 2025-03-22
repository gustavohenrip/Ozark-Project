#include "settings.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../main.hpp"
#include "settings/menu_color.hpp"
#include "settings/menu_position.hpp"
#include "settings/menu_fonts.hpp"
#include "settings/themes.hpp"
#include "settings/translations.hpp"
#include "settings/video_settings.hpp"
#include "menu/base/util/timers.hpp"
#include "global/auth_vars.hpp"
#include "utils/utils.hpp"
#include "menu/init.hpp"
#include "menu/base/submenus/main/network/ozark_detections.hpp"
#include "utils/log.hpp"

using namespace settingsMenuVars;

namespace settingsMenuVars {
	vars_ vars;
}

void settingsMenu::init() {
	setName("Settings");
	setParentSubmenu<mainMenu>();

	addOption(submenuOption("Translations")
		.addTranslation()
		.addSubmenu<translationMenu>()
		.addTooltip("Generate or select transaltion"));

	addOption(submenuOption("Themes")
		.addTranslation()
		.addSubmenu<themeMenu>()
		.addTooltip("Save/Load Ozark themes"));

	addOption(submenuOption("Color")
		.addTranslation()
		.addSubmenu<colorMenu>()
		.addTooltip("Create an Ozark Theme"));

	addOption(submenuOption("Position")
		.addTranslation()
		.addSubmenu<positionMenu>()
		.addTooltip("Edit menu placement"));

	addOption(submenuOption("Fonts")
		.addTranslation()
		.addSubmenu<fontMenu>());

	addOption(submenuOption("Video Setting")
		.addTranslation()
		.addSubmenu<videoSettingsMenu>()
		.addTooltip("Auto hide elements of the menu while streaming/recording"));

	addOption(submenuOption("Ozark Detection")
		.addTranslation()
		.addSubmenu<networkOzarkDetectsMenu>()
		.addTooltip("This is for what you want ozark users to see"));

	addOption(toggleOption("Open Tooltip")
		.addTranslation()
		.addToggle(menu::getBase()->openTooltip).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Show Information Panels")
		.addTranslation()
		.addToggle(menu::getBase()->panels).canBeSaved(name.getOriginal()));

	vars.timeButton = addOption(buttonOption(""));
}

/*Called while in submenu*/
void settingsMenu::update() {}

/*Called once on submenu open*/
void settingsMenu::updateOnce() {}

/*Called always*/
void settingsMenu::featureUpdate() {
	if (global::auth::g_hasLifetime) {
		vars.timeButton->setName("Time: Lifetime");
	} else {
		static int timer = 0;
		menu::util::getTimers()->runTimedFunction(&timer, 1000, [] {
			static char timeBuffer[100];

			if (global::auth::g_secondsLeft) {
				utils::secondsToTime t = utils::secondsToTime(global::auth::g_secondsLeft);

				sprintf(timeBuffer, "%sTime: %02d days %02d hours %02d mins %02d secs", (global::auth::g_secondsLeft % 2) ? "~q~" : "~e~", t.days, t.hours, t.minutes, t.seconds);
			} else {
				info("Unloading due to time being %i", global::auth::g_secondsLeft);
				menu::getInit()->unload();
			}

			global::auth::g_secondsLeft--;

			vars.timeButton->setName(timeBuffer);
		});
	}
}

settingsMenu* _instance;
settingsMenu* settingsMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new settingsMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

settingsMenu::~settingsMenu() { delete _instance; }