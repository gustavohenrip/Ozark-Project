#include "world_advanced_ui_vehicle.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../world_advanced_ui.hpp"
#include "world_advanced_ui_struct.hpp"
#include "utils/memory/memory.hpp"

using namespace WorldAdvancedUIVehicleMenuVars;

namespace WorldAdvancedUIVehicleMenuVars {
	Vars_ m_Vars;
}

void WorldAdvancedUIVehicleMenu::Init() {
	SetName("UI: Vehicle");
	SetParentSubmenu<WorldAdvancedUIMenu>();

	// Init
	m_Vars.m_InteriorLightColor.m_Color = (Rage::Types::UIElementColorStruct*)Memory::GetAddressFromInstruction(Global::Vars::g_SetVehicleLights + 0x45, 3, 7);
	m_Vars.m_PlateLightColor.m_Color = (Rage::Types::UIElementColorStruct*)Memory::GetAddressFromInstruction(Global::Vars::g_SetVehicleLights + 0x90, 3, 7);
	m_Vars.m_DashLightColor.m_Color = (Rage::Types::UIElementColorStruct*)Memory::GetAddressFromInstruction(Global::Vars::g_SetVehicleLights + 0xA9, 3, 7);
	m_Vars.m_DoorLightColor.m_Color = (Rage::Types::UIElementColorStruct*)Memory::GetAddressFromInstruction(Global::Vars::g_SetVehicleLights + 0xc2, 3, 7);

	m_Vars.m_InteriorLightColor.m_DefaultColor = *m_Vars.m_InteriorLightColor.m_Color;
	m_Vars.m_PlateLightColor.m_DefaultColor = *m_Vars.m_PlateLightColor.m_Color;
	m_Vars.m_DashLightColor.m_DefaultColor = *m_Vars.m_DashLightColor.m_Color;
	m_Vars.m_DoorLightColor.m_DefaultColor = *m_Vars.m_DoorLightColor.m_Color;

	// Configs
	WorldAdvancedUIStructMenuVars::ReadStructFromConfig(&m_Vars.m_InteriorLightColor, "UI: Vehicle", "Interior Light");
	WorldAdvancedUIStructMenuVars::ReadStructFromConfig(&m_Vars.m_PlateLightColor, "UI: Vehicle", "Plate Light");
	WorldAdvancedUIStructMenuVars::ReadStructFromConfig(&m_Vars.m_DashLightColor, "UI: Vehicle", "Dash Light");
	WorldAdvancedUIStructMenuVars::ReadStructFromConfig(&m_Vars.m_DoorLightColor, "UI: Vehicle", "Door Light");

	addOption(SubmenuOption("Interior Light")
		.addTranslation()
		.addSubmenu<WorldAdvancedUIStructMenu>()
		.addOnClick([] {
			WorldAdvancedUIStructMenuVars::m_Vars.m_Section = "UI: Vehicle";
			WorldAdvancedUIStructMenuVars::m_Vars.m_Key = "Interior Light";
			WorldAdvancedUIStructMenuVars::m_Vars.m_Color = &m_Vars.m_InteriorLightColor;
			WorldAdvancedUIStructMenu::GetInstance()->SetName("Interior Light");
			WorldAdvancedUIStructMenu::GetInstance()->SetParentSubmenu<WorldAdvancedUIVehicleMenu>();
		}));

	addOption(SubmenuOption("Plate Light")
		.addTranslation()
		.addSubmenu<WorldAdvancedUIStructMenu>()
		.addOnClick([] {
			WorldAdvancedUIStructMenuVars::m_Vars.m_Section = "UI: Vehicle";
			WorldAdvancedUIStructMenuVars::m_Vars.m_Key = "Plate Light";
			WorldAdvancedUIStructMenuVars::m_Vars.m_Color = &m_Vars.m_PlateLightColor;
			WorldAdvancedUIStructMenu::GetInstance()->SetName("Plate Light");
			WorldAdvancedUIStructMenu::GetInstance()->SetParentSubmenu<WorldAdvancedUIVehicleMenu>();
		}));

	addOption(SubmenuOption("Dash Light")
		.addTranslation()
		.addSubmenu<WorldAdvancedUIStructMenu>()
		.addOnClick([] {
			WorldAdvancedUIStructMenuVars::m_Vars.m_Section = "UI: Vehicle";
			WorldAdvancedUIStructMenuVars::m_Vars.m_Key = "Dash Light";
			WorldAdvancedUIStructMenuVars::m_Vars.m_Color = &m_Vars.m_DashLightColor;
			WorldAdvancedUIStructMenu::GetInstance()->SetName("Dash Light");
			WorldAdvancedUIStructMenu::GetInstance()->SetParentSubmenu<WorldAdvancedUIVehicleMenu>();
		}));

	addOption(SubmenuOption("Door Light")
		.addTranslation()
		.addSubmenu<WorldAdvancedUIStructMenu>()
		.addOnClick([] {
			WorldAdvancedUIStructMenuVars::m_Vars.m_Section = "UI: Vehicle";
			WorldAdvancedUIStructMenuVars::m_Vars.m_Key = "Door Light";
			WorldAdvancedUIStructMenuVars::m_Vars.m_Color = &m_Vars.m_DoorLightColor;
			WorldAdvancedUIStructMenu::GetInstance()->SetName("Door Light");
			WorldAdvancedUIStructMenu::GetInstance()->SetParentSubmenu<WorldAdvancedUIVehicleMenu>();
		}));
}

