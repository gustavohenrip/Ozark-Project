#include "network_protection_entity_blocking.hpp"
#include "menu/base/submenu_handler.hpp"
#include "network_protection_entity_events.hpp"

using namespace NetworkProtectionEntityBlockingMenuVars;

namespace NetworkProtectionEntityBlockingMenuVars {
	Vars_ m_Vars;
}

void NetworkProtectionEntityBlockingMenu::Init() {
	SetName("Entity Blocking");
	SetParentSubmenu<NetworkProtectionEntityEventsMenu>();

	addOption(ToggleOption("Automobile")
		.addTranslation()
		.addToggle(m_Vars.m_Toggled[0]));

	addOption(ToggleOption("Bike")
		.addTranslation()
		.addToggle(m_Vars.m_Toggled[1]));

	addOption(ToggleOption("Boat")
		.addTranslation()
		.addToggle(m_Vars.m_Toggled[2]));

	addOption(ToggleOption("Door")
		.addTranslation()
		.addToggle(m_Vars.m_Toggled[3]));

	addOption(ToggleOption("Heli")
		.addTranslation()
		.addToggle(m_Vars.m_Toggled[4]));

	addOption(ToggleOption("Object")
		.addTranslation()
		.addToggle(m_Vars.m_Toggled[5]));

	addOption(ToggleOption("Ped")
		.addTranslation()
		.addToggle(m_Vars.m_Toggled[6]));

	addOption(ToggleOption("Pickup")
		.addTranslation()
		.addToggle(m_Vars.m_Toggled[7]));

	addOption(ToggleOption("Pickup Placement")
		.addTranslation()
		.addToggle(m_Vars.m_Toggled[8]));

	addOption(ToggleOption("Plane")
		.addTranslation()
		.addToggle(m_Vars.m_Toggled[9]));

	addOption(ToggleOption("Submarine")
		.addTranslation()
		.addToggle(m_Vars.m_Toggled[10]));

	addOption(ToggleOption("Player")
		.addTranslation()
		.addToggle(m_Vars.m_Toggled[11]));

	addOption(ToggleOption("Trailer")
		.addTranslation()
		.addToggle(m_Vars.m_Toggled[12]));

	addOption(ToggleOption("Train")
		.addTranslation()
		.addToggle(m_Vars.m_Toggled[13]));
}

void NetworkProtectionEntityBlockingMenu::Update() {}

/*Called once on submenu open*/
void NetworkProtectionEntityBlockingMenu::UpdateOnce() {}

/*Called always*/
void NetworkProtectionEntityBlockingMenu::FeatureUpdate() {}

NetworkProtectionEntityBlockingMenu* _instance;
NetworkProtectionEntityBlockingMenu* NetworkProtectionEntityBlockingMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkProtectionEntityBlockingMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkProtectionEntityBlockingMenu::~NetworkProtectionEntityBlockingMenu() { delete _instance; }