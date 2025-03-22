#pragma once
#include "menu/base/submenu.hpp"

class NetworkSpoofingNameMenu : public Menu::Submenu {
public:
	static NetworkSpoofingNameMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkSpoofingNameMenu()
		: Menu::Submenu() {}

	~NetworkSpoofingNameMenu();
};

namespace NetworkSpoofingNameMenuVars {
	struct Vars_ {
		int m_Color;
		int m_Style;
		bool m_SaveToConfig;
		std::string m_TempName;
		std::string m_SpoofedName;
		char m_OriginalName[20];
		char m_PreviewName[50];
		std::shared_ptr<ButtonOption> m_PreviewButton;
	};

	void Spoof(NetworkSpoofingNameMenu* This, bool Reset, const char* Name = NULL);
	extern Vars_ m_Vars;
}