#include "network_recent_players.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network.hpp"
#include "utils/json.hpp"
#include "menu/base/util/timers.hpp"
#include "utils/va.hpp"
#include "menu/base/util/panels.hpp"
#include "rage/engine.hpp"
#include "recent/network_recent_players_selected.hpp"
#include "recent/network_recent_players_perm.hpp"
#include "utils/fiber_pool.hpp"
#include "utils/fibers.hpp"
#include "network_presence.hpp"
#include "utils/log.hpp"

using namespace NetworkRecentPlayersMenuVars;

namespace NetworkRecentPlayersMenuVars {
	Vars_ m_Vars;

	std::string GetCurrentTimeStr() {
		time_t curr_time;
		time(&curr_time);
		tm* curr_tm = localtime(&curr_time);

		char Buffer[40];
		strftime(Buffer, 40, "%a %d %b %Y %H:%M:%S", curr_tm);
		return std::string(Buffer);
	}

	void Save(std::string Name, std::string IP, uint64_t RockstarID) {
		nlohmann::json Json;
		std::vector<RecentPlayer> Players;
		std::vector<RecentPlayer> PermPlayers;

		std::ifstream File(Utils::GetConfig()->GetRecentPlayersPath());
		if (File.good()) {
			try {
				File >> Json;
			} catch (std::exception& E) {
				LOG_WARN("[JSON] %s", E.what());
			}

			File.close();
		}

		if (!Json["recent"].is_null()) {
			for (nlohmann::json::iterator it = Json["recent"].begin(); it != Json["recent"].end(); ++it) {
				RecentPlayer O;
				strcpy(O.m_Name, it.value()["name"].get<std::string>().c_str());
				strcpy(O.m_IP, it.value()["ip"].get<std::string>().c_str());
				strcpy(O.m_FirstEncountered, it.value()["first encountered"].get<std::string>().c_str());
				strcpy(O.m_LastEncountered, it.value()["last encountered"].get<std::string>().c_str());
				O.m_RockstarID = std::stoull(it.key());
				O.m_InternalEncountered = it.value()["internal"].get<int>();

				Players.push_back(O);
			}
		}

		if (!Json["perm"].is_null()) {
			for (nlohmann::json::iterator it = Json["perm"].begin(); it != Json["perm"].end(); ++it) {
				RecentPlayer O;
				strcpy(O.m_Name, it.value()["name"].get<std::string>().c_str());
				strcpy(O.m_IP, it.value()["ip"].get<std::string>().c_str());
				strcpy(O.m_FirstEncountered, it.value()["first encountered"].get<std::string>().c_str());
				strcpy(O.m_LastEncountered, it.value()["last encountered"].get<std::string>().c_str());

				O.m_RockstarID = std::stoull(it.key());
				O.m_InternalEncountered = it.value()["internal"].get<int>();

				PermPlayers.push_back(O);
			}
		}

		bool Exists = false;
		for (auto& P : Players) {
			if (P.m_RockstarID == RockstarID && !strcmp(P.m_Name, Name.c_str())) {
				Exists = true;
				strcpy(P.m_LastEncountered, GetCurrentTimeStr().c_str());
				strcpy(P.m_IP, IP.c_str());
				P.m_InternalEncountered = (int)time(0);
				break;
			}
		}

		if (!Exists) {
			RecentPlayer NewPlayer;
			strcpy(NewPlayer.m_Name, Name.c_str());
			strcpy(NewPlayer.m_IP, IP.c_str());
			strcpy(NewPlayer.m_FirstEncountered, GetCurrentTimeStr().c_str());
			strcpy(NewPlayer.m_LastEncountered, GetCurrentTimeStr().c_str());
			NewPlayer.m_InternalEncountered = (int)time(0);
			NewPlayer.m_RockstarID = RockstarID;
			Players.push_back(NewPlayer);
		}

		std::sort(Players.begin(), Players.end(), [](RecentPlayer& first, RecentPlayer& second) { return first.m_InternalEncountered > second.m_InternalEncountered; });

		std::ofstream Output(Utils::GetConfig()->GetRecentPlayersPath());
		if (Output.good()) {
			nlohmann::json Resp;

			for (auto& p : PermPlayers) {
				Resp["perm"][std::to_string(p.m_RockstarID)]["name"] = p.m_Name;
				Resp["perm"][std::to_string(p.m_RockstarID)]["ip"] = p.m_IP;
				Resp["perm"][std::to_string(p.m_RockstarID)]["first encountered"] = p.m_FirstEncountered;
				Resp["perm"][std::to_string(p.m_RockstarID)]["last encountered"] = p.m_LastEncountered;
				Resp["perm"][std::to_string(p.m_RockstarID)]["internal"] = p.m_InternalEncountered;
			}

			int Count = 0;
			for (auto& p : Players) {
				if (Count > 100) break;
				Resp["recent"][std::to_string(p.m_RockstarID)]["name"] = p.m_Name;
				Resp["recent"][std::to_string(p.m_RockstarID)]["ip"] = p.m_IP;
				Resp["recent"][std::to_string(p.m_RockstarID)]["first encountered"] = p.m_FirstEncountered;
				Resp["recent"][std::to_string(p.m_RockstarID)]["last encountered"] = p.m_LastEncountered;
				Resp["recent"][std::to_string(p.m_RockstarID)]["internal"] = p.m_InternalEncountered;
				Count++;
			}

			Output << Resp.dump(4);
			Output.close();
		}
	}

