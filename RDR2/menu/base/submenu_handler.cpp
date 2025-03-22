#include "submenu_handler.hpp"
#include "base.hpp"
#include "menu/base/submenus/main.hpp"
#include "utils/va.hpp"
#include "submenus/main/settings/translations.hpp"
#include "submenus/main/settings/selected_theme.hpp"

namespace menu {
	void submenuHandler::initSubmenus() {
		mainSubmenu = currentSubmenu = mainMenu::getInstance(); // inits every submenu

		// all are init'd, load default shit
		auto trans = utils::getConfig()->readString("Launch", "Translation", "--");
		if (trans) {
			if (strcmp(trans, "--")) {
				translationMenuVars::load(trans);
			}
		}

		auto theme = utils::getConfig()->readString("Launch", "Theme", "--");
		if (theme) {
			if (strcmp(theme, "--")) {
				selectedThemeMenuVars::loadTheme(utils::va::va("%s%s.json", utils::getConfig()->getThemePath(), theme));
			}
		}
	}

	void submenuHandler::update() {
		if (currentSubmenu) {
			currentSubmenu->updateMenus();
			
			if (nextSubmenu) {
				getBase()->currentOption = nextCurrentOption;
				getBase()->scrollOffset = nextScrollOffset;
				currentSubmenu = nextSubmenu;
				nextSubmenu = nullptr;
				currentSubmenu->updateOnce();
			}
		}
	}

	void submenuHandler::asyncUpdate() {
		for (std::size_t i = 0; i < submenus.size(); i++) {
			submenus.at(i)->featureUpdate();
		}
	}

	void submenuHandler::addSubmenu(submenu* sub) {
		submenus.push_back(sub);
	}

	void submenuHandler::setSubmenu(submenu* sub) {
		previousSubmenu = currentSubmenu;
		currentSubmenu->oldCurrentOption = getBase()->currentOption;
		currentSubmenu->oldScrollOffset = getBase()->scrollOffset;
		nextCurrentOption = 0;
		nextScrollOffset = 0;
		nextSubmenu = sub;
	}
	
	void submenuHandler::setSubmenuPrevious(bool reset) {
		previousSubmenu = currentSubmenu;
		if (currentSubmenu->parent == nullptr) {
			// main menu
			nextSubmenu = mainSubmenu;
			nextCurrentOption = 0;
			nextScrollOffset = 0;
			menu::getBase()->open = false;
		} else {
			nextSubmenu = currentSubmenu->parent;

			if (reset) {
				nextCurrentOption = 0;
				nextScrollOffset = 0;
			} else {
				nextCurrentOption = nextSubmenu->oldCurrentOption;
				nextScrollOffset = nextSubmenu->oldScrollOffset;
			}
		}
	}

	submenuHandler* getSubmenuHandler() {
		static submenuHandler instance;
		return &instance;
	}
}