#include "network_players_friendly.hpp"
#include "menu/base/submenu_handler.hpp"
#include "network_players_selected.hpp"
#include "global/arrays.hpp"
#include "utils/fiber_pool.hpp"
#include "utils/fibers.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/util/helpers.hpp"
#include "menu/base/util/global.hpp"
#include "rage/types/global_types.hpp"
#include "menu/base/util/timers.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "menu/base/util/control_manager.hpp"
#include "friendly/network_friendly_drops.hpp"
#include "menu/base/submenus/main/network/network_session.hpp"
#include "menu/base/submenus/main/player/wardrobe.hpp"
#include "menu/base/submenus/main/weapon/give/give_weapon.hpp"
#include <mutex>

using namespace NetworkPlayersFriendlyMenuVars;

TranslationString T_Giving("Giving", true, true);
TranslationString T_Gave("Gave", true, true);
TranslationString T_AllWeapons("all weapons", true, true);
TranslationString T_Weapons("weapons", true, true);

namespace NetworkPlayersFriendlyMenuVars {
	Vars_ m_Vars;

	ScrollStruct<uint32_t> Commends[] = {
		{ TranslationString("Friendly", true), 0xDAFB10F9 },
		{ TranslationString("Helpful", true), 0x893E1390 }
	};

	void GiveAllWeapons(Menu::PlayerVars Player) {
		Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s %s", TRANSLATE(T_Giving), Player.m_Name, TRANSLATE(T_AllWeapons)));

		Utils::GetFiberPool()->Push([=] {
			for (auto& Weapon : Global::Arrays::g_Weapons) {
				GiveWeaponMenuVars::GiveWeapon(Player.m_Ped, Weapon.m_Result, false, false);
				Utils::WaitFiber(10);
			}

			Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s %i %s", TRANSLATE(T_Gave), Player.m_Name, NUMOF(Global::Arrays::g_Weapons), TRANSLATE(T_Weapons)));
		});
	}

	void GiveOffTheRadar(Menu::PlayerVars Player) {
		std::vector<int> List = {
			Rage::Global::GiveOffTheRadar,
			Player.m_ID,
			Native::GetNetworkTime(),
			Native::GetNetworkTime() + 60000,
			1,	// has start
			1,	// has end
			Menu::Global(1630317).At(Player.m_ID, 595).At(506).As<int>()
		};

		Menu::Helpers::TriggerScriptEvent(List, 7, Player.m_ID);
	}

	void RemoveWantedLevel(Menu::PlayerVars Player) {
		std::vector<int> List = {
			Rage::Global::RemoveWantedLevel,
			Player.m_ID,
			Menu::Global(1630317).At(Player.m_ID, 595).At(506).As<int>()
		};

		Menu::Helpers::TriggerScriptEvent(List, 3, Player.m_ID);
	}

	void RemoveAttachments(Menu::PlayerVars Player) {
		Menu::Pools::GetObjectPool()->Run([&] (Object Obj) {
			uint64_t Address = Rage::Engine::GetEntityAddress(Obj);
			if (Address) {
				if (*(uint64_t*)(Address + 0x340) == 0) { // Weapon Object
					if (Native::GetEntityModel(Obj) != 0x4FAA899A) {
						if (Native::IsEntityAttachedToEntity(Obj, Player.m_Ped)) {
							Menu::GetControlManager()->RequestControl(Obj, [&] (Entity Ent) {
								Native::SetEntityAsMissionEntity(Ent, true, true);
								Native::DetachEntity(Ent, true, true);
								Native::DeleteEntity(&Ent);
							}, true);
						}
					}
				}
			}
		});

		Menu::Pools::GetVehiclePool()->Run([&] (Vehicle Veh) {
			if (Veh != Menu::GetLocalPlayer().m_Vehicle) {
				if (Native::GetEntityAttachedTo(Veh) == Player.m_Ped) {
					Menu::GetControlManager()->RequestControl(Veh, [&] (Entity Ent) {
						Native::SetEntityAsMissionEntity(Ent, true, true);
						Native::DetachEntity(Ent, true, true);
						Native::DeleteEntity(&Ent);
					}, true);
				}
			}
		}, true);

		Menu::Pools::GetPedPool()->Run([&] (Ped Ped) {
			if (!Native::IsPedAPlayer(Ped)) {
				if (!Native::IsPedInAnyVehicle(Ped, true)) {
					if (Native::IsEntityAttachedToEntity(Ped, Player.m_Ped)) {
						Menu::GetControlManager()->RequestControl(Ped, [&] (Entity Ent) {
							Native::SetEntityAsMissionEntity(Ent, true, true);
							Native::DetachEntity(Ent, true, true);
							Native::DeleteEntity(&Ent);
						}, true);
					}
				}
			}
		});
	}

	void CommendPlayer(Menu::PlayerVars Player, int Commend) {
		if (!Player.m_NetGamePlayer) return;

		Rage::Engine::SendIncrementStatEvent(Commends[Commend].m_Result, 16, Player.m_NetGamePlayer);
	}

