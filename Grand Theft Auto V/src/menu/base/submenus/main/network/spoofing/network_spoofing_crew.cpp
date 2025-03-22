#include "network_spoofing_crew.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_spoofing.hpp"
#include "crew/network_spoofing_crew_name.hpp"
#include "crew/network_spoofing_crew_tag.hpp"
#include "crew/network_spoofing_crew_motto.hpp"
#include "crew/network_spoofing_crew_rank.hpp"
#include "crew/network_spoofing_crew_icon.hpp"

using namespace NetworkSpoofingCrewMenuVars;

namespace NetworkSpoofingCrewMenuVars {
	Vars_ m_Vars;
}

void NetworkSpoofingCrewMenu::Init() {
	SetName("Crew");
	SetParentSubmenu<NetworkSpoofingMenu>();

	addOption(SubmenuOption("Crew Name")
		.addTranslation()
		.addSubmenu<NetworkSpoofingCrewNameMenu>());

	addOption(SubmenuOption("Crew Tag")
		.addTranslation()
		.addSubmenu<NetworkSpoofingCrewTagMenu>());

	addOption(SubmenuOption("Crew Motto")
		.addTranslation()
		.addSubmenu<NetworkSpoofingCrewMottoMenu>());

	addOption(SubmenuOption("Crew Rank")
		.addTranslation()
		.addSubmenu<NetworkSpoofingCrewRankMenu>());

	addOption(SubmenuOption("Crew Icon")
		.addTranslation()
		.addSubmenu<NetworkSpoofingCrewIconMenu>());
}

void NetworkSpoofingCrewMenu::Update() {}

/*Called once on submenu open*/
void NetworkSpoofingCrewMenu::UpdateOnce() {}

/*Called always*/
void NetworkSpoofingCrewMenu::FeatureUpdate() {}

NetworkSpoofingCrewMenu* _instance;
NetworkSpoofingCrewMenu* NetworkSpoofingCrewMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkSpoofingCrewMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkSpoofingCrewMenu::~NetworkSpoofingCrewMenu() { delete _instance; }