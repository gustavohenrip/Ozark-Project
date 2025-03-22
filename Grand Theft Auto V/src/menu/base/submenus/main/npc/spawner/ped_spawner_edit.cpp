#include "ped_spawner_edit.hpp"
#include "menu/base/submenu_handler.hpp"
#include "ped_spawner_manage.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/util/pools/pool_manager.hpp"

using namespace PedSpawnerEditMenuVars;

namespace PedSpawnerEditMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> Teleport[] = {
		{ TranslationString("To Me", true), 0 },
		{ TranslationString("To NPC", true), 1 }
	};

	void Run(int Index, bool EditingAll, bool EditingLocalPool, int* Var, Entity Ent) {
		if (EditingAll || EditingLocalPool) {
			std::vector<Entity> Entities;
			if (EditingAll) {
				for (auto& E : PedSpawnerManageMenuVars::m_Vars.m_SpawnedPeds) {
					Menu::GetControlManager()->RequestControl(E.m_Handle, [&] (Entity D) {
						Entities.push_back(D);
					}, true);
				}
			} else {
				Menu::Pools::GetPedPool()->Run([&] (Entity E) {
					Entities.push_back(E);
				});
			}

			for (auto& E : Entities) {
				switch (Index) {
					case 0: { // Teleport
						auto Forward = Menu::GetLocalPlayer().m_Coords + (Native::GetEntityForwardVector(Menu::GetLocalPlayer().m_Entity) * 5.f);
						Native::SetEntityCoords(E, Forward.m_X, Forward.m_Y, Forward.m_Z, false, false, false, false);
						break; }

					case 1: // Delete
						Native::SetEntityAsMissionEntity(E, true, true);
						Native::DeleteEntity(&E);
						break;

					case 2:
						Native::ResurrectPed(E);
						Native::ReviveInjuredPed(E);
						Native::SetEntityHealth(E, 100);
						Native::ClearPedTasksImmediately(E);
						break;

					case 3:
						Native::ApplyDamageToPed(E, 30000, true);
						break;

					case 4: {
						auto Coords = Native::GetEntityCoords(E, true);
						Native::AddExplosion(Coords.m_X, Coords.m_Y, Coords.m_Z, 0, 10.f, true, false, 0.f, false);
						break; }

					case 5:
						Native::ClonePed(E, 0.f, true, true);
						break;
				}
			}
		} else {
			Menu::GetControlManager()->RequestControl(Ent, [=] (Entity E) {
				switch (Index) {
					case 0:
						if (*Var == 0) {
							auto Forward = Menu::GetLocalPlayer().m_Coords + (Native::GetEntityForwardVector(Menu::GetLocalPlayer().m_Entity) * 5.f);
							Native::SetEntityCoords(E, Forward.m_X, Forward.m_Y, Forward.m_Z, false, false, false, false);
						} else {
							auto Forward = Native::GetEntityCoords(E, false) + (Native::GetEntityForwardVector(Menu::GetLocalPlayer().m_Entity) * 5.f);
							Native::SetEntityCoords(Menu::GetLocalPlayer().m_Entity, Forward.m_X, Forward.m_Y, Forward.m_Z, false, false, false, false);
						}
						break;

					case 1: // Delete
						Native::SetEntityAsMissionEntity(E, true, true);
						Native::DeleteEntity(&E);
						break;

					case 2:
						Native::ResurrectPed(E);
						Native::ReviveInjuredPed(E);
						Native::SetEntityHealth(E, 100);
						Native::ClearPedTasksImmediately(E);
						break;
						
					case 3:
						Native::ApplyDamageToPed(E, 30000, true);
						break;

					case 4: {
						auto Coords = Native::GetEntityCoords(E, true);
						Native::AddExplosion(Coords.m_X, Coords.m_Y, Coords.m_Z, 0, 10.f, true, false, 0.f, false);
						break; }

					case 5:
						Native::ClonePed(E, 0.f, true, true);
						break;
				}
			});
		}
	}
}

