#pragma once
#include "menu/base/submenu.hpp"
#include "rage/types/network_types.hpp"
#include "menu/base/util/rainbow.hpp"

class WorldAdvancedUIStructMenu : public Menu::Submenu {
public:
	static WorldAdvancedUIStructMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WorldAdvancedUIStructMenu()
		: Menu::Submenu() {}

	~WorldAdvancedUIStructMenu();
};

namespace WorldAdvancedUIStructMenuVars {
	struct CustomColor {
		Rage::Types::UIElementColorStruct m_DefaultColor;
		Rage::Types::UIElementColorStruct* m_Color;
		Color m_InternalColor;
		bool m_RainbowToggle;
		bool m_SaveToConfig;
		Menu::Rainbow::Rainbow m_Rainbow;
	};

	struct Vars_ {
		CustomColor* m_Color;
		std::string m_Section;
		std::string m_Key;
	};

	void ReadStructFromConfig(CustomColor* Ptr, std::string Section, std::string Key);
	extern Vars_ m_Vars;
}