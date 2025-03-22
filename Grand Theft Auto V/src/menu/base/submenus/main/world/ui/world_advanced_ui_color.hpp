#pragma once
#include "menu/base/submenu.hpp"
#include "rage/types/network_types.hpp"
#include "menu/base/util/rainbow.hpp"
#include "world_advanced_ui_struct.hpp"

class WorldAdvancedUIColorMenu : public Menu::Submenu {
public:
	static WorldAdvancedUIColorMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WorldAdvancedUIColorMenu()
		: Menu::Submenu() {}

	~WorldAdvancedUIColorMenu();
};

namespace WorldAdvancedUIColorMenuVars {
	struct CustomColor {
		Math::Vector3_<float> m_DefaultColor;
		Math::Vector3_<float>* m_Color;
		Color m_InternalColor;
		bool m_RainbowToggle;
		bool m_SaveToConfig;
		Menu::Rainbow::Rainbow m_Rainbow;
	};

	struct Vars_ {
		CustomColor* m_Color;
		WorldAdvancedUIStructMenuVars::CustomColor* m_Color2;
	};

	extern Vars_ m_Vars;
}