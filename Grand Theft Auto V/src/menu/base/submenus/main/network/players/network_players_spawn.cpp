#include "network_players_spawn.hpp"
#include "menu/base/submenu_handler.hpp"
#include "network_players_selected.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/timers.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/submenus/main/vehicle/vehicle_spawner.hpp"
#include "menu/base/submenus/main/npc/ped_spawner.hpp"
#include "menu/base/submenus/main/npc/ped_spawner.hpp"
#include "menu/base/util/panels.hpp"

using namespace NetworkPlayersSpawnMenuVars;

namespace NetworkPlayersSpawnMenuVars {
	Vars_ m_Vars;
}

void NetworkPlayersSpawnMenu::Init() {
	SetName("Spawn");
	SetParentSubmenu<NetworkPlayersSelectedMenu>();

	addOption(SubmenuOption("NPC Spawner")
		.addTranslation()
		.addOnClick([] { PedSpawnerMenu::GetInstance()->SetParentSubmenu<NetworkPlayersSpawnMenu>(); })
		.addSubmenu<PedSpawnerMenu>());

	addOption(SubmenuOption("Vehicle Spawner")
		.addTranslation()
		.addOnClick([] { VehicleSpawnerMenu::GetInstance()->SetParentSubmenu<NetworkPlayersSpawnMenu>(); })
		.addSubmenu<VehicleSpawnerMenu>());
}

void NetworkPlayersSpawnMenu::Update() {}

/*Called once on submenu open*/
void NetworkPlayersSpawnMenu::UpdateOnce() {
	Menu::Panels::GetPanelManager()->TogglePanelRender("PANEL_PLAYER_INFO", true);
}

/*Called always*/
void NetworkPlayersSpawnMenu::FeatureUpdate() {}

NetworkPlayersSpawnMenu* _instance;
NetworkPlayersSpawnMenu* NetworkPlayersSpawnMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkPlayersSpawnMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkPlayersSpawnMenu::~NetworkPlayersSpawnMenu() { delete _instance; }