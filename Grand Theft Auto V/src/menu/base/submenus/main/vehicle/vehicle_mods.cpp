#include "vehicle_mods.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "global/arrays.hpp"
#include "mods/vehicle_health.hpp"
#include "mods/vehicle_weapons.hpp"
#include "mods/vehicle_particle_fx.hpp"
#include "mods/vehicle_movement.hpp"
#include "mods/vehicle_boost.hpp"
#include "mods/vehicle_collision.hpp"
#include "mods/vehicle_gravity.hpp"
#include "mods/vehicle_multipliers.hpp"
#include "mods/vehicle_modifiers.hpp"
#include "mods/vehicle_autopilot.hpp"
#include "mods/vehicle_ramps.hpp"
#include "mods/vehicle_randomization.hpp"
#include "mods/vehicle_seats.hpp"
#include "mods/vehicle_speedometer.hpp"
#include "mods/vehicle_doors.hpp"
#include "mods/vehicle_tyre_tracks.hpp"
#include "menu/base/util/control_manager.hpp"

using namespace VehicleModsMenuVars;

namespace VehicleModsMenuVars {
	Vars_ m_Vars;

	void MaxUpgrades(Vehicle veh) {
		Native::SetVehicleFixed(veh);
		Native::SetVehicleDirtLevel(veh, 0);
		Native::SetVehicleTyresCanBurst(veh, false);
		Native::SetVehicleModKit(veh, 0);

		for (int i = MOD_SPOILER; i <= MOD_ARMOR; i++) {
			if (Native::GetNumVehicleMods(veh, i) > 0) {
				Native::SetVehicleMod(veh, i, Native::GetNumVehicleMods(veh, i) - 1, 0);
			}
		}

		for (int i = MOD_PLATEHOLDER; i <= MOD_LIVERY; i++) {
			if (Native::GetNumVehicleMods(veh, i) > 0) {
				Native::SetVehicleMod(veh, i, Native::GetNumVehicleMods(veh, i) - 1, 0);
			}
		}

		Native::SetVehicleMod(veh, MOD_HORNS, 30, 0);
		
		if (Native::GetEntityModel(veh) != joaat("vigilante")) {
			Native::SetVehicleWheelType(veh, 7);
			Native::SetVehicleMod(veh, MOD_FRONTWHEELS, 39, 1);
			Native::SetVehicleMod(veh, MOD_BACKWHEELS, 39, 1);
		}

		for (int i = MOD_UNK17; i <= MOD_XENONLIGHTS; i++) {
			Native::ToggleVehicleMod(veh, i, true);
		}

		Native::SetVehicleColours(veh, 160, 160);
		Native::SetVehicleExtraColours(veh, 160, 160);
		Native::SetVehicleWindowTint(veh, 5);

		Color temp;
		temp.R = Native::GetRandomIntInRange(0, 255);
		temp.G = Native::GetRandomIntInRange(0, 255);
		temp.B = Native::GetRandomIntInRange(0, 255);

		Native::SetVehicleTyreSmokeColor(veh, temp.R, temp.G, temp.B);
		Native::SetVehicleCustomPrimaryColour(veh, temp.R, temp.G, temp.B);
		Native::SetVehicleCustomSecondaryColour(veh, temp.R, temp.G, temp.B);

		Native::_SetVehicleNeonLightEnabled(veh, 0, true);
		Native::_SetVehicleNeonLightEnabled(veh, 1, true);
		Native::_SetVehicleNeonLightEnabled(veh, 2, true);
		Native::_SetVehicleNeonLightEnabled(veh, 3, true);
		Native::_SetVehicleNeonLightsColour(veh, temp.R, temp.G, temp.B);

		Native::SetVehicleNumberPlateTextIndex(veh, 5);
	}