void WorldAdvancedUIVehicleMenu::Update() {}

/*Called once on submenu open*/
void WorldAdvancedUIVehicleMenu::UpdateOnce() {}

/*Called always*/
void WorldAdvancedUIVehicleMenu::FeatureUpdate() {
	if (m_Vars.m_InteriorLightColor.m_RainbowToggle) {
		m_Vars.m_InteriorLightColor.m_Rainbow.Run();
		m_Vars.m_InteriorLightColor.m_Color->m_Color.m_X = m_Vars.m_InteriorLightColor.m_InternalColor.R / 255.f;
		m_Vars.m_InteriorLightColor.m_Color->m_Color.m_Y = m_Vars.m_InteriorLightColor.m_InternalColor.G / 255.f;
		m_Vars.m_InteriorLightColor.m_Color->m_Color.m_Z = m_Vars.m_InteriorLightColor.m_InternalColor.B / 255.f;
	}

	if (m_Vars.m_PlateLightColor.m_RainbowToggle) {
		m_Vars.m_PlateLightColor.m_Rainbow.Run();
		m_Vars.m_PlateLightColor.m_Color->m_Color.m_X = m_Vars.m_PlateLightColor.m_InternalColor.R / 255.f;
		m_Vars.m_PlateLightColor.m_Color->m_Color.m_Y = m_Vars.m_PlateLightColor.m_InternalColor.G / 255.f;
		m_Vars.m_PlateLightColor.m_Color->m_Color.m_Z = m_Vars.m_PlateLightColor.m_InternalColor.B / 255.f;
	}

	if (m_Vars.m_DashLightColor.m_RainbowToggle) {
		m_Vars.m_DashLightColor.m_Rainbow.Run();
		m_Vars.m_DashLightColor.m_Color->m_Color.m_X = m_Vars.m_DashLightColor.m_InternalColor.R / 255.f;
		m_Vars.m_DashLightColor.m_Color->m_Color.m_Y = m_Vars.m_DashLightColor.m_InternalColor.G / 255.f;
		m_Vars.m_DashLightColor.m_Color->m_Color.m_Z = m_Vars.m_DashLightColor.m_InternalColor.B / 255.f;
	}

	if (m_Vars.m_DoorLightColor.m_RainbowToggle) {
		m_Vars.m_DoorLightColor.m_Rainbow.Run();
		m_Vars.m_DoorLightColor.m_Color->m_Color.m_X = m_Vars.m_DoorLightColor.m_InternalColor.R / 255.f;
		m_Vars.m_DoorLightColor.m_Color->m_Color.m_Y = m_Vars.m_DoorLightColor.m_InternalColor.G / 255.f;
		m_Vars.m_DoorLightColor.m_Color->m_Color.m_Z = m_Vars.m_DoorLightColor.m_InternalColor.B / 255.f;
	}
}

WorldAdvancedUIVehicleMenu* _instance;
WorldAdvancedUIVehicleMenu* WorldAdvancedUIVehicleMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WorldAdvancedUIVehicleMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WorldAdvancedUIVehicleMenu::~WorldAdvancedUIVehicleMenu() { delete _instance; }