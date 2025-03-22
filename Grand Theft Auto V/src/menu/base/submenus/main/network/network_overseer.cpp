#include "network_overseer.hpp"
#include "overseer/network_selected_overseer.hpp"
#include "players/network_players_abusive.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network.hpp"
#include "network_session.hpp"
#include "menu/base/util/global.hpp"
#include "utils/json.hpp"
#include "utils/va.hpp"
#include "menu/base/util/panels.hpp"
#include "menu/base/util/timers.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/util/timers.hpp"
#include "rage/engine.hpp"
#include "players/network_players_abusive.hpp"
#include "players/network_players_trolling.hpp"
#include "players/network_players_removal.hpp"
#include "utils/log.hpp"

using namespace NetworkOverseerMenuVars;

namespace NetworkOverseerMenuVars {
	Vars_ m_Vars;

	void Load(NetworkOverseerMenu* This) {
		This->ClearOptionsOffset(2);
		m_Vars.m_Players.clear();

		try {
			std::ifstream Input(Utils::GetConfig()->GetOverseerPath());
			if (Input.good()) {
				nlohmann::json Json;
				Input >> Json;
				Input.close();

				for (auto it = Json.begin(); it != Json.end(); ++it) {
					Overseer Player;
					Player.m_Flags = it.value()["flags"].get<int>();
					Player.m_Name = it.value()["name"].get<std::string>();
					Player.m_Reason = it.value()["reason added"].get<std::string>();
					Player.m_RockstarID = std::stoi(it.key());
					m_Vars.m_Players.push_back(Player);
				}

				for (std::size_t i = 0; i < m_Vars.m_Players.size(); i++) {
					auto& Player = m_Vars.m_Players[i];

					This->addOption(SubmenuOption(Utils::VA::VA("%s%s:%i", (Player.m_Flags & OVERSEER_ENABLED) ? "" : "", Player.m_Name.c_str(), Player.m_RockstarID))
						.addOnClick([=] { NetworkOverseerSelectedMenuVars::m_Vars.m_Selected = i; })
						.addSubmenu<NetworkOverseerSelectedMenu>());
				}
			}

		} catch (std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}
		
		if (m_Vars.m_Players.size() == 0) {
			This->addOption(ButtonOption(This->getString("~m~None")));
		}
	}

	void Save(std::string Name, uint64_t RID, std::string Reason) {
		auto Vit = std::find_if(begin(m_Vars.m_Players), end(m_Vars.m_Players), [=](Overseer& Element) { return Element.m_RockstarID == RID; });
		if (Vit == end(m_Vars.m_Players)) {
			try {
				nlohmann::json Json;

				std::ifstream Input(Utils::GetConfig()->GetOverseerPath());
				if (Input.good()) {
					Input >> Json;
					Input.close();
				}

				Json[std::to_string(RID)]["name"] = Name;
				Json[std::to_string(RID)]["reason added"] = Reason;
				Json[std::to_string(RID)]["flags"] = OVERSEER_ENABLED | OVERSEER_NOTIFY;

				std::ofstream Output(Utils::GetConfig()->GetOverseerPath());
				if (Output.good()) {
					Output << Json.dump(4);
					Output.close();
				}
			} catch (std::exception& E) {
				LOG_WARN("[JSON] %s", E.what());
			}

			Load(NetworkOverseerMenu::GetInstance());
		}
	}

	bool ProcessJoinRequest(uint64_t RID, std::string Name, uint32_t IP) {
		auto Vit = std::find_if(begin(m_Vars.m_Players), end(m_Vars.m_Players), [=](Overseer& Element) {
			return Element.m_RockstarID == RID;
		});

		if (Vit != end(m_Vars.m_Players)) {
			int Flag = Vit->m_Flags;
			if ((Flag & OVERSEER_ENABLED) && (Flag & OVERSEER_BLOCK_JOIN_HOST)) {
				if (Flag & OVERSEER_NOTIFY) {
					char Buffer[16];
					sprintf(Buffer, "%i.%i.%i.%i", ((IP & 0xFF000000) >> 24), ((IP & 0xFF0000) >> 16), ((IP & 0xFF00) >> 8), (IP & 0xFF));
					Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s:%s %s", NetworkOverseerMenu::GetInstance()->getString("Prevented").c_str(), Name.c_str(), Buffer, NetworkOverseerMenu::GetInstance()->getString("joining due to Overseer configuration").c_str()));
				}

				return true;
			}
		}

		return false;
	}

	void ProcessJoinNotify(uint64_t RID, uint32_t IP, std::string Name) {
		if (RID) {
			auto Vit = std::find_if(begin(m_Vars.m_Players), end(m_Vars.m_Players), [=](Overseer& Element) {
				return Element.m_RockstarID == RID;
			});

			if (Vit != end(m_Vars.m_Players)) {
				int Flag = Vit->m_Flags;
				if ((Flag & OVERSEER_ENABLED) && (Flag & OVERSEER_NOTIFY)) {
					char Buffer[16];
					sprintf(Buffer, "%i.%i.%i.%i", ((IP & 0xFF000000) >> 24), ((IP & 0xFF0000) >> 16), ((IP & 0xFF00) >> 8), (IP & 0xFF));
					Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s:%s %s", Name.c_str(), Buffer, NetworkOverseerMenu::GetInstance()->getString("(enabled in your Overseer configuration) is joining").c_str()));
				}
			}
		}
	}

