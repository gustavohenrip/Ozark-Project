#include "teleport.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../main.hpp"
#include "utils/fibers.hpp"
#include "utils/fiber_pool.hpp"
#include "menu/base/util/control_manager.hpp"
#include "teleport/teleport_popular.hpp"
#include "teleport/teleport_store.hpp"
#include "teleport/teleport_directions.hpp"
#include "teleport/teleport_save_load.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "world/world_map.hpp"

using namespace TeleportMenuVars;

namespace TeleportMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> TeleportTo[] = {
		{ TranslationString("Nearest Player", true), 0 },
		{ TranslationString("Objective", true), 1 }
	};

	ScrollStruct<int> TeleportInto[] = {
		{ TranslationString("Nearest Vehicle", true), 0 },
		{ TranslationString("Last Vehicle", true), 1 }
	};

	int GetIdealSwitch(Math::Vector3<float> From, Math::Vector3<float> To) {
		return Native::GetIdealPlayerSwitchType(From.m_X, From.m_Y, From.m_Z, To.m_X, To.m_Y, To.m_Z);
	}

	void Teleport(Math::Vector3<float> Coords) {
		if (m_Vars.m_TeleportTransition) {
			Utils::GetFiberPool()->Push([=] {
				auto M = Native::GetHashKey(Menu::GetLocalPlayer().m_Connected ? "mp_m_freemode_01" : "player_zero");
				Menu::GetControlManager()->RequestModel(M, [=] (uint32_t Model) {
					auto Ped = Native::CreatePed(13, Model, Coords.m_X, Coords.m_Y, Coords.m_Z, 0.f, true, false);
					if (Native::DoesEntityExist(Ped)) {
						Native::StartPlayerSwitch(Menu::GetLocalPlayer().m_Ped, Ped, 0, GetIdealSwitch(Menu::GetLocalPlayer().m_Coords, Coords));
						Native::SetEntityAsMissionEntity(Ped, true, true);
						Native::DeleteEntity(&Ped);

						if (Native::IsPlayerSwitchInProgress()) {
							while (Native::GetPlayerSwitchState() != 8) {
								Utils::WaitFiber(100);
							}

							Native::SetEntityCoords(Menu::GetLocalPlayer().m_Entity, Coords.m_X, Coords.m_Y, Coords.m_Z, false, false, false, false);
						}
					}
				});
			});
		} else {
			Native::SetEntityCoords(Menu::GetLocalPlayer().m_Entity, Coords.m_X, Coords.m_Y, Coords.m_Z, false, false, false, false);
		}
	}

	void TeleportToWaypoint() {
		if (m_Vars.m_TeleportTransition) {
			Utils::GetFiberPool()->Push([=] {
				Math::Vector3<float> Coords = Native::GetBlipInfoIdCoord(Native::GetFirstBlipInfoId(8));
				if (Coords.IsZero()) {
					Menu::GetNotify()->NotifyStacked(TeleportMenu::GetInstance()->getString("Failed to find waypoint"));
					return;
				}

				auto M = Native::GetHashKey(Menu::GetLocalPlayer().m_Connected ? "mp_m_freemode_01" : "player_zero");
				Menu::GetControlManager()->RequestModel(M, [&] (uint32_t Model) {
					auto Ped = Native::CreatePed(13, Model, Coords.m_X, Coords.m_Y, Coords.m_Z, 0.f, true, false);
					if (Native::DoesEntityExist(Ped)) {
						Native::StartPlayerSwitch(Menu::GetLocalPlayer().m_Ped, Ped, 0, GetIdealSwitch(Menu::GetLocalPlayer().m_Coords, Coords));
						Native::SetEntityAsMissionEntity(Ped, true, true);
						Native::DeleteEntity(&Ped);

						if (Native::IsPlayerSwitchInProgress()) {
							while (Native::GetPlayerSwitchState() != 8) {
								Utils::WaitFiber(100);
							}

							bool GroundFound = false;
							float GroundCheckHeight[] = { 100.0f, 150.0f, 50.0f, 0.0f, 200.0f, 250.0f, 300.0f, 350.0f, 400.0f, 450.0f, 500.0f, 550.0f, 600.0f, 650.0f, 700.0f, 750.0f, 800.0f };
							for (int i = 0; i < sizeof(GroundCheckHeight) / sizeof(float); i++) {
								Native::SetEntityCoordsNoOffset(Menu::GetLocalPlayer().m_Entity, Coords.m_X, Coords.m_Y, GroundCheckHeight[i], 0, 0, 1);
								Utils::WaitFiber(100);
								if (Native::GetGroundZFor3DCoord(Coords.m_X, Coords.m_Y, GroundCheckHeight[i], &Coords.m_Z, 0)) {
									GroundFound = true;
									break;
								}
							}

							if (!GroundFound) {
								Coords.m_Z = 1000.0f;
								Native::GiveDelayedWeaponToPed(Menu::GetLocalPlayer().m_Ped, 0xFBAB5776, 1, 0);
							}

							Native::SetEntityCoords(Menu::GetLocalPlayer().m_Entity, Coords.m_X, Coords.m_Y, Coords.m_Z, false, false, false, false);
						}
					}
				});
			});
		} else {
			Utils::GetFiberPool()->Push([=] {
				Math::Vector3<float> Coords = Native::GetBlipInfoIdCoord(Native::GetFirstBlipInfoId(8));
				if (Coords.IsZero()) {
					Menu::GetNotify()->NotifyStacked(TeleportMenu::GetInstance()->getString("Failed to find waypoint"));
					return;
				}

				bool GroundFound = false;
				float GroundCheckHeight[] = { 100.0f, 150.0f, 50.0f, 0.0f, 200.0f, 250.0f, 300.0f, 350.0f, 400.0f, 450.0f, 500.0f, 550.0f, 600.0f, 650.0f, 700.0f, 750.0f, 800.0f };
				for (int i = 0; i < sizeof(GroundCheckHeight) / sizeof(float); i++) {
					Native::SetEntityCoordsNoOffset(Menu::GetLocalPlayer().m_Entity, Coords.m_X, Coords.m_Y, GroundCheckHeight[i], 0, 0, 1);
					Utils::WaitFiber(100);
					if (Native::GetGroundZFor3DCoord(Coords.m_X, Coords.m_Y, GroundCheckHeight[i], &Coords.m_Z, 0)) {
						GroundFound = true;
						break;
					}
				}

				if (!GroundFound) {
					Coords.m_Z = 1000.0f;
					Native::GiveDelayedWeaponToPed(Menu::GetLocalPlayer().m_Ped, 0xFBAB5776, 1, 0);
				}

				Native::SetEntityCoords(Menu::GetLocalPlayer().m_Entity, Coords.m_X, Coords.m_Y, Coords.m_Z, false, false, false, false);
			});
		}
	}

	void TeleportIntoNearestVehicle() {
		float ClosestDistance = std::numeric_limits<float>::max();
		Entity ClosestEntity = 0;

		Menu::Pools::GetVehiclePool()->Run([&] (Entity Veh) {
			float Distance = Menu::GetLocalPlayer().m_Coords.GetMathmaticalDistance(Native::GetEntityCoords(Veh, true));
			if (Distance < ClosestDistance) {
				ClosestDistance = Distance;
				ClosestEntity = Veh;
			}
		});

		if (ClosestEntity != 0) {
			if (Native::DoesEntityExist(ClosestEntity)) {
				Native::SetPedIntoVehicle(Menu::GetLocalPlayer().m_Ped, ClosestEntity, -1);
			}
		}
	}

	void TeleportIntoLastVehicle() {
		auto Vehicle = Native::GetPlayersLastVehicle();
		if (Native::DoesEntityExist(Vehicle)) {
			Native::SetPedIntoVehicle(Menu::GetLocalPlayer().m_Ped, Vehicle, -1);
		}
	}

	Player GetClosestPlayer() {
		float NearestDistance = std::numeric_limits<float>::max();
		Player NearestPlayer = -1;

		Menu::GetPlayerManager()->Foreach([&] (Menu::PlayerVars& Player) {
			auto Distance = Player.m_Coords.GetMathmaticalDistance(Menu::GetLocalPlayer().m_Coords);
			if (Distance < NearestDistance) {
				NearestDistance = Distance;
				NearestPlayer = Player.m_ID;
			}
		});

		return NearestPlayer;
	}

	void TeleportToNearestPlayer() {
		auto Closest = GetClosestPlayer();
		if (Closest != -1) {
			Teleport(Native::GetEntityCoords(Menu::GetPlayer(Closest).m_Ped, false));
		}
	}

	void TeleportToObjective() {
		bool Teleported = false;

		auto BlipList = Global::Vars::g_BlipList;
		for (int i = 0; i < 1500; i++) {
			auto Blip = BlipList->m_Blips[i];
			if (Blip) {
				auto Color = Blip->m_Color;
				auto Icon = Blip->m_Sprite;

				if ((Icon == 306) ||
					(Color == 66 && Icon == 1) ||
					(Color == 60 && Icon == 1) ||
					(Color == 5 && Icon == 1) ||
					(Color == 0 && Icon == 38) ||
					(Color == 2 && Icon == 1) ||
					(Color == 3 && Icon == 1)) {
					auto Coords = Math::Vector3_<float>::ToPadded(Blip->m_Coords);
					if (Coords.IsZero()) {
						Menu::GetNotify()->NotifyStacked(TeleportMenu::GetInstance()->getString("Failed to teleport to objective"));
					} else {
						Teleported = true;
						Teleport(Coords);
						break;
					}
				}
			}
		}

		if (!Teleported) {
			Menu::GetNotify()->NotifyStacked(TeleportMenu::GetInstance()->getString("No objective was found"));
		}
	}
}

