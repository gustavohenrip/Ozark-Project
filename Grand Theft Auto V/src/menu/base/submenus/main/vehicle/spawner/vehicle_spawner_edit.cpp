#include "vehicle_spawner_edit.hpp"
#include "menu/base/submenu_handler.hpp"
#include "vehicle_spawner_manage.hpp"
#include "menu/base/util/control_manager.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "menu/base/submenus/main/vehicle/mods/vehicle_movement.hpp"

using namespace VehicleSpawnerEditMenuVars;

namespace VehicleSpawnerEditMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> Driver[] = {
		{ TranslationString("Kick", true), 0 },
		{ TranslationString("Kill", true), 1 }
	};

	ScrollStruct<float> Boost[] = {
		{ TranslationString("Forwards", true), 100.f },
		{ TranslationString("Backwards", true), -100.f }
	};

	ScrollStruct<int> Teleport[] = {
		{ TranslationString("To Me", true), 0 },
		{ TranslationString("To Vehicle", true), 1 }
	};

	void Run(int Index, bool EditingAll, bool EditingLocalPool, int* Var, Entity Ent) {
		if (EditingAll || EditingLocalPool) {
			std::vector<Entity> Entities;
			if (EditingAll) {
				for (auto& E : VehicleSpawnerManageMenuVars::m_Vars.m_SpawnedVehicles) {
					Menu::GetControlManager()->RequestControl(E.m_Vehicle, [&] (Entity D) {
						Entities.push_back(D);
					}, true);
				}
			} else {
				Menu::Pools::GetVehiclePool()->Run([&] (Entity E) {
					Entities.push_back(E);
				});
			}

			for (auto& E : Entities) {
				switch (Index) {
					case 0: // Driver
						if (*Var == 0) {
							auto Ped = Native::GetPedInVehicleSeat(E, -1, 0);
							if (Ped) {
								Native::ClearPedTasksImmediately(Ped);
							}
						} else Native::SetVehicleOutOfControl(E, true, true);
						break;

					case 1: // Boost
						Native::SetVehicleForwardSpeed(E, Boost[*Var].m_Result);
						break;

					case 2: // Acrobatics
						VehicleModsMovementMenuVars::RunAcrobatic(E, Global::Arrays::g_Acrobatics[*Var].m_Result);
						break;

					case 3: { // Gravity
						uint64_t Address = Rage::Engine::GetEntityAddress(E);
						if (Address) {
							Rage::Engine::SetVehicleGravity(Address, Global::Arrays::g_Gravity[*Var].m_Result);
						}

						break; }

					case 4: { // Teleport
						auto Forward = Menu::GetLocalPlayer().m_Coords + (Native::GetEntityForwardVector(Menu::GetLocalPlayer().m_Entity) * 5.f);
						Native::SetEntityCoords(E, Forward.m_X, Forward.m_Y, Forward.m_Z, false, false, false, false);
						break; }

					case 5: // Fix
						Native::SetVehicleFixed(E);
						break;

					case 6: // Empty Vehicle Seats
						Native::TaskEveryoneLeaveVehicle(E);
						for (int i = -1; i < Native::GetVehicleNumberOfPassengers(E); i++) {
							auto Ped = Native::GetPedInVehicleSeat(E, i, 0);
							if (Ped) {
								Native::ClearPedTasksImmediately(Ped);
							}
						}

						break;

					case 7: // Random Paint
						Native::SetVehicleCustomPrimaryColour(E, Native::GetRandomIntInRange(0, 255), Native::GetRandomIntInRange(0, 255), Native::GetRandomIntInRange(0, 255));
						Native::SetVehicleCustomSecondaryColour(E, Native::GetRandomIntInRange(0, 255), Native::GetRandomIntInRange(0, 255), Native::GetRandomIntInRange(0, 255));
						break;

					case 8: // Honk Horn
						Native::StartVehicleHorn(E, 2000, 0, false);
						break;

					case 9: { // Drive to Me
						auto Ped = Native::GetPedInVehicleSeat(E, -1, 0);
						if (Ped) {
							Native::TaskVehicleDriveToCoord(Ped, E, Menu::GetLocalPlayer().m_Coords.m_X, Menu::GetLocalPlayer().m_Coords.m_Y, Menu::GetLocalPlayer().m_Coords.m_Z, 100, 1, Native::GetEntityModel(E), 16777216, 4, -1);
						}

						break; }

					case 10: // Explode
						Native::NetworkExplodeVehicle(E, true, false, false);
						break;

					case 11: // Delete
						Native::SetEntityAsMissionEntity(E, true, true);
						Native::DeleteEntity(&E);
						break;
				}
			}
		} else {
			Menu::GetControlManager()->RequestControl(Ent, [=] (Entity E) {
				switch (Index) {
					case 0: // Driver
						if (*Var == 0) {
							auto Ped = Native::GetPedInVehicleSeat(E, -1, 0);
							if (Ped) {
								Native::ClearPedTasksImmediately(Ped);
							}
						} else Native::SetVehicleOutOfControl(E, true, true);
						break;

					case 1: // Boost
						Native::SetVehicleForwardSpeed(E, Boost[*Var].m_Result);
						break;

					case 2: // Acrobatics
						VehicleModsMovementMenuVars::RunAcrobatic(E, Global::Arrays::g_Acrobatics[*Var].m_Result);
						break;

					case 3: { // Gravity
						uint64_t Address = Rage::Engine::GetEntityAddress(E);
						if (Address) {
							Rage::Engine::SetVehicleGravity(Address, Global::Arrays::g_Gravity[*Var].m_Result);
						}

						break; }

					case 4:
						if (*Var == 0) {
							auto Forward = Menu::GetLocalPlayer().m_Coords + (Native::GetEntityForwardVector(Menu::GetLocalPlayer().m_Entity) * 5.f);
							Native::SetEntityCoords(E, Forward.m_X, Forward.m_Y, Forward.m_Z, false, false, false, false);
						} else {
							auto Forward = Native::GetEntityCoords(E, false) + (Native::GetEntityForwardVector(Menu::GetLocalPlayer().m_Entity) * 5.f);
							Native::SetEntityCoords(Menu::GetLocalPlayer().m_Entity, Forward.m_X, Forward.m_Y, Forward.m_Z, false, false, false, false);
						}
						break;

					case 5: // Fix
						Native::SetVehicleFixed(E);
						break;

					case 6: // Empty Vehicle Seats
						Native::TaskEveryoneLeaveVehicle(E);
						for (int i = -1; i < Native::GetVehicleNumberOfPassengers(E); i++) {
							auto Ped = Native::GetPedInVehicleSeat(E, i, 0);
							if (Ped) {
								Native::ClearPedTasksImmediately(Ped);
							}
						}

						break;

					case 7: // Random Paint
						Native::SetVehicleCustomPrimaryColour(E, Native::GetRandomIntInRange(0, 255), Native::GetRandomIntInRange(0, 255), Native::GetRandomIntInRange(0, 255));
						Native::SetVehicleCustomSecondaryColour(E, Native::GetRandomIntInRange(0, 255), Native::GetRandomIntInRange(0, 255), Native::GetRandomIntInRange(0, 255));
						break;

					case 8: // Honk Horn
						Native::StartVehicleHorn(E, 2000, 0, false);
						break;

					case 9: { // Drive to Me
						auto Ped = Native::GetPedInVehicleSeat(E, -1, 0);
						if (Ped) {
							Native::TaskVehicleDriveToCoord(Ped, E, Menu::GetLocalPlayer().m_Coords.m_X, Menu::GetLocalPlayer().m_Coords.m_Y, Menu::GetLocalPlayer().m_Coords.m_Z, 100, 1, Native::GetEntityModel(E), 16777216, 4, -1);
						}

						break; }

					case 10: // Explode
						Native::NetworkExplodeVehicle(E, true, false, false);
						break;

					case 11: // Delete
						Native::SetEntityAsMissionEntity(E, true, true);
						Native::DeleteEntity(&E);
						break;
				}
			});
		}
	}
}

