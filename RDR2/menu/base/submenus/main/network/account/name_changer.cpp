#include "name_changer.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../account.hpp"
#include "utils/caller.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/notify.hpp"

using namespace networkAccountNameChangerMenuVars;

namespace networkAccountNameChangerMenuVars {
	vars_ vars;

	scrollStruct<const char*> textColors[] = {
		"Default", "",
		"White", "~q~",
		"Red", "~e~",
		"Light Red", "~t8~",
		"Yellow", "~o~",
		"Yellow 2", "~t4~",
		"Orange", "~d~",
		"Orange 2", "~t2~",
		"Grey", "~m~",
		"Light Grey", "~t~",
		"Black", "~v~",
		"Pink", "~u~",
		"Light Pink", "~t5~",
		"Blue", "~pa~",
		"Light Blue", "~t3~",
		"Dark Blue", "~t7~",
		"Purple", "~t1~",
		"Green", "~t6~",
	};

	scrollStruct<const char*> textSettings[] = {
		"Normal", "",
		"Bold", "~bold~",
		"Italic", "~italic~",
	};

	void spoof(bool reset, const char* name = NULL) {
		if (reset) {
			vars.spoofedName = std::string(vars.originalName);

			auto local = rage::engine::getNetworkPlayerManager()->localNetGamePlayer;
			if (local) {
				auto info = local->playerInfo;
				if (info) {
					strncpy(info->netInfo.name, vars.spoofedName.c_str(), 32);
				}
			}

			if (vars.saveToConfig) {
				utils::getConfig()->writeString("Spoofing: Name", "Name", vars.spoofedName.c_str());
			}

			return;
		}

		if (name) {
			char tmp[32];
			snprintf(tmp, 32, "%s%s%s", textSettings[vars.style].result, textColors[vars.color].result, name);
			vars.spoofedName = std::string(tmp);

			auto local = rage::engine::getNetworkPlayerManager()->localNetGamePlayer;
			if (local) {
				auto info = local->playerInfo;
				if (info) {
					strncpy(info->netInfo.name, vars.spoofedName.c_str(), 32);

					if (vars.saveToConfig) {
						utils::getConfig()->writeString("Spoofing: Name", "Name", vars.spoofedName.c_str());
					}
				}
			}
		}
	}
}

void networkAccountNameChangerMenu::init() {
	setName("Spoofing: Name");
	setParentSubmenu<networkAccountMenu>();

	auto gtag = rage::engine::getGameInfoString("gtag");
	if (gtag) {
		strncpy(vars.originalName, gtag, 32);
		vars.spoofedName = vars.originalName;
		vars.tempName = vars.originalName;
	}

	addOption(buttonOption("Custom Input")
		.addTranslation()
		.addKeyboard("Enter name", 32, [] (const char* name) { vars.tempName = std::string(name); }));

	addOption(buttonOption("Reset to Default")
		.addTranslation()
		.addOnClick([] { spoof(true); }));

	addOption(toggleOption("Save to Config")
		.addTranslation()
		.addToggle(vars.saveToConfig).canBeSaved(name.getOriginal())
		.addOnClick([] { spoof(true); }));

	addOption(breakOption("Text Settings")
		.addTranslation());

	addOption(scrollOption<const char*>(SCROLL, "Color")
		.addTranslation()
		.addScroll(vars.color, 0, NUMOF(textColors), textColors).canBeSaved(name.getOriginal()));

	addOption(scrollOption<const char*>(SCROLL, "Style")
		.addTranslation()
		.addScroll(vars.style, 0, NUMOF(textSettings), textSettings).canBeSaved(name.getOriginal()));

	vars.previewButton = addOption(buttonOption("")
		.addOnClick([] { spoof(false, vars.tempName.c_str()); }));

	if (vars.saveToConfig) {
		vars.spoofedName = vars.tempName = utils::getConfig()->readString("Spoofing: Name", "Name", vars.spoofedName.c_str());
	}
}

/*Called while in submenu*/
void networkAccountNameChangerMenu::update() {
	snprintf(vars.previewName, 50, "Apply: %s%s%s", textSettings[vars.style].result, textColors[vars.color].result, vars.tempName.c_str());
	vars.previewButton->setName(vars.previewName);
}

/*Called once on submenu open*/
void networkAccountNameChangerMenu::updateOnce() {}

/*Called always*/
void networkAccountNameChangerMenu::featureUpdate() {}

networkAccountNameChangerMenu* _instance;
networkAccountNameChangerMenu* networkAccountNameChangerMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkAccountNameChangerMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkAccountNameChangerMenu::~networkAccountNameChangerMenu() { delete _instance; }