void TeleportMenu::Init() {
	SetName("Teleport");
	SetParentSubmenu<MainMenu>();

	addString("Failed to find waypoint");
	addString("Failed to teleport to objective");
	addString("No objective was found");

	addOption(SubmenuOption("Save and Load")
		.addTranslation().addHotkey()
		.addSubmenu<TeleportSaveLoadMenu>());

	addOption(SubmenuOption("Popular Locations")
		.addTranslation().addHotkey()
		.addSubmenu<TeleportPopularMenu>());

	addOption(SubmenuOption("Store Locations")
		.addTranslation().addHotkey()
		.addSubmenu<TeleportStoreMenu>());

	addOption(SubmenuOption("Directions")
		.addTranslation().addHotkey()
		.addSubmenu<TeleportDirectionsMenu>());

	addOption(SubmenuOption("IPLs")
		.addTranslation().addHotkey()
		.addSubmenu<WorldMapMenu>());

	addOption(ToggleOption("Transition")
		.addTranslation()
		.addToggle(m_Vars.m_TeleportTransition).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Auto Waypoint")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_AutoWaypoint).canBeSaved(m_ParentNameStack));

	addOption(ScrollOption<int>(SCROLLSELECT, "Teleport Into")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_TeleportInto, 0, NUMOF(TeleportInto), TeleportInto)
		.addOnClick([] { if (m_Vars.m_TeleportInto == 0) TeleportIntoNearestVehicle(); else TeleportIntoLastVehicle(); }));

	addOption(ScrollOption<int>(SCROLLSELECT, "Teleport To")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_TeleportTo, 0, NUMOF(TeleportTo), TeleportTo)
		.addOnClick([] { if (m_Vars.m_TeleportTo == 0) TeleportToNearestPlayer(); else TeleportToObjective(); }));
}

void TeleportMenu::Update() {}

/*Called once on submenu open*/
void TeleportMenu::UpdateOnce() {}

/*Called always*/
void TeleportMenu::FeatureUpdate() {
	if (m_Vars.m_AutoWaypoint) {
		if (Native::IsWaypointActive() && !m_Vars.m_HasTeleportedFlag) {
			TeleportToWaypoint();
			m_Vars.m_HasTeleportedFlag = true;
			m_Vars.m_WaypointTimer = GetTickCount() + 3000;
		}

		if (m_Vars.m_WaypointTimer < GetTickCount()) {
			m_Vars.m_HasTeleportedFlag = false;
		}
	}
}

TeleportMenu* _instance;
TeleportMenu* TeleportMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new TeleportMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

TeleportMenu::~TeleportMenu() { delete _instance; }