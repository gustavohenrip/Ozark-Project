#pragma once
#include "menu/base/submenu.hpp"
#include "world_advanced_ui_color.hpp"
#include "world_advanced_ui_struct.hpp"

class WorldAdvancedUITrafficLightMenu : public Menu::Submenu {
public:
	static WorldAdvancedUITrafficLightMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WorldAdvancedUITrafficLightMenu()
		: Menu::Submenu() {}

	~WorldAdvancedUITrafficLightMenu();
};

namespace WorldAdvancedUITrafficLightMenuVars {
	
	struct Vars_ {
		float* m_FarFadeStart;
		float* m_FarFadeEnd;
		float* m_NearFadeStart;
		float* m_NearFadeEnd;

		WorldAdvancedUIColorMenuVars::CustomColor m_RedColor;
		WorldAdvancedUIColorMenuVars::CustomColor m_GreenColor;
		WorldAdvancedUIColorMenuVars::CustomColor m_AmberColor;
		WorldAdvancedUIColorMenuVars::CustomColor m_WalkColor;
		WorldAdvancedUIColorMenuVars::CustomColor m_DontWalkColor;
		WorldAdvancedUIStructMenuVars::CustomColor m_NearColor;
	};

	extern Vars_ m_Vars;
}