#include "network_protection_ghost_mode.hpp"
#include "menu/base/submenu_handler.hpp"
#include "network_protection_entity_events.hpp"
#include "ghost/network_protection_ghost_mode_selected.hpp"

using namespace NetworkProtectionGhostModeMenuVars;

namespace NetworkProtectionGhostModeMenuVars {
	Vars_ m_Vars;

	std::pair<std::string, eNetObjectTypes> Types[] = {
		{ "Automobile", NetObjectAutomobile },
		{ "Bike", NetObjectBike },
		{ "Boat", NetObjectBoat },
		{ "Helicopter", NetObjectHeli },
		{ "Object", NetObjectObject },
		{ "Ped", NetObjectPed },
		{ "Pickup", NetObjectPickup },
		{ "Plane", NetObjectPlane },
		{ "Submarine", NetObjectSubmarine },
		{ "Player", NetObjectPlayer },
		{ "Trailer", NetObjectTrailer },
		{ "Train", NetObjectTrain }
	};
}

void NetworkProtectionGhostModeMenu::Init() {
	SetName("Ghost Mode");
	SetParentSubmenu<NetworkProtectionEntityEventsMenu>();

	for (auto Type : Types) {
		m_Vars.m_Ghosts[Type.second].m_Type = Type.second;
		m_Vars.m_Ghosts[Type.second].m_Name = Type.first;

		m_Vars.m_Ghosts[Type.second].m_Enabled = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Enabled", false, { Type.first });
		m_Vars.m_Ghosts[Type.second].m_DisableForSession = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Ghost for All Players", false, { Type.first });
	}

	addOption(SubmenuOption("Player")
		.addTranslation()
		.addSubmenu<NetworkProtectionGhostModeSelectedMenu>()
		.addOnClick([] { NetworkProtectionGhostModeSelectedMenuVars::m_Vars.m_Selected = &m_Vars.m_Ghosts[NetObjectPlayer]; })
		.addTooltip("Requires you to join a new session after enabling"));

	addOption(SubmenuOption("Automobile")
		.addTranslation()
		.addSubmenu<NetworkProtectionGhostModeSelectedMenu>()
		.addOnClick([] { NetworkProtectionGhostModeSelectedMenuVars::m_Vars.m_Selected = &m_Vars.m_Ghosts[NetObjectAutomobile]; }));

	addOption(SubmenuOption("Bike")
		.addTranslation()
		.addSubmenu<NetworkProtectionGhostModeSelectedMenu>()
		.addOnClick([] { NetworkProtectionGhostModeSelectedMenuVars::m_Vars.m_Selected = &m_Vars.m_Ghosts[NetObjectBike]; }));

	addOption(SubmenuOption("Boat")
		.addTranslation()
		.addSubmenu<NetworkProtectionGhostModeSelectedMenu>()
		.addOnClick([] { NetworkProtectionGhostModeSelectedMenuVars::m_Vars.m_Selected = &m_Vars.m_Ghosts[NetObjectBoat]; }));

	addOption(SubmenuOption("Helicopter")
		.addTranslation()
		.addSubmenu<NetworkProtectionGhostModeSelectedMenu>()
		.addOnClick([] { NetworkProtectionGhostModeSelectedMenuVars::m_Vars.m_Selected = &m_Vars.m_Ghosts[NetObjectHeli]; }));

	addOption(SubmenuOption("Object")
		.addTranslation()
		.addSubmenu<NetworkProtectionGhostModeSelectedMenu>()
		.addOnClick([] { NetworkProtectionGhostModeSelectedMenuVars::m_Vars.m_Selected = &m_Vars.m_Ghosts[NetObjectObject]; }));

	addOption(SubmenuOption("Ped")
		.addTranslation()
		.addSubmenu<NetworkProtectionGhostModeSelectedMenu>()
		.addOnClick([] { NetworkProtectionGhostModeSelectedMenuVars::m_Vars.m_Selected = &m_Vars.m_Ghosts[NetObjectPed]; }));

	addOption(SubmenuOption("Pickup")
		.addTranslation()
		.addSubmenu<NetworkProtectionGhostModeSelectedMenu>()
		.addOnClick([] { NetworkProtectionGhostModeSelectedMenuVars::m_Vars.m_Selected = &m_Vars.m_Ghosts[NetObjectPickup]; }));

	addOption(SubmenuOption("Plane")
		.addTranslation()
		.addSubmenu<NetworkProtectionGhostModeSelectedMenu>()
		.addOnClick([] { NetworkProtectionGhostModeSelectedMenuVars::m_Vars.m_Selected = &m_Vars.m_Ghosts[NetObjectPlane]; }));

	addOption(SubmenuOption("Submarine")
		.addTranslation()
		.addSubmenu<NetworkProtectionGhostModeSelectedMenu>()
		.addOnClick([] { NetworkProtectionGhostModeSelectedMenuVars::m_Vars.m_Selected = &m_Vars.m_Ghosts[NetObjectSubmarine]; }));

	addOption(SubmenuOption("Trailer")
		.addTranslation()
		.addSubmenu<NetworkProtectionGhostModeSelectedMenu>()
		.addOnClick([] { NetworkProtectionGhostModeSelectedMenuVars::m_Vars.m_Selected = &m_Vars.m_Ghosts[NetObjectTrailer]; }));

	addOption(SubmenuOption("Train")
		.addTranslation()
		.addSubmenu<NetworkProtectionGhostModeSelectedMenu>()
		.addOnClick([] { NetworkProtectionGhostModeSelectedMenuVars::m_Vars.m_Selected = &m_Vars.m_Ghosts[NetObjectTrain]; }));
}

void NetworkProtectionGhostModeMenu::Update() {}

/*Called once on submenu open*/
void NetworkProtectionGhostModeMenu::UpdateOnce() {
	NetworkProtectionGhostModeSelectedMenuVars::m_Vars.m_Populated = false;
}

/*Called always*/
void NetworkProtectionGhostModeMenu::FeatureUpdate() {}

NetworkProtectionGhostModeMenu* _instance;
NetworkProtectionGhostModeMenu* NetworkProtectionGhostModeMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkProtectionGhostModeMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkProtectionGhostModeMenu::~NetworkProtectionGhostModeMenu() { delete _instance; }