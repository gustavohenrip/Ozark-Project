#include "network_casino_rigging_blackjack_selected.hpp"
#include "menu/base/submenu_handler.hpp"
#include "network_casino_rigging_blackjack.hpp"
#include "menu/base/submenus/main/network.hpp"
#include "menu/base/util/global.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/notify.hpp"
#include "network_casino_rigging_blackjack_cards.hpp"

using namespace NetworkCasinoRiggingBlackjackSelectedMenuVars;

namespace NetworkCasinoRiggingBlackjackSelectedMenuVars {
	Vars_ m_Vars;

	void EndGame(NetworkCasinoRiggingBlackjackSelectedMenu* This) {
		uint64_t BlackjackLocals = Rage::Engine::GetScriptLocals("blackjack");
		if (BlackjackLocals) {
			if (Menu::ScriptLocal(BlackjackLocals, 122).At(1515).At(m_Vars.m_Selected, 1).As<int>() == 4) {
				NetworkCasinoRiggingBlackjackMenuVars::BlackjackTable Table;
				if (NetworkCasinoRiggingBlackjackMenuVars::GetBlackjackTable(m_Vars.m_Selected, &Table)) {
					if (Table.m_TotalPlayers) {
						int Index = 0;
						for (int i = 0; i < 4; i++) {
							if (Table.m_Players[i].m_Active) {
								Index = i;
								break;
							}
						}

						if (NetworkCasinoRiggingBlackjackMenuVars::MigrateBlackjackHost()) {
							NetworkCasinoRiggingBlackjackMenuVars::RigPlayerHandAtBlackjackTable(m_Vars.m_Selected, Index, 1, 2, true);
						}
					}
				}
			} else {
				Menu::GetNotify()->NotifyStacked(This->getString("The game needs to be in progress to end it"), Global::UIVars::g_NotificationError);
			}
		}
	}
}

void NetworkCasinoRiggingBlackjackSelectedMenu::Init() {
	SetName("Blackjack");
	SetParentSubmenu<NetworkCasinoRiggingBlackjackMenu>();

	addString("The game needs to be in progress to end it");

	addOption(ButtonOption("End Game")
		.addTranslation()
		.addOnClick([this] { EndGame(this); }));

	addOption(BreakOption("Players")
		.addTranslation());

	addOption(SubmenuOption("Dealer")
		.addSubmenu<NetworkCasinoRiggingBlackjackCardsMenu>()
		.addOnClick([=] { m_Vars.m_SelectedSeat = 4; }));
}

void NetworkCasinoRiggingBlackjackSelectedMenu::Update() {
	if (Native::_GetNumberOfInstancesOfScriptWithNameHash(joaat("am_mp_casino")) <= 0) {
		Menu::GetSubmenuHandler()->SetSubmenu(NetworkMenu::GetInstance());
		return;
	}

	NetworkCasinoRiggingBlackjackMenuVars::DrawESP(m_Vars.m_Selected);
}

/*Called once on submenu open*/
void NetworkCasinoRiggingBlackjackSelectedMenu::UpdateOnce() {
	ClearOptionsOffset(3);

	NetworkCasinoRiggingBlackjackMenuVars::BlackjackTable Table;
	if (NetworkCasinoRiggingBlackjackMenuVars::GetBlackjackTable(m_Vars.m_Selected, &Table)) {
		for (int i = 0; i < 4; i++) {
			if (Table.m_Players[i].m_Active) {
				addOption(SubmenuOption(Menu::GetPlayer(Table.m_Players[i].m_PhysicalIndex).m_Name)
					.addSubmenu<NetworkCasinoRiggingBlackjackCardsMenu>()
					.addOnClick([=] { m_Vars.m_SelectedSeat = i; }));
			}
		}
	}

	/*
		-> End Game
		-> ~ Players ~ (inc. dealer)
			-> First Card
			-> Second Card
	*/
}

/*Called always*/
void NetworkCasinoRiggingBlackjackSelectedMenu::FeatureUpdate() {
	if (Native::_GetNumberOfInstancesOfScriptWithNameHash(joaat("am_mp_casino")) > 0) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 5; j++) {
				if (m_Vars.m_Tables[i].m_PlayerCards[j].m_First || m_Vars.m_Tables[i].m_PlayerCards[j].m_Second) {
					auto First = m_Vars.m_Tables[i].m_PlayerCards[j].m_First ? NetworkCasinoRiggingBlackjackCardsMenuVars::Cards[m_Vars.m_Tables[i].m_PlayerCards[j].m_FirstCard].m_Result : -1;
					auto Second = m_Vars.m_Tables[i].m_PlayerCards[j].m_Second ? NetworkCasinoRiggingBlackjackCardsMenuVars::Cards[m_Vars.m_Tables[i].m_PlayerCards[j].m_SecondCard].m_Result: -1;

					if (j == 4) {
						// Reserved by us for dealer
						NetworkCasinoRiggingBlackjackMenuVars::RigDealerHandAtBlackjackTable(i, First, Second);
					} else {
						NetworkCasinoRiggingBlackjackMenuVars::RigPlayerHandAtBlackjackTable(i, j, First, Second);
					}
				}
			}
		}
	}
}

NetworkCasinoRiggingBlackjackSelectedMenu* _instance;
NetworkCasinoRiggingBlackjackSelectedMenu* NetworkCasinoRiggingBlackjackSelectedMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkCasinoRiggingBlackjackSelectedMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkCasinoRiggingBlackjackSelectedMenu::~NetworkCasinoRiggingBlackjackSelectedMenu() { delete _instance; }