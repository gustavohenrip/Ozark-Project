#include "object.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../main.hpp"
#include "object/object_building.hpp"
#include "object/object_spawner.hpp"
#include "object/object_local.hpp"

using namespace ObjectMenuVars;

namespace ObjectMenuVars {
	Vars_ m_Vars;
}

void ObjectMenu::Init() {
	SetName("Object");
	SetParentSubmenu<MainMenu>();

	addOption(SubmenuOption("Object Spawner")
		.addTranslation()
		.addSubmenu<ObjectSpawnerMenu>());

	addOption(SubmenuOption("Object Building")
		.addTranslation()
		.addSubmenu<ObjectBuildingMenu>());

	addOption(SubmenuOption("Local Objects")
		.addTranslation()
		.addSubmenu<ObjectLocalMenu>());
}

void ObjectMenu::Update() {}

/*Called once on submenu open*/
void ObjectMenu::UpdateOnce() {}

/*Called always*/
void ObjectMenu::FeatureUpdate() {}

ObjectMenu* _instance;
ObjectMenu* ObjectMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ObjectMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

ObjectMenu::~ObjectMenu() { delete _instance; }