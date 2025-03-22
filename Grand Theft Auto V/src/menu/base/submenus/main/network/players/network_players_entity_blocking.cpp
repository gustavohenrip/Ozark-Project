#include "network_players_entity_blocking.hpp"
#include "menu/base/submenu_handler.hpp"
#include "network_players_selected.hpp"

using namespace NetworkPlayersEntityBlockingMenuVars;

namespace NetworkPlayersEntityBlockingMenuVars {
	Vars_ m_Vars;
}

void NetworkPlayersEntityBlockingMenu::Init() {
	SetName("Entity Blocking");
	SetParentSubmenu<NetworkPlayersSelectedMenu>();

	addOption(ToggleOption("Automobile")
		.addTranslation()
		.addToggle(Menu::GetSelectedPlayer().m_BlockEntities[0])
		.addOnUpdate([] (ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_BlockEntities[0]); }));

	addOption(ToggleOption("Bike")
		.addTranslation()
		.addToggle(Menu::GetSelectedPlayer().m_BlockEntities[1])
		.addOnUpdate([] (ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_BlockEntities[1]); }));

	addOption(ToggleOption("Boat")
		.addTranslation()
		.addToggle(Menu::GetSelectedPlayer().m_BlockEntities[2])
		.addOnUpdate([] (ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_BlockEntities[2]); }));

	addOption(ToggleOption("Door")
		.addTranslation()
		.addToggle(Menu::GetSelectedPlayer().m_BlockEntities[3])
		.addOnUpdate([] (ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_BlockEntities[3]); }));

	addOption(ToggleOption("Heli")
		.addTranslation()
		.addToggle(Menu::GetSelectedPlayer().m_BlockEntities[4])
		.addOnUpdate([] (ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_BlockEntities[4]); }));

	addOption(ToggleOption("Object")
		.addTranslation()
		.addToggle(Menu::GetSelectedPlayer().m_BlockEntities[5])
		.addOnUpdate([] (ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_BlockEntities[5]); }));

	addOption(ToggleOption("Ped")
		.addTranslation()
		.addToggle(Menu::GetSelectedPlayer().m_BlockEntities[6])
		.addOnUpdate([] (ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_BlockEntities[6]); }));

	addOption(ToggleOption("Pickup")
		.addTranslation()
		.addToggle(Menu::GetSelectedPlayer().m_BlockEntities[7])
		.addOnUpdate([] (ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_BlockEntities[7]); }));

	addOption(ToggleOption("Pickup Placement")
		.addTranslation()
		.addToggle(Menu::GetSelectedPlayer().m_BlockEntities[8])
		.addOnUpdate([] (ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_BlockEntities[8]); }));

	addOption(ToggleOption("Plane")
		.addTranslation()
		.addToggle(Menu::GetSelectedPlayer().m_BlockEntities[9])
		.addOnUpdate([] (ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_BlockEntities[9]); }));

	addOption(ToggleOption("Submarine")
		.addTranslation()
		.addToggle(Menu::GetSelectedPlayer().m_BlockEntities[10])
		.addOnUpdate([] (ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_BlockEntities[10]); }));

	addOption(ToggleOption("Player")
		.addTranslation()
		.addToggle(Menu::GetSelectedPlayer().m_BlockEntities[11])
		.addOnUpdate([] (ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_BlockEntities[11]); }));

	addOption(ToggleOption("Trailer")
		.addTranslation()
		.addToggle(Menu::GetSelectedPlayer().m_BlockEntities[12])
		.addOnUpdate([] (ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_BlockEntities[12]); }));

	addOption(ToggleOption("Train")
		.addTranslation()
		.addToggle(Menu::GetSelectedPlayer().m_BlockEntities[13])
		.addOnUpdate([] (ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_BlockEntities[13]); }));
}

void NetworkPlayersEntityBlockingMenu::Update() {}

/*Called once on submenu open*/
void NetworkPlayersEntityBlockingMenu::UpdateOnce() {}

/*Called always*/
void NetworkPlayersEntityBlockingMenu::FeatureUpdate() {}

NetworkPlayersEntityBlockingMenu* _instance;
NetworkPlayersEntityBlockingMenu* NetworkPlayersEntityBlockingMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkPlayersEntityBlockingMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkPlayersEntityBlockingMenu::~NetworkPlayersEntityBlockingMenu() { delete _instance; }