#include "vehicle_gun.hpp"
#include "menu/base/submenu_handler.hpp"
#include "weapon_mods.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/util/camera.hpp"

using namespace VehicleGunMenuVars;

namespace VehicleGunMenuVars {
	Vars_ m_Vars;

	// Hash, Distance
	std::pair<uint32_t, float> Vehicles[] = {
		{ 0x5A82F9AE, 9.0f },
		{ 0x5BA0FF1E, 5.0f },
		{ 0x18619B7E, 5.0f },
		{ 0x322CF98F, 5.0f },
		{ 0xB2E046FB, 5.0f },
		{ 0xE83C17, 5.0f },
		{ 0xA29F78B0, 7.0f },
		{ 0x42BC5E19, 9.0f },
		{ 0x1FD824AF, 5.0f },
		{ 0x8B213907, 5.0f },
		{ 0x2C1FEA99, 5.0f },
		{ 0xEA6A047F, 5.0f },
		{ 0x432AA566, 5.0f },
		{ 0xEB298297, 5.0f },
		{ 0xB44F0582, 5.0f },
		{ 0x9472CD24, 5.0f },
		{ 0x23CA25F2, 5.0f },
		{ 0x619C1B82, 6.0f },
		{ 0xB472D2B5, 5.0f },
		{ 0xC2974024, 5.0f }
	};
}

void VehicleGunMenu::Init() {
	SetName("Vehicle Gun");
	SetParentSubmenu<WeaponModsMenu>();

	addOption(ToggleOption("Toggle Vehicle Gun")
		.addTranslation()
		.addToggle(m_Vars.m_Toggle).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Sit Inside")
		.addTranslation()
		.addToggle(m_Vars.m_SitInside).canBeSaved(m_ParentNameStack));

	addOption(BreakOption("Vehicles")
		.addTranslation());

	for (auto& Vehicle : Vehicles) {
		addOption(RadioOption(Native::_GetLabelText(Native::GetDisplayNameFromVehicleModel(Vehicle.first)))
			.addTranslation()
			.addRadio(m_Vars.m_Radio));
	}
}

void VehicleGunMenu::Update() {}

/*Called once on submenu open*/
void VehicleGunMenu::UpdateOnce() {}

/*Called always*/
void VehicleGunMenu::FeatureUpdate() {
	if (m_Vars.m_Toggle) {
		if (!Menu::GetLocalPlayer().m_IsAiming) {
			if (m_Vars.m_Vehicle != 0 && Native::DoesEntityExist(m_Vars.m_Vehicle)) {
				Native::SetEntityAsMissionEntity(m_Vars.m_Vehicle, true, true);
				Native::DeleteEntity(&m_Vars.m_Vehicle);
				m_Vars.m_Vehicle = 0;
			}

			return;
		}

		if (m_Vars.m_Vehicle == 0 || !Native::DoesEntityExist(m_Vars.m_Vehicle)) {
			uint32_t Model = Vehicles[m_Vars.m_Radio.m_Selected].first;
			Menu::GetControlManager()->SimpleRequestModel(Model);

			auto Coords = Menu::GetLocalPlayer().m_Coords;
			m_Vars.m_Vehicle = Native::CreateVehicle(Model, Coords.m_X, Coords.m_Y, Coords.m_Z + 15.0f, Menu::GetLocalPlayer().m_Heading, true, true, false);
			if (Native::DoesEntityExist(m_Vars.m_Vehicle)) {
				Native::DecorSetInt(m_Vars.m_Vehicle, "MPBitset", 0);
				Native::SetVehicleIsStolen(m_Vars.m_Vehicle, false);
			}
		} else {
			auto Position = Native::GetOffsetFromEntityInWorldCoords(Menu::GetLocalPlayer().m_Ped, 0.4f, 0.0f, 0.8f);
			auto Direction = Menu::Camera::GetCameraDirection();
			auto VehiclePosition = Native::GetEntityCoords(m_Vars.m_Vehicle, false);
			auto Rotation = Native::GetEntityRotation(Menu::GetLocalPlayer().m_Ped, 2);

			float Distance = 5.f;
			for (auto& Vehicle : Vehicles) {
				if (Native::GetEntityModel(m_Vars.m_Vehicle) == Vehicle.first) {
					Distance = Vehicle.second;
					break;
				}
			}

			Math::Vector3<float> Force = {
				((Position.m_X + (Direction.m_X * Distance)) - VehiclePosition.m_X) * 4,
				((Position.m_Y + (Direction.m_Y * Distance)) - VehiclePosition.m_Y) * 4,
				((Position.m_Z + (Direction.m_Z * Distance)) - VehiclePosition.m_Z) * 4
			};

			Native::SetEntityVelocity(m_Vars.m_Vehicle, Force.m_X, Force.m_Y, Force.m_Z);
			Native::SetEntityRotation(m_Vars.m_Vehicle, Rotation.m_X, Rotation.m_Y, Native::GetGameplayCamRot(2).m_Z, 2, true);

			if (Menu::GetLocalPlayer().m_IsShooting) {
				auto Coords = Menu::GetLocalPlayer().m_Coords;
				Math::Vector3<float> Offset = Native::GetOffsetFromEntityGivenWorldCoords(m_Vars.m_Vehicle, Coords.m_X, Coords.m_Y, Coords.m_Z);
				Native::ApplyForceToEntity(m_Vars.m_Vehicle, 1, -Offset.m_X * fabs(100.0f / Offset.m_Y), -Offset.m_Y * fabs(100.0f / Offset.m_Y), -Offset.m_Z * fabs(100.0f / Offset.m_Y), 0.f, 0.f, 0.f, false, true, true, true, false, true);

				if (m_Vars.m_SitInside) {
					Native::SetPedIntoVehicle(Menu::GetLocalPlayer().m_Ped, m_Vars.m_Vehicle, -1);
					Native::SetVehicleEngineOn(m_Vars.m_Vehicle, true, true, false);
				}

				m_Vars.m_Vehicle = 0;
			}
		}
	}
}

VehicleGunMenu* _instance;
VehicleGunMenu* VehicleGunMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleGunMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleGunMenu::~VehicleGunMenu() { delete _instance; }