	void Load() {
		std::ifstream File(Utils::GetConfig()->GetRecentPlayersPath());
		if (File.good()) {
			nlohmann::json Json;

			try {
				File >> Json;
			} catch (std::exception& E) {
				LOG_WARN("[JSON] %s", E.what());
			}

			File.close();

			Json = Json["recent"];

			m_Vars.m_Players.clear();
			for (nlohmann::json::iterator it = Json.begin(); it != Json.end(); ++it) {
				RecentPlayer O;
				strcpy(O.m_Name, it.value()["name"].get<std::string>().c_str());
				if (strlen(O.m_Name) == 0) strcpy(O.m_Name, "Invalid Name");

				strcpy(O.m_IP, it.value()["ip"].get<std::string>().c_str());
				strcpy(O.m_FirstEncountered, it.value()["first encountered"].get<std::string>().c_str());
				strcpy(O.m_LastEncountered, it.value()["last encountered"].get<std::string>().c_str());

				O.m_InternalEncountered = it.value()["internal"].get<int>();
				O.m_RockstarID = std::stoull(it.key());

				if (m_Vars.m_Players.size() == 100) break;
				if (O.m_RockstarID > 0x1000) m_Vars.m_Queue.push_back(O.m_RockstarID);
				m_Vars.m_Players.push_back({ it.key(), O });
			}

			std::sort(m_Vars.m_Players.begin(), m_Vars.m_Players.end(), [] (std::pair<std::string, RecentPlayer>& first, std::pair<std::string, RecentPlayer>& second) { return first.second.m_InternalEncountered > second.second.m_InternalEncountered; });
		}
	}
}

void NetworkRecentPlayersMenu::Init() {
	SetName("Recent Players");
	SetParentSubmenu<NetworkMenu>();

	addOption(SubmenuOption("Permanent")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkRecentPlayersPermanentMenu>()
		.addOnHover([] { Menu::Panels::GetPanelManager()->TogglePanelRender("PANEL_RECENT_PLAYER", false); }));

	addOption(BreakOption("Players")
		.addTranslation());

	addString("~m~None");

	NetworkRecentPlayersSelectedMenu::GetInstance();
}

void NetworkRecentPlayersMenu::Update() {
	static int TIMERS[2];
	static bool UpdatePlayers = true;
	static bool UpdateOnline = true;

	Menu::Timers::RunTimedFunction(&TIMERS[0], 1000, [] { UpdatePlayers = true; });
	Menu::Timers::RunTimedFunction(&TIMERS[1], 4000, [] { UpdateOnline = true; });

	if (UpdatePlayers) {
		UpdatePlayers = false;

		ClearOptionsOffset(2);

		if (m_Vars.m_Players.size()) {
			for (std::size_t i = 0; i < m_Vars.m_Players.size(); i++) {
				auto Player = m_Vars.m_Players[i];

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
						NetworkRecentPlayersSelectedMenuVars::m_Vars.m_Type = 1;
						NetworkRecentPlayersSelectedMenuVars::m_Vars.m_SelectedPlayer = i;
						m_Vars.m_SelectedList = &m_Vars.m_Players;
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
			for (std::size_t i = 0; i < m_Vars.m_Players.size(); i++) {
				m_Vars.m_Queue.push_back(m_Vars.m_Players[i].second.m_RockstarID);
			}
		}
	}
}

/*Called once on submenu open*/
void NetworkRecentPlayersMenu::UpdateOnce() {
	Load();
}

/*Called always*/
void NetworkRecentPlayersMenu::FeatureUpdate() {}

NetworkRecentPlayersMenu* _instance;
NetworkRecentPlayersMenu* NetworkRecentPlayersMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkRecentPlayersMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkRecentPlayersMenu::~NetworkRecentPlayersMenu() { delete _instance; }