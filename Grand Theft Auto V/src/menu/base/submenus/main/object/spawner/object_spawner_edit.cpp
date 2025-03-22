#include "object_spawner_edit.hpp"
#include "menu/base/submenu_handler.hpp"
#include "object_spawner_manage.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/util/pools/pool_manager.hpp"

using namespace ObjectSpawnerEditMenuVars;

namespace ObjectSpawnerEditMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> Teleport[] = {
		{ TranslationString("To Me", true), 0 },
		{ TranslationString("To Object", true), 1 }
	};

	void Run(int Index, bool EditingAll, bool EditingLocalPool, int* Var, Entity Ent) {
		if (EditingAll || EditingLocalPool) {
			std::vector<Entity> Entities;
			if (EditingAll) {
				for (auto& E : ObjectSpawnerManageMenuVars::m_Vars.m_SpawnedObjects) {
					Menu::GetControlManager()->RequestControl(E.m_Handle, [&] (Entity D) {
						Entities.push_back(D);
					}, true);
				}
			} else {
				Menu::Pools::GetObjectPool()->Run([&] (Entity E) {
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

					case 2: // Freeze Position
						Native::FreezeEntityPosition(E, true);
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

					case 2: // Freeze Position
						Native::FreezeEntityPosition(E, true);
						break;
				}
			});
		}
	}
}

void ObjectSpawnerEditMenu::Init() {
	SetName("Manage");
	SetParentSubmenu<ObjectSpawnerManageMenu>();

	// TODO: Add more, not important rn

	addOption(SubmenuOption("ESP")
		.addTranslation()
		.addSubmenu<ESPMenu>()
		.addOnClick([] {
			ESPMenu::GetInstance()->SetParentSubmenu<ObjectSpawnerEditMenu>();
			if (m_Vars.m_EditingAll) {
				ESPMenuVars::m_Vars.m_ESP = &m_Vars.m_ESP;
			} else {
				ESPMenuVars::m_Vars.m_ESP = &ObjectSpawnerManageMenuVars::m_Vars.m_SpawnedObjects[m_Vars.m_SelectedObject].m_ESP;
			}

			ESPMenuVars::m_Vars.m_Player = false;
		}));

	addOption(ScrollOption<int>(SCROLLSELECT, "Teleport")
		.addTranslation()
		.addOnClick([] { Run(0, m_Vars.m_EditingAll, false, &m_Vars.m_Teleport, ObjectSpawnerManageMenuVars::m_Vars.m_SpawnedObjects[m_Vars.m_SelectedObject].m_Handle); })
		.addScroll(m_Vars.m_Teleport, 0, NUMOF(Teleport), Teleport)
		.addRequirement([] { return !m_Vars.m_EditingAll; }));

	addOption(ButtonOption("Freeze Position")
		.addTranslation()
		.addOnClick([] { Run(2, m_Vars.m_EditingAll, false, 0, ObjectSpawnerManageMenuVars::m_Vars.m_SpawnedObjects[m_Vars.m_SelectedObject].m_Handle); }));
		
	addOption(ButtonOption("Delete")
		.addTranslation()
		.addOnClick([] {
			Run(1, m_Vars.m_EditingAll, false, nullptr, ObjectSpawnerManageMenuVars::m_Vars.m_SpawnedObjects[m_Vars.m_SelectedObject].m_Handle);
			if (!m_Vars.m_EditingAll) ObjectSpawnerManageMenuVars::m_Vars.m_SpawnedObjects.erase(ObjectSpawnerManageMenuVars::m_Vars.m_SpawnedObjects.begin() + m_Vars.m_SelectedObject);
		}));
}

void ObjectSpawnerEditMenu::Update() {}

/*Called once on submenu open*/
void ObjectSpawnerEditMenu::UpdateOnce() {}

/*Called always*/
void ObjectSpawnerEditMenu::FeatureUpdate() {
	for (auto& E : ObjectSpawnerManageMenuVars::m_Vars.m_SpawnedObjects) {
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

ObjectSpawnerEditMenu* _instance;
ObjectSpawnerEditMenu* ObjectSpawnerEditMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ObjectSpawnerEditMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

ObjectSpawnerEditMenu::~ObjectSpawnerEditMenu() { delete _instance; }