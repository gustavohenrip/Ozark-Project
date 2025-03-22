#include "network_message_save_load.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_message.hpp"
#include "../recent_players.hpp"
#include "../recent/selected_recent_player.hpp"
#include "menu/base/util/panel_manager.hpp"

using namespace networkMessageSaveLoadMenuVars;

namespace networkMessageSaveLoadMenuVars {
	vars_ vars;

	void load(networkMessageSaveLoadMenu* pthis) {
		std::ifstream file(utils::getConfig()->getTextMessageSavePath());
		if (file.good()) {
			nlohmann::json json;
			file >> json;
			file.close();

			nlohmann::json& data = json["messages"];
			if (data.size()) {
				vars.messages.clear();

				for (nlohmann::json::iterator it = data.begin(); it != data.end(); ++it) {
					vars.messages.push_back(std::make_pair(it.key(), it.value().get<std::string>()));
				}
			}
		} else {
			std::ofstream output(utils::getConfig()->getTextMessageSavePath());
			if (output.good()) {
				nlohmann::json json;
				json["messages"]["Hello, Friend!"] = "Hello, Friend!";

				output << json.dump(4);
				output.close();
			}
		}

		pthis->clearOptionsOffset(2);

		for (auto& tp : vars.messages) {
			pthis->addOption(buttonOption(tp.first)
				.addOnClick([=] {
					networkMessageMenuVars::vars.message = tp.second;
					menu::getSubmenuHandler()->setSubmenuPrevious();
				}));
		}
	}

	void save(std::string name, std::string message) {
		std::ifstream file(utils::getConfig()->getTextMessageSavePath());
		if (file.good()) {
			nlohmann::json json;
			file >> json;
			file.close();

			json["messages"][name] = message;
			std::ofstream output(utils::getConfig()->getTextMessageSavePath());
			if (output.good()) {
				output << json.dump(4);
				output.close();
			}
		}
	}
}

void networkMessageSaveLoadMenu::init() {
	setName("Message Save and Load");
	setParentSubmenu<networkMessageMenu>();

	addOption(buttonOption("+ Add Message")
		.addTranslation()
		.addKeyboard("Enter your message", 100, [this] (const char* msg) {
			std::string message = native::getOnscreenKeyboardResult();

			std::string temp = message;
			if (temp.size() > 32) {
				temp.erase(temp.begin() + 32, temp.end());
				temp += std::string("...");
			}

			save(temp, message);
			load(this);
		}));

	addOption(breakOption("Messages")
		.addTranslation());

	load(this);
}

/*Called while in submenu*/
void networkMessageSaveLoadMenu::update() {
	if (networkMessageMenuVars::vars.submenuIdentifier == 0) {
		menu::getPanelManager()->panelPlayer(menu::getSelectedPlayer().id);
	} else if (networkMessageMenuVars::vars.submenuIdentifier == 1) {
		auto player = networkRecentPlayersMenuVars::vars.selectedList->at(networkSelectedRecentPlayerMenuVars::vars.selectedPlayer);
		menu::getPanelManager()->panelRecentPlayer(player.second.name, player.second.firstEncountered, player.second.lastEncountered, player.second.ip, player.first);
	}
}

/*Called once on submenu open*/
void networkMessageSaveLoadMenu::updateOnce() {
	load(this);
}

/*Called always*/
void networkMessageSaveLoadMenu::featureUpdate() {}

networkMessageSaveLoadMenu* _instance;
networkMessageSaveLoadMenu* networkMessageSaveLoadMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkMessageSaveLoadMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkMessageSaveLoadMenu::~networkMessageSaveLoadMenu() { delete _instance; }