	void Downgrade(Vehicle veh) {
		Native::SetVehicleFixed(veh);
		Native::SetVehicleDirtLevel(veh, 0);
		Native::SetVehicleTyresCanBurst(veh, true);
		Native::SetVehicleModKit(veh, 0);
		
		for (int i = MOD_SPOILER; i <= MOD_ARMOR; i++) {
			Native::RemoveVehicleMod(veh, i);
		}

		for (int i = MOD_PLATEHOLDER; i <= MOD_LIVERY; i++) {
			Native::RemoveVehicleMod(veh, i);
		}

		Native::RemoveVehicleMod(veh, MOD_HORNS);
		Native::RemoveVehicleMod(veh, MOD_FRONTWHEELS);
		
		for (int i = MOD_UNK17; i <= MOD_XENONLIGHTS; i++) {
			Native::ToggleVehicleMod(veh, i, false);
		}

		Native::ClearVehicleCustomPrimaryColour(veh);
		Native::ClearVehicleCustomSecondaryColour(veh);
		Native::SetVehicleColours(veh, 135, 135);
		Native::SetVehicleExtraColours(veh, 135, 135);
		Native::SetVehicleWindowTint(veh, 0);
		Native::SetVehicleNumberPlateTextIndex(veh, 1);

		Native::_SetVehicleNeonLightEnabled(veh, 0, false);
		Native::_SetVehicleNeonLightEnabled(veh, 1, false);
		Native::_SetVehicleNeonLightEnabled(veh, 2, false);
		Native::_SetVehicleNeonLightEnabled(veh, 3, false);
	}

	Vehicle CloneVehicle(Vehicle Veh) {
		Vehicle SpawnedVehicle = 0;

		if (Native::DoesEntityExist(Veh)) {
			uint32_t M = Native::GetEntityModel(Veh);
			Menu::GetControlManager()->SimpleRequestModel(M);

			Math::Vector3<float> Coords = Native::GetEntityCoords(Veh, true);
			Math::Vector3<float> SpawnCoords = Coords + (Native::GetEntityForwardVector(Veh) * 5.f);

			SpawnedVehicle = Native::CreateVehicle(M, SpawnCoords.m_X, SpawnCoords.m_Y, SpawnCoords.m_Z, Native::GetEntityHeading(Veh), true, true, 0);
			if (Native::DoesEntityExist(SpawnedVehicle)) {
				Native::SetVehicleDirtLevel(SpawnedVehicle, Native::GetVehicleDirtLevel(Veh));
				Native::SetVehicleTyresCanBurst(SpawnedVehicle, Native::GetVehicleTyresCanBurst(Veh));
				Native::SetVehicleModKit(SpawnedVehicle, 0);

				for (int i = MOD_SPOILER; i <= MOD_ARMOR; i++) {
					if (Native::GetNumVehicleMods(Veh, i) > 0) {
						Native::SetVehicleMod(SpawnedVehicle, i, Native::GetVehicleMod(Veh, i), 0);
					}
				}

				for (int i = MOD_PLATEHOLDER; i <= MOD_LIVERY; i++) {
					if (Native::GetNumVehicleMods(Veh, i) > 0) {
						Native::SetVehicleMod(SpawnedVehicle, i, Native::GetVehicleMod(Veh, i), 0);
					}
				}

				Native::SetVehicleWheelType(SpawnedVehicle, Native::GetVehicleWheelType(Veh));
				Native::SetVehicleMod(SpawnedVehicle, MOD_FRONTWHEELS, Native::GetVehicleMod(Veh, MOD_FRONTWHEELS), Native::GetVehicleModVariation(Veh, MOD_FRONTWHEELS));
				Native::SetVehicleMod(SpawnedVehicle, MOD_BACKWHEELS, Native::GetVehicleMod(Veh, MOD_BACKWHEELS), Native::GetVehicleModVariation(Veh, MOD_FRONTWHEELS));

				for (int i = MOD_UNK17; i <= MOD_XENONLIGHTS; i++) {
					Native::ToggleVehicleMod(SpawnedVehicle, Native::GetVehicleModVariation(Veh, i), true);
				}

				int Colors[2];
				int ExtraColors[2];
				Color TyreSmoke;
				Color CustomPrimary;
				Color CustomSecondary;
				Color Neon;

				Native::GetVehicleCustomPrimaryColour(Veh, &CustomPrimary.R, &CustomPrimary.G, &CustomPrimary.B);
				Native::GetVehicleCustomSecondaryColour(Veh, &CustomSecondary.R, &CustomSecondary.G, &CustomSecondary.B);
				Native::_GetVehicleNeonLightsColour(Veh, &Neon.R, &Neon.G, &Neon.B);
				Native::GetVehicleTyreSmokeColor(Veh, &TyreSmoke.R, &TyreSmoke.G, &TyreSmoke.B);
				Native::GetVehicleColours(Veh, &Colors[0], &Colors[1]);
				Native::GetVehicleExtraColours(Veh, &ExtraColors[0], &ExtraColors[1]);

				Native::SetVehicleColours(SpawnedVehicle, Colors[0], Colors[1]);
				Native::SetVehicleExtraColours(SpawnedVehicle, ExtraColors[0], ExtraColors[1]);
				Native::SetVehicleWindowTint(SpawnedVehicle, Native::GetVehicleWindowTint(Veh));

				Native::SetVehicleTyreSmokeColor(SpawnedVehicle, TyreSmoke.R, TyreSmoke.G, TyreSmoke.B);
				Native::SetVehicleCustomPrimaryColour(SpawnedVehicle, CustomPrimary.R, CustomPrimary.G, CustomPrimary.B);
				Native::SetVehicleCustomSecondaryColour(SpawnedVehicle, CustomSecondary.R, CustomSecondary.G, CustomSecondary.B);

				for (int i = 0; i < 4; i++) {
					Native::_SetVehicleNeonLightEnabled(SpawnedVehicle, i, Native::_IsVehicleNeonLightEnabled(Veh, i));
				}

				Native::_SetVehicleNeonLightsColour(SpawnedVehicle, Neon.R, Neon.G, Neon.B);
				Native::SetVehicleNumberPlateTextIndex(SpawnedVehicle, Native::GetVehicleNumberPlateTextIndex(Veh));
				Native::SetVehicleNumberPlateText(SpawnedVehicle, Native::GetVehicleNumberPlateText(Veh));
			}
		}

		return SpawnedVehicle;
	}

