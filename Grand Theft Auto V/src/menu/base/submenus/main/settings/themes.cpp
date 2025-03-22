#include "themes.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../settings.hpp"
#include <filesystem>
#include "utils/va.hpp"
#include "utils/json.hpp"
#include "themes_selected.hpp"
#include "utils/log.hpp"

using namespace ThemesMenuVars;

namespace ThemesMenuVars {
	Vars_ m_Vars;

	void SaveTheme(std::string Name) {
		bool Dupe = false;
		int NewDupe = 1;

		if (std::filesystem::exists(Utils::VA::VA("%s%s.json", Utils::GetConfig()->GetThemePath().c_str(), Name.c_str()))) {
			for (int i = 1; i < 10; i++) {
				if (std::filesystem::exists(Utils::VA::VA("%s%s (%i).json", Utils::GetConfig()->GetThemePath().c_str(), Name.c_str(), i))) {
					NewDupe = i + 1;
				} else {
					if (i != 1) {
						Dupe = true;
						break;
					}
				}
			}
		}

		try {
			std::ofstream Output;
			if (Dupe) Output = std::ofstream(Utils::VA::VA("%s%s (%i).json", Utils::GetConfig()->GetThemePath().c_str(), Name.c_str(), NewDupe));
			else Output = std::ofstream(Utils::VA::VA("%s%s.json", Utils::GetConfig()->GetThemePath().c_str(), Name.c_str()));

			nlohmann::json Json;
			Json["Position"]["X"] = Global::UIVars::g_MenuX;
			Json["Position"]["Y"] = Global::UIVars::g_MenuY;

			Json["Font"]["Title"] = Global::UIVars::g_TitleFont;
			Json["Font"]["Header"] = Global::UIVars::g_HeaderFont;
			Json["Font"]["Option"] = Global::UIVars::g_OptionFont;
			Json["Font"]["Break"] = Global::UIVars::g_BreakFont;

			std::pair<std::string, Color> Texts[] = {
				{ "Title", Global::UIVars::g_TitleTextColor },
				{ "Subtitle", Global::UIVars::g_MenuHeaderTextColor },
				{ "Open Tooltip", Global::UIVars::g_OpenToolTipColor },
				{ "Break", Global::UIVars::g_BreakTextColor },
				{ "Main", Global::UIVars::g_MainTextColor },
				{ "Selected", Global::UIVars::g_SelectedTextColor },
			};

			for (auto& Text : Texts) {
				Json["Color"]["Text"][Text.first] = {
					Text.second.R,
					Text.second.G,
					Text.second.B,
					Text.second.A
				};
			}

			Json["Color"]["Toggle"]["On"] = {
				Global::UIVars::g_ToggleOnColor.R,
				Global::UIVars::g_ToggleOnColor.G,
				Global::UIVars::g_ToggleOnColor.B,
				Global::UIVars::g_ToggleOnColor.A
			};

			Json["Color"]["Toggle"]["Off"] = {
				Global::UIVars::g_ToggleOffColor.R,
				Global::UIVars::g_ToggleOffColor.G,
				Global::UIVars::g_ToggleOffColor.B,
				Global::UIVars::g_ToggleOffColor.A
			};

			Json["Color"]["Header"]["Color"] = {
				Global::UIVars::g_TitleHeaderColor.R,
				Global::UIVars::g_TitleHeaderColor.G,
				Global::UIVars::g_TitleHeaderColor.B,
				Global::UIVars::g_TitleHeaderColor.A
			};

			Json["Color"]["Header"]["Subtitle"] = {
				Global::UIVars::g_MenuHeaderColor.R,
				Global::UIVars::g_MenuHeaderColor.G,
				Global::UIVars::g_MenuHeaderColor.B,
				Global::UIVars::g_MenuHeaderColor.A
			};

			Json["Color"]["Footer"] = {
				Global::UIVars::g_MenuFooterColor.R,
				Global::UIVars::g_MenuFooterColor.G,
				Global::UIVars::g_MenuFooterColor.B,
				Global::UIVars::g_MenuFooterColor.A
			};

			Json["Color"]["Scroller"] = {
				Global::UIVars::g_ScrollerColor.R,
				Global::UIVars::g_ScrollerColor.G,
				Global::UIVars::g_ScrollerColor.B,
				Global::UIVars::g_ScrollerColor.A
			};

			Json["Color"]["Background"] = {
				Global::UIVars::g_BackgroundColor.R,
				Global::UIVars::g_BackgroundColor.G,
				Global::UIVars::g_BackgroundColor.B,
				Global::UIVars::g_BackgroundColor.A
			};

			Json["Color"]["Notification - Success"] = {
				Global::UIVars::g_NotificationSuccess.R,
				Global::UIVars::g_NotificationSuccess.G,
				Global::UIVars::g_NotificationSuccess.B,
				Global::UIVars::g_NotificationSuccess.A
			};

			Json["Color"]["Notification - Error"] = {
				Global::UIVars::g_NotificationError.R,
				Global::UIVars::g_NotificationError.G,
				Global::UIVars::g_NotificationError.B,
				Global::UIVars::g_NotificationError.A
			};

			Output << Json.dump(4);
			Output.close();
		} catch (std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}

		ThemesMenu::GetInstance()->UpdateOnce();
	}

