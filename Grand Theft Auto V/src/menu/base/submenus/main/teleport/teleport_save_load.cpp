#include "teleport_save_load.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../teleport.hpp"
#include "utils/json.hpp"
#include "utils/log.hpp"
#include <filesystem>

using namespace TeleportSaveLoadMenuVars;

namespace TeleportSaveLoadMenuVars {
	Vars_ m_Vars;

	void Save(std::string Name, Math::Vector3<float> Coords) {
		try {
			std::ifstream Stream(Utils::GetConfig()->GetTeleportSavePath());
			if (Stream.good()) {
				nlohmann::json Json;
				Stream >> Json;
				Stream.close();

				Json["Teleports"][Name] = { Coords.m_X, Coords.m_Y, Coords.m_Z };
				std::ofstream Output(Utils::GetConfig()->GetTeleportSavePath());
				if (Output.good()) {
					Output << Json.dump(4);
					Output.close();
				}
			}
		} catch (std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}
	}

	void Load(TeleportSaveLoadMenu* pThis) {
		try {
			std::ifstream Stream(Utils::GetConfig()->GetTeleportSavePath());
			if (Stream.good()) {
				nlohmann::json Json;
				Stream >> Json;
				Stream.close();

				nlohmann::json& Data = Json["Teleports"];
				if (Data.size()) {
					m_Vars.m_Locations.clear();

					for (auto& it = Data.begin(); it != Data.end(); ++it) {
						nlohmann::json& Coords = *it;
						m_Vars.m_Locations.push_back({ it.key(), { Coords[0].get<float>(), Coords[1].get<float>(), Coords[2].get<float>() } });
					}
				}
			}
		} catch (std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}

		pThis->ClearOptionsOffset(1);

		if (m_Vars.m_Locations.size()) {
			pThis->addOption(BreakOption("Locations"));

			for (auto& Location : m_Vars.m_Locations) {
				pThis->addOption(ButtonOption(Location.first)
					.addOnClick([=] { TeleportMenuVars::Teleport(Location.second); }));
			}
		}
	}
}

void TeleportSaveLoadMenu::Init() {
	SetName("Save Load");
	SetParentSubmenu<TeleportMenu>();

	addOption(ButtonOption("Save Current Location")
		.addTranslation().addHotkey()
		.addKeyboard("Enter save name", 30, [this] (const char* Name) {
			Save(Name, Menu::GetLocalPlayer().m_Coords);
			Load(this);
		}));

	if (!std::filesystem::exists(Utils::GetConfig()->GetTeleportSavePath())) {
		try {
			std::ofstream Output(Utils::GetConfig()->GetTeleportSavePath());
			if (Output.good()) {
				nlohmann::json Json;
				Json["Teleports"]["Empty"] = { 0.f, 0.f, 0.f };
				Output << Json.dump(4);
				Output.close();
			}
		} catch (std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}
	}
}

void TeleportSaveLoadMenu::Update() {}

/*Called once on submenu open*/
void TeleportSaveLoadMenu::UpdateOnce() {
	Load(this);
}

/*Called always*/
void TeleportSaveLoadMenu::FeatureUpdate() {}

TeleportSaveLoadMenu* _instance;
TeleportSaveLoadMenu* TeleportSaveLoadMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new TeleportSaveLoadMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

TeleportSaveLoadMenu::~TeleportSaveLoadMenu() { delete _instance; }