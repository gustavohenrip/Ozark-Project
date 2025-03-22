#include "network_players_removal.hpp"
#include "menu/base/submenu_handler.hpp"
#include "network_players_selected.hpp"
#include "../network_session.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/helpers.hpp"
#include "menu/base/submenus/main/network/network_host_toolkit.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/util/global.hpp"
#include "rage/types/global_types.hpp"
#include "utils/caller.hpp"
#include "utils/log.hpp"
#include "global/arrays.hpp"
#include "utils/fiber_pool.hpp"
#include "utils/fibers.hpp"

using namespace NetworkPlayersRemovalMenuVars;

namespace NetworkPlayersRemovalMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> Crashes[] = {
		{ "Crash V1", 0 },
		{ "Crash V2", 0 },
		{ "Crash V3", 0 },
	};

	void CrashPlayer(Menu::PlayerVars& Player, int ID) {
		if (m_Vars.m_TargetedCrash/* || (GetTickCount() - m_Vars.m_TargetedCrashTimeout) < 10000*/) return;

		if (Player.m_NetGamePlayer) {
			if (Player.m_NetGamePlayer->m_PlayerInfo) {
				LOG_INFO("Started a V%i crash against %s for 5 seconds", ID, Player.m_Name);
				
				m_Vars.m_TargetedCrashID = ID;
				m_Vars.m_TargetedCrashTimeout = GetTickCount() + 5000;
				m_Vars.m_TargetedCrashTarget = Player.m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID;
				m_Vars.m_TargetedCrash = true;

				Math::Vector3<float> Coords = Native::GetOffsetFromEntityInWorldCoords(Player.m_Entity, Native::GetRandomFloatInRange(0.f, 10.f), Native::GetRandomFloatInRange(0.f, 10.f), 0.f);

				Native::RequestModel(0x8E9254FB);
				Native::CreateVehicle(0x8E9254FB, Coords.m_X, Coords.m_Y, Coords.m_Z, 0.f, true, true, 0);

				Native::RequestModel(0x67B3F020);
				Native::CreateVehicle(0x67B3F020, Coords.m_X, Coords.m_Y, Coords.m_Z, 0.f, true, true, 0);

				Native::RequestModel(0x1ABA13B5);
				Native::CreateVehicle(0x1ABA13B5, Coords.m_X, Coords.m_Y, Coords.m_Z, 0.f, true, true, 0);

				Native::RequestModel(0x5A82F9AE);
				Native::CreateVehicle(0x5A82F9AE, Coords.m_X, Coords.m_Y, Coords.m_Z, 0.f, true, true, 0);

				Native::RequestModel(0x94204D89);
				Native::CreateVehicle(0x94204D89, Coords.m_X, Coords.m_Y, Coords.m_Z, 0.f, true, true, 0);

				Native::RequestModel(0x9C9EFFD8);
				Native::CreatePed(21, 0x9C9EFFD8, Coords.m_X, Coords.m_Y, Coords.m_Z, Native::GetRandomFloatInRange(0.f, 100.f), true, false);

				Native::RequestModel(0x705E61F2);
				Native::CreatePed(21, 0x705E61F2, Coords.m_X, Coords.m_Y, Coords.m_Z, Native::GetRandomFloatInRange(0.f, 100.f), true, false);

				Native::RequestModel(0x028ABF95);
				Native::CreatePed(21, 0x028ABF95, Coords.m_X, Coords.m_Y, Coords.m_Z, Native::GetRandomFloatInRange(0.f, 100.f), true, false);

				Native::RequestModel(0x91CA3E2C);
				Native::CreatePed(21, 0x91CA3E2C, Coords.m_X, Coords.m_Y, Coords.m_Z, Native::GetRandomFloatInRange(0.f, 100.f), true, false);

				Native::RequestModel(0xe6bb31ab);
				Native::CreateObject(0xe6bb31ab, Coords.m_X, Coords.m_Y, Coords.m_Z, true, true, false);

				Native::RequestModel(0x7f2b2371);
				Native::CreateObject(0x7f2b2371, Coords.m_X, Coords.m_Y, Coords.m_Z, true, true, false);

				Native::RequestModel(0x91cd3b2);
				Native::CreateObject(0x91cd3b2, Coords.m_X, Coords.m_Y, Coords.m_Z, true, true, false);

				Native::RequestModel(0x76d281de);
				Native::CreateObject(0x76d281de, Coords.m_X, Coords.m_Y, Coords.m_Z, true, true, false);
			}
		}
	}

	void ScriptHostKick(Menu::PlayerVars& Player) {
		Utils::GetFiberPool()->Push([=] {
			if (NetworkHostToolkitMenuVars::ForceHostOfScript("freemode", Menu::GetLocalPlayer().m_NetGamePlayer, false)) {
				Utils::WaitFiber(250);
				Menu::Global(::Rage::Global::HostDropKick + Player.m_ID + 1).As<int>() = 1;
			}
		});
	}

	void SessionHostKick(Menu::PlayerVars& Player) {
		if (Rage::Engine::GetHostNetGamePlayer() == Menu::GetLocalPlayer().m_NetGamePlayer) {
			if (Player.m_NetGamePlayer != Menu::GetLocalPlayer().m_NetGamePlayer) {
				Native::NetworkSessionKickPlayer(Player.m_ID);
			}
		} else Menu::GetNotify()->NotifyStacked(NetworkPlayersRemovalMenu::GetInstance()->getString("You need to be host to use this kick"));
	}

	void BreakFreemodeKick(Menu::PlayerVars& Player) {
		if (Player.m_NetGamePlayer == Rage::Engine::GetHostNetGamePlayer()) {
			Menu::GetNotify()->NotifyStacked(NetworkPlayersRemovalMenu::GetInstance()->getString("You can't use this kick on the host"));
			return;
		}

		Menu::Helpers::TriggerScriptEvent({ -966559987, 18707275, 55963135, 20806885, 56512451, -21520996 }, 6, Player.m_ID);
		Menu::Helpers::TriggerScriptEvent({ -1054826273, -33111572, -28521729, -7891846 }, 4, Player.m_ID);
		Menu::Helpers::TriggerScriptEvent({ -1729804184, 31689453, 91436768, 9588623, -50805664, -10510254 }, 6, Player.m_ID);
	}
}