	void Process(Player ID, int Flag) {
		if (Flag & OVERSEER_ENABLED) {
			if (Flag & OVERSEER_EXPLODE) {
				NetworkPlayersAbusiveMenuVars::Explode(Menu::GetPlayer(ID), 1);
			}

			if (Flag & OVERSEER_CRASH) {
				NetworkPlayersRemovalMenuVars::CrashPlayer(Menu::GetPlayer(ID), 2);
			}

			if (Flag & OVERSEER_KICK) {
				if (Menu::GetLocalPlayer().m_IsScriptHost) {
					NetworkPlayersRemovalMenuVars::ScriptHostKick(Menu::GetPlayer(ID));
				}

				if (Rage::Engine::GetHostNetGamePlayer() == Menu::GetLocalPlayer().m_NetGamePlayer) {
					NetworkPlayersRemovalMenuVars::SessionHostKick(Menu::GetPlayer(ID));
				} else {
					if (Rage::Engine::GetHostNetGamePlayer() != Menu::GetPlayer(ID).m_NetGamePlayer) {
						NetworkPlayersRemovalMenuVars::BreakFreemodeKick(Menu::GetPlayer(ID));
					}
				}
			}

			if (Flag & OVERSEER_CAGE) {
				NetworkPlayersTrollingMenuVars::CagePlayer(Menu::GetPlayer(ID));
			}

			if (Flag & OVERSEER_KICK_FROM_VEHICLE) {
				NetworkPlayersAbusiveMenuVars::KickFromVehiclePlayer(Menu::GetPlayer(ID));
			}

			if (Flag & OVERSEER_SET_ON_FIRE) {
				NetworkPlayersAbusiveMenuVars::SetOnFire(Menu::GetPlayer(ID));
			}

			if (Flag & OVERSEER_RAGDOLL) {
				NetworkPlayersAbusiveMenuVars::RagdollPlayer(Menu::GetPlayer(ID));
			}

			if (Flag & OVERSEER_GLITCH_PHYSICS) {
				NetworkPlayersAbusiveMenuVars::GlitchPhysics(Menu::GetPlayer(ID));
			}

			if (Flag & OVERSEER_BLACK_SCREEN) {
				NetworkPlayersAbusiveMenuVars::BlackScreen(Menu::GetPlayer(ID));
			}

			if (Flag & OVERSEER_BLAME) {
				NetworkPlayersAbusiveMenuVars::Blame(Menu::GetPlayer(ID));
			}

			if (Flag & OVERSEER_GIVE_WANTED_LEVEL) {
				NetworkPlayersAbusiveMenuVars::GiveWantedLevel(Menu::GetPlayer(ID), 1);
			}

			if (Flag & OVERSEER_SEND_TO_ISLAND) {
				NetworkPlayersAbusiveMenuVars::SendPlayerTo(Menu::GetPlayer(ID));
			}

			if (Flag & OVERSEER_REMOVE_WEAPONS) {
				Native::RemoveAllPedWeapons(Menu::GetPlayer(ID).m_Ped, true);
			}

			if (Flag & OVERSEER_HOSTILE_CLONE) {
				Menu::GetPlayer(ID).m_CloneType = 1;
				NetworkPlayersTrollingMenuVars::ClonePlayer(Menu::GetPlayer(ID));
			}
		}
	}
}

void NetworkOverseerMenu::Init() {
	SetName("Overseer");
	SetParentSubmenu<NetworkSessionMenu>();

	addString("~m~None");
	addString("Prevented");
	addString("joining due to Overseer configuration");
	addString("(enabled in your Overseer configuration) is joining");

	addOption(ToggleOption("Enable Overseer")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_EnableOverseer).canBeSaved(m_ParentNameStack));

	addOption(BreakOption("Players")
		.addTranslation());

	Load(this);

	NetworkOverseerSelectedMenu::GetInstance(); // Init
}

void NetworkOverseerMenu::Update() {}

/*Called once on submenu open*/
void NetworkOverseerMenu::UpdateOnce() {
	Load(this);
}

/*Called always*/
void NetworkOverseerMenu::FeatureUpdate() {
	static int Timer = 0;

	Menu::Timers::RunTimedFunction(&Timer, 2000, [] {
		Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
			if (Player.m_NetGamePlayer) {
				if (Player.m_NetGamePlayer->m_PlayerInfo) {
					auto Vit = std::find_if(begin(m_Vars.m_Players), end(m_Vars.m_Players), [=](Overseer& Element) {
						return Element.m_RockstarID == Player.m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID;
					});

					if (Vit != end(m_Vars.m_Players)) {
						int Flag = Vit->m_Flags;
						if ((Flag & OVERSEER_ENABLED)) {
							Process(Player.m_ID, Flag);
						}
					}
				}
			}
		});
	});
}

NetworkOverseerMenu* _instance;
NetworkOverseerMenu* NetworkOverseerMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkOverseerMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkOverseerMenu::~NetworkOverseerMenu() { delete _instance; }