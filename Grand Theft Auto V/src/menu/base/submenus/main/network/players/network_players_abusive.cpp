#include "network_players_abusive.hpp"
#include "menu/base/submenu_handler.hpp"
#include "network_players_selected.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/util/timers.hpp"
#include "menu/base/util/helpers.hpp"
#include "rage/types/global_types.hpp"
#include "abusive/network_abusive_attackers.hpp"
#include "abusive/network_abusive_explosions.hpp"
#include "../network_session.hpp"
#include "network_players_selected.hpp"
#include "menu/base/submenus/main/network/network_overseer.hpp"
#include "utils/json.hpp"
#include "utils/va.hpp"
#include "rage/engine.hpp"
#include "utils/log.hpp"

using namespace NetworkPlayersAbusiveMenuVars;

namespace NetworkPlayersAbusiveMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> ExplosionType[] = {
		{ TranslationString("Normal", true), 0 },
		{ TranslationString("Stealth", true), 1 },
		{ TranslationString("Airstrike", true), 2 }
	};

	ScrollStruct<int> CEOType[] = {
		{ TranslationString("Kick", true), 0 },
		{ TranslationString("Ban", true), 1 }
	};

	ScrollStruct<int> Rapes[] = {
		{ TranslationString("Rape Them", true), 0 },
		{ TranslationString("Rape You", true), 1 }
	};

	ScrollStruct<int> SendTo[] = {
		{ TranslationString("Island", true), 0 },
	};

	void DumpInfo(Menu::PlayerVars Player) {
		try {
			std::ofstream Output(std::string(Utils::GetConfig()->GetPlayerLogPath()) + Player.m_Name + ".json");

			nlohmann::json Json;
			Json["Name"] = Player.m_Name;

			if (Player.m_NetGamePlayer) {
				if (Player.m_NetGamePlayer->m_PlayerInfo) {
					Json["Internal IP"] = std::string(Utils::VA::VA("%i.%i.%i.%i", ((Player.m_NetGamePlayer->m_PlayerInfo->m_InternalIP & 0xFF000000) >> 24), ((Player.m_NetGamePlayer->m_PlayerInfo->m_InternalIP & 0xFF0000) >> 16), ((Player.m_NetGamePlayer->m_PlayerInfo->m_InternalIP & 0xFF00) >> 8), (Player.m_NetGamePlayer->m_PlayerInfo->m_InternalIP & 0xFF)));
					Json["Internal Port"] = std::to_string(Player.m_NetGamePlayer->m_PlayerInfo->m_InternalPort);

					Json["External IP"] = std::string(Utils::VA::VA("%i.%i.%i.%i", ((Player.m_NetGamePlayer->m_PlayerInfo->m_ExternalIP & 0xFF000000) >> 24), ((Player.m_NetGamePlayer->m_PlayerInfo->m_ExternalIP & 0xFF0000) >> 16), ((Player.m_NetGamePlayer->m_PlayerInfo->m_ExternalIP & 0xFF00) >> 8), (Player.m_NetGamePlayer->m_PlayerInfo->m_ExternalIP & 0xFF)));
					Json["External Port"] = std::to_string(Player.m_NetGamePlayer->m_PlayerInfo->m_ExternalPort);

					Json["Peer Token"] = std::string(Utils::VA::VA("0x%llx", Player.m_NetGamePlayer->m_PlayerInfo->m_HostToken));
					Json["Rockstar ID"] = std::string(Utils::VA::VA("%i", Player.m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID));
				}
			}

			Output << Json.dump(4);
			Output.close();
		} catch (std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}
	}

	void GiveWantedLevel(Menu::PlayerVars Player, int Level) {
		Native::ReportCrime(Player.m_ID, 8, Native::GetWantedLevelThreshold(Level));
	}

	void SetOnFire(Menu::PlayerVars Player) {
		if (!Player.m_InVehicle) {
			if (Native::GetEntitySpeed(Player.m_Ped) > 1.5f) {
				Math::Vector3<float> FVector = Native::GetEntityForwardVector(Player.m_Ped);

				for (int i = 1; i <= 3; i++) {
					Math::Vector3<float> Coords = Player.m_Coords + (FVector * i);
					Native::StartScriptFire(Coords.m_X, Coords.m_Y, Coords.m_Z, 5, true);
				}
			} else {
				Native::StartScriptFire(Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, 5, true);
			}
		}
	}

	void Explode(Menu::PlayerVars Player, int Type) {
		switch (Type) {
			case 0: // Normal
				Native::AddExplosion(Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, 29, 10000.f, true, false, 0.f, false);
				break;

			case 1: // Stealth
				Native::AddExplosion(Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, 29, 10000.f, false, true, 0.f, false);
				break;

			case 2: // Airstrike
				Menu::GetControlManager()->SimpleRequestWeaponAsset(0x47757124);
				Menu::GetControlManager()->SimpleRequestWeaponAsset(0xF8A3939F);
				
				if (Native::GetEntitySpeed(Player.m_Entity) > 1.5f) {
					Math::Vector3<float> FVector = Player.m_Coords + (Native::GetEntityForwardVector(Player.m_Entity) * 5.f);
					Native::ShootSingleBulletBetweenCoordsIgnoreEntity(FVector.m_X, FVector.m_Y, FVector.m_Z, FVector.m_X, FVector.m_Y, FVector.m_Z, Native::GetWeaponDamageType(0x47757124), false, 0x47757124, 0, true, false, 500.0f, Menu::GetLocalPlayer().m_Entity, 0);
					Native::ShootSingleBulletBetweenCoordsIgnoreEntity(FVector.m_X, FVector.m_Y, FVector.m_Z + 60.0f, FVector.m_X, FVector.m_Y, FVector.m_Z, Native::GetWeaponDamageType(0xF8A3939F), false, 0xF8A3939F, 0, true, false, 500.0f, Menu::GetLocalPlayer().m_Entity, 0);
				} else {
					Native::ShootSingleBulletBetweenCoordsIgnoreEntity(Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, Native::GetWeaponDamageType(0x47757124), false, 0x47757124, 0, true, false, 500.0f, Menu::GetLocalPlayer().m_Entity, 0);
					Native::ShootSingleBulletBetweenCoordsIgnoreEntity(Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z + 60.0f, Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, Native::GetWeaponDamageType(0xF8A3939F), false, 0xF8A3939F, 0, true, false, 500.0f, Menu::GetLocalPlayer().m_Entity, 0);
				}

				break;
		}
	}

	void CEOBan(Menu::PlayerVars Player) {
		Menu::Helpers::TriggerScriptEvent({ Rage::Global::BanFromCEO, Player.m_ID, 1 }, 3, Player.m_ID);
	}

	void CEOKick(Menu::PlayerVars Player) {
		Menu::Helpers::TriggerScriptEvent({ Rage::Global::KickFromCEO, Player.m_ID, 1, 5 }, 4, Player.m_ID);
	}

	void Blame(Menu::PlayerVars Player) {
		Menu::GetPlayerManager()->Foreach([=] (Menu::PlayerVars& P) {
			Native::AddOwnedExplosion(Player.m_Ped, P.m_Coords.m_X, P.m_Coords.m_Y, P.m_Coords.m_Z, 1, 10.f, true, false, 0.f);
		});
	}

	void Rape(Menu::PlayerVars Player) {
		Native::DetachEntity(Menu::GetLocalPlayer().m_Ped, true, true);
		// Native::ClearPedTasksImmediately(Menu::GetLocalPlayer().m_Ped);

		if (Player.m_Rape) {
			Menu::GetControlManager()->RequestAnimation("rcmpaparazzo_2", [=] {
				Native::AttachEntityToEntity(Menu::GetLocalPlayer().m_Ped, Player.m_Ped, -1, 0.f, Player.m_RapeType == 0 ? -0.3f : 0.3f, 0.f, 0.f, 0.f, 0.f, false, false, false, false, 0, false);
				Native::TaskPlayAnim(Menu::GetLocalPlayer().m_Ped, "rcmpaparazzo_2", Player.m_RapeType == 0 ? "shag_loop_a" : "shag_loop_poppy", 8.f, 0.f, -1, 1, 0, false, false, false);
			});
		}
	}

	void BlackScreen(Menu::PlayerVars Player) {
		Menu::Helpers::TriggerScriptEvent({ Rage::Global::ApartmentTeleport, Player.m_ID, 0, 0 }, 4, Player.m_ID);
	}

	void GlitchPhysics(Menu::PlayerVars Player) {
		Menu::GetControlManager()->RequestModel(0xd5c0bc07, [=] (uint32_t Model) { // prop_acc_guitar_01:0xd5c0bc07
			Object Guitar = Native::CreateObject(Model, Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, true, true, false);
			if (Native::DoesEntityExist(Guitar)) {
				Native::SetEntityVisible(Guitar, false, false);
				Native::AttachEntityToEntity(Guitar, Player.m_Ped, Native::GetPedBoneIndex(Player.m_Ped, 23553), 0.5f, -0.2f, 0.f, 0.f, 75.0f, 0.f, true, true, false, true, 1, true);
				Native::SetEntityCollision(Guitar, true, true);
				Native::SetModelAsNoLongerNeeded(Model);
			}
		});
	}

	void TazePlayer(Menu::PlayerVars Player) {
		Menu::GetControlManager()->RequestWeaponAsset(0x3656c8c1, [=] (uint32_t Hash) {
			Native::ShootSingleBulletBetweenCoordsIgnoreEntity(Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z + 1.f, Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, 10, 0, Hash, Menu::GetLocalPlayer().m_Ped, true, false, 100.f, Menu::GetLocalPlayer().m_Ped, 0);
		});
	}

	void KickFromVehiclePlayer(Menu::PlayerVars Player) {
		if (Player.m_InVehicle) {
			Menu::GetControlManager()->RequestControl(Player.m_Vehicle, [] (Entity) {}, true);
		}
	}

	void RagdollPlayer(Menu::PlayerVars Player) {
		Rage::Engine::SendRagdollEvent(Player.m_ID);
	}

	void RainRocketsPlayer(Menu::PlayerVars& Player) {
		Menu::Timers::RunTimedFunction(&Player.m_RainRocketsTimer, 750, [=] {
			Menu::GetControlManager()->RequestWeaponAsset(0x13579279, [=] (uint32_t Model) {
				Math::Vector3<float> StartPosition = Player.m_Coords;
				StartPosition.m_X += Native::GetRandomFloatInRange(-20.f, 20.f);
				StartPosition.m_Y += Native::GetRandomFloatInRange(-20.f, 20.f);

				Native::ShootSingleBulletBetweenCoordsIgnoreEntity(StartPosition.m_X, StartPosition.m_Y, StartPosition.m_Z + Native::GetRandomFloatInRange(50.f, 150.f), StartPosition.m_X, StartPosition.m_Y, StartPosition.m_Z, 250, 0, Model, Menu::GetLocalPlayer().m_Ped, true, false, 1000.f, Menu::GetLocalPlayer().m_Entity, 0);
			});
		});
	}

	void SendPlayerTo(Menu::PlayerVars Player) {
		switch (Player.m_SendToID) {
			case 0:	Menu::Helpers::TriggerScriptEvent({ Rage::Global::IslandTeleport, 20, 20, }, 3, Player.m_ID); break;
			default: break;
		}
	}
}