void NetworkPlayersRemovalMenu::Init() {
	SetName("Removals");
	SetParentSubmenu<NetworkPlayersSelectedMenu>();

	addString("You can't use this kick on the host");
	addString("You need to be host to use this kick");
	addString("Script Host Kick");
	addString("Session Host Kick");
	addString("Break Freemode Kick");
	addString("Crash");
}

void NetworkPlayersRemovalMenu::Update() {}

/*Called once on submenu open*/
void NetworkPlayersRemovalMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	if (Global::Vars::g_NetworkMenuType == SELECTED_PLAYER) {
		SetParentSubmenu<NetworkPlayersSelectedMenu>();

		addOption(ButtonOption(getString("Script Host Kick"))
			.addOnClick([] { ScriptHostKick(Menu::GetSelectedPlayer()); }));

		addOption(ButtonOption(getString("Session Host Kick"))
			.addOnClick([] { SessionHostKick(Menu::GetSelectedPlayer()); }));

		addOption(ButtonOption(getString("Break Freemode Kick"))
			.addOnClick([] { BreakFreemodeKick(Menu::GetSelectedPlayer()); }));

		addOption(ScrollOption<int>(SCROLLSELECT, getString("Crash"))
			.addScroll(m_Vars.m_Crash, 0, NUMOF(Crashes), Crashes)
			.addOnClick([] { CrashPlayer(Menu::GetSelectedPlayer(), m_Vars.m_Crash + 1); }));
	} else {
		SetParentSubmenu<NetworkSessionMenu>();

		addOption(ButtonOption(getString("Script Host Kick"))
			.addOnClick([] { Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) { ScriptHostKick(Player); }); }));
		
		addOption(ButtonOption(getString("Session Host Kick"))
			.addOnClick([] { Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) { SessionHostKick(Player); }); }));

		addOption(ButtonOption(getString("Break Freemode Kick"))
			.addOnClick([] { Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) { if (Player.m_NetGamePlayer != Rage::Engine::GetHostNetGamePlayer()) BreakFreemodeKick(Player); }); }));

		addOption(ScrollOption<int>(SCROLLSELECT, getString("Crash"))
			.addScroll(m_Vars.m_Crash, 0, NUMOF(Crashes), Crashes)
			.addOnClick([] {
				if (m_Vars.m_SessionCrash) return;
				m_Vars.m_SessionCrashID = m_Vars.m_Crash + 1;
				m_Vars.m_SessionCrashTimeout = GetTickCount() + 5000;
				m_Vars.m_SessionCrash = true;
			}));
	}
}

/*Called always*/
void NetworkPlayersRemovalMenu::FeatureUpdate() {
	if (m_Vars.m_SessionCrash) {
		if (GetTickCount() > m_Vars.m_SessionCrashTimeout) {
			m_Vars.m_SessionCrash = false;
		}
	}

	if (m_Vars.m_TargetedCrash) {
		if (GetTickCount() > m_Vars.m_TargetedCrashTimeout || !Menu::Helpers::GetPlayerFromRID(m_Vars.m_TargetedCrashTarget).m_Connected) {
			m_Vars.m_TargetedCrash = false;
			m_Vars.m_BrokenEntities.clear();
			LOG_INFO("Disabling crash for player");
		}
	}
}

NetworkPlayersRemovalMenu* _instance;
NetworkPlayersRemovalMenu* NetworkPlayersRemovalMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkPlayersRemovalMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkPlayersRemovalMenu::~NetworkPlayersRemovalMenu() { delete _instance; }