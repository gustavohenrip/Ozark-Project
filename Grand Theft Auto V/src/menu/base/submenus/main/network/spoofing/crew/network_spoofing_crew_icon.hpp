#pragma once
#include "menu/base/submenu.hpp"

class NetworkSpoofingCrewIconMenu : public Menu::Submenu {
public:
	static NetworkSpoofingCrewIconMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkSpoofingCrewIconMenu()
		: Menu::Submenu() {}

	~NetworkSpoofingCrewIconMenu();
};

namespace NetworkSpoofingCrewIconMenuVars {
	struct Vars_ {
		int m_SpoofedIcon = -1;
		std::string m_TempName;

		bool m_SaveToConfig;

		char m_PreviewName[50];
		std::shared_ptr<ButtonOption> m_PreviewButton;
	};

	void Spoof(NetworkSpoofingCrewIconMenu* This, bool Reset, int Icon = -1);
	extern Vars_ m_Vars;
}