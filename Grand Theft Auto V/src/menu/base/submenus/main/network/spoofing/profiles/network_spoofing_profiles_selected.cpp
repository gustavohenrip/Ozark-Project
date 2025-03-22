#include "network_spoofing_profiles_selected.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_spoofing_profiles.hpp"
#include "utils/va.hpp"
#include "../network_spoofing_ip.hpp"
#include "../network_spoofing_name.hpp"
#include "../network_spoofing_rid.hpp"
#include "../crew/network_spoofing_crew_icon.hpp"
#include "../crew/network_spoofing_crew_motto.hpp"
#include "../crew/network_spoofing_crew_name.hpp"
#include "../crew/network_spoofing_crew_rank.hpp"
#include "../crew/network_spoofing_crew_tag.hpp"

using namespace NetworkSpoofingProfilesSelectedMenuVars;

namespace NetworkSpoofingProfilesSelectedMenuVars {
	Vars_ m_Vars;

	void LoadProfile() {
		auto& Profile = NetworkSpoofingProfilesMenuVars::m_Vars.m_Profiles[m_Vars.m_Selected];
		auto& Active = NetworkSpoofingProfilesMenuVars::m_Vars.m_Active.m_Profile;

		Active.m_RockstarID = Profile.m_RockstarID;
		Active.m_IP = Profile.m_IP;
		Active.m_Name = Profile.m_Name;
		Active.m_FileName = Profile.m_FileName;
		Active.m_Stats = Profile.m_Stats;
		Active.m_CrewIcon = Profile.m_CrewIcon;
		Active.m_CrewMotto = Profile.m_CrewMotto;
		Active.m_CrewName = Profile.m_CrewName;
		Active.m_CrewRank = Profile.m_CrewRank;
		Active.m_CrewTag = Profile.m_CrewTag;

		if (NetworkSpoofingProfilesMenuVars::m_Vars.m_Active.m_Active) {
			NetworkSpoofingProfilesMenuVars::DisableActiveProfile();
		}

		NetworkSpoofingProfilesMenuVars::m_Vars.m_Active.m_Active = true;
		NetworkSpoofingProfilesMenuVars::m_Vars.m_Active.m_SpoofName = m_Vars.m_SpoofName;
		NetworkSpoofingProfilesMenuVars::m_Vars.m_Active.m_SpoofRockstarID = m_Vars.m_SpoofRockstarID;
		NetworkSpoofingProfilesMenuVars::m_Vars.m_Active.m_SpoofIP = m_Vars.m_SpoofIP;
		NetworkSpoofingProfilesMenuVars::m_Vars.m_Active.m_SpoofStats = m_Vars.m_SpoofStats;
		NetworkSpoofingProfilesMenuVars::m_Vars.m_Active.m_SpoofCrew = m_Vars.m_SpoofCrew;

		if (m_Vars.m_SpoofIP) NetworkSpoofingIPMenuVars::SpoofIP(NetworkSpoofingIPMenu::GetInstance(), Profile.m_IP);
		if (m_Vars.m_SpoofName) NetworkSpoofingNameMenuVars::Spoof(NetworkSpoofingNameMenu::GetInstance(), false, Profile.m_Name.c_str());
		if (m_Vars.m_SpoofRockstarID) NetworkSpoofingRockstarIDMenuVars::SpoofRockstarID(NetworkSpoofingRockstarIDMenu::GetInstance(), Profile.m_RockstarID);

		if (m_Vars.m_SpoofCrew) {
			NetworkSpoofingCrewIconMenuVars::Spoof(NetworkSpoofingCrewIconMenu::GetInstance(), false, Active.m_CrewIcon);
			NetworkSpoofingCrewMottoMenuVars::Spoof(NetworkSpoofingCrewMottoMenu::GetInstance(), false, Active.m_CrewMotto.c_str());
			NetworkSpoofingCrewNameMenuVars::Spoof(NetworkSpoofingCrewNameMenu::GetInstance(), false, Active.m_CrewName.c_str());
			NetworkSpoofingCrewRankMenuVars::Spoof(NetworkSpoofingCrewRankMenu::GetInstance(), false, Active.m_CrewRank.c_str());
			NetworkSpoofingCrewTagMenuVars::Spoof(NetworkSpoofingCrewTagMenu::GetInstance(), false, Active.m_CrewTag.c_str());
		}
	}
}

void NetworkSpoofingProfilesSelectedMenu::Init() {
	SetName("", false, false);
	SetParentSubmenu<NetworkSpoofingProfilesMenu>();

	addOption(ToggleOption("Spoof Name")
		.addTranslation()
		.addToggle(m_Vars.m_SpoofName));

	addOption(ToggleOption("Spoof Rockstar ID")
		.addTranslation()
		.addToggle(m_Vars.m_SpoofRockstarID));

	addOption(ToggleOption("Spoof IP")
		.addTranslation()
		.addToggle(m_Vars.m_SpoofIP));

	addOption(ToggleOption("Spoof Stats")
		.addTranslation()
		.addToggle(m_Vars.m_SpoofStats));

	addOption(ToggleOption("Spoof Crew")
		.addTranslation()
		.addToggle(m_Vars.m_SpoofCrew));

	addOption(ButtonOption("Load Profile")
		.addTranslation()
		.addOnClick(LoadProfile));
}

void NetworkSpoofingProfilesSelectedMenu::Update() {}

/*Called once on submenu open*/
void NetworkSpoofingProfilesSelectedMenu::UpdateOnce() {}

/*Called always*/
void NetworkSpoofingProfilesSelectedMenu::FeatureUpdate() {

}

NetworkSpoofingProfilesSelectedMenu* _instance;
NetworkSpoofingProfilesSelectedMenu* NetworkSpoofingProfilesSelectedMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkSpoofingProfilesSelectedMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkSpoofingProfilesSelectedMenu::~NetworkSpoofingProfilesSelectedMenu() { delete _instance; }