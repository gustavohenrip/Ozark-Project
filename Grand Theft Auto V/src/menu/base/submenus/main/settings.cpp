#include "settings.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../main.hpp"
#include "settings/translations.hpp"
#include "settings/themes.hpp"
#include "settings/position.hpp"
#include "settings/fonts.hpp"
#include "settings/videos.hpp"
#include "settings/colors.hpp"
#include "settings/hotkey_menu.hpp"
#include "menu/init.hpp"

using namespace SettingsMenuVars;

namespace SettingsMenuVars {
	Vars_ m_Vars;
}

void SettingsMenu::Init() {
	SetName("Settings");
	SetParentSubmenu<MainMenu>();

	addOption(SubmenuOption("Translations")
		.addTranslation().addHotkey()
		.addSubmenu<TranslationMenu>());

	addOption(SubmenuOption("Hotkeys")
		.addTranslation().addHotkey()
		.addSubmenu<HotkeysMenu>());

	addOption(SubmenuOption("Themes")
		.addTranslation().addHotkey()
		.addSubmenu<ThemesMenu>());

	addOption(SubmenuOption("Colors")
		.addTranslation().addHotkey()
		.addSubmenu<ColorsMenu>());

	addOption(SubmenuOption("Positions")  
		.addTranslation().addHotkey()
		.addSubmenu<PositionMenu>());

	addOption(SubmenuOption("Fonts")
		.addTranslation().addHotkey()
		.addSubmenu<FontsMenu>());

	addOption(SubmenuOption("Videos")
		.addTranslation().addHotkey()
		.addSubmenu<VideosMenu>());

	addOption(ToggleOption("Open Tooltip")
		.addTranslation().addHotkey()
		.addToggle(Menu::GetBase()->m_OpenTooltip).canBeSaved(m_ParentNameStack));

	addOption(ButtonOption("Unload")
		.addTranslation().addHotkey()
		.addOnClick([] { Global::Vars::g_UnloadingQueue = true; }));
}

void SettingsMenu::Update() {}

/*Called once on submenu open*/
void SettingsMenu::UpdateOnce() {}

/*Called always*/
void SettingsMenu::FeatureUpdate() {}

SettingsMenu* _instance;
SettingsMenu* SettingsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new SettingsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

SettingsMenu::~SettingsMenu() { delete _instance; }