#include "rainbow_config_load.hpp"
#include "menu/base/submenu_handler.hpp"
#include "rainbow_config.hpp"
#include "utils/va.hpp"
#include "utils/json.hpp"
#include "utils/log.hpp"

using namespace RainbowConfigLoadMenuVars;

namespace RainbowConfigLoadMenuVars {
	Vars_ m_Vars;

	void SaveConfig(const char* Name) {
		try {
			nlohmann::json Json;
			
			std::ifstream Input(Utils::GetConfig()->GetRainbowPath());
			if (Input.good()) {
				Input >> Json;
				Input.close();
			}

			std::ofstream Output(Utils::GetConfig()->GetRainbowPath());
			if (Output.good()) {
				Json[Name] = nlohmann::json::array({ m_Vars.m_Rainbow->m_Min, m_Vars.m_Rainbow->m_Max, m_Vars.m_Rainbow->m_Steps });
				Output << Json.dump(4);
				Output.close();
			}
		} catch (std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}
	}

	void LoadConfig(std::string Name) {
		try {
			std::ifstream Input(Utils::GetConfig()->GetRainbowPath());
			if (Input.good()) {
				nlohmann::json Json;
				Input >> Json;
				Input.close();

				if (!Json[Name].is_null()) {
					m_Vars.m_Rainbow->m_Min = Json[Name][0].get<int>();
					m_Vars.m_Rainbow->m_Max = Json[Name][1].get<int>();
					m_Vars.m_Rainbow->m_Steps = Json[Name][2].get<int>();
				}
			}
		} catch (std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}
	}
}

void RainbowConfigLoadMenu::Init() {
	SetName("Rainbow Config");
	SetParentSubmenu<RainbowConfigMenu>();

	addOption(ButtonOption("Save Rainbow")
		.addTranslation()
		.addKeyboard("Enter rainbow config name", 30, [] (const char* Name) {
			SaveConfig(Name);
		}));

	addOption(BreakOption("Rainbows"));

	addString("~m~None");
}

void RainbowConfigLoadMenu::Update() {}

/*Called once on submenu open*/
void RainbowConfigLoadMenu::UpdateOnce() {
	ClearOptionsOffset(2);

	try {
		std::ifstream Input(Utils::GetConfig()->GetRainbowPath());
		if (Input.good()) {
			nlohmann::json Json;
			Input >> Json;
			Input.close();

			if (Json.size() > 0) {
				std::vector<std::string> names;

				for (auto& it = Json.begin(); it != Json.end(); ++it) {
					names.push_back(it.key());
				}

				for (auto& name : names) {
					addOption(ButtonOption(name)
						.addOnClick([=] { LoadConfig(name); }));
				}
			} else {
				addOption(ButtonOption(getString("~m~None")));
			}
		} else {
			addOption(ButtonOption(getString("~m~None")));
		}
	} catch (std::exception& E) {
		LOG_WARN("[JSON] %s", E.what());
	}
}

/*Called always*/
void RainbowConfigLoadMenu::FeatureUpdate() {}

RainbowConfigLoadMenu* _instance;
RainbowConfigLoadMenu* RainbowConfigLoadMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new RainbowConfigLoadMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

RainbowConfigLoadMenu::~RainbowConfigLoadMenu() { delete _instance; }