#include "network_account_win_loss.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_account.hpp"
#include "wins/network_account_win_loss_edit.hpp"

using namespace NetworkAccountWinsLossesMenuVars;

namespace NetworkAccountWinsLossesMenuVars {
	Vars_ m_Vars;

	void Set(std::string Win, std::string Loss, bool Total = false) {
		NetworkAccountWinLossEditMenuVars::m_Vars.m_Wins.first = Win;
		NetworkAccountWinLossEditMenuVars::m_Vars.m_Losses.first = Loss;
		NetworkAccountWinLossEditMenuVars::m_Vars.m_Total = Total;
	}
}

void NetworkAccountWinsLossesMenu::Init() {
	SetName("Wins and Losses");
	SetParentSubmenu<NetworkAccountMenu>();
	
	addOption(SubmenuOption("Races")
		.addTranslation()
		.addSubmenu<NetworkAccountWinLossEditMenu>()
		.addOnClick([] { Set("MPPLY_TOTAL_RACES_WON", "MPPLY_TOTAL_RACES_LOST"); }));

	addOption(SubmenuOption("Deathmatch")
		.addTranslation()
		.addSubmenu<NetworkAccountWinLossEditMenu>()
		.addOnClick([] { Set("MPPLY_TOTAL_DEATHMATCH_WON", "MPPLY_TOTAL_DEATHMATCH_LOST"); }));

	addOption(SubmenuOption("Team Deathmatch")
		.addTranslation()
		.addSubmenu<NetworkAccountWinLossEditMenu>()
		.addOnClick([] { Set("MPPLY_TOTAL_TDEATHMATCH_WON", "MPPLY_TOTAL_TDEATHMATCH_LOST"); }));

	addOption(SubmenuOption("Darts")
		.addTranslation()
		.addSubmenu<NetworkAccountWinLossEditMenu>()
		.addOnClick([] { Set("MPPLY_DARTS_TOTAL_WINS", "MPPLY_DARTS_TOTAL_MATCHES", true); }));

	addOption(SubmenuOption("Arm Wrestling")
		.addTranslation()
		.addSubmenu<NetworkAccountWinLossEditMenu>()
		.addOnClick([] { Set("MPPLY_ARMWRESTLING_TOTAL_WINS", "MPPLY_ARMWRESTLING_TOTAL_MATCH", true); }));

	addOption(SubmenuOption("Tennis")
		.addTranslation()
		.addSubmenu<NetworkAccountWinLossEditMenu>()
		.addOnClick([] { Set("MPPLY_TENNIS_MATCHES_WON", "MPPLY_TENNIS_MATCHES_LOST"); }));

	addOption(SubmenuOption("Golf")
		.addTranslation()
		.addSubmenu<NetworkAccountWinLossEditMenu>()
		.addOnClick([] { Set("MPPLY_GOLF_WINS", "MPPLY_GOLF_LOSSES"); }));

	addOption(SubmenuOption("Shooting Range")
		.addTranslation()
		.addSubmenu<NetworkAccountWinLossEditMenu>()
		.addOnClick([] { Set("MPPLY_SHOOTINGRANGE_WINS", "MPPLY_SHOOTINGRANGE_LOSSES"); }));
}

void NetworkAccountWinsLossesMenu::Update() {}

/*Called once on submenu open*/
void NetworkAccountWinsLossesMenu::UpdateOnce() {}

/*Called always*/
void NetworkAccountWinsLossesMenu::FeatureUpdate() {}

NetworkAccountWinsLossesMenu* _instance;
NetworkAccountWinsLossesMenu* NetworkAccountWinsLossesMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkAccountWinsLossesMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkAccountWinsLossesMenu::~NetworkAccountWinsLossesMenu() { delete _instance; }