	void SpawnBodyguards(Menu::PlayerVars Player, int Count) {
		for (int i = 0; i < Count; i++) {
			Ped Ped = Native::ClonePed(Player.m_Ped, Player.m_Heading, true, false);
			if (Native::DoesEntityExist(Ped)) {
				Native::GiveWeaponToPed(Ped, 0x1B06D571, -1, false, true);
				Native::SetPedAsGroupMember(Ped, Native::GetPlayerGroup(Player.m_ID));
			}
		}
	}

	void CopyPlayerOutfit(Menu::PlayerVars Player) {
		Native::ClearAllPedProps(Menu::GetLocalPlayer().m_Ped);

		for (auto& Component : WardrobeMenuVars::Components) {
			Native::SetPedComponentVariation(Menu::GetLocalPlayer().m_Ped, Component.m_Result, Native::GetPedDrawableVariation(Player.m_Ped, Component.m_Result), Native::GetPedTextureVariation(Player.m_Ped, Component.m_Result), 0);
		}

		for (auto& Prop : WardrobeMenuVars::Accessories) {
			Native::SetPedPropIndex(Menu::GetLocalPlayer().m_Ped, Prop.m_Result, Native::GetPedPropIndex(Player.m_Ped, Prop.m_Result), Native::GetPedPropTextureIndex(Player.m_Ped, Prop.m_Result), true);
		}
	}

	void ParachutePlayer(Menu::PlayerVars Player) {
		Math::Vector3<float> Rotation = Native::GetEntityRotation(Player.m_Ped, 0);
		Math::Vector3<float> Coords = Player.m_Coords;

		Native::SetEntityCoords(Menu::GetLocalPlayer().m_Ped, Coords.m_X + 10.0f, Coords.m_Y + 10.0f, Coords.m_Z + 24.0f, true, false, false, false);
		Native::SetEntityRotation(Menu::GetLocalPlayer().m_Ped, Rotation.m_X, Rotation.m_Y, Rotation.m_Z, 0, true);
		Native::TaskParachuteToTarget(Menu::GetLocalPlayer().m_Ped, Coords.m_X, Coords.m_Y, Coords.m_Z);
	}
}

void NetworkPlayersFriendlyMenu::Init() {
	SetName("Friendly");
	SetParentSubmenu<NetworkPlayersSelectedMenu>();

	NetworkFriendlyDropsMenu::GetInstance(); // Init

	addString("Drops");
	addString("Give All Weapons");
	addString("Remove Attachments");
	addString("Demi Godmode");
	addString("Give Off the Radar");
	addString("Disable Police");
	addString("Revenge");
	addString("Revenge");
	addString("Commend");
	addString("Spawn Bodyguard(s)");
}

void NetworkPlayersFriendlyMenu::Update() {}

/*Called once on submenu open*/
void NetworkPlayersFriendlyMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	if (Global::Vars::g_NetworkMenuType == SELECTED_PLAYER) {
		SetParentSubmenu<NetworkPlayersSelectedMenu>();

		addOption(SubmenuOption(getString("Drops"))
			.addSubmenu<NetworkFriendlyDropsMenu>());

		addOption(ToggleOption(getString("Demi Godmode"))
			.addToggle(Menu::GetSelectedPlayer().m_DemiGodmode)
			.addOnUpdate([](ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_DemiGodmode); })
			.addTooltip("Only works on foot"));

		addOption(ToggleOption(getString("Give Off the Radar"))
			.addToggle(Menu::GetSelectedPlayer().m_OffTheRadar)
			.addOnUpdate([](ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_OffTheRadar); }));

		addOption(ToggleOption(getString("Disable Police"))
			.addToggle(Menu::GetSelectedPlayer().m_DisablePolice)
			.addOnUpdate([](ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_DisablePolice); }));

		addOption(ToggleOption(getString("Revenge"))
			.addToggle(Menu::GetSelectedPlayer().m_Revenge)
			.addOnUpdate([](ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_Revenge); })
			.addTooltip("Kills people who kill them"));

		addOption(ToggleOption("Force Visible")
			.addTranslation()
			.addToggle(Menu::GetSelectedPlayer().m_ForceVisible)
			.addOnUpdate([](ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_ForceVisible); }));

		addOption(ScrollOption<uint32_t>(SCROLLSELECT, getString("Commend"))
			.addScroll(m_Vars.m_Commend, 0, NUMOF(Commends), Commends)
			.addOnClick([] { CommendPlayer(Menu::GetSelectedPlayer(), m_Vars.m_Commend); }));

		addOption(NumberOption<int>(SCROLLSELECT, getString("Spawn Bodyguard(s)"))
			.addNumber(m_Vars.m_BodyguardCount, "%i", 1).addMin(1).addMax(20)
			.addOnClick([] { SpawnBodyguards(Menu::GetSelectedPlayer(), m_Vars.m_BodyguardCount); }));

		addOption(ButtonOption("Copy Outfit")
			.addTranslation()
			.addOnClick([] { CopyPlayerOutfit(Menu::GetSelectedPlayer()); }));

		addOption(ButtonOption("Parachute")
			.addTranslation()
			.addOnClick([] { ParachutePlayer(Menu::GetSelectedPlayer()); }));

		addOption(ButtonOption(getString("Give All Weapons"))
			.addOnClick([] { GiveAllWeapons(Menu::GetSelectedPlayer()); }));

		addOption(ButtonOption(getString("Remove Attachments"))
			.addOnClick([] { RemoveAttachments(Menu::GetSelectedPlayer()); }));
	} else {
		SetParentSubmenu<NetworkSessionMenu>();

		addOption(SubmenuOption(getString("Drops"))
			.addSubmenu<NetworkFriendlyDropsMenu>());

		addOption(ToggleOption(getString("Demi Godmode"))
			.addToggle(m_Vars.m_DemiGodmode)
			.addTooltip("Only works on foot"));

		addOption(ToggleOption(getString("Give Off the Radar"))
			.addToggle(m_Vars.m_OffTheRadar));

		addOption(ToggleOption(getString("Disable Police"))
			.addToggle(m_Vars.m_DisablePolice));

		addOption(ToggleOption(getString("Revenge"))
			.addToggle(m_Vars.m_Revenge)
			.addTooltip("Kills people who kill them"));

		addOption(ButtonOption(getString("Give All Weapons"))
			.addOnClick([] {
			Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
				GiveAllWeapons(Player);
			});
		}));

		addOption(ButtonOption(getString("Remove Attachments"))
			.addOnClick([] {
			Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
				RemoveAttachments(Player);
			});
		}));

		// Bullet Impact <List>

		addOption(ScrollOption<uint32_t>(SCROLLSELECT, getString("Commend"))
			.addScroll(m_Vars.m_Commend, 0, NUMOF(Commends), Commends)
			.addOnClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					CommendPlayer(Player, m_Vars.m_Commend);
				});
			}));

		addOption(NumberOption<int>(SCROLLSELECT, getString("Spawn Bodyguard(s)"))
			.addNumber(m_Vars.m_BodyguardCount, "%i", 1).addMin(1).addMax(20)
			.addOnClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					SpawnBodyguards(Player, m_Vars.m_BodyguardCount);
				});
			}));
	}
}