	void BurnShell(Vehicle Veh, bool Scorch) {
		Native::SetEntityRenderScorched(Veh, Scorch);
	}
}

void VehicleModsMenu::Init() {
	SetName("Vehicle Mods");
	SetParentSubmenu<VehicleMenu>();

	addOption(ToggleOption("Godmode")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Godmode).canBeSaved(m_ParentNameStack)
		.addOnClick([] { if(!m_Vars.m_Godmode) Native::SetEntityInvincible(Menu::GetLocalPlayer().m_Vehicle, false); }));

	addOption(ToggleOption("Invisibility")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Invisibility).canBeSaved(m_ParentNameStack)
		.addOnClick([] { if (!m_Vars.m_Invisibility) Native::SetEntityVisible(Menu::GetLocalPlayer().m_Vehicle, true, true); }));

	addOption(ToggleOption("Seatbelt")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Seatbelt).canBeSaved(m_ParentNameStack)
		.addOnClick([] { if (!m_Vars.m_Seatbelt) Native::SetPedCanBeKnockedOffVehicle(Menu::GetLocalPlayer().m_Ped, 0); }));

	addOption(ButtonOption("Max Upgrades")
		.addTranslation().addHotkey()
		.addOnClick([] { MaxUpgrades(Menu::GetLocalPlayer().m_Vehicle); }));

	addOption(ButtonOption("Downgrade")
		.addTranslation().addHotkey()
		.addOnClick([] { Downgrade(Menu::GetLocalPlayer().m_Vehicle); }));

	addOption(SubmenuOption("Health")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleModsHealthMenu>());

	addOption(SubmenuOption("Weapons")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleModsWeaponsMenu>());

	addOption(SubmenuOption("Particle FX")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleModsParticleFXMenu>());

	addOption(SubmenuOption("Movement")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleModsMovementMenu>());

	addOption(SubmenuOption("Boost")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleModsBoostMenu>());

	addOption(SubmenuOption("Collision")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleModsCollisionMenu>());

	addOption(SubmenuOption("Gravity")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleModsGravityMenu>());

	addOption(SubmenuOption("Multipliers")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleModsMultipliersMenu>());

	addOption(SubmenuOption("Modifiers")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleModsModifiersMenu>());

	addOption(SubmenuOption("Autopilot")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleModsAutopilotMenu>());

	addOption(SubmenuOption("Ramps")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleModsRampMenu>());

	addOption(SubmenuOption("Randomization")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleModsRandomizationMenu>());

	addOption(SubmenuOption("Seats")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleModsSeatMenu>());

	addOption(SubmenuOption("Speedometer")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleModsSpeedometerMenu>());

	addOption(SubmenuOption("Doors")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleModsDoorMenu>());

	addOption(SubmenuOption("Tyre Tracks")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleModsTyreTracksMenu>());

	addOption(ButtonOption("Clone")
		.addTranslation().addHotkey()
		.addOnClick([] { if (Menu::GetLocalPlayer().m_InVehicle) CloneVehicle(Menu::GetLocalPlayer().m_Vehicle); }));

	addOption(ToggleOption("Burn Shell")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_BurnShell)
		.addOnClick([] { if (!m_Vars.m_BurnShell) BurnShell(Menu::GetLocalPlayer().m_Vehicle, false); }));

	addOption(ToggleOption("Auto Flip")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_AutoFlip));

	addOption(ScrollOption<int>(SCROLL, "Opacity")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_Opacity, 0, NUMOF(Global::Arrays::g_Opacity), Global::Arrays::g_Opacity)
		.addOnClick([] { Native::SetEntityAlpha(Menu::GetLocalPlayer().m_Vehicle, Global::Arrays::g_Opacity[m_Vars.m_Opacity].m_Result, false); }));

	addOption(ButtonOption("Delete")
		.addTranslation().addHotkey()
		.addOnClick([] {
			if (Menu::GetLocalPlayer().m_InVehicle) {
				Menu::GetControlManager()->RequestControl(Menu::GetLocalPlayer().m_Vehicle, [] (Vehicle Veh) {
					Native::SetEntityAsMissionEntity(Veh, true, true);
					Native::DeleteEntity(&Veh);
				});
			}
	}));
}

