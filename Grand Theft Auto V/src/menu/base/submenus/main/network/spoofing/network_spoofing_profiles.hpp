#pragma once
#include "menu/base/submenu.hpp"

class NetworkSpoofingProfilesMenu : public Menu::Submenu {
public:
	static NetworkSpoofingProfilesMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkSpoofingProfilesMenu()
		: Menu::Submenu() {}

	~NetworkSpoofingProfilesMenu();
};

namespace NetworkSpoofingProfilesMenuVars {
	struct SpoofProfile {
		int m_RockstarID;
		std::string m_IP;
		std::string m_Name;
		std::string m_FileName;
		std::vector<int> m_Stats;

		int m_CrewIcon;
		std::string m_CrewMotto;
		std::string m_CrewName;
		std::string m_CrewRank;
		std::string m_CrewTag;
	};

	struct SpoofActive {
		bool m_Active;
		bool m_SpoofName;
		bool m_SpoofRockstarID;
		bool m_SpoofIP;
		bool m_SpoofStats;
		bool m_SpoofCrew;
		SpoofProfile m_Profile;
	};

	struct Vars_ {
		std::vector<SpoofProfile> m_Profiles;
		SpoofActive m_Active;
	};

	void SaveProfile(SpoofProfile Profile);
	void DisableActiveProfile();
	extern Vars_ m_Vars;
}