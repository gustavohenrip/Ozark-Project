#include "vehicle_movement.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_mods.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "menu/base/util/camera.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/util/timers.hpp"
#include "movement/vehicle_movement_acrobatics.hpp"
#include "movement/vehicle_movement_parachute.hpp"

using namespace VehicleModsMovementMenuVars;

namespace VehicleModsMovementMenuVars {
	Vars_ m_Vars;

	void BypassMaxSpeed() {
		if (Menu::GetLocalPlayer().m_InVehicle) {
			Native::SetEntityMaxSpeed(Menu::GetLocalPlayer().m_Vehicle, 9999999.f);
		}
	}

	void LimitMaxSpeed() {
		if (Menu::GetLocalPlayer().m_InVehicle) {
			Native::SetEntityMaxSpeed(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_LimitMaxSpeed ? m_Vars.m_LimitMaxSpeedVal / Global::Vars::m_SelectedMetricSystem.first : 500.f);
		}
	}

	void RunAcrobatic(Vehicle Veh, std::pair<Math::Vector3_<float>, Math::Vector3_<float>> Data) {
		if (Veh) {
			Native::ApplyForceToEntity(Veh, 1, Data.first.m_X, Data.first.m_Y, Data.first.m_Z, Data.second.m_X, Data.second.m_Y, Data.second.m_Z, 0, true, true, true, false, true);
		}
	}
}

void VehicleModsMovementMenu::Init() {
	SetName("Movement");
	SetParentSubmenu<VehicleModsMenu>();

	addOption(SubmenuOption("Acrobatics")
		.addTranslation()
		.addSubmenu<VehicleModsMovementAcrobaticsMenu>());

	addOption(SubmenuOption("Parachute")
		.addTranslation()
		.addSubmenu<VehicleModsMovementParachuteMenu>());

	addOption(ToggleOption("Bypass Max Speed")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DisableMaxSpeed));

	addOption(NumberOption<float>(TOGGLE, "Limit Max Speed")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_LimitMaxSpeed)
		.addNumber(m_Vars.m_LimitMaxSpeedVal, "%.0f", 1.f).addMin(0).addMax(99999)
		.addOnClick([] { m_Vars.m_DisableMaxSpeed = false; LimitMaxSpeed(); }));

	addOption(NumberOption<float>(TOGGLE, "Flying Vehicle")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_FlyingVehicle)
		.addNumber(m_Vars.m_FlyingVehicleSpeed, "%.2f", 0.01f).addMin(0.5f).addMax(1000.f)
		.addOnClick([] { if (!m_Vars.m_FlyingVehicle) if (Menu::GetLocalPlayer().m_InVehicle) Native::SetEntityCollision(Menu::GetLocalPlayer().m_Vehicle, true, true); }));
}

void VehicleModsMovementMenu::Update() {}

/*Called once on submenu open*/
void VehicleModsMovementMenu::UpdateOnce() {}

/*Called always*/
void VehicleModsMovementMenu::FeatureUpdate() {
	if (Menu::GetLocalPlayer().m_InVehicle) {
		if (m_Vars.m_FlyingVehicle) {
			Native::SetEntityCollision(Menu::GetLocalPlayer().m_Vehicle, false, true);
			Native::FreezeEntityPosition(Menu::GetLocalPlayer().m_Vehicle, false);

			if (Menu::Util::GetInput()->IsPressed(true, 'W') || Menu::Util::GetInput()->IsPressed(false, ControlAttack)) {
				Math::Vector3<float> Coords = Menu::GetLocalPlayer().m_Coords + (Menu::Camera::GetCameraDirection() * m_Vars.m_FlyingVehicleSpeed);
				Math::Vector3<float> CameraRotation = Native::GetGameplayCamRot(2);

				Native::SetEntityCoords(Menu::GetLocalPlayer().m_Vehicle, Coords.m_X, Coords.m_Y, Coords.m_Z, true, true, true, false);
				Native::SetEntityRotation(Menu::GetLocalPlayer().m_Vehicle, CameraRotation.m_X, CameraRotation.m_Y, CameraRotation.m_Z, 2, true);
			}
		}

		if (m_Vars.m_DisableMaxSpeed) {
			BypassMaxSpeed();
		}

		if (m_Vars.m_LimitMaxSpeed) {
			LimitMaxSpeed();
		}
	}
}

VehicleModsMovementMenu* _instance;
VehicleModsMovementMenu* VehicleModsMovementMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleModsMovementMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleModsMovementMenu::~VehicleModsMovementMenu() { delete _instance; }