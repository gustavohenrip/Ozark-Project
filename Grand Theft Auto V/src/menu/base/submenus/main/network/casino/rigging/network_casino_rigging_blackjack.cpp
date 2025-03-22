#include "network_casino_rigging_blackjack.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_casino_rigging.hpp"
#include "menu/base/submenus/main/network/network_host_toolkit.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/global.hpp"
#include "menu/base/submenus/main/network.hpp"
#include "network_casino_rigging_blackjack_selected.hpp"

using namespace NetworkCasinoRiggingBlackjackMenuVars;

namespace NetworkCasinoRiggingBlackjackMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> Bets[] = {
		{ TranslationString("1"), 0 },
		{ TranslationString("10"), 0 },
		{ TranslationString("50"), 0 },
		{ TranslationString("100"), 0 },
		{ TranslationString("250"), 0 },
		{ TranslationString("500"), 0 },
		{ TranslationString("1000"), 0 },
		{ TranslationString("1337"), 0 },
		{ TranslationString("2500"), 0 },
		{ TranslationString("5000"), 0 },
		{ TranslationString("10000"), 0 },
		{ TranslationString("25000"), 0 },
		{ TranslationString("50000"), 0 },
	};

	Math::Vector3<float> GetBlackjackTableCoords(int TableIndex) {
		switch (TableIndex) {
			case 0: return { 1148.837f, 269.747f, -52.8409f };
			case 1: return { 1151.84f, 266.747f, -52.8409f };
			case 2: return { 1129.406f, 262.3578f, -52.041f };
			case 3: return { 1144.429f, 247.3352f, -52.041f };
		}

		return { 0.0f, 0.0f, 0.0f };
	}

	bool MigrateBlackjackHost() {
		return NetworkHostToolkitMenuVars::ForceHostOfScript("blackjack", Menu::GetLocalPlayer().m_NetGamePlayer, false);
	}

	const char* GetBlackjackTableName(int TableIndex) {
		switch (TableIndex) {
			case 0: return "Standard Table";
			case 1: return "Standard Table";
			case 2: return "VIP Table";
			case 3: return "VIP Table";
		}

		return "Unknown";
	}

	bool GetBlackjackTable(int TableIndex, BlackjackTable* Table) {
		if (Table) {
			ZeroMemory(Table, sizeof(BlackjackTable));

			uint64_t BlackjackLocals = Rage::Engine::GetScriptLocals("blackjack");
			if (BlackjackLocals) {
				for (int i = 0; i < 32; i++) {
					int PlayerTable = Menu::ScriptLocal(BlackjackLocals, 1782).At(i, 8).At(4).As<int>();
					if (PlayerTable != -1 && PlayerTable == TableIndex) {
						int PlayerSeat = Menu::ScriptLocal(BlackjackLocals, 1782).At(i, 8).At(3).As<int>();
						if (PlayerSeat != -1) {
							int PlayerIndex = (PlayerSeat - (TableIndex * 4));
							if (PlayerIndex >= 0 && PlayerIndex <= 3) {
								for (int j = 0; j < 32; j++) {
									int TheirTableIndex = Menu::ScriptLocal(BlackjackLocals, 1782).At(j, 8).At(4).As<int>();
									if (TheirTableIndex != -1 && TheirTableIndex == TableIndex) {
										int TheirSeatIndex = Menu::ScriptLocal(BlackjackLocals, 1782).At(j, 8).At(3).As<int>();
										if (TheirSeatIndex != -1) {
											int TheirPlayerIndex = (TheirSeatIndex - (TheirTableIndex * 4));
											if (TheirPlayerIndex > PlayerIndex) {
												Table->m_Players[PlayerIndex].m_PlayersAhead++;
											} else if (TheirPlayerIndex < PlayerIndex) {
												Table->m_Players[PlayerIndex].m_PlayersBehind++;
											}
										}
									}
								}

								Table->m_Players[PlayerIndex].m_PlayerBet = Menu::ScriptLocal(BlackjackLocals, 1782).At(i, 8).At(1).As<int>();
								Table->m_Players[PlayerIndex].m_PhysicalIndex = i;
								Table->m_Players[PlayerIndex].m_Active = true;
								Table->m_TotalPlayers++;
							}
						}
					}
				}
			}

			Table->m_TableIndex = TableIndex;
			return (Table->m_TotalPlayers > 0);
		}

		return false;
	}

	void RigPlayerHandAtBlackjackTable(int TableIndex, int PlayerIndex, int FirstCard, int SecondCard, bool Bypass) {
		// PlayerIndex is NOT PhysicalIndex, it's an index from 0-3 (Seat Index)
		// Must be looped because '== 2' check only happens on one frame
		BlackjackTable Table;
		if (GetBlackjackTable(TableIndex, &Table)) {
			uint64_t BlackjackLocals = Rage::Engine::GetScriptLocals("blackjack");
			if (BlackjackLocals) {
				if (Table.m_Players[PlayerIndex].m_Active) {
					if (Menu::ScriptLocal(BlackjackLocals, 122).At(1515).At(TableIndex, 1).As<int>() == 2 || Bypass) { // Only do rig once before bet screen, this is the safest point to do it
						if (NetworkCasinoRiggingBlackjackMenuVars::MigrateBlackjackHost()) {
							if (FirstCard != -1) Menu::ScriptLocal(BlackjackLocals, 122).At(1).At(TableIndex, 211).At(Table.m_Players[PlayerIndex].m_PlayersBehind, 1).As<int>() = FirstCard;
							if (SecondCard != -1) Menu::ScriptLocal(BlackjackLocals, 122).At(1).At(TableIndex, 211).At(Table.m_Players[PlayerIndex].m_PlayersBehind + Table.m_TotalPlayers + 1, 1).As<int>() = SecondCard;
						}
					}
				}
			}
		}
	}

	void RigDealerHandAtBlackjackTable(int TableIndex, int FirstCard, int SecondCard) {
		// Must be looped because '== 2' check only happens on one frame
		BlackjackTable Table;
		if (GetBlackjackTable(TableIndex, &Table)) {
			uint64_t BlackjackLocals = Rage::Engine::GetScriptLocals("blackjack");
			if (BlackjackLocals) {
				if (Menu::ScriptLocal(BlackjackLocals, 122).At(1515).At(TableIndex, 1).As<int>() == 2) { // Only do rig once before bet screen, this is the safest point to do it
					if (NetworkCasinoRiggingBlackjackMenuVars::MigrateBlackjackHost()) {
						if (FirstCard != -1) Menu::ScriptLocal(BlackjackLocals, 122).At(1).At(TableIndex, 211).At(Table.m_TotalPlayers, 1).As<int>() = FirstCard;
						if (SecondCard != -1) Menu::ScriptLocal(BlackjackLocals, 122).At(1).At(TableIndex, 211).At(Table.m_TotalPlayers + Table.m_TotalPlayers + 1, 1).As<int>() = SecondCard;
					}
				}
			}
		}
	}

	void DrawESP(int TableIndex) {
		Math::Vector3<float> MachineCoords = GetBlackjackTableCoords(TableIndex);
		Math::Vector3<float> StartCoords = Menu::GetLocalPlayer().m_Coords;
		Menu::GetRenderer()->DrawLine({ StartCoords.m_X, StartCoords.m_Y, StartCoords.m_Z }, { MachineCoords.m_X, MachineCoords.m_Y, MachineCoords.m_Z }, Global::UIVars::g_TitleHeaderColor);

		Native::SetDrawOrigin(MachineCoords.m_X, MachineCoords.m_Y, MachineCoords.m_Z, NULL);

		if (*(uint32_t*)Global::Vars::g_DrawOriginIndex != 0xFFFFFFFF) {
			int FreeSeats = 4;

			BlackjackTable Table;
			if (GetBlackjackTable(TableIndex, &Table)) {
				FreeSeats -= Table.m_TotalPlayers;
			}

			char Text[100];
			sprintf_s(Text, "%s\n%i Free Seats", GetBlackjackTableName(TableIndex), FreeSeats);
			Menu::GetRenderer()->DrawText(Text, { 0.f, 0.f }, 0.3f, 0, { 255, 255, 255 }, JUSTIFY_CENTER);

			Native::ClearDrawOrigin();
		}
	}
}

