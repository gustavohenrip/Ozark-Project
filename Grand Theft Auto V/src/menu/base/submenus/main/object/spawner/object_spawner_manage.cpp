#include "object_spawner_manage.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../object_spawner.hpp"
#include "utils/va.hpp"
#include "object_spawner_edit.hpp"

using namespace ObjectSpawnerManageMenuVars;

namespace ObjectSpawnerManageMenuVars {
	Vars_ m_Vars;
}

void ObjectSpawnerManageMenu::Init() {
	SetName("Manage");
	SetParentSubmenu<ObjectSpawnerMenu>();

	addOption(SubmenuOption("All Objects")
		.addTranslation()
		.addOnClick([] { ObjectSpawnerEditMenuVars::m_Vars.m_EditingAll = true; })
		.addSubmenu<ObjectSpawnerEditMenu>());

	addOption(ButtonOption("Clear Manager")
		.addTranslation()
		.addOnClick([this] { m_Vars.m_SpawnedObjects.clear(); UpdateOnce(); })
		.addTooltip("Does NOT delete the objects from the world, only from the manager"));

	addOption(BreakOption("Objects")
		.addTranslation());

	addString("~m~None");
}

void ObjectSpawnerManageMenu::Update() {}

/*Called once on submenu open*/
void ObjectSpawnerManageMenu::UpdateOnce() {
	ClearOptionsOffset(3);

	for (std::size_t i = 0; i < m_Vars.m_SpawnedObjects.size(); i++) {
		if (!Native::DoesEntityExist(m_Vars.m_SpawnedObjects[i].m_Handle)) {
			m_Vars.m_SpawnedObjects.erase(m_Vars.m_SpawnedObjects.begin() + i);
		}
	}

	if (!m_Vars.m_SpawnedObjects.empty()) {
		for (std::size_t i = 0; i < m_Vars.m_SpawnedObjects.size(); i++) {
			addOption(SubmenuOption(Utils::VA::VA("[%i] %s", i + 1, m_Vars.m_SpawnedObjects[i].m_Name.c_str()))
				.addOnClick([=] {
					ObjectSpawnerEditMenuVars::m_Vars.m_EditingAll = false;
					ObjectSpawnerEditMenuVars::m_Vars.m_SelectedObject = i;
				})
				.addSubmenu<ObjectSpawnerEditMenu>());
		}
	} else {
		addOption(ButtonOption(getString("~m~None")));
	}
}

/*Called always*/
void ObjectSpawnerManageMenu::FeatureUpdate() {}

ObjectSpawnerManageMenu* _instance;
ObjectSpawnerManageMenu* ObjectSpawnerManageMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ObjectSpawnerManageMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

ObjectSpawnerManageMenu::~ObjectSpawnerManageMenu() { delete _instance; }