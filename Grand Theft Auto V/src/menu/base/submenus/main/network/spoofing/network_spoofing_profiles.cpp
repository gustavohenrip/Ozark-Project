#include "network_spoofing_profiles.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_spoofing.hpp"
#include "profiles/network_spoofing_profiles_selected.hpp"
#include "utils/json.hpp"
#include "network_spoofing_ip.hpp"
#include "network_spoofing_name.hpp"
#include "network_spoofing_rid.hpp"
#include "crew/network_spoofing_crew_icon.hpp"
#include "crew/network_spoofing_crew_motto.hpp"
#include "crew/network_spoofing_crew_name.hpp"
#include "crew/network_spoofing_crew_rank.hpp"
#include "crew/network_spoofing_crew_tag.hpp"
#include "utils/log.hpp"

using namespace NetworkSpoofingProfilesMenuVars;

namespace NetworkSpoofingProfilesMenuVars {
	Vars_ m_Vars;

	void DisableActiveProfile() {
		m_Vars.m_Active.m_Active = false;

		if (m_Vars.m_Active.m_SpoofIP) NetworkSpoofingIPMenuVars::SpoofIP(NetworkSpoofingIPMenu::GetInstance(), "", true);
		if (m_Vars.m_Active.m_SpoofName) NetworkSpoofingNameMenuVars::Spoof(NetworkSpoofingNameMenu::GetInstance(), true);
		if (m_Vars.m_Active.m_SpoofRockstarID) NetworkSpoofingRockstarIDMenuVars::SpoofRockstarID(NetworkSpoofingRockstarIDMenu::GetInstance(), 0, true);

		if (m_Vars.m_Active.m_SpoofCrew) {
			NetworkSpoofingCrewIconMenuVars::Spoof(NetworkSpoofingCrewIconMenu::GetInstance(), true);
			NetworkSpoofingCrewMottoMenuVars::Spoof(NetworkSpoofingCrewMottoMenu::GetInstance(), true);
			NetworkSpoofingCrewNameMenuVars::Spoof(NetworkSpoofingCrewNameMenu::GetInstance(), true);
			NetworkSpoofingCrewRankMenuVars::Spoof(NetworkSpoofingCrewRankMenu::GetInstance(), true);
			NetworkSpoofingCrewTagMenuVars::Spoof(NetworkSpoofingCrewTagMenu::GetInstance(), true);
		}
	}

	void SaveProfile(SpoofProfile Profile) {
		if (Profile.m_FileName.empty()) Profile.m_FileName = Profile.m_Name;
		Profile.m_FileName += ".json";

		nlohmann::json Json;
		Json["rid"] = Profile.m_RockstarID;
		Json["name"] = Profile.m_Name;
		Json["ip"] = Profile.m_IP;
		Json["stats"] = Profile.m_Stats;
		Json["crew"]["icon"] = Profile.m_CrewIcon;
		Json["crew"]["motto"] = Profile.m_CrewMotto;
		Json["crew"]["name"] = Profile.m_CrewName;
		Json["crew"]["rank"] = Profile.m_CrewRank;
		Json["crew"]["tag"] = Profile.m_CrewTag;

		std::ofstream Output(Utils::GetConfig()->GetSpoofingProfilesPath() + Profile.m_FileName);
		if (Output.good()) {
			Output << Json.dump(4);
			Output.close();
		}
	}
}

void NetworkSpoofingProfilesMenu::Init() {
	SetName("Profiles");
	SetParentSubmenu<NetworkSpoofingMenu>();

	addOption(ButtonOption("Disable Active Profile")
		.addTranslation()
		.addOnClick(DisableActiveProfile)
		.addRequirement([] { return m_Vars.m_Active.m_Active; }));

	addOption(BreakOption("Profiles")
		.addTranslation()
		.addRequirement([] { return m_Vars.m_Active.m_Active; }));

	addString("~m~None");
}

void NetworkSpoofingProfilesMenu::Update() {}

/*Called once on submenu open*/
void NetworkSpoofingProfilesMenu::UpdateOnce() {
	ClearOptionsOffset(2);

	m_Vars.m_Profiles.clear();

	auto Profiles = Utils::GetConfig()->GetFilesInDirectory(Utils::GetConfig()->GetSpoofingProfilesPath(), ".json");
	if (Profiles.size()) {
		for (auto& Profile : Profiles) {
			std::ifstream Input(Utils::GetConfig()->GetSpoofingProfilesPath() + Profile + ".json");
			if (Input.good()) {
				try {
					nlohmann::json Json;
					Input >> Json;
					Input.close();

					SpoofProfile Info;
					Info.m_FileName = Profile;
					Info.m_RockstarID = Json["rid"].get<int>();
					Info.m_IP = Json["ip"];
					Info.m_Name = Json["name"];
					Info.m_CrewIcon = Json["crew"]["icon"];
					Info.m_CrewMotto = Json["crew"]["motto"];
					Info.m_CrewName = Json["crew"]["name"];
					Info.m_CrewRank = Json["crew"]["rank"];
					Info.m_CrewTag = Json["crew"]["tag"];

					for (auto& it = Json["stats"].begin(); it != Json["stats"].end(); ++it) {
						Info.m_Stats.push_back(it->get<int>());
					}

					std::size_t Index = m_Vars.m_Profiles.size();
					if (Index < 0) Index = 0;
					m_Vars.m_Profiles.push_back(Info);

					addOption(SubmenuOption(Profile)
						.addSubmenu<NetworkSpoofingProfilesSelectedMenu>()
						.addOnClick([=] {
						NetworkSpoofingProfilesSelectedMenu::GetInstance()->SetName(Profile, false, false);
						NetworkSpoofingProfilesSelectedMenuVars::m_Vars.m_Selected = Index;
					}));
				} catch (std::exception& E) {
					LOG_WARN("[JSON] %s", E.what());
				}
			}
		}
	} else {
		addOption(ButtonOption(getString("~m~None")));
	}
}

/*Called always*/
void NetworkSpoofingProfilesMenu::FeatureUpdate() {}

NetworkSpoofingProfilesMenu* _instance;
NetworkSpoofingProfilesMenu* NetworkSpoofingProfilesMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkSpoofingProfilesMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkSpoofingProfilesMenu::~NetworkSpoofingProfilesMenu() { delete _instance; }