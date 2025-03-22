#include "network_recent_players_selected.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_recent_players.hpp"
#include "utils/json.hpp"
#include "menu/base/util/panels.hpp"
#include "../network_presence.hpp"
#include "network_recent_players_perm.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/presence.hpp"
#include "utils/log.hpp"

using namespace NetworkRecentPlayersSelectedMenuVars;

namespace NetworkRecentPlayersSelectedMenuVars {
	Vars_ m_Vars;

	void MakePermanent() {
		try {
			nlohmann::json Json;

			std::ifstream File(Utils::GetConfig()->GetRecentPlayersPath());
			if (File.good()) {
				File >> Json;
				File.close();

				std::ofstream Output(Utils::GetConfig()->GetRecentPlayersPath());
				if (Output.good()) {
					auto p = NetworkRecentPlayersMenuVars::m_Vars.m_SelectedList->at(m_Vars.m_SelectedPlayer).second;
					Json["perm"][std::to_string(p.m_RockstarID)]["name"] = p.m_Name;
					Json["perm"][std::to_string(p.m_RockstarID)]["ip"] = p.m_IP;
					Json["perm"][std::to_string(p.m_RockstarID)]["first encountered"] = p.m_FirstEncountered;
					Json["perm"][std::to_string(p.m_RockstarID)]["last encountered"] = p.m_LastEncountered;
					Json["perm"][std::to_string(p.m_RockstarID)]["internal"] = p.m_InternalEncountered;
					Output << Json.dump(4);
					Output.close();
				}
			}
		} catch (std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}
	}
}

void NetworkRecentPlayersSelectedMenu::Init() {
	SetName("Recent Player");
	SetParentSubmenu<NetworkRecentPlayersMenu>();

	addOption(ButtonOption("Add to Permanent List")
		.addTranslation()
		.addRequirement([] { return m_Vars.m_Type == 1; })
		.addOnClick(MakePermanent));

	addOption(ButtonOption("Show Profile")
		.addTranslation()
		.addOnClick([] {
			Rage::Network::NetworkHandle Handle;
			Native::NetworkHandleFromUserId(std::to_string(NetworkRecentPlayersMenuVars::m_Vars.m_SelectedList->at(m_Vars.m_SelectedPlayer).second.m_RockstarID).c_str(), &Handle.m_Handle, (sizeof(Handle) / 4));
			Native::NetworkShowProfileUi(&Handle.m_Handle);
		}));

	addOption(ButtonOption("Send Friend Request")
		.addTranslation()
		.addOnClick([] { Rage::Engine::SendFriendRequest(NetworkRecentPlayersMenuVars::m_Vars.m_SelectedList->at(m_Vars.m_SelectedPlayer).second.m_RockstarID); }));

	addOption(ButtonOption("Invite to Session")
		.addTranslation()
		.addOnClick([] { Menu::Presence::InvitePlayer(NetworkRecentPlayersMenuVars::m_Vars.m_SelectedList->at(m_Vars.m_SelectedPlayer).second.m_RockstarID); }));

	addOption(ScrollOption<int>(SCROLLSELECT, "Join Player")
		.addTranslation()
		.addScroll(NetworkPresenceMenuVars::m_Vars.m_JoinThemType, 0, NUMOF(NetworkPresenceMenuVars::Types), NetworkPresenceMenuVars::Types)
		.addOnClick([] { NetworkPresenceMenuVars::JoinPlayer(NetworkRecentPlayersMenuVars::m_Vars.m_SelectedList->at(m_Vars.m_SelectedPlayer).second.m_RockstarID); })
		.addTooltip("Force join this player, even if they're in an invite only!"));
}

void NetworkRecentPlayersSelectedMenu::Update() {
	if (m_Vars.m_Type == 1) {
		SetParentSubmenu<NetworkRecentPlayersMenu>();
	} else {
		SetParentSubmenu<NetworkRecentPlayersPermanentMenu>();
	}
}

/*Called once on submenu open*/
void NetworkRecentPlayersSelectedMenu::UpdateOnce() {
	auto Player = NetworkRecentPlayersMenuVars::m_Vars.m_SelectedList->at(m_Vars.m_SelectedPlayer);
	Menu::Panels::GetPanelManager()->SetStructure(Menu::Panels::GetPanelManager()->GetChild(Menu::Panels::GetPanelManager()->GetParent("PANEL_RECENT_PLAYER"), "PANEL_RECENT_PLAYER_INFO"), (void*)&Player.second, sizeof(Player.second));
}

/*Called always*/
void NetworkRecentPlayersSelectedMenu::FeatureUpdate() {}

NetworkRecentPlayersSelectedMenu* _instance;
NetworkRecentPlayersSelectedMenu* NetworkRecentPlayersSelectedMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkRecentPlayersSelectedMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkRecentPlayersSelectedMenu::~NetworkRecentPlayersSelectedMenu() { delete _instance; }