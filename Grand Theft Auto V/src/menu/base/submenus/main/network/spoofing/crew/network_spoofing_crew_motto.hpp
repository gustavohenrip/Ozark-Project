#pragma once
#include "menu/base/submenu.hpp"

class NetworkSpoofingCrewMottoMenu : public Menu::Submenu {
public:
	static NetworkSpoofingCrewMottoMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkSpoofingCrewMottoMenu()
		: Menu::Submenu() {}

	~NetworkSpoofingCrewMottoMenu();
};

namespace NetworkSpoofingCrewMottoMenuVars {
	struct Vars_ {
		std::string m_TempName;
		std::string m_SpoofedName;

		bool m_SaveToConfig;

		char m_PreviewName[50];
		std::shared_ptr<ButtonOption> m_PreviewButton;
	};

	void Spoof(NetworkSpoofingCrewMottoMenu* This, bool Reset, const char* Name = NULL);
	extern Vars_ m_Vars;
}