#include "world_advanced_ui_traffic_lights.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../world_advanced_ui.hpp"
#include "utils/memory/memory.hpp"
#include "world_advanced_ui_color.hpp"
#include "utils/log.hpp"
#include "world_advanced_ui_struct.hpp"

using namespace WorldAdvancedUITrafficLightMenuVars;

namespace WorldAdvancedUITrafficLightMenuVars {
	Vars_ m_Vars;

	void ReadConfig(WorldAdvancedUITrafficLightMenu* This, WorldAdvancedUIColorMenuVars::CustomColor* Color, std::string Name) {
		Color->m_SaveToConfig = Utils::GetConfig()->ReadBool(This->m_ParentNameStack, "Save", Color->m_SaveToConfig, { Name });
		if (Color->m_SaveToConfig) {
			Color->m_RainbowToggle = Utils::GetConfig()->ReadBool(This->m_ParentNameStack, "Rainbow", Color->m_RainbowToggle, { Name });
			if (Color->m_RainbowToggle) {
				Color->m_Rainbow.Add(&Color->m_InternalColor);
				Color->m_Rainbow.Toggle(true);
			}

			Utils::GetConfig()->ReadVector(This->m_ParentNameStack, Name.c_str(), Color->m_Color, { Name });
		}
	}

	void WriteConfig(WorldAdvancedUITrafficLightMenu* This, WorldAdvancedUIColorMenuVars::CustomColor Color, std::string Name) {
		Utils::GetConfig()->WriteBool(This->m_ParentNameStack, "Save", Color.m_SaveToConfig, { Name });
		Utils::GetConfig()->WriteBool(This->m_ParentNameStack, "Rainbow", Color.m_RainbowToggle, { Name });
		Utils::GetConfig()->WriteVector(This->m_ParentNameStack, Name.c_str(), *Color.m_Color, { Name });
	}
}