void NetworkPlayersAbusiveMenu::Init() {
	SetName("Abusive");
	SetParentSubmenu<NetworkPlayersSelectedMenu>();

	NetworkAbusiveExplosionsMenu::GetInstance();
	NetworkAbusiveAttackersMenu::GetInstance();

	addString("Custom Explosion");
	addString("Attackers");
	addString("Always Wanted");
	addString("Freeze");
	addString("Taze");
	addString("Ragdoll");
	addString("Kick from Vehicle");
	addString("Glitch Physics");
	addString("Forcefield");
	addString("Rain Rockets");
	addString("Blame");
	addString("Black Screen");
	addString("Set on Fire");
	addString("Explode");
	addString("CEO");
	addString("Rape");
	addString("Send To");
	addString("Break Freemode");
	addString("Block Passive");
	addString("Dump Info");
	addString("Add to Overseer");
}

void NetworkPlayersAbusiveMenu::Update() {}

/*Called once on submenu open*/
void NetworkPlayersAbusiveMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	if (Global::Vars::g_NetworkMenuType == SELECTED_PLAYER) {
		SetParentSubmenu<NetworkPlayersSelectedMenu>();

		addOption(SubmenuOption(getString("Custom Explosion"))
			.addSubmenu<NetworkAbusiveExplosionsMenu>());

		addOption(SubmenuOption(getString("Attackers"))
			.addSubmenu<NetworkAbusiveAttackersMenu>());

		addOption(ToggleOption(getString("Block Passive"))
			.addToggle(Menu::GetSelectedPlayer().m_BlockPassive)
			.addOnClick([]{
				if (Menu::GetSelectedPlayer().m_BlockPassive) {
					Menu::Helpers::TriggerScriptEvent({ Rage::Global::BlockPassive, 1, 1 }, 3, Menu::GetSelectedPlayer().m_ID);
				} else {
					Menu::Helpers::TriggerScriptEvent({ Rage::Global::BlockPassive, 2, 0 }, 3, Menu::GetSelectedPlayer().m_ID);
				}
			})
			.addTooltip("This will block them from being able to enable passive mode."));

		addOption(ToggleOption(getString("Always Wanted"))
			.addToggle(Menu::GetSelectedPlayer().m_AlwaysWanted)
			.addOnUpdate([](ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_AlwaysWanted); }));

		addOption(ToggleOption(getString("Freeze"))
			.addToggle(Menu::GetSelectedPlayer().m_DisableTasks)
			.addOnUpdate([](ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_DisableTasks); }));

		addOption(ToggleOption(getString("Taze"))
			.addToggle(Menu::GetSelectedPlayer().m_Taze)
			.addOnUpdate([](ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_Taze); })
			.addTooltip("You have to be near them"));

		addOption(ToggleOption(getString("Ragdoll"))
			.addToggle(Menu::GetSelectedPlayer().m_Ragdoll)
			.addOnUpdate([](ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_Ragdoll); }));

		addOption(ToggleOption(getString("Kick from Vehicle"))
			.addToggle(Menu::GetSelectedPlayer().m_KickFromVehicle)
			.addOnUpdate([](ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_KickFromVehicle); }));

		addOption(ToggleOption(getString("Glitch Physics"))
			.addToggle(Menu::GetSelectedPlayer().m_GlitchPhysics)
			.addOnUpdate([](ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_GlitchPhysics); }));

		addOption(ToggleOption(getString("Forcefield"))
			.addToggle(Menu::GetSelectedPlayer().m_Forcefield)
			.addOnUpdate([](ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_Forcefield); }));

		addOption(ToggleOption(getString("Rain Rockets"))
			.addToggle(Menu::GetSelectedPlayer().m_RainRockets)
			.addOnUpdate([](ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_RainRockets); }));

		addOption(ButtonOption(getString("Blame"))
			.addOnClick([] { Blame(Menu::GetSelectedPlayer()); }));

		addOption(ButtonOption(getString("Black Screen"))
			.addOnClick([] { BlackScreen(Menu::GetSelectedPlayer()); }));

		addOption(ButtonOption(getString("Set on Fire"))
			.addOnClick([] { SetOnFire(Menu::GetSelectedPlayer()); }));

		addOption(ButtonOption(getString("Add to Overseer"))
			.addTranslation()
			.addOnClick([] {
				if (IsValidPtr(Menu::GetSelectedPlayer().m_NetGamePlayer)) {
					if (IsValidPtr(Menu::GetSelectedPlayer().m_NetGamePlayer->m_PlayerInfo)) {
						NetworkOverseerMenuVars::Save(Menu::GetSelectedPlayer().m_Name, Menu::GetSelectedPlayer().m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID, "Manually Added");
					}
				}
			}));

		addOption(ButtonOption(getString("Dump Info"))
			.addTranslation()
			.addOnClick([] { DumpInfo(Menu::GetSelectedPlayer()); })
			.addTooltip("Dump players information to a file in the Ozark folder"));

		addOption(ScrollOption<int>(SCROLLSELECT, getString("Explode"))
			.addScroll(Menu::GetSelectedPlayer().m_Explode, 0, NUMOF(ExplosionType), ExplosionType)
			.addOnUpdate([](ScrollOption<int>* Option) { Option->addScroll(Menu::GetSelectedPlayer().m_Explode, 0, NUMOF(ExplosionType), ExplosionType); })
			.addOnClick([] { Explode(Menu::GetSelectedPlayer(), Menu::GetSelectedPlayer().m_Explode); }));

		addOption(ScrollOption<int>(SCROLLSELECT, getString("CEO"))
			.addScroll(Menu::GetSelectedPlayer().m_CEO, 0, NUMOF(CEOType), CEOType)
			.addOnClick([] { if (Menu::GetSelectedPlayer().m_CEO == 0) CEOKick(Menu::GetSelectedPlayer()); else CEOBan(Menu::GetSelectedPlayer()); }));

		addOption(ScrollOption<int>(SCROLLSELECT, getString("Send To"))
			.addScroll(Menu::GetSelectedPlayer().m_SendToID, 0, NUMOF(SendTo), SendTo)
			.addOnClick([] { SendPlayerTo(Menu::GetSelectedPlayer()); }));

		addOption(ScrollOption<int>(TOGGLE, getString("Rape"))
			.addToggle(Menu::GetSelectedPlayer().m_Rape)
			.addScroll(Menu::GetSelectedPlayer().m_RapeType, 0, NUMOF(Rapes), Rapes)
			.addOnClick([] { Rape(Menu::GetSelectedPlayer()); })
			.addOnUpdate([](ScrollOption<int>* Option) {
				Option->addToggle(Menu::GetSelectedPlayer().m_Rape).addScroll(Menu::GetSelectedPlayer().m_RapeType, 0, NUMOF(Rapes), Rapes);
			}).addRequirement([] { return Menu::GetSelectedPlayer().m_ID != Menu::GetLocalPlayer().m_ID; }));
	} else {
		SetParentSubmenu<NetworkSessionMenu>();

		addOption(SubmenuOption(getString("Custom Explosion"))
			.addSubmenu<NetworkAbusiveExplosionsMenu>());

		addOption(SubmenuOption(getString("Attackers"))
			.addSubmenu<NetworkAbusiveAttackersMenu>());

		addOption(ToggleOption(getString("Always Wanted"))
			.addToggle(m_Vars.m_AlwaysWanted));

		addOption(ToggleOption(getString("Freeze"))
			.addToggle(m_Vars.m_DisableTasks));

		addOption(ToggleOption(getString("Taze"))
			.addToggle(m_Vars.m_Taze)
			.addTooltip("You have to be near them"));

		addOption(ToggleOption(getString("Ragdoll"))
			.addToggle(m_Vars.m_Ragdoll));

		addOption(ToggleOption(getString("Kick from Vehicle"))
			.addToggle(m_Vars.m_KickFromVehicle));

		addOption(ToggleOption(getString("Glitch Physics"))
			.addToggle(m_Vars.m_GlitchPhysics));

		addOption(ToggleOption(getString("Forcefield"))
			.addToggle(m_Vars.m_Forcefield));

		addOption(ToggleOption(getString("Rain Rockets"))
			.addToggle(m_Vars.m_RainRockets));

		addOption(ButtonOption(getString("Black Screen"))
			.addOnClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					BlackScreen(Player);
				});
			}));

		addOption(ButtonOption(getString("Set on Fire"))
			.addOnClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					SetOnFire(Player);
				});
			}));

		addOption(ButtonOption(getString("Dump Info"))
			.addTranslation()
			.addOnClick([] {
				Menu::GetPlayerManager()->Foreach([&](Menu::PlayerVars& Player) {
					NetworkPlayersAbusiveMenuVars::DumpInfo(Player);
				});
			})
			.addTooltip("Dump players information to a file in the Ozark folder"));

		addOption(ScrollOption<int>(SCROLLSELECT, getString("Explode"))
			.addScroll(m_Vars.m_Explode, 0, NUMOF(ExplosionType), ExplosionType)
			.addOnClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					Explode(Player, m_Vars.m_Explode);
				});
			}));

		addOption(ScrollOption<int>(SCROLLSELECT, getString("CEO"))
			.addScroll(m_Vars.m_CEO, 0, NUMOF(CEOType), CEOType)
			.addOnClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					if (m_Vars.m_CEO == 0) CEOKick(Player); else CEOBan(Player);
				});
			}));
	}
}

