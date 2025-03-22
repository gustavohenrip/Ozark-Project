#include "network_text_save_load.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_players_text.hpp"
#include "utils/json.hpp"
#include "utils/log.hpp"

using namespace NetworkTextSaveLoadMenuVars;

namespace NetworkTextSaveLoadMenuVars {
	Vars_ m_Vars;

	void Load(NetworkTextSaveLoadMenu* pThis) {
		try {
			std::ifstream file(Utils::GetConfig()->GetTextMessagePath());
			if (file.good()) {
				nlohmann::json Json;
				file >> Json;
				file.close();

				nlohmann::json& Data = Json["messages"];
				if (Data.size()) {
					m_Vars.m_Messages.clear();

					for (nlohmann::json::iterator it = Data.begin(); it != Data.end(); ++it) {
						m_Vars.m_Messages.push_back(std::make_pair(it.key(), it.value().get<std::string>()));
					}
				}
			} else {
				std::ofstream Output(Utils::GetConfig()->GetTextMessagePath());
				if (Output.good()) {
					nlohmann::json Json;
					Json["messages"]["Hello, Friend!"] = "Hello, Friend!";

					Output << Json.dump(4);
					Output.close();
				}
			}
		} catch (std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}

		pThis->ClearOptionsOffset(2);

		for (auto& Message : m_Vars.m_Messages) {
			pThis->addOption(ButtonOption(Message.first)
				.addOnClick([=] {
					Menu::GetSelectedPlayer().m_TextMessageMsg = Message.second;
					Menu::GetSubmenuHandler()->SetSubmenuPrevious();
				}));
		}
	}

	void Save(std::string name, std::string message) {
		try {
			std::ifstream File(Utils::GetConfig()->GetTextMessagePath());
			if (File.good()) {
				nlohmann::json Json;
				File >> Json;
				File.close();

				Json["messages"][name] = message;

				std::ofstream Output(Utils::GetConfig()->GetTextMessagePath());
				if (Output.good()) {
					Output << Json.dump(4);
					Output.close();
				}
			}
		} catch (std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}
	}
}

void NetworkTextSaveLoadMenu::Init() {
	SetName("Save and Load");
	SetParentSubmenu<NetworkPlayersTextMenu>();

	addOption(ButtonOption("+ Add Message")
		.addTranslation()
		.addKeyboard("Enter your message", 100, [this] (const char* Msg) {
			std::string Message = Msg;
			std::string Temp = Message;

			if (Temp.size() > 32) {
				Temp.erase(Temp.begin() + 32, Temp.end());
				Temp += std::string("...");
			}

			Save(Temp, Message);
			Load(this);
		}));

	addOption(BreakOption("Messages")
		.addTranslation());

	Load(this);
}

void NetworkTextSaveLoadMenu::Update() {}

/*Called once on submenu open*/
void NetworkTextSaveLoadMenu::UpdateOnce() {
	Load(this);
}

/*Called always*/
void NetworkTextSaveLoadMenu::FeatureUpdate() {}

NetworkTextSaveLoadMenu* _instance;
NetworkTextSaveLoadMenu* NetworkTextSaveLoadMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkTextSaveLoadMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkTextSaveLoadMenu::~NetworkTextSaveLoadMenu() { delete _instance; }