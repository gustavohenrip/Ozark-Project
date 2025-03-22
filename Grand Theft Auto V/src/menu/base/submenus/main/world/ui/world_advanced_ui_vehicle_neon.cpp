#include "world_advanced_ui_vehicle_neon.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../world_advanced_ui.hpp"
#include "utils/memory/memory.hpp"

using namespace WorldAdvancedUIVehicleNeonsMenuVars;

namespace WorldAdvancedUIVehicleNeonsMenuVars {
	Vars_ m_Vars;
}

void WorldAdvancedUIVehicleNeonsMenu::Init() {
	SetName("UI: Vehicle Neons");
	SetParentSubmenu<WorldAdvancedUIMenu>();

	float* Intensity = (float*)(Memory::GetAddressFromInstruction(Global::Vars::g_SetVehicleNeonUI + 0x35, 4, 8));
	float* Radius = (float*)(Memory::GetAddressFromInstruction(Global::Vars::g_SetVehicleNeonUI + 0x57, 4, 8));
	float* FalloffExponent = (float*)(Memory::GetAddressFromInstruction(Global::Vars::g_SetVehicleNeonUI + 0x79, 4, 8));
	float* CapsuleExtentSides = (float*)(Memory::GetAddressFromInstruction(Global::Vars::g_SetVehicleNeonUI + 0x9B, 4, 8));
	float* CapsuleExtentFrontBack = (float*)(Memory::GetAddressFromInstruction(Global::Vars::g_SetVehicleNeonUI + 0xBD, 4, 8));
	float* ClipPlaneHeight = (float*)(Memory::GetAddressFromInstruction(Global::Vars::g_SetVehicleNeonUI + 0xDF, 4, 8));
	float* BikeClipPlaneHeight = (float*)(Memory::GetAddressFromInstruction(Global::Vars::g_SetVehicleNeonUI + 0xFB, 4, 8));

	m_Vars.m_Intensity = *Intensity;
	m_Vars.m_Radius = *Radius;
	m_Vars.m_FalloffExponent = *FalloffExponent;
	m_Vars.m_CapsuleExtentSides = *CapsuleExtentSides;
	m_Vars.m_CapsuleExtentFrontBack = *CapsuleExtentFrontBack;
	m_Vars.m_ClipPlaneHeight = *ClipPlaneHeight;
	m_Vars.m_BikeClipPlaneHeight = *BikeClipPlaneHeight;

	addOption(NumberOption<float>(SCROLL, "Intensity")
		.addTranslation()
		.addNumber(*Intensity, "%.2f", 0.01f).setScrollSpeed(10).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Radius")
		.addTranslation()
		.addNumber(*Radius, "%.2f", 0.01f).setScrollSpeed(10).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Falloff Exponent")
		.addTranslation()
		.addNumber(*FalloffExponent, "%.2f", 0.01f).setScrollSpeed(10).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Capsule Extent Sides")
		.addTranslation()
		.addNumber(*CapsuleExtentSides, "%.2f", 0.01f).setScrollSpeed(10).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Capsule Extent Front Back")
		.addTranslation()
		.addNumber(*CapsuleExtentFrontBack, "%.2f", 0.01f).setScrollSpeed(10).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Clip Plane Height")
		.addTranslation()
		.addNumber(*ClipPlaneHeight, "%.2f", 0.01f).setScrollSpeed(10).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Bike Clip Plane Height")
		.addTranslation()
		.addNumber(*BikeClipPlaneHeight, "%.2f", 0.01f).setScrollSpeed(10).canBeSaved(m_ParentNameStack));

	addOption(ButtonOption("Reset to Default")
		.addTranslation()
		.addSprite("Ozark", "spinner")
		.addSpriteScale(0.01232f, 0.02184f)
		.addSpriteRotate()
		.addOnClick([=] {
			*Intensity = m_Vars.m_Intensity;
			*Radius = m_Vars.m_Radius;
			*FalloffExponent = m_Vars.m_FalloffExponent;
			*CapsuleExtentSides = m_Vars.m_CapsuleExtentSides;
			*CapsuleExtentFrontBack = m_Vars.m_CapsuleExtentFrontBack;
			*ClipPlaneHeight = m_Vars.m_ClipPlaneHeight;
			*BikeClipPlaneHeight = m_Vars.m_BikeClipPlaneHeight;

			for (auto& O : m_Options) {
				if (strcmp(O->GetName().GetOriginal().c_str(), "Reset to Default")) {
					O->InvokeSave(m_ParentNameStack);
				}
			}
		}));
}

void WorldAdvancedUIVehicleNeonsMenu::Update() {}

/*Called once on submenu open*/
void WorldAdvancedUIVehicleNeonsMenu::UpdateOnce() {}

/*Called always*/
void WorldAdvancedUIVehicleNeonsMenu::FeatureUpdate() {}

WorldAdvancedUIVehicleNeonsMenu* _instance;
WorldAdvancedUIVehicleNeonsMenu* WorldAdvancedUIVehicleNeonsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WorldAdvancedUIVehicleNeonsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WorldAdvancedUIVehicleNeonsMenu::~WorldAdvancedUIVehicleNeonsMenu() { delete _instance; }