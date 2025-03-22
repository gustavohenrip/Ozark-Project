#include "vehicle_movement_parachute.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_movement.hpp"
#include "rage/engine.hpp"

using namespace VehicleModsMovementParachuteMenuVars;

namespace VehicleModsMovementParachuteMenuVars {
	Vars_ m_Vars;

	void Deploy() {
		if (Menu::GetLocalPlayer().m_InVehicle) {
			if (Native::GetEntityHeightAboveGround(Menu::GetLocalPlayer().m_Vehicle) > 5.f) {
				if (Native::IsThisModelAHeli(Native::GetEntityModel(Menu::GetLocalPlayer().m_Vehicle))) return;

				uint64_t Address = Rage::Engine::GetEntityAddress(Menu::GetLocalPlayer().m_Vehicle);
				if (Address) {
					uint64_t VehicleModelInfo = *(uint64_t*)(Address + 0x20);
					if (VehicleModelInfo) {
						*(uint8_t*)(VehicleModelInfo + 0x58C) |= 1;
					}

					*(bool*)(Address + 0x1A5D) = true;
					Native::_SetVehicleParachuteActive(Menu::GetLocalPlayer().m_Vehicle, true);
				}
			}
		}
	}
}

void VehicleModsMovementParachuteMenu::Init() {
	SetName("Parachute");
	SetParentSubmenu<VehicleModsMovementMenu>();

	addOption(ButtonOption("Deploy")
		.addTranslation().addHotkey()
		.addOnClick(Deploy));

	addOption(ToggleOption("Auto Deploy")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_AutoDeploy).canBeSaved(m_ParentNameStack));
}

void VehicleModsMovementParachuteMenu::Update() {}

/*Called once on submenu open*/
void VehicleModsMovementParachuteMenu::UpdateOnce() {}

/*Called always*/
void VehicleModsMovementParachuteMenu::FeatureUpdate() {
	if (m_Vars.m_AutoDeploy) {
		Deploy();
	}
}

VehicleModsMovementParachuteMenu* _instance;
VehicleModsMovementParachuteMenu* VehicleModsMovementParachuteMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleModsMovementParachuteMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleModsMovementParachuteMenu::~VehicleModsMovementParachuteMenu() { delete _instance; }