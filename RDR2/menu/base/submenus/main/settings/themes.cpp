#include "themes.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../settings.hpp"
#include "utils/json.hpp"
#include "utils/config.hpp"
#include "utils/va.hpp"
#include "selected_theme.hpp"
#include "menu/base/util/panel_manager.hpp"

using namespace themeMenuVars;

namespace themeMenuVars {
	vars_ vars;

	void saveTheme(const char* name) {
		bool dupe = false;
		int newDupe = 1;

		if (utils::getConfig()->doesFileExist(utils::va::va("%s%s.json", utils::getConfig()->getThemePath(), name))) {
			// one exists, let's find if there's more that we named (1), (2) etc
			for (int i = 1; i < 10; i++) {
				if (utils::getConfig()->doesFileExist(utils::va::va("%s%s (%i).json", utils::getConfig()->getThemePath(), name, i))) {
					newDupe = i + 1;
				} else {
					if (i != 1) {
						dupe = true;
						break;
					}
				}
			}
		}

		std::ofstream output;
		if (dupe) output = std::ofstream(utils::va::va("%s%s (%i).json", utils::getConfig()->getThemePath(), name, newDupe));
		else output = std::ofstream(utils::va::va("%s%s.json", utils::getConfig()->getThemePath(), name));

		nlohmann::json temp;
		temp["position"]["x"] = global::uivars::menuX;
		temp["position"]["y"] = global::uivars::menuY;

		temp["font"]["title"] = global::uivars::titleFont;
		temp["font"]["header"] = global::uivars::headerFont;
		temp["font"]["option"] = global::uivars::optionFont;
		temp["font"]["break"] = global::uivars::breakFont;

		temp["color"]["title"] = utils::getConfig()->createColorString(global::uivars::titleText);
		temp["color"]["menu subtitle"] = utils::getConfig()->createColorString(global::uivars::menuHeaderText);
		temp["color"]["open tooltip"] = utils::getConfig()->createColorString(global::uivars::openToolTip);
		temp["color"]["break"] = utils::getConfig()->createColorString(global::uivars::breakText);
		temp["color"]["main"] = utils::getConfig()->createColorString(global::uivars::mainText);
		temp["color"]["selected"] = utils::getConfig()->createColorString(global::uivars::selectedText);
		temp["color"]["toggle on"] = utils::getConfig()->createColorString(global::uivars::toggleOn);
		temp["color"]["toggle off"] = utils::getConfig()->createColorString(global::uivars::toggleOff);
		temp["color"]["submenu indicator"] = utils::getConfig()->createColorString(global::uivars::submenuIndicator);
		temp["color"]["title header"] = utils::getConfig()->createColorString(global::uivars::titleHeader);
		temp["color"]["menu header"] = utils::getConfig()->createColorString(global::uivars::menuHeader);
		temp["color"]["menu footer"] = utils::getConfig()->createColorString(global::uivars::menuFooter);
		temp["color"]["menu footer arrows"] = utils::getConfig()->createColorString(global::uivars::menuFooterArrows);
		temp["color"]["scroller"] = utils::getConfig()->createColorString(global::uivars::scroller);
		temp["color"]["background"] = utils::getConfig()->createColorString(global::uivars::background);

		output << temp.dump(4);
		output.close();

		themeMenu::getInstance()->update();
	}
}

void themeMenu::init() {
	setName("Themes");
	setParentSubmenu<settingsMenu>();

	addOption(buttonOption("Save Theme")
		.addTranslation()
		.addKeyboard("Enter theme name", 30, [] (const char* name) {
			if (name) saveTheme(name);
		}));

	addOption(breakOption("Themes")
		.addTranslation());
}

/*Called while in submenu*/
void themeMenu::update() {
	clearOptionsOffset(2);

	vars.cachedThemes.clear();

	// show all with submenu -> load, delete, set as load default
	std::vector<std::string> themes = utils::getConfig()->getFilesInDirectory(utils::getConfig()->getThemePath(), ".json");
	if (themes.size()) {
		for (auto& theme : themes) {
			std::pair<std::string, struct stat> info;
			info.first = theme;

			if (stat(utils::va::va("%s%s.json", utils::getConfig()->getThemePath(), theme.c_str()), &info.second) == 0) {
				int index = (int)vars.cachedThemes.size();
				if (index < 0) index = 0;
				vars.cachedThemes.push_back(info);

				addOption(submenuOption("Load: " + theme)
					.addSubmenu<selectedThemeMenu>()
					.addOnClick([=] { selectedThemeMenuVars::vars.selectedTheme = index; })
					.addOnHover([=] { menu::getPanelManager()->panelFile(theme, info.second); }));
			}
		}
	} else {
		addOption(buttonOption("No themes found"));
	}
}

/*Called once on submenu open*/
void themeMenu::updateOnce() {}

/*Called always*/
void themeMenu::featureUpdate() {}

themeMenu* _instance;
themeMenu* themeMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new themeMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

themeMenu::~themeMenu() { delete _instance; }