void NetworkCasinoRiggingBlackjackMenu::Init() {
	SetName("Blackjack");
	SetParentSubmenu<NetworkCasinoRiggingMenu>();

	addString("~m~No Tables Found");
	addString("Tables");
	addString("Standard Table");
	addString("VIP Table");
	addString("Unknown");
}

void NetworkCasinoRiggingBlackjackMenu::Update() {
	if (Native::_GetNumberOfInstancesOfScriptWithNameHash(joaat("am_mp_casino")) <= 0) {
		Menu::GetSubmenuHandler()->SetSubmenu(NetworkMenu::GetInstance());
	}
}

/*Called once on submenu open*/
void NetworkCasinoRiggingBlackjackMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	uint64_t BlackjackLocals = Rage::Engine::GetScriptLocals("blackjack");
	if (BlackjackLocals) {
		addOption(ScrollOption<int>(TOGGLE, "Minimum Bet")
			.addToggle(m_Vars.m_MinimumBetLoop)
			.addScroll(m_Vars.m_MinimumBet, 0, NUMOF(Bets), Bets)
			.addOnHover([] {
				Global::UIVars::g_InsideCasinoBlackjack = true;
				Global::UIVars::g_InsideCasinoBlackjackHigh = false;
			}));

		addOption(ScrollOption<int>(TOGGLE, "Maximum Bet")
			.addToggle(m_Vars.m_MaximumBetLoop)
			.addScroll(m_Vars.m_MaximumBet, 0, NUMOF(Bets), Bets));

		addOption(BreakOption(getString("Tables")));

		for (int i = 0; i < 4; i++) {
			addOption(SubmenuOption(Utils::VA::VA("[%i] %s", i + 1, getString(GetBlackjackTableName(i)).c_str()))
				.addSubmenu<NetworkCasinoRiggingBlackjackSelectedMenu>()
				.addOnClick([=] { NetworkCasinoRiggingBlackjackSelectedMenuVars::m_Vars.m_Selected = i; })
				.addOnHover([=] {
					if (i > 1) {
						Global::UIVars::g_InsideCasinoBlackjack = false;
						Global::UIVars::g_InsideCasinoBlackjackHigh = true;
					} else {
						Global::UIVars::g_InsideCasinoBlackjack = true;
						Global::UIVars::g_InsideCasinoBlackjackHigh = false;
					}
					
					DrawESP(i);
					// Info Box (players, bets, etc)
				}));
		}
	} else {
		addOption(ButtonOption(getString("~m~No Tables Found")));
	}
}

/*Called always*/
void NetworkCasinoRiggingBlackjackMenu::FeatureUpdate() {
	if (Native::_GetNumberOfInstancesOfScriptWithNameHash(joaat("blackjack")) > 0) {
		uint64_t BlackjackLocals = Rage::Engine::GetScriptLocals("blackjack");
		if (BlackjackLocals) {
			if (m_Vars.m_MinimumBetLoop) {
				Menu::ScriptLocal(BlackjackLocals, 2039).At(246).As<int>() = std::atoi(Bets[m_Vars.m_MinimumBet].m_Name.GetOriginal().c_str());
			}

			if (m_Vars.m_MaximumBetLoop) {
				Menu::ScriptLocal(BlackjackLocals, 2039).At(245).As<int>() = std::atoi(Bets[m_Vars.m_MaximumBet].m_Name.GetOriginal().c_str());
			}
		}
	}
}

NetworkCasinoRiggingBlackjackMenu* _instance;
NetworkCasinoRiggingBlackjackMenu* NetworkCasinoRiggingBlackjackMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkCasinoRiggingBlackjackMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkCasinoRiggingBlackjackMenu::~NetworkCasinoRiggingBlackjackMenu() { delete _instance; }