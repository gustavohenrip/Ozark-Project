#include "network_recent_players_perm.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_recent_players.hpp"
#include "utils/json.hpp"
#include "menu/base/util/timers.hpp"
#include "utils/va.hpp"
#include "menu/base/util/panels.hpp"
#include "rage/engine.hpp"
#include "network_recent_players_selected.hpp"
#include "utils/log.hpp"

using namespace NetworkRecentPlayersPermanentMenuVars;

namespace NetworkRecentPlayersPermanentMenuVars {
	Vars_ m_Vars;

	void Load() {
		try {
			std::ifstream File(Utils::GetConfig()->GetRecentPlayersPath());
			if (File.good()) {
				nlohmann::json Json;
				File >> Json;
				File.close();

				Json = Json["perm"];

				NetworkRecentPlayersMenuVars::m_Vars.m_PermanentPlayers.clear();

				for (nlohmann::json::iterator it = Json.begin(); it != Json.end(); ++it) {
					NetworkRecentPlayersMenuVars::RecentPlayer O;
					strcpy(O.m_Name, it.value()["name"].get<std::string>().c_str());
					if (strlen(O.m_Name) == 0) strcpy(O.m_Name, "Invalid Name");

					strcpy(O.m_IP, it.value()["ip"].get<std::string>().c_str());
					strcpy(O.m_FirstEncountered, it.value()["first encountered"].get<std::string>().c_str());
					strcpy(O.m_LastEncountered, it.value()["last encountered"].get<std::string>().c_str());
					O.m_InternalEncountered = it.value()["internal"].get<int>();
					O.m_RockstarID = std::stoull(it.key());

					if (NetworkRecentPlayersMenuVars::m_Vars.m_PermanentPlayers.size() == 100) break;
					m_Vars.m_Queue.push_back(O.m_RockstarID);
					NetworkRecentPlayersMenuVars::m_Vars.m_PermanentPlayers.push_back({ it.key(), O });
				}

				std::sort(NetworkRecentPlayersMenuVars::m_Vars.m_PermanentPlayers.begin(), NetworkRecentPlayersMenuVars::m_Vars.m_PermanentPlayers.end(), [](std::pair<std::string, NetworkRecentPlayersMenuVars::RecentPlayer>& first, std::pair<std::string, NetworkRecentPlayersMenuVars::RecentPlayer>& second) { return first.second.m_InternalEncountered > second.second.m_InternalEncountered; });
			}
		} catch (std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}
	}
}

void NetworkRecentPlayersPermanentMenu::Init() {
	SetName("Permanent");
	SetParentSubmenu<NetworkRecentPlayersMenu>();

	addString("~m~None");
}

void NetworkRecentPlayersPermanentMenu::Update() {
	static int TIMERS[2];
	static bool UpdatePlayers = true;
	static bool UpdateOnline = true;

	Menu::Timers::RunTimedFunction(&TIMERS[0], 1000, [] { UpdatePlayers = true; });
	Menu::Timers::RunTimedFunction(&TIMERS[1], 4000, [] { UpdateOnline = true; });

	if (UpdatePlayers) {
		UpdatePlayers = false;

		ClearOptionsOffset(0);

		if (NetworkRecentPlayersMenuVars::m_Vars.m_PermanentPlayers.size()) {
			for (std::size_t i = 0; i < NetworkRecentPlayersMenuVars::m_Vars.m_PermanentPlayers.size(); i++) {
				auto Player = NetworkRecentPlayersMenuVars::m_Vars.m_PermanentPlayers[i];

				bool InSession = false;
				Menu::GetPlayerManager()->Foreach([&] (Menu::PlayerVars& PlayerI) {
					if (PlayerI.m_NetGamePlayer) {
						if (PlayerI.m_NetGamePlayer->m_PlayerInfo) {
							if (PlayerI.m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID == Player.second.m_RockstarID) {
								InSession = true;
							}
						}
					}
				}, true);

				addOption(SubmenuOption(Utils::VA::VA("%s%s", (Global::Vars::g_OnlineRockstarIds[Player.second.m_RockstarID] || InSession) ? "" : "~c~", Player.second.m_Name))
					.addOnClick([=] {
						NetworkRecentPlayersSelectedMenuVars::m_Vars.m_Type = 2;
						NetworkRecentPlayersSelectedMenuVars::m_Vars.m_SelectedPlayer = i;
						NetworkRecentPlayersMenuVars::m_Vars.m_SelectedList = &NetworkRecentPlayersMenuVars::m_Vars.m_PermanentPlayers;
					})
					.addSubmenu<NetworkRecentPlayersSelectedMenu>()
					.addOnHover([=] {
						auto Parent = Menu::Panels::GetPanelManager()->GetParent("PANEL_RECENT_PLAYER");
						if (Parent) {
							Menu::Panels::GetPanelManager()->SetStructure(Menu::Panels::GetPanelManager()->GetChild(Parent, "PANEL_RECENT_PLAYER_INFO"), (void*)&Player.second, sizeof(Player.second));
							Parent->m_Render = true;
						}
					}));
			}
		} else {
			addOption(ButtonOption(getString("~m~None")));
		}
	}

	if (UpdateOnline) {
		UpdateOnline = false;

		for (std::size_t i = 0; i < m_Vars.m_Queue.size(); i++) {
			auto vit = std::find_if(Global::Vars::g_OnlineRockstarIdQueue.begin(), Global::Vars::g_OnlineRockstarIdQueue.end(), [=] (uint64_t value) {
				return value == m_Vars.m_Queue[i];
			});

			if (vit == Global::Vars::g_OnlineRockstarIdQueue.end()) {
				Global::Vars::g_OnlineRockstarIdQueue.push_back(m_Vars.m_Queue[i]);
			}
		}

		std::size_t ProcessSize = m_Vars.m_Queue.size() > 32 ? 32 : m_Vars.m_Queue.size();

		Rage::Network::RockstarIdentifier* Identifiers = (Rage::Network::RockstarIdentifier*)malloc(ProcessSize * sizeof(Rage::Network::RockstarIdentifier));
		memset(Identifiers, 0, ProcessSize * sizeof(Rage::Network::RockstarIdentifier));

		for (std::size_t i = 0; i < ProcessSize; i++) {
			Identifiers[i].m_RockstarID = m_Vars.m_Queue.back();
			Identifiers[i].m_Type = 3;
			m_Vars.m_Queue.pop_back();
		}

		Native::ClearGamerTaskData();
		Rage::Engine::SendSessionInfoRequest(Identifiers, (int)ProcessSize);
		free(Identifiers);

		// recreate the queue
		if (m_Vars.m_Queue.size() == 0) {
			for (std::size_t i = 0; i < NetworkRecentPlayersMenuVars::m_Vars.m_PermanentPlayers.size(); i++) {
				m_Vars.m_Queue.push_back(NetworkRecentPlayersMenuVars::m_Vars.m_PermanentPlayers[i].second.m_RockstarID);
			}
		}
	}
}

/*Called once on submenu open*/
void NetworkRecentPlayersPermanentMenu::UpdateOnce() {
	Load();
}

/*Called always*/
void NetworkRecentPlayersPermanentMenu::FeatureUpdate() {}

NetworkRecentPlayersPermanentMenu* _instance;
NetworkRecentPlayersPermanentMenu* NetworkRecentPlayersPermanentMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkRecentPlayersPermanentMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkRecentPlayersPermanentMenu::~NetworkRecentPlayersPermanentMenu() { delete _instance; }