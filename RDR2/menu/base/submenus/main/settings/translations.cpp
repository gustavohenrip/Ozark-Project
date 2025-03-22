#include "translations.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../settings.hpp"
#include "utils/json.hpp"
#include "utils/config.hpp"
#include "utils/va.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "menu/base/util/fiber_pool.hpp"
#include "menu/base/util/notify.hpp"
#include "selected_translation.hpp"

using namespace translationMenuVars;

namespace translationMenuVars {
	vars_ vars;

	void createTemplate() {
		std::ofstream output(utils::va::va("%sTemplate.json", utils::getConfig()->getTranslationPath()));
		if (output.good()) {
			nlohmann::json json;

			for (auto& submenu : menu::getSubmenuHandler()->getSubmenus()) {
				if (submenu) {
					std::string subName = submenu->name.getOriginal();
					for (auto& option : submenu->options) {
						if (option->getName().hasTranslation() && !option->getName().getOriginal().empty()) {
							json["strings"][subName][option->getName().getOriginal()] = option->getName().getOriginal();
						}

						if (option->getTooltip().hasTranslation() && !option->getTooltip().getOriginal().empty()) {
							json["strings"][subName][option->getTooltip().getOriginal()] = option->getTooltip().getOriginal();
						}
					}

					if (submenu->name.hasTranslation()) {
						json["strings"]["subs"][submenu->name.getOriginal()] = submenu->name.getOriginal();
					}

					for (auto& str : submenu->getStrings()) {
						if (str.hasTranslation()) {
							json["strings"][subName][str.getOriginal()] = str.getOriginal();
						}
					}
				}
			}

			output << json.dump(4);
			output.close();
		}

		translationMenu::getInstance()->updateOnce();
	}

	void reset() {
		for (auto& submenu : menu::getSubmenuHandler()->getSubmenus()) {
			std::string subName = submenu->name.getOriginal();
			for (auto& option : submenu->options) {
				option->getName().reset();
				option->getTooltip().reset();
			}

			if (submenu->name.hasTranslation()) {
				submenu->name.reset();
			}

			for (auto& str : submenu->getStrings()) str.reset();
		}
	}

	void load(const char* file) {
		try {
			std::ifstream input(utils::va::va("%s%s.json", utils::getConfig()->getTranslationPath(), file));
			if (input.good()) {
				nlohmann::json json = nlohmann::json::parse(input);

				for (auto& submenu : menu::getSubmenuHandler()->getSubmenus()) {
					std::string subName = submenu->name.getOriginal();
					for (auto& option : submenu->options) {
						if (option->getName().hasTranslation()) {
							option->getName().setMapped(jsonGet<std::string>(json["strings"][subName], option->getName().getOriginal(), option->getName().getOriginal()).c_str());
						}

						if (option->getTooltip().hasTranslation()) {
							option->getTooltip().setMapped(jsonGet<std::string>(json["strings"][subName], option->getTooltip().getOriginal(), option->getTooltip().getOriginal()).c_str());
						}
					}

					if (submenu->name.hasTranslation()) {
						if (!json["strings"]["subs"][subName].is_null()) {
							submenu->name.setMapped(json["strings"]["subs"][subName].get<std::string>().c_str());
						}
					}

					for (auto& str : submenu->getStrings()) {
						if (str.hasTranslation()) {
							str.setMapped(jsonGet<std::string>(json["strings"][subName], str.getOriginal(), str.getOriginal()).c_str());
						}
					}
				}
			} else {
				menu::getNotify()->right("~e~Error~q~: Failed to open translation file");
			}
		} catch (nlohmann::json::parse_error & e) {
			if (e.id == 101) {
				menu::getNotify()->right("~e~Error~q~: Failed to verify translation encoding. Please ensure translation is UTF-8.");
			} else {
				menu::getNotify()->right(utils::va::va("~e~Error~q~: Failed to load translation: %i", e.id));
			}
		} catch (std::exception & e) {
			menu::getNotify()->right(utils::va::va("~e~Error~q~: Failed to load translation. Exception: %s", e.what()));
		}
	}