void WorldAdvancedUITrafficLightMenu::Init() {
	SetName("UI: Traffic Lights");
	SetParentSubmenu<WorldAdvancedUIMenu>();

	m_Vars.m_FarFadeStart = (float*)Memory::GetAddressFromInstruction(Global::Vars::g_UpdateTrafficLights + 0x149, 4, 8);	// updated
	m_Vars.m_FarFadeEnd = (float*)Memory::GetAddressFromInstruction(Global::Vars::g_UpdateTrafficLights + 0x13D, 4, 8);		// updated
	m_Vars.m_NearFadeStart = (float*)Memory::GetAddressFromInstruction(Global::Vars::g_UpdateTrafficLights + 0x187, 4, 8);	// updated
	m_Vars.m_NearFadeEnd = (float*)Memory::GetAddressFromInstruction(Global::Vars::g_UpdateTrafficLights + 0x17C, 4, 8);	// updated

	m_Vars.m_NearColor.m_Color = (Rage::Types::UIElementColorStruct*)Memory::GetAddressFromInstruction(Global::Vars::g_SetTrafficLights + 0x28, 3, 7);
	m_Vars.m_RedColor.m_Color = (Math::Vector3_<float>*)Memory::GetAddressFromInstruction(Global::Vars::g_SetTrafficLightColors + 0x63, 3, 7);
	m_Vars.m_GreenColor.m_Color = (Math::Vector3_<float>*)Memory::GetAddressFromInstruction(Global::Vars::g_SetTrafficLightColors + 0x8D, 3, 7);
	m_Vars.m_AmberColor.m_Color = (Math::Vector3_<float>*)Memory::GetAddressFromInstruction(Global::Vars::g_SetTrafficLightColors + 0xBD, 3, 7);
	m_Vars.m_WalkColor.m_Color = (Math::Vector3_<float>*)Memory::GetAddressFromInstruction(Global::Vars::g_SetTrafficLights + 0x13F, 3, 7);
	m_Vars.m_DontWalkColor.m_Color = (Math::Vector3_<float>*)Memory::GetAddressFromInstruction(Global::Vars::g_SetTrafficLights + 0x14E, 3, 7);

	m_Vars.m_NearColor.m_DefaultColor = *m_Vars.m_NearColor.m_Color;
	m_Vars.m_RedColor.m_DefaultColor = *m_Vars.m_RedColor.m_Color;
	m_Vars.m_GreenColor.m_DefaultColor = *m_Vars.m_GreenColor.m_Color;
	m_Vars.m_AmberColor.m_DefaultColor = *m_Vars.m_AmberColor.m_Color;
	m_Vars.m_WalkColor.m_DefaultColor = *m_Vars.m_WalkColor.m_Color;
	m_Vars.m_DontWalkColor.m_DefaultColor = *m_Vars.m_DontWalkColor.m_Color;

	// Configs
	WorldAdvancedUIStructMenuVars::ReadStructFromConfig(&m_Vars.m_NearColor, "UI: Traffic Lights", "Near Color");

	addOption(SubmenuOption("Near Color")
		.addTranslation()
		.addSubmenu<WorldAdvancedUIStructMenu>()
		.addOnClick([] {
			WorldAdvancedUIStructMenuVars::m_Vars.m_Section = "UI: Traffic Lights";
			WorldAdvancedUIStructMenuVars::m_Vars.m_Key = "Near Color";
			WorldAdvancedUIStructMenuVars::m_Vars.m_Color = &m_Vars.m_NearColor;
			WorldAdvancedUIStructMenu::GetInstance()->SetName("Near Color");
			WorldAdvancedUIStructMenu::GetInstance()->SetParentSubmenu<WorldAdvancedUITrafficLightMenu>();
		}));

	addOption(SubmenuOption("Red Color")
		.addTranslation()
		.addSubmenu<WorldAdvancedUIColorMenu>()
		.addOnClick([] {
			WorldAdvancedUIColorMenu::GetInstance()->SetParentSubmenu<WorldAdvancedUITrafficLightMenu>();
			WorldAdvancedUIColorMenuVars::m_Vars.m_Color = &m_Vars.m_RedColor;
		}));

	addOption(SubmenuOption("Amber Color")
		.addTranslation()
		.addSubmenu<WorldAdvancedUIColorMenu>()
		.addOnClick([] {
			WorldAdvancedUIColorMenu::GetInstance()->SetParentSubmenu<WorldAdvancedUITrafficLightMenu>();
			WorldAdvancedUIColorMenuVars::m_Vars.m_Color = &m_Vars.m_AmberColor;
		}));

	addOption(SubmenuOption("Green Color")
		.addTranslation()
		.addSubmenu<WorldAdvancedUIColorMenu>()
		.addOnClick([] {
			WorldAdvancedUIColorMenu::GetInstance()->SetParentSubmenu<WorldAdvancedUITrafficLightMenu>();
			WorldAdvancedUIColorMenuVars::m_Vars.m_Color = &m_Vars.m_GreenColor;
		}));

	addOption(SubmenuOption("Walk Color")
		.addTranslation()
		.addSubmenu<WorldAdvancedUIColorMenu>()
		.addOnClick([] {
			WorldAdvancedUIColorMenu::GetInstance()->SetParentSubmenu<WorldAdvancedUITrafficLightMenu>();
			WorldAdvancedUIColorMenuVars::m_Vars.m_Color = &m_Vars.m_WalkColor;
		}));

	addOption(SubmenuOption("Don't Walk Color")
		.addTranslation()
		.addSubmenu<WorldAdvancedUIColorMenu>()
		.addOnClick([] {
			WorldAdvancedUIColorMenu::GetInstance()->SetParentSubmenu<WorldAdvancedUITrafficLightMenu>();
			WorldAdvancedUIColorMenuVars::m_Vars.m_Color = &m_Vars.m_DontWalkColor;
		}));

	addOption(NumberOption<float>(SCROLL, "Far Fade Start")
		.addTranslation()
		.addNumber(*m_Vars.m_FarFadeStart, "%.0f", 1.f).addMin(-100.f).addMax(1000000.f));

	addOption(NumberOption<float>(SCROLL, "Far Fade End")
		.addTranslation()
		.addNumber(*m_Vars.m_FarFadeEnd, "%.0f", 1.f).addMin(-100.f).addMax(1000000.f));

	addOption(NumberOption<float>(SCROLL, "Near Fade Start")
		.addTranslation()
		.addNumber(*m_Vars.m_NearFadeStart, "%.0f", 1.f).addMin(-100.f).addMax(1000000.f));

	addOption(NumberOption<float>(SCROLL, "Near Fade End")
		.addTranslation()
		.addNumber(*m_Vars.m_NearFadeEnd, "%.0f", 1.f).addMin(-100.f).addMax(1000000.f));

	ReadConfig(this, &m_Vars.m_RedColor, "Red Color");
	ReadConfig(this, &m_Vars.m_GreenColor, "Green Color");
	ReadConfig(this, &m_Vars.m_AmberColor, "Amber Color");
	ReadConfig(this, &m_Vars.m_WalkColor, "Walk Color");
	ReadConfig(this, &m_Vars.m_DontWalkColor, "Dont Walk Color");
}

void WorldAdvancedUITrafficLightMenu::Update() {

}

/*Called once on submenu open*/
void WorldAdvancedUITrafficLightMenu::UpdateOnce() {
	auto ConvertVector = [] (Math::Vector4<float> Vec) -> Math::Vector3_<float> {
		return { Vec.m_X, Vec.m_Y, Vec.m_Z };
	};
	
	WriteConfig(this, m_Vars.m_RedColor, "Red Color");
	WriteConfig(this, m_Vars.m_GreenColor, "Green Color");
	WriteConfig(this, m_Vars.m_AmberColor, "Amber Color");
	WriteConfig(this, m_Vars.m_WalkColor, "Walk Color");
	WriteConfig(this, m_Vars.m_DontWalkColor, "Dont Walk Color");
}

/*Called always*/
void WorldAdvancedUITrafficLightMenu::FeatureUpdate() {
	WorldAdvancedUIColorMenuVars::CustomColor* List[] = {
		&m_Vars.m_RedColor,
		&m_Vars.m_GreenColor,
		&m_Vars.m_AmberColor,
		&m_Vars.m_WalkColor,
		&m_Vars.m_DontWalkColor
	};

	for (auto& L : List) {
		if (L->m_RainbowToggle) {
			L->m_Rainbow.Run();
			L->m_Color->m_X = L->m_InternalColor.R / 255.f;
			L->m_Color->m_Y = L->m_InternalColor.G / 255.f;
			L->m_Color->m_Z = L->m_InternalColor.B / 255.f;
		}
	}
}

WorldAdvancedUITrafficLightMenu* _instance;
WorldAdvancedUITrafficLightMenu* WorldAdvancedUITrafficLightMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WorldAdvancedUITrafficLightMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WorldAdvancedUITrafficLightMenu::~WorldAdvancedUITrafficLightMenu() { delete _instance; }