#include "selected_theme.hpp"
#include "menu/base/submenu_handler.hpp"
#include "themes.hpp"
#include "utils/config.hpp"
#include "utils/va.hpp"
#include "utils/json.hpp"
#include "menu/base/util/panel_manager.hpp"

using namespace selectedThemeMenuVars;

namespace selectedThemeMenuVars {
	vars_ vars;

	void loadTheme(const char* name) {
		std::ifstream file(name);
		if (file.good()) {
			nlohmann::json temp;
			file >> temp;
			file.close();

			if (!temp["position"].is_null()) {
				if (!temp["position"]["x"].is_null()) global::uivars::menuX = temp["position"]["x"].get<float>();
				if (!temp["position"]["y"].is_null()) global::uivars::menuY = temp["position"]["y"].get<float>();
			}

			if (!temp["font"].is_null()) {
				if (!temp["font"]["title"].is_null()) global::uivars::titleFont = temp["font"]["title"].get<int>();
				if (!temp["font"]["header"].is_null()) global::uivars::headerFont = temp["font"]["header"].get<int>();
				if (!temp["font"]["option"].is_null()) global::uivars::optionFont = temp["font"]["option"].get<int>();
				if (!temp["font"]["break"].is_null()) global::uivars::breakFont = temp["font"]["break"].get<int>();
			}

			if (!temp["color"].is_null()) {
				if (!temp["color"]["title"].is_null()) global::uivars::titleText = utils::getConfig()->parseColorString(temp["color"]["title"].get<std::string>());
				if (!temp["color"]["menu subtitle"].is_null()) global::uivars::menuHeaderText = utils::getConfig()->parseColorString(temp["color"]["menu subtitle"].get<std::string>());
				if (!temp["color"]["open tooltip"].is_null()) global::uivars::openToolTip = utils::getConfig()->parseColorString(temp["color"]["open tooltip"].get<std::string>());
				if (!temp["color"]["break"].is_null()) global::uivars::breakText = utils::getConfig()->parseColorString(temp["color"]["break"].get<std::string>());
				if (!temp["color"]["main"].is_null()) global::uivars::mainText = utils::getConfig()->parseColorString(temp["color"]["main"].get<std::string>());
				if (!temp["color"]["selected"].is_null()) global::uivars::selectedText = utils::getConfig()->parseColorString(temp["color"]["selected"].get<std::string>());
				if (!temp["color"]["toggle on"].is_null()) global::uivars::toggleOn = utils::getConfig()->parseColorString(temp["color"]["toggle on"].get<std::string>());
				if (!temp["color"]["toggle off"].is_null()) global::uivars::toggleOff = utils::getConfig()->parseColorString(temp["color"]["toggle off"].get<std::string>());
				if (!temp["color"]["submenu indicator"].is_null()) global::uivars::submenuIndicator = utils::getConfig()->parseColorString(temp["color"]["submenu indicator"].get<std::string>());
				if (!temp["color"]["title header"].is_null()) global::uivars::titleHeader = utils::getConfig()->parseColorString(temp["color"]["title header"].get<std::string>());
				if (!temp["color"]["menu header"].is_null()) global::uivars::menuHeader = utils::getConfig()->parseColorString(temp["color"]["menu header"].get<std::string>());
				if (!temp["color"]["menu footer"].is_null()) global::uivars::menuFooter = utils::getConfig()->parseColorString(temp["color"]["menu footer"].get<std::string>());
				if (!temp["color"]["menu footer arrows"].is_null()) global::uivars::menuFooterArrows = utils::getConfig()->parseColorString(temp["color"]["menu footer arrows"].get<std::string>());
				if (!temp["color"]["scroller"].is_null()) global::uivars::scroller = utils::getConfig()->parseColorString(temp["color"]["scroller"].get<std::string>());
				if (!temp["color"]["background"].is_null()) global::uivars::background = utils::getConfig()->parseColorString(temp["color"]["background"].get<std::string>());
			}
		}
	}
}

void selectedThemeMenu::init() {
	setName("Themes");
	setParentSubmenu<themeMenu>();

	addOption(buttonOption("Load")
		.addTranslation()
		.addOnClick([] { loadTheme(utils::va::va("%s%s.json", utils::getConfig()->getThemePath(), themeMenuVars::vars.cachedThemes[vars.selectedTheme].first.c_str())); }));

	addOption(buttonOption("Delete")
		.addTranslation()
		.addOnClick([] {
			remove(utils::va::va("%s%s.json", utils::getConfig()->getThemePath(), themeMenuVars::vars.cachedThemes[vars.selectedTheme].first.c_str()));
			menu::getSubmenuHandler()->setSubmenuPrevious(true);
		}));

	addOption(buttonOption("Set as Load Default")
		.addTranslation()
		.addOnClick([] {
			utils::getConfig()->writeString("Launch", "Theme", themeMenuVars::vars.cachedThemes[vars.selectedTheme].first.c_str());
		}));
}

/*Called while in submenu*/
void selectedThemeMenu::update() {
	menu::getPanelManager()->panelFile(themeMenuVars::vars.cachedThemes[vars.selectedTheme].first, themeMenuVars::vars.cachedThemes[vars.selectedTheme].second);
}

/*Called once on submenu open*/
void selectedThemeMenu::updateOnce() {
	setName(themeMenuVars::vars.cachedThemes[vars.selectedTheme].first, false);
}

/*Called always*/
void selectedThemeMenu::featureUpdate() {}

selectedThemeMenu* _instance;
selectedThemeMenu* selectedThemeMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new selectedThemeMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

selectedThemeMenu::~selectedThemeMenu() { delete _instance; }