/*Called always*/
void NetworkPlayersAbusiveMenu::FeatureUpdate() {
	Menu::GetPlayerManager()->Foreach([] (Menu::PlayerVars& Player) {
		if (Player.m_AlwaysWanted || m_Vars.m_AlwaysWanted) {
			GiveWantedLevel(Player, 5);
		}

		if (Player.m_DisableTasks || m_Vars.m_DisableTasks) {
			Native::ClearPedTasksImmediately(Player.m_Ped);
		}

		if (Player.m_Taze || m_Vars.m_Taze) {
			Menu::Timers::RunTimedFunction(&Player.m_TazeTimer, 5000, [=] {
				TazePlayer(Player);
			});
		}

		if (Player.m_KickFromVehicle || m_Vars.m_KickFromVehicle) {
			Menu::Timers::RunTimedFunction(&Player.m_KickFromVehicleTimer, 1000, [=] {
				KickFromVehiclePlayer(Player);
			});
		}

		if (Player.m_GlitchPhysics || m_Vars.m_GlitchPhysics) {
			Menu::Timers::RunTimedFunction(&Player.m_GlitchPhysicsTimer, 1000, [=] {
				Object Guitar = Native::GetClosestObjectOfType(Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, 5.f, 0xd5c0bc07, false, false, false);
				if (!Guitar || !Native::DoesEntityExist(Guitar) || !Native::IsEntityAttachedToEntity(Guitar, Player.m_Ped)) {
					GlitchPhysics(Player);
				}
			});
		}

		if (Player.m_Ragdoll || m_Vars.m_Ragdoll) {
			Menu::Timers::RunTimedFunction(&Player.m_RagdollTimer, 1000, [=] {
				RagdollPlayer(Player);
			});
		}

		if (Player.m_RainRockets || m_Vars.m_RainRockets) {
			RainRocketsPlayer(Player);
		}

		if (Player.m_Forcefield || m_Vars.m_Forcefield) {
			Native::AddExplosion(Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, 29, 100.f, false, true, 0.f, true);
		}
	}, true);
}

NetworkPlayersAbusiveMenu* _instance;
NetworkPlayersAbusiveMenu* NetworkPlayersAbusiveMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkPlayersAbusiveMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkPlayersAbusiveMenu::~NetworkPlayersAbusiveMenu() { delete _instance; }