	void Reset() {
		Global::UIVars::g_MenuX = 0.70f;
		Global::UIVars::g_MenuY = 0.327f;
		Global::UIVars::g_TitleFont = 1;
		Global::UIVars::g_HeaderFont = 0;
		Global::UIVars::g_OptionFont = 0;
		Global::UIVars::g_BreakFont = 0;
		Global::UIVars::g_TitleTextColor = Color(255, 255, 255, 255);
		Global::UIVars::g_MenuHeaderTextColor = Color(255, 255, 255, 255);
		Global::UIVars::g_OpenToolTipColor = Color(74, 127, 184, 255);
		Global::UIVars::g_BreakTextColor = Color(255, 255, 255, 255);
		Global::UIVars::g_MainTextColor = Color(255, 255, 255, 255);
		Global::UIVars::g_SelectedTextColor = Color(0, 0, 0, 255);
		Global::UIVars::g_ToggleOnColor = Color(130, 214, 157, 255);
		Global::UIVars::g_ToggleOffColor = Color(200, 55, 80, 255);

		Global::UIVars::g_MenuHeaderColor = Color(0, 0, 0, 235);
		Global::UIVars::g_TitleHeaderColor = Color(0x4c25dbff);
		Global::UIVars::g_MenuFooterColor = Color(0, 0, 0, 210);
		Global::UIVars::g_ScrollerColor = Color(255, 255, 255, 255);
		Global::UIVars::g_BackgroundColor = Color(0, 0, 0, 255);
		Global::UIVars::g_NotificationSuccess = Color::Green();
		Global::UIVars::g_NotificationError = Color::Red();
	}
}

void ThemesMenu::Init() {
	SetName("Themes");
	SetParentSubmenu<SettingsMenu>();

	addOption(ButtonOption("Save Theme")
		.addTranslation()
		.addKeyboard("Enter theme name", 50, [] (const char* Name) {
			if (Name) SaveTheme(Name);
		}));

	addOption(ButtonOption("Reset to Default")
		.addTranslation()
		.addSprite("Ozark", "spinner")
		.addSpriteScale(0.01232f, 0.02184f)
		.addSpriteRotate()
		.addOnClick(Reset));

	addOption(BreakOption("Themes")
		.addTranslation());

	addString("~m~None");

	ThemeSelectedMenu::GetInstance();
}

void ThemesMenu::Update() {}

/*Called once on submenu open*/
void ThemesMenu::UpdateOnce() {
	ClearOptionsOffset(3);

	m_Vars.m_CachedThemes.clear();

	std::vector<std::string> Themes = Utils::GetConfig()->GetFilesInDirectory(Utils::GetConfig()->GetThemePath(), ".json");
	if (Themes.size()) {
		for (auto& Theme : Themes) {
			std::pair<std::string, struct stat> Info;
			Info.first = Theme;

			if (stat(Utils::VA::VA("%s%s.json", Utils::GetConfig()->GetThemePath().c_str(), Theme.c_str()), &Info.second) == 0) {
				std::size_t Index = m_Vars.m_CachedThemes.size();
				if (Index < 0) Index = 0;
				m_Vars.m_CachedThemes.push_back(Info);

				addOption(SubmenuOption(Theme)
					.addSubmenu<ThemeSelectedMenu>()
					.addOnClick([=] { ThemeSelectedMenuVars::m_Vars.m_Selected = Index; }));
			}
		}
	} else {
		addOption(ButtonOption(getString("~m~None")));
	}
}

/*Called always*/
void ThemesMenu::FeatureUpdate() {}

ThemesMenu* _instance;
ThemesMenu* ThemesMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ThemesMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

ThemesMenu::~ThemesMenu() { delete _instance; }