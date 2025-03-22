#include "ped.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../main.hpp"
#include "npc/ped_spawner.hpp"
#include "npc/ped_local.hpp"

using namespace PedMenuVars;

namespace PedMenuVars {
	Vars_ m_Vars;
}

void PedMenu::Init() {
	SetName("NPC");
	SetParentSubmenu<MainMenu>();

	addOption(SubmenuOption("NPC Spawner")
		.addTranslation()
		.addOnClick([] {PedSpawnerMenu::GetInstance()->SetParentSubmenu<PedMenu>(); })
		.addSubmenu<PedSpawnerMenu>());

	addOption(SubmenuOption("Local NPCs")
		.addTranslation()
		.addSubmenu<PedLocalMenu>());
}

void PedMenu::Update() {}

/*Called once on submenu open*/
void PedMenu::UpdateOnce() {}

/*Called always*/
void PedMenu::FeatureUpdate() {}

PedMenu* _instance;
PedMenu* PedMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new PedMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

PedMenu::~PedMenu() { delete _instance; }