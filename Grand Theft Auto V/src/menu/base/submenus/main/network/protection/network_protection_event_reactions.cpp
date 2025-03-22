#include "network_protection_event_reactions.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_protection.hpp"
#include "reactions/network_protection_event_reactions_selected.hpp"
#include "../players/network_players_abusive.hpp"
#include "../players/network_players_trolling.hpp"
#include "../players/network_players_removal.hpp"
#include "rage/engine.hpp"
#include "utils/fiber_pool.hpp"
#include "../network_overseer.hpp"
#include "utils/fibers.hpp"

using namespace NetworkProtectionEventReactionsMenuVars;

namespace NetworkProtectionEventReactionsMenuVars {
	Vars_ m_Vars;

	std::string Reasons[] = {
		"Death",
		"Aim",
		"Report",
		"Spoofed Name",
		"Spoofed Peer",
		"Spoofed RID",
		"Spoofed IP",
		"Arxan",
		"Crash Attempt",
		"Spectating You",
		"Money Drop",
		"Quantum Kick", // fuck unbound
		"Script Migration"
	};

	void ProcessEvent(eEventReactions Event, Player ID) {
		Utils::GetFiberPool()->Push([=] {
			auto& Player = Menu::GetPlayer(ID);
			if (!Player.m_Connected) return;

			if (m_Vars.m_Reactions[Event].m_Explode) {
				NetworkPlayersAbusiveMenuVars::Explode(Player, 1);
				Utils::WaitFiber(250);
			}

			if (m_Vars.m_Reactions[Event].m_Crash) {
				NetworkPlayersRemovalMenuVars::CrashPlayer(Player, 2);
				Utils::WaitFiber(250);
			}

			if (m_Vars.m_Reactions[Event].m_Kick) {
				if (Menu::GetLocalPlayer().m_IsScriptHost) {
					NetworkPlayersRemovalMenuVars::ScriptHostKick(Player);
					Utils::WaitFiber(250);
				}

				if (Rage::Engine::GetHostNetGamePlayer() == Menu::GetLocalPlayer().m_NetGamePlayer) {
					NetworkPlayersRemovalMenuVars::SessionHostKick(Player);
					Utils::WaitFiber(250);
				} else {
					if (Rage::Engine::GetHostNetGamePlayer() != Player.m_NetGamePlayer) {
						NetworkPlayersRemovalMenuVars::BreakFreemodeKick(Player);
						Utils::WaitFiber(250);
					}
				}
			}

			if (m_Vars.m_Reactions[Event].m_AddToOverseer) {
				if (IsValidPtr(Player.m_NetGamePlayer)) {
					if (IsValidPtr(Player.m_NetGamePlayer->m_PlayerInfo)) {
						NetworkOverseerMenuVars::Save(Player.m_Name, Player.m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID, "Event Reaction for " + Reasons[Event]);
					}
				}
				Utils::WaitFiber(250);
			}

			if (m_Vars.m_Reactions[Event].m_Cage) {
				NetworkPlayersTrollingMenuVars::CagePlayer(Player);
				Utils::WaitFiber(250);
			}

			if (m_Vars.m_Reactions[Event].m_KickFromVehicle) {
				NetworkPlayersAbusiveMenuVars::KickFromVehiclePlayer(Player);
				Utils::WaitFiber(250);
			}

			if (m_Vars.m_Reactions[Event].m_SetOnFire) {
				NetworkPlayersAbusiveMenuVars::SetOnFire(Player);
				Utils::WaitFiber(250);
			}

			if (m_Vars.m_Reactions[Event].m_Ragdoll) {
				NetworkPlayersAbusiveMenuVars::RagdollPlayer(Player);
				Utils::WaitFiber(250);
			}

			if (m_Vars.m_Reactions[Event].m_GlitchPhysics) {
				NetworkPlayersAbusiveMenuVars::GlitchPhysics(Player);
				Utils::WaitFiber(250);
			}

			if (m_Vars.m_Reactions[Event].m_BlackScreen) {
				NetworkPlayersAbusiveMenuVars::BlackScreen(Player);
				Utils::WaitFiber(250);
			}

			if (m_Vars.m_Reactions[Event].m_Blame) {
				NetworkPlayersAbusiveMenuVars::Blame(Player);
				Utils::WaitFiber(250);
			}

			if (m_Vars.m_Reactions[Event].m_GiveWantedLevel) {
				NetworkPlayersAbusiveMenuVars::GiveWantedLevel(Player, 1);
				Utils::WaitFiber(250);
			}

			if (m_Vars.m_Reactions[Event].m_SendToIsland) {
				NetworkPlayersAbusiveMenuVars::SendPlayerTo(Player);
				Utils::WaitFiber(250);
			}

			if (m_Vars.m_Reactions[Event].m_RemoveWeapons) {
				Native::RemoveAllPedWeapons(Player.m_Ped, true);
				Utils::WaitFiber(250);
			}

			if (m_Vars.m_Reactions[Event].m_HostileClone) {
				Player.m_CloneType = 1;
				NetworkPlayersTrollingMenuVars::ClonePlayer(Player);
				Utils::WaitFiber(250);
			}
		});
	}
}

