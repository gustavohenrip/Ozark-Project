#include "network_selected_overseer.hpp"
#include "menu/base/submenu_handler.hpp"
#include "menu/base/submenus/main/network/network_overseer.hpp"
#include "menu/base/util/global.hpp"
#include "utils/json.hpp"
#include "utils/va.hpp"
#include "menu/base/util/panels.hpp"
#include "menu/base/util/presence.hpp"
#include "../network_presence.hpp"
#include "utils/log.hpp"

using namespace NetworkOverseerSelectedMenuVars;

namespace NetworkOverseerSelectedMenuVars {
	Vars_ m_Vars;

	void Save() {
		int Flag = NetworkOverseerMenuVars::m_Vars.m_Players[m_Vars.m_Selected].m_Flags;

		if (m_Vars.m_Enabled) Flag |= OVERSEER_ENABLED;
		else Flag &= ~OVERSEER_ENABLED;

		if (m_Vars.m_BlackScreen) Flag |= OVERSEER_BLACK_SCREEN;
		else Flag &= ~OVERSEER_BLACK_SCREEN;

		if (m_Vars.m_Blame) Flag |= OVERSEER_BLAME;
		else Flag &= ~OVERSEER_BLAME;

		if (m_Vars.m_BlockJoinHost) Flag |= OVERSEER_BLOCK_JOIN_HOST;
		else Flag &= ~OVERSEER_BLOCK_JOIN_HOST;

		if (m_Vars.m_Cage) Flag |= OVERSEER_CAGE;
		else Flag &= ~OVERSEER_CAGE;

		if (m_Vars.m_Crash) Flag |= OVERSEER_CRASH;
		else Flag &= ~OVERSEER_CRASH;

		if (m_Vars.m_Explode) Flag |= OVERSEER_EXPLODE;
		else Flag &= ~OVERSEER_EXPLODE;

		if (m_Vars.m_GiveWantedLevel) Flag |= OVERSEER_GIVE_WANTED_LEVEL;
		else Flag &= ~OVERSEER_GIVE_WANTED_LEVEL;

		if (m_Vars.m_GlitchPhysics) Flag |= OVERSEER_GLITCH_PHYSICS;
		else Flag &= ~OVERSEER_GLITCH_PHYSICS;

		if (m_Vars.m_HostileClone) Flag |= OVERSEER_HOSTILE_CLONE;
		else Flag &= ~OVERSEER_HOSTILE_CLONE;

		if (m_Vars.m_Kick) Flag |= OVERSEER_KICK;
		else Flag &= ~OVERSEER_KICK;

		if (m_Vars.m_KickFromVehicle) Flag |= OVERSEER_KICK_FROM_VEHICLE;
		else Flag &= ~OVERSEER_KICK_FROM_VEHICLE;

		if (m_Vars.m_Notify) Flag |= OVERSEER_NOTIFY;
		else Flag &= ~OVERSEER_NOTIFY;

		if (m_Vars.m_Ragdoll) Flag |= OVERSEER_RAGDOLL;
		else Flag &= ~OVERSEER_RAGDOLL;

		if (m_Vars.m_RemoveWeapons) Flag |= OVERSEER_REMOVE_WEAPONS;
		else Flag &= ~OVERSEER_REMOVE_WEAPONS;

		if (m_Vars.m_SendToIsland) Flag |= OVERSEER_SEND_TO_ISLAND;
		else Flag &= ~OVERSEER_SEND_TO_ISLAND;

		if (m_Vars.m_SetOnFire) Flag |= OVERSEER_SET_ON_FIRE;
		else Flag &= ~OVERSEER_SET_ON_FIRE;

		NetworkOverseerMenuVars::m_Vars.m_Players[m_Vars.m_Selected].m_Flags = Flag;

		// Write
		try {
			nlohmann::json Json;

			std::ifstream Input(Utils::GetConfig()->GetOverseerPath());
			if (Input.good()) {
				Input >> Json;
				Input.close();
			}

			Json[std::to_string(NetworkOverseerMenuVars::m_Vars.m_Players[m_Vars.m_Selected].m_RockstarID)]["flags"] = Flag;

			std::ofstream Output(Utils::GetConfig()->GetOverseerPath());
			if (Output.good()) {
				Output << Json.dump(4);
				Output.close();
			}
		} catch (std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}
	}
}

