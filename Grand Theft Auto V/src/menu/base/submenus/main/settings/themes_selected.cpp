#include "themes_selected.hpp"
#include "menu/base/submenu_handler.hpp"
#include "themes.hpp"
#include "utils/va.hpp"
#include "utils/json.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/log.hpp"

using namespace ThemeSelectedMenuVars;

namespace ThemeSelectedMenuVars {
	Vars_ m_Vars;

	Color ReadColor(nlohmann::json Json) {
		if (Json.is_null()) return Color(0, 0, 0, 255);
		return Color(Json[0].get<int>(), Json[1].get<int>(), Json[2].get<int>(), Json[3].get<int>());
	}

	void LoadTheme(std::string Name) {
		try {
			std::ifstream Input(Utils::VA::VA("%s%s.json", Utils::GetConfig()->GetThemePath().c_str(), Name.c_str()));
			if (Input.good()) {
				nlohmann::json Json = nlohmann::json::parse(Input);
				Input.close();

				if (!Json["Position"].is_null()) {
					Global::UIVars::g_MenuX = Json["Position"]["X"].get<float>();
					Global::UIVars::g_MenuY = Json["Position"]["Y"].get<float>();
				}

				if (!Json["Font"].is_null()) {
					Global::UIVars::g_TitleFont = Json["Font"]["Title"].get<int>();
					Global::UIVars::g_HeaderFont = Json["Font"]["Header"].get<int>();
					Global::UIVars::g_OptionFont = Json["Font"]["Option"].get<int>();
					Global::UIVars::g_BreakFont = Json["Font"]["Break"].get<int>();
				}

				if (!Json["Color"].is_null()) {
					if (!Json["Color"]["Text"].is_null()) {
						Global::UIVars::g_TitleTextColor = ReadColor(Json["Color"]["Text"]["Title"]);
						Global::UIVars::g_MenuHeaderTextColor = ReadColor(Json["Color"]["Text"]["Subtitle"]);
						Global::UIVars::g_OpenToolTipColor = ReadColor(Json["Color"]["Text"]["Open Tooltip"]);
						Global::UIVars::g_BreakTextColor = ReadColor(Json["Color"]["Text"]["Break"]);
						Global::UIVars::g_MainTextColor = ReadColor(Json["Color"]["Text"]["Main"]);
						Global::UIVars::g_SelectedTextColor = ReadColor(Json["Color"]["Text"]["Selected"]);
					}

					if (!Json["Color"]["Toggle"].is_null()) {
						Global::UIVars::g_ToggleOnColor = ReadColor(Json["Color"]["Toggle"]["On"]);
						Global::UIVars::g_ToggleOffColor = ReadColor(Json["Color"]["Toggle"]["Off"]);
					}

					Global::UIVars::g_MenuHeaderColor = ReadColor(Json["Color"]["Header"]["Subtitle"]);
					Global::UIVars::g_TitleHeaderColor = ReadColor(Json["Color"]["Header"]["Color"]);
					Global::UIVars::g_MenuFooterColor = ReadColor(Json["Color"]["Footer"]);
					Global::UIVars::g_ScrollerColor = ReadColor(Json["Color"]["Scroller"]);
					Global::UIVars::g_BackgroundColor = ReadColor(Json["Color"]["Background"]);
					Global::UIVars::g_NotificationSuccess = ReadColor(Json["Color"]["Notification - Success"]);
					Global::UIVars::g_NotificationError = ReadColor(Json["Color"]["Notification - Error"]);
				}
			} else {
				Menu::GetNotify()->NotifyStacked(ThemeSelectedMenu::GetInstance()->getString("Failed to open theme file!"), Global::UIVars::g_NotificationError);
			}
		} catch (std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}
	}
}

void ThemeSelectedMenu::Init() {
	SetName("Theme");
	SetParentSubmenu<ThemesMenu>();

	addString("Failed to open theme file!");

	addOption(ButtonOption("Load")
		.addTranslation()
		.addOnClick([] { LoadTheme(ThemesMenuVars::m_Vars.m_CachedThemes[m_Vars.m_Selected].first); }));

	addOption(ButtonOption("Delete")
		.addTranslation()
		.addOnClick([] {
			remove(Utils::VA::VA("%s%s.json", Utils::GetConfig()->GetThemePath().c_str(), ThemesMenuVars::m_Vars.m_CachedThemes[m_Vars.m_Selected].first.c_str()));
			Menu::GetSubmenuHandler()->SetSubmenuPrevious(true);
		}));

	addOption(ButtonOption("Set as Load Default")
		.addTranslation()
		.addOnClick([this] { Utils::GetConfig()->WriteString(m_ParentNameStack, "Launch", ThemesMenuVars::m_Vars.m_CachedThemes[m_Vars.m_Selected].first); }));

	std::string Launch = Utils::GetConfig()->ReadString(m_ParentNameStack, "Launch", "-none-");
	if (Launch.compare("-none-")) {
		LoadTheme(Launch);
	}
}

void ThemeSelectedMenu::Update() {}

/*Called once on submenu open*/
void ThemeSelectedMenu::UpdateOnce() {}

/*Called always*/
void ThemeSelectedMenu::FeatureUpdate() {}

ThemeSelectedMenu* _instance;
ThemeSelectedMenu* ThemeSelectedMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ThemeSelectedMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

ThemeSelectedMenu::~ThemeSelectedMenu() { delete _instance; }