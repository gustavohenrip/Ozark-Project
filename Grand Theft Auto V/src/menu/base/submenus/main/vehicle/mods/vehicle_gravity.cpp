#include "vehicle_gravity.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_mods.hpp"
#include "rage/engine.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/control_manager.hpp"

using namespace VehicleModsGravityMenuVars;

namespace VehicleModsGravityMenuVars {
	Vars_ m_Vars;

	void Gravity() {
		if (Menu::GetLocalPlayer().m_InVehicle) {
			if (Rage::Engine::GetEntityAddress(Menu::GetLocalPlayer().m_Vehicle) == 0) return;
			Rage::Engine::SetVehicleGravity(Rage::Engine::GetEntityAddress(Menu::GetLocalPlayer().m_Vehicle), Global::Arrays::g_Gravity[m_Vars.m_Gravity].m_Result);
		}
	}
}

void VehicleModsGravityMenu::Init() {
	SetName("Gravity");
	SetParentSubmenu<VehicleModsMenu>();

	addOption(ScrollOption<float>(SCROLL, "Gravity")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_Gravity, 0, NUMOF(Global::Arrays::g_Gravity), Global::Arrays::g_Gravity)
		.addOnClick(Gravity));

	addOption(ToggleOption("Drive on Water")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DriveOnWater)
		.addOnClick([] {
			if (!m_Vars.m_DriveOnWater) {
				if (Native::DoesEntityExist(m_Vars.m_DriveOnWaterEntity)) {
					Native::SetEntityAsMissionEntity(m_Vars.m_DriveOnWaterEntity, true, true);
					Native::DeleteEntity(&m_Vars.m_DriveOnWaterEntity);
				}
			}
		}));

	addOption(ToggleOption("Slippy Wheels")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_SlippyWheels)
		.addOnClick([] { if (!m_Vars.m_SlippyWheels) if (Menu::GetLocalPlayer().m_InVehicle) Native::SetVehicleReduceGrip(Menu::GetLocalPlayer().m_Vehicle, false); }));

	addOption(ToggleOption("Freeze")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Freeze)
		.addOnClick([] { if (!m_Vars.m_Freeze) if (Menu::GetLocalPlayer().m_InVehicle) Native::FreezeEntityPosition(Menu::GetLocalPlayer().m_Vehicle, false); }));

	addOption(ToggleOption("Stick to Ground")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_StickToGround));

	addOption(ButtonOption("Place on Ground")
		.addTranslation().addHotkey()
		.addOnClick([] { if (Menu::GetLocalPlayer().m_InVehicle) Native::SetVehicleOnGroundProperly(Menu::GetLocalPlayer().m_Vehicle, 0); }));
}

void VehicleModsGravityMenu::Update() {}

/*Called once on submenu open*/
void VehicleModsGravityMenu::UpdateOnce() {}

/*Called always*/
void VehicleModsGravityMenu::FeatureUpdate() {
	if (Menu::GetLocalPlayer().m_InVehicle) {
		if (m_Vars.m_SlippyWheels) {
			Native::SetVehicleReduceGrip(Menu::GetLocalPlayer().m_Vehicle, true);
		}

		if (m_Vars.m_DriveOnWater) {
			auto Coords = Menu::GetLocalPlayer().m_Coords;

			float WaterHeight = 0.f;
			Native::GetWaterHeight(Coords.m_X, Coords.m_Y, Coords.m_Z, &WaterHeight);

			Math::Vector3<float> Adjusted = { Coords.m_X, Coords.m_Y, WaterHeight };

			if (Coords.m_Z - WaterHeight <= 3.f) {
				if (!Native::DoesEntityExist(m_Vars.m_DriveOnWaterEntity)) {
					Menu::GetControlManager()->SimpleRequestModel(0xC42C019A);
					m_Vars.m_DriveOnWaterEntity = Native::CreateObject(0xC42C019A, Coords.m_X, Coords.m_Y, Coords.m_Z, true, false, false);
					Native::SetModelAsNoLongerNeeded(0xC42C019A);
				} else {
					Native::SetEntityVisible(m_Vars.m_DriveOnWaterEntity, false, false);
					Native::FreezeEntityPosition(m_Vars.m_DriveOnWaterEntity, true);
					Native::SetEntityRotation(m_Vars.m_DriveOnWaterEntity, 180.f, 90.f, 180.f, 2, true);
					Native::SetEntityCoords(m_Vars.m_DriveOnWaterEntity, Adjusted.m_X, Adjusted.m_Y, Adjusted.m_Z, false, false, false, false);
				}
			}
		}

		if (m_Vars.m_Freeze) {
			Native::FreezeEntityPosition(Menu::GetLocalPlayer().m_Vehicle, true);
		}

		if (m_Vars.m_StickToGround) {
			Native::SetVehicleOnGroundProperly(Menu::GetLocalPlayer().m_Vehicle, 0);
		}
	}
}

VehicleModsGravityMenu* _instance;
VehicleModsGravityMenu* VehicleModsGravityMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleModsGravityMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleModsGravityMenu::~VehicleModsGravityMenu() { delete _instance; }