	void updateExisting(const char* name) {
		nlohmann::json current;

		std::ifstream input(utils::va::va("%s%s.json", utils::getConfig()->getTranslationPath(), name));
		if (input.good()) {
			input >> current;
			input.close();

			for (auto& submenu : menu::getSubmenuHandler()->getSubmenus()) {
				if (submenu) {
					std::string subName = submenu->name.getOriginal();
					for (auto& option : submenu->options) {
						if (option->getName().hasTranslation() && !option->getName().getOriginal().empty()) {
							if (current["strings"][subName][option->getName().getOriginal()].is_null()) {
								current["strings"][subName][option->getName().getOriginal()] = option->getName().getOriginal();
							}
						}

						if (option->getTooltip().hasTranslation() && !option->getTooltip().getOriginal().empty()) {
							if (current["strings"][subName][option->getTooltip().getOriginal()].is_null()) {
								current["strings"][subName][option->getTooltip().getOriginal()] = option->getTooltip().getOriginal();
							}
						}
					}

					if (submenu->name.hasTranslation()) {
						if (current["strings"]["subs"][submenu->name.getOriginal()].is_null()) {
							current["strings"]["subs"][submenu->name.getOriginal()] = submenu->name.getOriginal();
						}
					}

					for (auto& str : submenu->getStrings()) {
						if (str.hasTranslation()) {
							if (current["strings"][subName][str.getOriginal()].is_null()) {
								current["strings"][subName][str.getOriginal()] = str.getOriginal();
							}
						}
					}
				}
			}

			std::ofstream output(utils::va::va("%s%s.json", utils::getConfig()->getTranslationPath(), name));
			if (output.good()) {
				output << current.dump(4);
				output.close();
			}
		}

		translationMenu::getInstance()->updateOnce();
	}
}

void translationMenu::init() {
	setName("Translations");
	setParentSubmenu<settingsMenu>();

	addOption(buttonOption("Create Template Translation")
		.addTranslation()
		.addOnClick([] { menu::util::getFiberPool()->run(createTemplate); }));

	addOption(scrollOption<int>(SCROLLSELECT, "Update Existing Translation")
		.addTranslation()
		.addScroll(vars.updateTranslation, 0, NUMOF(vars.translationList), vars.translationList)
		.addOnUpdate([] (scrollOption<int>* option) { option->addScroll(vars.updateTranslation, 0, vars.listSize, vars.translationList); })
		.addOnClick([] { updateExisting(vars.translationList[vars.updateTranslation].name.c_str()); })
		.addRequirement([] { return vars.listSize != 0; }));

	addOption(buttonOption("Reset to English")
		.addTranslation()
		.addOnClick([] { menu::util::getFiberPool()->run(reset); }));

	addOption(breakOption("Translations")
		.addTranslation());
}

/*Called while in submenu*/
void translationMenu::update() {}

/*Called once on submenu open*/
void translationMenu::updateOnce() {
	clearOptionsOffset(4);

	vars.cachedTranslations.clear();

	int size = 0;

	std::vector<std::string> translations = utils::getConfig()->getFilesInDirectory(utils::getConfig()->getTranslationPath(), ".json");
	if (translations.size()) {
		for (auto& translation : translations) {
			vars.translationList[size].name = translation.c_str();
			vars.translationList[size].result = size;
			size++;

			std::pair<std::string, struct stat> info;
			info.first = translation;

			if (stat(utils::va::va("%s%s.json", utils::getConfig()->getTranslationPath(), translation.c_str()), &info.second) == 0) {
				int index = (int)vars.cachedTranslations.size();
				if (index < 0) index = 0;
				vars.cachedTranslations.push_back(info);

				addOption(submenuOption("Load: " + translation)
					.addSubmenu<selectedTranslationMenu>()
					.addOnClick([=] { selectedTranslationMenuVars::vars.selectedTranslation = index; })
					.addOnHover([=] { menu::getPanelManager()->panelFile(translation, info.second); }));
			}
		}
	} else {
		addOption(buttonOption("No translations found"));
	}

	vars.listSize = size;
}

/*Called always*/
void translationMenu::featureUpdate() {}

translationMenu* _instance;
translationMenu* translationMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new translationMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

translationMenu::~translationMenu() { delete _instance; }