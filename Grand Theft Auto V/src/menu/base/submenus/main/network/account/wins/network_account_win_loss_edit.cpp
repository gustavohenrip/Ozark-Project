#include "network_account_win_loss_edit.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_account_win_loss.hpp"
#include "menu/base/util/stats.hpp"

using namespace NetworkAccountWinLossEditMenuVars;

namespace NetworkAccountWinLossEditMenuVars {
	Vars_ m_Vars;
}

void NetworkAccountWinLossEditMenu::Init() {
	SetName("Wins and Losses");
	SetParentSubmenu<NetworkAccountWinsLossesMenu>();

	addOption(NumberOption<int>(SCROLL, "Wins")
		.addTranslation()
		.addNumber(m_Vars.m_Wins.second, "%i", 1).addMin(0).addMax(std::numeric_limits<int>::max())
		.addOnClick([] { Stats::Interface<int>::Set(m_Vars.m_Wins.first.c_str(), m_Vars.m_Wins.second); }));

	addOption(NumberOption<int>(SCROLL, "Losses")
		.addTranslation()
		.addNumber(m_Vars.m_Losses.second, "%i", 1).addMin(0).addMax(std::numeric_limits<int>::max())
		.addOnClick([] { Stats::Interface<int>::Set(m_Vars.m_Losses.first.c_str(), m_Vars.m_Losses.second); })
		.addRequirement([] { return !m_Vars.m_Total; }));

	addOption(NumberOption<int>(SCROLL, "Matches")
		.addTranslation()
		.addNumber(m_Vars.m_Losses.second, "%i", 1).addMin(0).addMax(std::numeric_limits<int>::max())
		.addOnClick([] { Stats::Interface<int>::Set(m_Vars.m_Losses.first.c_str(), m_Vars.m_Losses.second); })
		.addRequirement([] { return m_Vars.m_Total; }));
}

void NetworkAccountWinLossEditMenu::Update() {}

/*Called once on submenu open*/
void NetworkAccountWinLossEditMenu::UpdateOnce() {
	m_Vars.m_Wins.second = Stats::Interface<int>::Get(m_Vars.m_Wins.first.c_str());
	m_Vars.m_Losses.second = Stats::Interface<int>::Get(m_Vars.m_Losses.first.c_str());
}

/*Called always*/
void NetworkAccountWinLossEditMenu::FeatureUpdate() {}

NetworkAccountWinLossEditMenu* _instance;
NetworkAccountWinLossEditMenu* NetworkAccountWinLossEditMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkAccountWinLossEditMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkAccountWinLossEditMenu::~NetworkAccountWinLossEditMenu() { delete _instance; }