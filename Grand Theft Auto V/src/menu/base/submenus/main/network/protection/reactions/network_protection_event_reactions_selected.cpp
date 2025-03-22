#include "network_protection_event_reactions_selected.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_protection_event_reactions.hpp"

using namespace NetworkProtectionEventReactionsSelectedMenuVars;

namespace NetworkProtectionEventReactionsSelectedMenuVars {
	Vars_ m_Vars;
}

void NetworkProtectionEventReactionsSelectedMenu::Init() {
	SetName("Event Reactions");
	SetParentSubmenu<NetworkProtectionEventReactionsMenu>();

	addOption(ToggleOption("Explode")
		.addTranslation()
		.addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_Explode)
		.addOnUpdate([](ToggleOption* Option) { Option->addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_Explode); }));

	addOption(ToggleOption("Crash")
		.addTranslation()
		.addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_Crash)
		.addOnUpdate([](ToggleOption* Option) { Option->addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_Crash); }));

	addOption(ToggleOption("Kick")
		.addTranslation()
		.addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_Kick)
		.addOnUpdate([](ToggleOption* Option) { Option->addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_Kick); }));

	addOption(ToggleOption("Add to Overseer")
		.addTranslation()
		.addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_AddToOverseer)
		.addOnUpdate([](ToggleOption* Option) { Option->addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_AddToOverseer); }));

	addOption(ToggleOption("Cage")
		.addTranslation()
		.addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_Cage)
		.addOnUpdate([](ToggleOption* Option) { Option->addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_Cage); }));

	addOption(ToggleOption("Kick from Vehicle")
		.addTranslation()
		.addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_KickFromVehicle)
		.addOnUpdate([](ToggleOption* Option) { Option->addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_KickFromVehicle); }));

	addOption(ToggleOption("Set on Fire")
		.addTranslation()
		.addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_SetOnFire)
		.addOnUpdate([](ToggleOption* Option) { Option->addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_SetOnFire); }));

	addOption(ToggleOption("Ragdoll")
		.addTranslation()
		.addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_Ragdoll)
		.addOnUpdate([](ToggleOption* Option) { Option->addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_Ragdoll); }));

	addOption(ToggleOption("Glitch Physics")
		.addTranslation()
		.addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_GlitchPhysics)
		.addOnUpdate([](ToggleOption* Option) { Option->addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_GlitchPhysics); }));

	addOption(ToggleOption("Black Screen")
		.addTranslation()
		.addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_BlackScreen)
		.addOnUpdate([](ToggleOption* Option) { Option->addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_BlackScreen); }));

	addOption(ToggleOption("Blame")
		.addTranslation()
		.addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_Blame)
		.addOnUpdate([](ToggleOption* Option) { Option->addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_Blame); }));

	addOption(ToggleOption("Give Wanted Level")
		.addTranslation()
		.addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_GiveWantedLevel)
		.addOnUpdate([](ToggleOption* Option) { Option->addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_GiveWantedLevel); }));

	addOption(ToggleOption("Send to Island")
		.addTranslation()
		.addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_SendToIsland)
		.addOnUpdate([](ToggleOption* Option) { Option->addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_SendToIsland); }));

	addOption(ToggleOption("Remove Weapons")
		.addTranslation()
		.addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_RemoveWeapons)
		.addOnUpdate([](ToggleOption* Option) { Option->addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_RemoveWeapons); }));

	addOption(ToggleOption("Hostile Clone")
		.addTranslation()
		.addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_HostileClone)
		.addOnUpdate([](ToggleOption* Option) { Option->addToggle(NetworkProtectionEventReactionsMenuVars::m_Vars.m_Reactions[m_Vars.m_Selected].m_HostileClone); }));
}

void NetworkProtectionEventReactionsSelectedMenu::Update() {}

/*Called once on submenu open*/
void NetworkProtectionEventReactionsSelectedMenu::UpdateOnce() {}

/*Called always*/
void NetworkProtectionEventReactionsSelectedMenu::FeatureUpdate() {}

NetworkProtectionEventReactionsSelectedMenu* _instance;
NetworkProtectionEventReactionsSelectedMenu* NetworkProtectionEventReactionsSelectedMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkProtectionEventReactionsSelectedMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkProtectionEventReactionsSelectedMenu::~NetworkProtectionEventReactionsSelectedMenu() { delete _instance; }