void VehicleModsMenu::Update() {}

/*Called once on submenu open*/
void VehicleModsMenu::UpdateOnce() {}

/*Called always*/
void VehicleModsMenu::FeatureUpdate() {
	if (Menu::GetLocalPlayer().m_InVehicle) {
		if (m_Vars.m_Godmode) {
			Native::SetEntityInvincible(Menu::GetLocalPlayer().m_Vehicle, true);
		}

		if (m_Vars.m_Invisibility) {
			Native::SetEntityVisible(Menu::GetLocalPlayer().m_Vehicle, false, true);
		}

		if (m_Vars.m_Seatbelt) {
			Native::SetPedCanBeKnockedOffVehicle(Menu::GetLocalPlayer().m_Ped, 1);
		}

		if (m_Vars.m_BurnShell) {
			BurnShell(Menu::GetLocalPlayer().m_Vehicle, true);
		}

		if (m_Vars.m_AutoFlip) {
			if (!Native::IsEntityUpright(Menu::GetLocalPlayer().m_Vehicle, 120.f)) {
				Math::Vector3<float> Rotation = Native::GetEntityRotation(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::SetEntityRotation(Menu::GetLocalPlayer().m_Vehicle, 0.f, Rotation.m_Y, Rotation.m_Z, 0, true);
			}
		}
	}
}

VehicleModsMenu* _instance;
VehicleModsMenu* VehicleModsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleModsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleModsMenu::~VehicleModsMenu() { delete _instance; }