void VehicleSpawnerEditMenu::Init() {
	SetName("Manage");
	SetParentSubmenu<VehicleSpawnerManageMenu>();

	addOption(SubmenuOption("ESP")
		.addTranslation()
		.addSubmenu<ESPMenu>()
		.addOnClick([] {
			ESPMenu::GetInstance()->SetParentSubmenu<VehicleSpawnerEditMenu>();
			ESPMenuVars::m_Vars.m_ESP = &m_Vars.m_ESP;
			ESPMenuVars::m_Vars.m_Player = false;
		}));

	addOption(ScrollOption<int>(SCROLLSELECT, "Driver")
		.addTranslation()
		.addOnClick([] { Run(0, m_Vars.m_EditingAll, false, &m_Vars.m_Driver, VehicleSpawnerManageMenuVars::m_Vars.m_SpawnedVehicles[m_Vars.m_SelectedVehicle].m_Vehicle); })
		.addScroll(m_Vars.m_Driver, 0, NUMOF(Driver), Driver));

	addOption(ScrollOption<float>(SCROLLSELECT, "Boost")
		.addTranslation()
		.addOnClick([] { Run(1, m_Vars.m_EditingAll, false, &m_Vars.m_Gravity, VehicleSpawnerManageMenuVars::m_Vars.m_SpawnedVehicles[m_Vars.m_SelectedVehicle].m_Vehicle); })
		.addScroll(m_Vars.m_Boost, 0, NUMOF(Boost), Boost));

	addOption(ScrollOption<std::pair<Math::Vector3_<float>, Math::Vector3_<float>>>(SCROLLSELECT, "Acrobatics")
		.addTranslation()
		.addOnClick([] { Run(2, m_Vars.m_EditingAll, false, &m_Vars.m_Acrobatics, VehicleSpawnerManageMenuVars::m_Vars.m_SpawnedVehicles[m_Vars.m_SelectedVehicle].m_Vehicle); })
		.addScroll(m_Vars.m_Acrobatics, 0, NUMOF(Global::Arrays::g_Acrobatics), Global::Arrays::g_Acrobatics));

	addOption(ScrollOption<float>(SCROLL, "Gravity")
		.addTranslation()
		.addOnClick([] { Run(3, m_Vars.m_EditingAll, false, &m_Vars.m_Gravity, VehicleSpawnerManageMenuVars::m_Vars.m_SpawnedVehicles[m_Vars.m_SelectedVehicle].m_Vehicle); })
		.addScroll(m_Vars.m_Gravity, 0, NUMOF(Global::Arrays::g_Gravity), Global::Arrays::g_Gravity));

	addOption(ScrollOption<int>(SCROLLSELECT, "Teleport")
		.addTranslation()
		.addOnClick([] { Run(4, m_Vars.m_EditingAll, false, &m_Vars.m_Teleport, VehicleSpawnerManageMenuVars::m_Vars.m_SpawnedVehicles[m_Vars.m_SelectedVehicle].m_Vehicle); })
		.addScroll(m_Vars.m_Teleport, 0, NUMOF(Teleport), Teleport)
		.addRequirement([] { return !m_Vars.m_EditingAll; }));

	addOption(ButtonOption("Fix")
		.addTranslation()
		.addOnClick([] { Run(5, m_Vars.m_EditingAll, false, nullptr, VehicleSpawnerManageMenuVars::m_Vars.m_SpawnedVehicles[m_Vars.m_SelectedVehicle].m_Vehicle); }));

	addOption(ButtonOption("Empty Vehicle Seats")
		.addTranslation()
		.addOnClick([] { Run(6, m_Vars.m_EditingAll, false, nullptr, VehicleSpawnerManageMenuVars::m_Vars.m_SpawnedVehicles[m_Vars.m_SelectedVehicle].m_Vehicle); }));

	addOption(ButtonOption("Random Paint")
		.addTranslation()
		.addOnClick([] { Run(7, m_Vars.m_EditingAll, false, nullptr, VehicleSpawnerManageMenuVars::m_Vars.m_SpawnedVehicles[m_Vars.m_SelectedVehicle].m_Vehicle); }));

	addOption(ButtonOption("Honk Horn")
		.addTranslation()
		.addOnClick([] { Run(8, m_Vars.m_EditingAll, false, nullptr, VehicleSpawnerManageMenuVars::m_Vars.m_SpawnedVehicles[m_Vars.m_SelectedVehicle].m_Vehicle); }));

	addOption(ButtonOption("Drive to Me")
		.addTranslation()
		.addOnClick([] { Run(9, m_Vars.m_EditingAll, false, nullptr, VehicleSpawnerManageMenuVars::m_Vars.m_SpawnedVehicles[m_Vars.m_SelectedVehicle].m_Vehicle); }));

	addOption(ButtonOption("Explode")
		.addTranslation()
		.addOnClick([] { Run(10, m_Vars.m_EditingAll, false, nullptr, VehicleSpawnerManageMenuVars::m_Vars.m_SpawnedVehicles[m_Vars.m_SelectedVehicle].m_Vehicle); }));

	addOption(ButtonOption("Delete")
		.addTranslation()
		.addOnClick([] {
			Run(11, m_Vars.m_EditingAll, false, nullptr, VehicleSpawnerManageMenuVars::m_Vars.m_SpawnedVehicles[m_Vars.m_SelectedVehicle].m_Vehicle);
			if (!m_Vars.m_EditingAll) VehicleSpawnerManageMenuVars::m_Vars.m_SpawnedVehicles.erase(VehicleSpawnerManageMenuVars::m_Vars.m_SpawnedVehicles.begin() + m_Vars.m_SelectedVehicle);
		}));
}