void NetworkProtectionEventReactionsMenu::Init() {
	SetName("Event Reactions");
	SetParentSubmenu<NetworkProtectionMenu>();

	for (std::size_t i = 0; i < NUMOF(Reasons); i++) {
		addOption(SubmenuOption(Reasons[i])
			.addTranslation()
			.addSubmenu<NetworkProtectionEventReactionsSelectedMenu>()
			.addOnClick([=] {
				NetworkProtectionEventReactionsSelectedMenu::GetInstance()->SetName(Reasons[i], false, false);
				NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected = i;
			}));

		m_Vars.m_Reactions[i].m_Explode = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Explode", false, { Reasons[i] });
		m_Vars.m_Reactions[i].m_Crash = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Crash", false, { Reasons[i] });
		m_Vars.m_Reactions[i].m_Kick = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Kick", false, { Reasons[i] });
		m_Vars.m_Reactions[i].m_AddToOverseer = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Add to Overseer", false, { Reasons[i] });
		m_Vars.m_Reactions[i].m_Cage = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Cage", false, { Reasons[i] });
		m_Vars.m_Reactions[i].m_KickFromVehicle = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Kick from Vehicle", false, { Reasons[i] });
		m_Vars.m_Reactions[i].m_SetOnFire = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Set on Fire", false, { Reasons[i] });
		m_Vars.m_Reactions[i].m_Ragdoll = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Ragdoll", false, { Reasons[i] });
		m_Vars.m_Reactions[i].m_GlitchPhysics = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Glitch Physics", false, { Reasons[i] });
		m_Vars.m_Reactions[i].m_BlackScreen = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Black Screen", false, { Reasons[i] });
		m_Vars.m_Reactions[i].m_Blame = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Blame", false, { Reasons[i] });
		m_Vars.m_Reactions[i].m_GiveWantedLevel = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Give Wanted Level", false, { Reasons[i] });
		m_Vars.m_Reactions[i].m_SendToIsland = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Send to Island", false, { Reasons[i] });
		m_Vars.m_Reactions[i].m_RemoveWeapons = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Remove Weapons", false, { Reasons[i] });
		m_Vars.m_Reactions[i].m_HostileClone = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Hostile Clone", false, { Reasons[i] });
	}
}

void NetworkProtectionEventReactionsMenu::Update() {}

