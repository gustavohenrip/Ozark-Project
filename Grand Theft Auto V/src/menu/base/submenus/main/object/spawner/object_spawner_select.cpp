#include "object_spawner_select.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../object_spawner.hpp"

using namespace ObjectSpawnerSelectMenuVars;

namespace ObjectSpawnerSelectMenuVars {
	Vars_ m_Vars;
}

void ObjectSpawnerSelectMenu::Init() {
	SetName("Object Spawner");
	SetParentSubmenu<ObjectSpawnerMenu>();
}

void ObjectSpawnerSelectMenu::Update() {}

/*Called once on submenu open*/
void ObjectSpawnerSelectMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	for (int i = 0; i < m_Vars.m_ListSize; i++) {
		addOption(ButtonOption(m_Vars.m_ListPtr[i].m_Name.GetOriginal())
			.addOnClick([=] { ObjectSpawnerMenuVars::SpawnObject(m_Vars.m_ListPtr[i].m_Result, m_Vars.m_ListPtr[i].m_Name.GetOriginal().c_str()); }));
	}
}

/*Called always*/
void ObjectSpawnerSelectMenu::FeatureUpdate() {}

ObjectSpawnerSelectMenu* _instance;
ObjectSpawnerSelectMenu* ObjectSpawnerSelectMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ObjectSpawnerSelectMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

ObjectSpawnerSelectMenu::~ObjectSpawnerSelectMenu() { delete _instance; }