void VehicleSpawnerEditMenu::Update() {
	if (!m_Vars.m_EditingAll) {
		if (!VehicleSpawnerManageMenuVars::m_Vars.m_SpawnedVehicles[m_Vars.m_SelectedVehicle].m_Vehicle || !Native::DoesEntityExist(VehicleSpawnerManageMenuVars::m_Vars.m_SpawnedVehicles[m_Vars.m_SelectedVehicle].m_Vehicle)) {
			Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		}
	}
}

/*Called once on submenu open*/
void VehicleSpawnerEditMenu::UpdateOnce() {}

/*Called always*/
void VehicleSpawnerEditMenu::FeatureUpdate() {
	for (auto& E : VehicleSpawnerManageMenuVars::m_Vars.m_SpawnedVehicles) {
		// Name ESP
		if (E.m_ESP.m_NameLineOfSightRainbowToggle || m_Vars.m_ESP.m_NameLineOfSightRainbowToggle) {
			E.m_ESP.m_NameLineOfSightRainbow.Run();
		}

		if (E.m_ESP.m_NameRainbowToggle || m_Vars.m_ESP.m_NameRainbowToggle) {
			E.m_ESP.m_NameRainbow.Run();
		}

		if (E.m_ESP.m_Name || m_Vars.m_ESP.m_Name) {
			ESPMenuVars::NameESP(E.m_ESP, E.m_Vehicle, E.m_Name);
		}

		// Snapline ESP
		if (E.m_ESP.m_SnaplineLineOfSightRainbowToggle || m_Vars.m_ESP.m_SnaplineLineOfSightRainbowToggle) {
			E.m_ESP.m_SnaplineLineOfSightRainbow.Run();
		}

		if (E.m_ESP.m_SnaplineRainbowToggle || m_Vars.m_ESP.m_SnaplineRainbowToggle) {
			E.m_ESP.m_SnaplineRainbow.Run();
		}

		if (E.m_ESP.m_Snapline || m_Vars.m_ESP.m_Snapline) {
			ESPMenuVars::SnaplineESP(E.m_ESP, E.m_Vehicle);
		}

		// 3D ESP
		if (E.m_ESP.m_3DLineOfSightRainbowToggle || m_Vars.m_ESP.m_3DLineOfSightRainbowToggle) {
			E.m_ESP.m_3DLineOfSightRainbow.Run();
		}

		if (E.m_ESP.m_3DRainbowToggle || m_Vars.m_ESP.m_3DRainbowToggle) {
			E.m_ESP.m_3DRainbow.Run();
		}

		if (E.m_ESP.m_3D || m_Vars.m_ESP.m_3D) {
			ESPMenuVars::_3DESP(E.m_ESP, E.m_Vehicle);
		}
	}
}

VehicleSpawnerEditMenu* _instance;
VehicleSpawnerEditMenu* VehicleSpawnerEditMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleSpawnerEditMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleSpawnerEditMenu::~VehicleSpawnerEditMenu() { delete _instance; }