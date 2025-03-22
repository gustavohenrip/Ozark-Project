#include "network_players_teleport.hpp"
#include "menu/base/submenu_handler.hpp"
#include "network_players_selected.hpp"
#include "teleport/network_teleport_apartment.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/va.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/submenus/main/network/network_session.hpp"
#include "utils/fiber_pool.hpp"
#include "utils/fibers.hpp"
#include "menu/base/submenus/main/teleport.hpp"

using namespace NetworkPlayersTeleportMenuVars;

namespace NetworkPlayersTeleportMenuVars {
	Vars_ m_Vars;

	void TeleportToPlayer(Menu::PlayerVars Player) {
		// Native::SetNewWaypoint(Player.m_Coords.m_X, Player.m_Coords.m_Y);
		// TeleportMenuVars::TeleportToWaypoint();
		Native::SetEntityCoords(Menu::GetLocalPlayer().m_Entity, Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, true, true, true, false);
	}

	void TeleportToMe(Menu::PlayerVars Player) {
		if (Player.m_InVehicle) {
			Menu::GetControlManager()->RequestControl(Player.m_Entity, [] (Entity Veh) {
				Native::SetEntityCoords(Veh, Menu::GetLocalPlayer().m_Coords.m_X, Menu::GetLocalPlayer().m_Coords.m_Y, Menu::GetLocalPlayer().m_Coords.m_Z, true, true, true, false);
			});
		} else {
			Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s", Player.m_Name, NetworkPlayersTeleportMenu::GetInstance()->getString("isn't in a vehicle").c_str()));
		}
	}

	void TeleportIntoVehicle(Menu::PlayerVars Player) {
		if (Player.m_InVehicle) {
			bool Inside = false;
			for (int i = -1; i < Native::GetVehicleMaxNumberOfPassengers(Player.m_Entity); i++) {
				if (Native::IsVehicleSeatFree(Player.m_Entity, i, 1)) {
					Native::SetPedIntoVehicle(Menu::GetLocalPlayer().m_Ped, Player.m_Entity, i);
					Inside = true;
					break;
				}
			}

			if (!Inside) {
				Menu::GetNotify()->NotifyStacked(NetworkPlayersTeleportMenu::GetInstance()->getString("No free seats, or they're too far away").c_str());
			}
		} else {
			Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s", Player.m_Name, NetworkPlayersTeleportMenu::GetInstance()->getString("isn't in a vehicle").c_str()));
		}
	}

	void TeleportAllPlayersToMe() {
		float Offset = 2.f;
		Menu::GetPlayerManager()->Foreach([&](Menu::PlayerVars& Player) {
			if (Player.m_InVehicle) {
				auto Coords = Native::GetOffsetFromEntityInWorldCoords(Menu::GetLocalPlayer().m_Ped, .0f, Offset, .0f);
				Offset *= 2.f;
				Menu::GetControlManager()->RequestControl(Player.m_Entity, [=](Entity Veh) {
					Native::SetEntityCoords(Veh, Coords.m_X, Coords.m_Y, Coords.m_Z, true, true, true, false);
				});
			}
		});
	}
}

void NetworkPlayersTeleportMenu::Init() {
	SetName("Teleport");
	SetParentSubmenu<NetworkPlayersSelectedMenu>();

	NetworkTeleportApartmentMenu::GetInstance();

	addString("Apartments");
	addString("Teleport to Player");
	addString("Teleport to Me");
	addString("Teleport Into Vehicle");
	addString("No free seats, or they're too far away");
	addString("isn't in a vehicle");
}

void NetworkPlayersTeleportMenu::Update() {}

/*Called once on submenu open*/
void NetworkPlayersTeleportMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	if (Global::Vars::g_NetworkMenuType == SELECTED_PLAYER) {
		SetParentSubmenu<NetworkPlayersSelectedMenu>();

		addOption(SubmenuOption(getString("Apartments"))
			.addSubmenu<NetworkTeleportApartmentMenu>());

		addOption(ButtonOption(getString("Teleport to Player"))
			.addOnClick([] { TeleportToPlayer(Menu::GetSelectedPlayer()); }));

		addOption(ButtonOption(getString("Teleport to Me"))
			.addOnClick([] { TeleportToMe(Menu::GetSelectedPlayer()); }));

		addOption(ButtonOption(getString("Teleport Into Vehicle"))
			.addOnClick([] { TeleportIntoVehicle(Menu::GetSelectedPlayer()); }));
	} else {
		SetParentSubmenu<NetworkSessionMenu>();

		addOption(SubmenuOption(getString("Apartments"))
			.addSubmenu<NetworkTeleportApartmentMenu>());

		addOption(ButtonOption(getString("Teleport to Me"))
			.addOnClick([] {
				TeleportAllPlayersToMe();
			}));
	}
}

/*Called always*/
void NetworkPlayersTeleportMenu::FeatureUpdate() {}

NetworkPlayersTeleportMenu* _instance;
NetworkPlayersTeleportMenu* NetworkPlayersTeleportMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkPlayersTeleportMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkPlayersTeleportMenu::~NetworkPlayersTeleportMenu() { delete _instance; }