/*Called always*/
void NetworkPlayersFriendlyMenu::FeatureUpdate() {
	Menu::GetPlayerManager()->Foreach([] (Menu::PlayerVars& Player) {
		if (Player.m_ForceVisible) {
			if (!Native::IsEntityVisible(Player.m_Entity)) {
				Native::SetEntityLocallyVisible(Player.m_Entity);
				Native::SetEntityVisible(Player.m_Entity, true, true);
			}
		}

		if (Player.m_DemiGodmode || m_Vars.m_DemiGodmode) {
			if (!Player.m_InVehicle && !Native::IsEntityDead(Player.m_Ped, 0)) {
				Menu::Timers::RunTimedFunction(&Player.m_DemiGodmodeTimer, 250, [=] {
					int Armor = Native::GetPedArmour(Player.m_Ped);
					int ArmorMax = Native::GetPlayerMaxArmour(Player.m_ID);
					int Health = Native::GetEntityHealth(Player.m_Ped);
					int HealthMax = Native::GetEntityMaxHealth(Player.m_Ped);

					if (Armor < ArmorMax) {
						Native::CreateAmbientPickup(0x4BFB42D1, Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, 0, ArmorMax - Armor, 0xE6CB661E, false, true);
					}

					if (Health < HealthMax) {
						Native::CreateAmbientPickup(0x8F707C18, Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, 0, HealthMax - Health, 0xE6CB661E, false, true);
					}
				});
			}
		}

		if (Player.m_OffTheRadar || m_Vars.m_OffTheRadar) {
			Menu::Timers::RunTimedFunction(&Player.m_OffTheRadarTimer, 1000, [=] {
				GiveOffTheRadar(Player);
			});
		}

		if (Player.m_DisablePolice || m_Vars.m_DisablePolice) {
			Menu::Timers::RunTimedFunction(&Player.m_DisablePoliceTimer, 500, [=] {
				RemoveWantedLevel(Player);
			});
		}

		if (Player.m_Revenge || m_Vars.m_Revenge) {
			if (Native::IsEntityDead(Player.m_Ped, 1)) {
				Entity Killer = Native::GetPedSourceOfDeath(Player.m_Ped);
				if (Killer) {
					if (Native::IsPedAPlayer(Killer)) {
						if (!Native::IsEntityDead(Killer, 1)) {
							auto Coords = Native::GetEntityCoords(Killer, true);
							Native::AddExplosion(Coords.m_X, Coords.m_Y, Coords.m_Z, 1, 100.f, true, false, 0.f, false);
						}
					}
				}
			}
		}
	}, true);
}

NetworkPlayersFriendlyMenu* _instance;
NetworkPlayersFriendlyMenu* NetworkPlayersFriendlyMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkPlayersFriendlyMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkPlayersFriendlyMenu::~NetworkPlayersFriendlyMenu() { delete _instance; }