#include "network_account_kd.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_account.hpp"
#include "menu/base/util/stats.hpp"
#include "utils/va.hpp"

using namespace NetworkAccountKDMenuVars;

namespace NetworkAccountKDMenuVars {
	Vars_ m_Vars;

	float Calculate() {
		float Calculated = 0.f;
		if (m_Vars.m_Deaths == 0) {
			Calculated = (float)m_Vars.m_Kills;
		} else Calculated = (float)(m_Vars.m_Kills / m_Vars.m_Deaths);

		return Calculated;
	}

	void SetKD() {
		Stats::Interface<int>::Set("MPPLY_KILLS_PLAYERS", m_Vars.m_Kills);
		Stats::Interface<int>::Set("MPPLY_DEATHS_PLAYER", m_Vars.m_Deaths);
	}
}

void NetworkAccountKDMenu::Init() {
	SetName("K/D");
	SetParentSubmenu<NetworkAccountMenu>();

	addOption(NumberOption<int>(SCROLL, "Set Kills")
		.addTranslation()
		.addNumber(m_Vars.m_Kills, "%i", 1).addMin(0).setScrollSpeed(0));

	addOption(NumberOption<int>(SCROLL, "Set Deaths")
		.addTranslation()
		.addNumber(m_Vars.m_Deaths, "%i", 1).addMin(0).setScrollSpeed(0));

	m_Vars.m_Button = addOption(ButtonOption("")
		.addOnClick(SetKD));

	addString("Set K/D:");
}

void NetworkAccountKDMenu::Update() {
	m_Vars.m_Calculated = Calculate();
	m_Vars.m_Button->setName(std::string(Utils::VA::VA("%s %.2f", getString("Set K/D:").c_str(), m_Vars.m_Calculated)).c_str());
}

/*Called once on submenu open*/
void NetworkAccountKDMenu::UpdateOnce() {
	m_Vars.m_Kills = Stats::Interface<int>::Get("MPPLY_KILLS_PLAYERS");
	m_Vars.m_Deaths = Stats::Interface<int>::Get("MPPLY_DEATHS_PLAYER");
}

/*Called always*/
void NetworkAccountKDMenu::FeatureUpdate() {}

NetworkAccountKDMenu* _instance;
NetworkAccountKDMenu* NetworkAccountKDMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkAccountKDMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkAccountKDMenu::~NetworkAccountKDMenu() { delete _instance; }