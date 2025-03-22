#pragma once
#include "menu/base/submenu.hpp"

class NetworkSpoofingProfilesSelectedMenu : public Menu::Submenu {
public:
	static NetworkSpoofingProfilesSelectedMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkSpoofingProfilesSelectedMenu()
		: Menu::Submenu() {}

	~NetworkSpoofingProfilesSelectedMenu();
};

namespace NetworkSpoofingProfilesSelectedMenuVars {
	struct Vars_ {
		std::size_t m_Selected;
		bool m_SpoofName = true;
		bool m_SpoofRockstarID = true;
		bool m_SpoofIP = true;
		bool m_SpoofStats = true;
		bool m_SpoofCrew = true;
	};

	extern Vars_ m_Vars;
}