void PedSpawnerEditMenu::Init() {
	SetName("Edit");
	SetParentSubmenu<PedSpawnerManageMenu>();

	// TODO: Add more, not important rn

	addOption(SubmenuOption("ESP")
		.addTranslation()
		.addSubmenu<ESPMenu>()
		.addOnClick([] {
			ESPMenu::GetInstance()->SetParentSubmenu<PedSpawnerEditMenu>();
			if (m_Vars.m_EditingAll) {
				ESPMenuVars::m_Vars.m_ESP = &m_Vars.m_ESP;
			} else {
				ESPMenuVars::m_Vars.m_ESP = &PedSpawnerManageMenuVars::m_Vars.m_SpawnedPeds[m_Vars.m_SelectedPed].m_ESP;
			}

			ESPMenuVars::m_Vars.m_Player = false;
		}));

	addOption(ScrollOption<int>(SCROLLSELECT, "Teleport")
		.addTranslation()
		.addOnClick([] { Run(0, m_Vars.m_EditingAll, false, &m_Vars.m_Teleport, PedSpawnerManageMenuVars::m_Vars.m_SpawnedPeds[m_Vars.m_SelectedPed].m_Handle); })
		.addScroll(m_Vars.m_Teleport, 0, NUMOF(Teleport), Teleport)
		.addRequirement([] { return !m_Vars.m_EditingAll; }));

	addOption(ButtonOption("Revive")
		.addTranslation()
		.addOnClick([] { Run(2, m_Vars.m_EditingAll, false, nullptr, PedSpawnerManageMenuVars::m_Vars.m_SpawnedPeds[m_Vars.m_SelectedPed].m_Handle); }));

	addOption(ButtonOption("Kill")
		.addTranslation()
		.addOnClick([] { Run(3, m_Vars.m_EditingAll, false, nullptr, PedSpawnerManageMenuVars::m_Vars.m_SpawnedPeds[m_Vars.m_SelectedPed].m_Handle); }));

	addOption(ButtonOption("Explode")
		.addTranslation()
		.addOnClick([] { Run(4, m_Vars.m_EditingAll, false, nullptr, PedSpawnerManageMenuVars::m_Vars.m_SpawnedPeds[m_Vars.m_SelectedPed].m_Handle); }));

	addOption(ButtonOption("Clone")
		.addTranslation()
		.addOnClick([] { Run(5, m_Vars.m_EditingAll, false, nullptr, PedSpawnerManageMenuVars::m_Vars.m_SpawnedPeds[m_Vars.m_SelectedPed].m_Handle); }));

	addOption(ButtonOption("Delete")
		.addTranslation()
		.addOnClick([] {
			Run(1, m_Vars.m_EditingAll, false, nullptr, PedSpawnerManageMenuVars::m_Vars.m_SpawnedPeds[m_Vars.m_SelectedPed].m_Handle);
			if (!m_Vars.m_EditingAll) PedSpawnerManageMenuVars::m_Vars.m_SpawnedPeds.erase(PedSpawnerManageMenuVars::m_Vars.m_SpawnedPeds.begin() + m_Vars.m_SelectedPed);
		}));
}

void PedSpawnerEditMenu::Update() {}

/*Called once on submenu open*/
void PedSpawnerEditMenu::UpdateOnce() {}

/*Called always*/
void PedSpawnerEditMenu::FeatureUpdate() {
	for (auto& E : PedSpawnerManageMenuVars::m_Vars.m_SpawnedPeds) {
		// Name ESP
		if (E.m_ESP.m_NameLineOfSightRainbowToggle || m_Vars.m_ESP.m_NameLineOfSightRainbowToggle) {
			E.m_ESP.m_NameLineOfSightRainbow.Run();
		}

		if (E.m_ESP.m_NameRainbowToggle || m_Vars.m_ESP.m_NameRainbowToggle) {
			E.m_ESP.m_NameRainbow.Run();
		}

		if (E.m_ESP.m_Name || m_Vars.m_ESP.m_Name) {
			ESPMenuVars::NameESP(E.m_ESP, E.m_Handle, E.m_Name);
		}

		// Snapline ESP
		if (E.m_ESP.m_SnaplineLineOfSightRainbowToggle || m_Vars.m_ESP.m_SnaplineLineOfSightRainbowToggle) {
			E.m_ESP.m_SnaplineLineOfSightRainbow.Run();
		}

		if (E.m_ESP.m_SnaplineRainbowToggle || m_Vars.m_ESP.m_SnaplineRainbowToggle) {
			E.m_ESP.m_SnaplineRainbow.Run();
		}

		if (E.m_ESP.m_Snapline || m_Vars.m_ESP.m_Snapline) {
			ESPMenuVars::SnaplineESP(E.m_ESP, E.m_Handle);
		}

		// 3D ESP
		if (E.m_ESP.m_3DLineOfSightRainbowToggle || m_Vars.m_ESP.m_3DLineOfSightRainbowToggle) {
			E.m_ESP.m_3DLineOfSightRainbow.Run();
		}

		if (E.m_ESP.m_3DRainbowToggle || m_Vars.m_ESP.m_3DRainbowToggle) {
			E.m_ESP.m_3DRainbow.Run();
		}

		if (E.m_ESP.m_3D || m_Vars.m_ESP.m_3D) {
			ESPMenuVars::_3DESP(E.m_ESP, E.m_Handle);
		}
	}
}

PedSpawnerEditMenu* _instance;
PedSpawnerEditMenu* PedSpawnerEditMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new PedSpawnerEditMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

PedSpawnerEditMenu::~PedSpawnerEditMenu() { delete _instance; }