void NetworkOverseerSelectedMenu::Init() {
	SetName("Overseer");
	SetParentSubmenu<NetworkOverseerMenu>();

	addOption(ToggleOption("Enabled")
		.addTranslation()
		.addToggle(m_Vars.m_Enabled)
		.addOnClick(Save));

	addOption(ToggleOption("Notify")
		.addTranslation()
		.addToggle(m_Vars.m_Notify)
		.addOnClick(Save));

	addOption(ToggleOption("Black Screen")
		.addTranslation()
		.addToggle(m_Vars.m_BlackScreen)
		.addOnClick(Save));

	addOption(ToggleOption("Blame")
		.addTranslation()
		.addToggle(m_Vars.m_Blame)
		.addOnClick(Save));

	addOption(ToggleOption("Block Join (Host)")
		.addTranslation()
		.addToggle(m_Vars.m_BlockJoinHost)
		.addOnClick(Save));

	addOption(ToggleOption("Cage")
		.addTranslation()
		.addToggle(m_Vars.m_Cage)
		.addOnClick(Save));

	addOption(ToggleOption("Crash")
		.addTranslation()
		.addToggle(m_Vars.m_Crash)
		.addOnClick(Save));

	addOption(ToggleOption("Explode")
		.addTranslation()
		.addToggle(m_Vars.m_Explode)
		.addOnClick(Save));

	addOption(ToggleOption("Give Wanted Level")
		.addTranslation()
		.addToggle(m_Vars.m_GiveWantedLevel)
		.addOnClick(Save));

	addOption(ToggleOption("Glitch Physics")
		.addTranslation()
		.addToggle(m_Vars.m_GlitchPhysics)
		.addOnClick(Save));

	addOption(ToggleOption("Hostile Clone")
		.addTranslation()
		.addToggle(m_Vars.m_HostileClone)
		.addOnClick(Save));

	addOption(ToggleOption("Kick")
		.addTranslation()
		.addToggle(m_Vars.m_Kick)
		.addOnClick(Save));

	addOption(ToggleOption("Kick from Vehicle")
		.addTranslation()
		.addToggle(m_Vars.m_KickFromVehicle)
		.addOnClick(Save));

	addOption(ToggleOption("Ragdoll")
		.addTranslation()
		.addToggle(m_Vars.m_Ragdoll)
		.addOnClick(Save));

	addOption(ToggleOption("Remove Weapons")
		.addTranslation()
		.addToggle(m_Vars.m_RemoveWeapons)
		.addOnClick(Save));

	addOption(ToggleOption("Send to Island")
		.addTranslation()
		.addToggle(m_Vars.m_SendToIsland)
		.addOnClick(Save));

	addOption(ToggleOption("Set on Fire")
		.addTranslation()
		.addToggle(m_Vars.m_SetOnFire)
		.addOnClick(Save));
}

void NetworkOverseerSelectedMenu::Update() {}

/*Called once on submenu open*/
void NetworkOverseerSelectedMenu::UpdateOnce() {
	int Flag = NetworkOverseerMenuVars::m_Vars.m_Players[m_Vars.m_Selected].m_Flags;

	m_Vars.m_BlackScreen = Flag & OVERSEER_BLACK_SCREEN;
	m_Vars.m_Blame = Flag & OVERSEER_BLAME;
	m_Vars.m_BlockJoinHost = Flag & OVERSEER_BLOCK_JOIN_HOST;
	m_Vars.m_Cage = Flag & OVERSEER_CAGE;
	m_Vars.m_Crash = Flag & OVERSEER_CRASH;
	m_Vars.m_Enabled = Flag & OVERSEER_ENABLED;
	m_Vars.m_Explode = Flag & OVERSEER_EXPLODE;
	m_Vars.m_GiveWantedLevel = Flag & OVERSEER_GIVE_WANTED_LEVEL;
	m_Vars.m_GlitchPhysics = Flag & OVERSEER_GLITCH_PHYSICS;
	m_Vars.m_HostileClone = Flag & OVERSEER_HOSTILE_CLONE;
	m_Vars.m_Kick = Flag & OVERSEER_KICK;
	m_Vars.m_KickFromVehicle = Flag & OVERSEER_KICK_FROM_VEHICLE;
	m_Vars.m_Notify = Flag & OVERSEER_NOTIFY;
	m_Vars.m_Ragdoll = Flag & OVERSEER_RAGDOLL;
	m_Vars.m_RemoveWeapons = Flag & OVERSEER_REMOVE_WEAPONS;
	m_Vars.m_SendToIsland = Flag & OVERSEER_SEND_TO_ISLAND;
	m_Vars.m_SetOnFire = Flag & OVERSEER_SET_ON_FIRE;
}

/*Called always*/
void NetworkOverseerSelectedMenu::FeatureUpdate() {}

NetworkOverseerSelectedMenu* _instance;
NetworkOverseerSelectedMenu* NetworkOverseerSelectedMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkOverseerSelectedMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkOverseerSelectedMenu::~NetworkOverseerSelectedMenu() { delete _instance; }