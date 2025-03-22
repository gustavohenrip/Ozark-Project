#pragma once
#include "menu/base/submenu.hpp"

class NetworkSpoofingCrewNameMenu : public Menu::Submenu {
public:
	static NetworkSpoofingCrewNameMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkSpoofingCrewNameMenu()
		: Menu::Submenu() {}

	~NetworkSpoofingCrewNameMenu();
};

namespace NetworkSpoofingCrewNameMenuVars {
	struct Vars_ {
		std::string m_TempName;
		std::string m_SpoofedName;

		bool m_SaveToConfig;

		char m_PreviewName[50];
		std::shared_ptr<ButtonOption> m_PreviewButton;
	};

	void Spoof(NetworkSpoofingCrewNameMenu* This, bool Reset, const char* Name = NULL);
	extern Vars_ m_Vars;
}