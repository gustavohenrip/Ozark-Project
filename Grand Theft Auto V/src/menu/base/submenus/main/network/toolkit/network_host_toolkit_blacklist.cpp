#include "network_host_toolkit_blacklist.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_host_toolkit.hpp"
#include "menu/base/util/timers.hpp"
#include "rage/api/social.hpp"
#include "menu/base/util/panels.hpp"

using namespace NetworkHostToolkitBlacklistMenuVars;

namespace NetworkHostToolkitBlacklistMenuVars {
	Vars_ m_Vars;

	void RemovePlayer(uint64_t RockstarID) {
		if (::Global::Vars::g_NetworkBaseConfig) {
			Rage::Network::NetworkBaseConfig* RealConfig = (*(Rage::Network::NetworkBaseConfig**)((uint64_t)::Global::Vars::g_NetworkBaseConfig));
			
			if (Global::Vars::g_Steam) {
				RealConfig = (Rage::Network::NetworkBaseConfig*)(*(uint64_t*)((uint64_t)::Global::Vars::g_NetworkBaseConfig) - 0x50);
			}

			if (RealConfig) {
				if (RealConfig->m_LastBlacklisted.m_RockstarID == RockstarID) {
					RealConfig->m_LastBlacklisted.m_RockstarID = 0;
					RealConfig->m_LastBlacklistedTimestamp = 0;
				}

				// If they're the only one in the list
				if (RealConfig->m_FirstBlacklistedPlayer) {
					if (RealConfig->m_FirstBlacklistedPlayer->m_Player.m_RockstarID == RockstarID) {
						if (RealConfig->m_FirstBlacklistedPlayer->m_Next == nullptr) {
							RealConfig->m_FirstBlacklistedPlayer = nullptr;
							RealConfig->m_LastBlacklistedPlayer = nullptr;
						} else {
							// Replace it with the next
							RealConfig->m_FirstBlacklistedPlayer = RealConfig->m_FirstBlacklistedPlayer->m_Next;
							RealConfig->m_FirstBlacklistedPlayer->m_Last = nullptr;
						}

						return;
					}
				}

				// If they're not the first
				Rage::Network::BlacklistedPlayer* CurrentPlayer = RealConfig->m_FirstBlacklistedPlayer;
				Rage::Network::BlacklistedPlayer* LastPlayer = nullptr;

				while (CurrentPlayer) {
					if (CurrentPlayer->m_Player.m_RockstarID == RockstarID) {
						if (!IsValidPtr(CurrentPlayer->m_Next)) {
							LastPlayer->m_Next = nullptr;
							return;
						}

						LastPlayer->m_Next = CurrentPlayer->m_Next;
						CurrentPlayer->m_Last = LastPlayer;
						return;
					}

					LastPlayer = CurrentPlayer;
					CurrentPlayer = CurrentPlayer->m_Next;
				}
			}
		}
	}
}

void NetworkHostToolkitBlacklistMenu::Init() {
	SetName("Blacklisted Players");
	SetParentSubmenu<NetworkHostToolkitMenu>();

	addOption(ButtonOption("Delete All")
		.addTranslation().addHotkey()
		.addOnClick([] {
			if (::Global::Vars::g_NetworkBaseConfig) {
				Rage::Network::NetworkBaseConfig* RealConfig = (*(Rage::Network::NetworkBaseConfig**)((uint64_t)::Global::Vars::g_NetworkBaseConfig));
				if (Global::Vars::g_Steam) {
					RealConfig = (Rage::Network::NetworkBaseConfig*)(*(uint64_t*)((uint64_t)::Global::Vars::g_NetworkBaseConfig) - 0x50);
				}

				if (RealConfig) {
					RealConfig->m_LastBlacklisted.m_RockstarID = 0;
					RealConfig->m_LastBlacklistedTimestamp = 0;
					RealConfig->m_FirstBlacklistedPlayer = nullptr;
					RealConfig->m_LastBlacklistedPlayer = nullptr;
				}
			}

			m_Vars.m_Players.clear();
		}));

	addOption(BreakOption("Players")
		.addTranslation());

	addString("~m~None");
	addString("Click on this player to remove them from the blacklist");
}