/*Called once on submenu open*/
void NetworkProtectionEventReactionsMenu::UpdateOnce() {
	// save config
	if (Menu::GetSubmenuHandler()->GetPreviousSubmenu() == NetworkProtectionEventReactionsSelectedMenu::GetInstance()) {
		Utils::GetConfig()->WriteBool(m_ParentNameStack, "Explode", m_Vars.m_Reactions[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected].m_Explode, { Reasons[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected] });
		Utils::GetConfig()->WriteBool(m_ParentNameStack, "Crash", m_Vars.m_Reactions[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected].m_Crash, { Reasons[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected] });
		Utils::GetConfig()->WriteBool(m_ParentNameStack, "Kick", m_Vars.m_Reactions[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected].m_Kick, { Reasons[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected] });
		Utils::GetConfig()->WriteBool(m_ParentNameStack, "Add to Overseer", m_Vars.m_Reactions[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected].m_AddToOverseer, { Reasons[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected] });
		Utils::GetConfig()->WriteBool(m_ParentNameStack, "Cage", m_Vars.m_Reactions[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected].m_Cage, { Reasons[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected] });
		Utils::GetConfig()->WriteBool(m_ParentNameStack, "Kick from Vehicle", m_Vars.m_Reactions[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected].m_KickFromVehicle, { Reasons[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected] });
		Utils::GetConfig()->WriteBool(m_ParentNameStack, "Set on Fire", m_Vars.m_Reactions[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected].m_SetOnFire, { Reasons[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected] });
		Utils::GetConfig()->WriteBool(m_ParentNameStack, "Ragdoll", m_Vars.m_Reactions[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected].m_Ragdoll, { Reasons[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected] });
		Utils::GetConfig()->WriteBool(m_ParentNameStack, "Glitch Physics", m_Vars.m_Reactions[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected].m_GlitchPhysics, { Reasons[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected] });
		Utils::GetConfig()->WriteBool(m_ParentNameStack, "Black Screen", m_Vars.m_Reactions[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected].m_BlackScreen, { Reasons[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected] });
		Utils::GetConfig()->WriteBool(m_ParentNameStack, "Blame", m_Vars.m_Reactions[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected].m_Blame, { Reasons[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected] });
		Utils::GetConfig()->WriteBool(m_ParentNameStack, "Give Wanted Level", m_Vars.m_Reactions[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected].m_GiveWantedLevel, { Reasons[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected] });
		Utils::GetConfig()->WriteBool(m_ParentNameStack, "Send to Island", m_Vars.m_Reactions[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected].m_SendToIsland, { Reasons[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected] });
		Utils::GetConfig()->WriteBool(m_ParentNameStack, "Remove Weapons", m_Vars.m_Reactions[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected].m_RemoveWeapons, { Reasons[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected] });
		Utils::GetConfig()->WriteBool(m_ParentNameStack, "Hostile Clone", m_Vars.m_Reactions[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected].m_HostileClone, { Reasons[NetworkProtectionEventReactionsSelectedMenuVars::m_Vars.m_Selected] });
	}
}

/*Called always*/
void NetworkProtectionEventReactionsMenu::FeatureUpdate() {
	static uint32_t LastOnDeath = 0;
	static uint32_t LastOnAim[32];

	if (m_Vars.m_Reactions[EVENT_DEATH].IsActive()) {
		if (Native::IsEntityDead(Menu::GetLocalPlayer().m_Ped, 0)) {
			if (GetTickCount() - LastOnDeath > 2000) {
				Ped Killer = Native::GetPedSourceOfDeath(Menu::GetLocalPlayer().m_Ped);
				if (Killer && Killer != Menu::GetLocalPlayer().m_Ped && Native::IsPedAPlayer(Killer)) {
					Player PlayerKiller = Native::NetworkGetPlayerIndexFromPed(Killer);
					if (PlayerKiller != Menu::GetLocalPlayer().m_ID && Menu::GetPlayer(PlayerKiller).m_Connected) {
						LastOnDeath = GetTickCount();
						ProcessEvent(EVENT_DEATH, PlayerKiller);
					}
				}
			}
		}
	}

	if (m_Vars.m_Reactions[EVENT_AIM].IsActive()) {
		Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
			if (GetTickCount() - LastOnAim[Player.m_ID] > 2000) {
				if (Native::IsPlayerTargettingEntity(Player.m_ID, Menu::GetLocalPlayer().m_Entity)) {
					LastOnAim[Player.m_ID] = GetTickCount();
					ProcessEvent(EVENT_AIM, Player.m_ID);
				}
			}
		});
	}
}

NetworkProtectionEventReactionsMenu* _instance;
NetworkProtectionEventReactionsMenu* NetworkProtectionEventReactionsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkProtectionEventReactionsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkProtectionEventReactionsMenu::~NetworkProtectionEventReactionsMenu() { delete _instance; }