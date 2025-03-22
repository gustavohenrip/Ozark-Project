#pragma once
#include "menu/base/submenu.hpp"

class NetworkAccountOutfitsEditNameMenu : public Menu::Submenu {
public:
	static NetworkAccountOutfitsEditNameMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkAccountOutfitsEditNameMenu()
		: Menu::Submenu() {}

	~NetworkAccountOutfitsEditNameMenu();
};

namespace NetworkAccountOutfitsEditNameMenuVars {
	struct Vars_ {
		int m_Color;
		int m_Style;
		int m_Icon;
		std::string m_TempName;
		char m_PreviewName[50];
		std::shared_ptr<ButtonOption> m_PreviewButton;
	};

	extern Vars_ m_Vars;
}