void NetworkHostToolkitBlacklistMenu::Update() {
	static int Timer = 0;

	Menu::Timers::RunTimedFunction(&Timer, 1000, [this] {
		ClearOptionsOffset(2);

		std::vector<Rage::Network::BlacklistedPlayer> Players;

		if (::Global::Vars::g_NetworkBaseConfig) {
			Rage::Network::NetworkBaseConfig* RealConfig = (*(Rage::Network::NetworkBaseConfig**)((uint64_t)::Global::Vars::g_NetworkBaseConfig));
			if (Global::Vars::g_Steam) {
				RealConfig = (Rage::Network::NetworkBaseConfig*)(*(uint64_t*)((uint64_t)::Global::Vars::g_NetworkBaseConfig) - 0x50);
			}

			Rage::Network::BlacklistedPlayer* BlacklistedPlayers = RealConfig->m_FirstBlacklistedPlayer;
			if (IsValidPtr(BlacklistedPlayers)) {
				for (; BlacklistedPlayers; BlacklistedPlayers = BlacklistedPlayers->m_Next) {
					Players.push_back(*BlacklistedPlayers);
				}
			}
		}

		for (auto& Player : Players) {
			auto Vit = std::find_if(m_Vars.m_Players.begin(), m_Vars.m_Players.end(), [=] (Blacklist& Element) { return Element.m_Player.m_Player.m_RockstarID == Player.m_Player.m_RockstarID; });
			if (Vit == m_Vars.m_Players.end()) {
				m_Vars.m_Players.push_back({ Player, false, "" });
			}
		}

		std::vector<uint64_t> Delete;

		for (auto& Player : m_Vars.m_Players) {
			auto Vit = std::find_if(Players.begin(), Players.end(), [=] (Rage::Network::BlacklistedPlayer& Element) { return Element.m_Player.m_RockstarID == Player.m_Player.m_Player.m_RockstarID; });
			if (Vit == Players.end()) {
				Delete.push_back(Player.m_Player.m_Player.m_RockstarID); // Deleted
				continue;
			}

			if (!Player.m_API) {
				Player.m_API = true;

				if (Player.m_Player.m_Player.m_RockstarID > 0x1000) {
					Rage::API::GetSocialClub()->GetProfileFromRockstarID(Player.m_Player.m_Player.m_RockstarID, [] (bool Successful, Rage::API::RockstarAccount Account) {
						if (Successful) {
							auto Vit = std::find_if(m_Vars.m_Players.begin(), m_Vars.m_Players.end(), [=] (Blacklist& Element) { return Element.m_Player.m_Player.m_RockstarID == Account.m_RockstarID; });
							if (Vit != m_Vars.m_Players.end()) {
								Vit->m_ResolvedName = Account.m_Name;
							}
						}
					});
				}
			}

			std::string Name = std::to_string(Player.m_Player.m_Player.m_RockstarID);
			if (!Player.m_ResolvedName.empty()) Name = Player.m_ResolvedName;

			addOption(ButtonOption(Name)
				.addOnClick([=] { RemovePlayer(Player.m_Player.m_Player.m_RockstarID); })
				.addOnHover([=] {
					Menu::Panels::PanelCustomBlacklistedPlayer Blacklist;
					Blacklist.m_RockstarID = Player.m_Player.m_Player.m_RockstarID;
					Blacklist.m_BlacklistedTime = Player.m_Player.m_Timestamp;
					if (!Player.m_ResolvedName.empty()) strcpy(Blacklist.m_Name, Player.m_ResolvedName.c_str());
					else strcpy(Blacklist.m_Name, "Querying...");

					auto Parent = Menu::Panels::GetPanelManager()->GetParent("PANEL_BLACKLISTED_PLAYERS");
					if (Parent) {
						Menu::Panels::GetPanelManager()->SetStructure(Menu::Panels::GetPanelManager()->GetChild(Parent, "PANEL_BLACKLISTED_PLAYERS_INFO"), (void*)&Blacklist, sizeof(Blacklist));
						Parent->m_Render = true;
					}
				})
				.addTooltip(getString("Click on this player to remove them from the blacklist")));
		}

		for (uint64_t D : Delete) {
			m_Vars.m_Players.erase(std::remove_if(m_Vars.m_Players.begin(), m_Vars.m_Players.end(), [=] (Blacklist& Element) { return Element.m_Player.m_Player.m_RockstarID == D; }), m_Vars.m_Players.end());
		}

		if (m_Vars.m_Players.size() == 0) {
			auto Parent = Menu::Panels::GetPanelManager()->GetParent("PANEL_BLACKLISTED_PLAYERS");
			if (Parent) {
				Menu::Panels::GetPanelManager()->NullStructure(Menu::Panels::GetPanelManager()->GetChild(Parent, "PANEL_BLACKLISTED_PLAYERS_INFO"));
			}

			addOption(ButtonOption(getString("~m~None")));
		}
	});

	if (Menu::GetBase()->m_CurrentOption == 0) Menu::Panels::GetPanelManager()->TogglePanelRender("PANEL_BLACKLISTED_PLAYERS", false);
}

/*Called once on submenu open*/
void NetworkHostToolkitBlacklistMenu::UpdateOnce() {}

/*Called always*/
void NetworkHostToolkitBlacklistMenu::FeatureUpdate() {}

NetworkHostToolkitBlacklistMenu* _instance;
NetworkHostToolkitBlacklistMenu* NetworkHostToolkitBlacklistMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkHostToolkitBlacklistMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkHostToolkitBlacklistMenu::~NetworkHostToolkitBlacklistMenu() { delete _instance; }