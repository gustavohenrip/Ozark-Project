#include "hooks.hpp"
#include "menu/base/util/notify.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/player_manager.hpp"
#include "utils/va.hpp"
#include "menu/base/submenus/main/vehicle/spawner/vehicle_spawner_manage.hpp"
#include "menu/base/submenus/main/npc/spawner/ped_spawner_manage.hpp"
#include "menu/base/submenus/main/object/spawner/object_spawner_manage.hpp"
#include "utils/log.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_game_events.hpp"
#include "menu/base/submenus/main/network/network_protection.hpp"
#include "menu/base/util/helpers.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_entity_limits.hpp"
#include "global/arrays.hpp"
#include "utils/fiber_pool.hpp"
#include "menu/base/submenus/main.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_entity_blocking.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_game_events.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_notifications.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_entity_events.hpp"
#include "utils/caller.hpp"
#include "utils/memory/memory.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_event_reactions.hpp"
#include "utils/utils.hpp"

TranslationString T_OwnershipMigrationCurrentVehicle("Ownership Migration - Current Vehicle", true, true);
TranslationString T_OwnershipMigrationSpawnedVehicle("Ownership Migration - Spawned Vehicle", true, true);
TranslationString T_OwnershipMigrationSpawnedNPC("Ownership Migration - Spawned NPC", true, true);
TranslationString T_OwnershipMigrationSpawnedObject("Ownership Migration - Spawned Object", true, true);
TranslationString T_CrashModel("Crash Model", true, true);
TranslationString T_QuantumCrash("Quantum Crash", true, true);
TranslationString T_Clone("Clone", true, true);

bool Menu::Hooks::ReceiveCloneCreateHook(uint64_t This, Rage::Network::NetGamePlayer* NetGamePlayer, uint64_t r8, uint16_t CloneType, uint16_t NetworkID, uint16_t Flag, Rage::BitBuffer* BitBuffer, int Timestamp) {
	if (CloneType == NetObjectPickupPlacement) return false;
	
	auto IsCrashModel = [] (uint32_t Model) {
		switch (Model) {
			case 0x3F039CBA:
			case 0x856CFB02:
			case 0x2D7030F3:
			case 0x0703F106:
			case 0x431d501c:
			case 0x1CFC0E72:
			case 0x1E15B3EC:
			case 0x1CB5C0C3:
				return true;
		}

		return false;
	};

	auto IsValid = [=] {
		return NetGamePlayer && (NetGamePlayer->m_ID >= 0 && NetGamePlayer->m_ID <= 34);
	};

	if (NetworkProtectionEntityBlockingMenuVars::m_Vars.m_Toggled[CloneType]) {
		return false;
	}

	if (NetworkProtectionEntityEventsMenuVars::m_Vars.m_DisableModderCreate) {
		if (IsValid()) {
			if (Menu::GetPlayer(NetGamePlayer->m_ID).m_IsModder) {
				return false;
			}
		}
	}

	if (IsValid()) {
		if (Menu::GetPlayer(NetGamePlayer->m_ID).m_BlockEntities[CloneType]) {
			return false;
		}

		if (Menu::GetPlayer(NetGamePlayer->m_ID).m_BlacklistRequests > GetTickCount()) {
			return false;
		}
	}

	Rage::Network::SyncTree* SyncTree = Rage::Engine::GetSyncTreeFromCloneType(CloneType);
	if (!SyncTree) {
		if (IsValid()) {
			LOG_INFO("Preventing %s creating due to bad sync tree", NetGamePlayer->m_PlayerInfo->m_Name);
		}

		return false;
	}

	if (SyncTree->m_DataNodeCount == 0 || SyncTree->m_DataNodeCount > 100) {
		return false;
	}

	Rage::BitBuffer BitBufferCache;
	memcpy(&BitBufferCache, BitBuffer, sizeof(BitBufferCache));

	Rage::Network::NetObj* Object = Rage::Engine::GetNetworkObjectFromNetworkID(This, NetworkID, true);
	if (Object) {
		if (Object->m_Owner == Menu::GetLocalPlayer().m_ID) {
			if (Object->m_Entity) {
				uint32_t Handle = Rage::Engine::GetEntityHandleFromAddress(Object->m_Entity);
				if (Handle) {
					auto ProtectVar = NetworkProtectionGameEventsMenuVars::m_Vars.m_SpawnedEntityOwnerMigration;

					// Handle checks for notifying
					bool Notified = false;
					if (Menu::GetLocalPlayer().m_InVehicle) {
						if (Handle == Menu::GetLocalPlayer().m_Vehicle) {
							if (ProtectVar.m_Notify) {
								if (IsValid()) {
									Notified = true;
									Menu::GetNotify()->NotifyStackedProtectionEvent(NetGamePlayer->m_PlayerInfo->m_Name, TRANSLATE(T_OwnershipMigrationCurrentVehicle), false, ProtectVar.m_Block);
								}
							}

							if (ProtectVar.m_Block) {
								return false;
							}
						}
					}

					if (!Notified) {
						if (VehicleSpawnerManageMenuVars::m_Vars.m_SpawnedVehicles.size()) {
							for (auto& Veh : VehicleSpawnerManageMenuVars::m_Vars.m_SpawnedVehicles) {
								if (Handle == Veh.m_Vehicle) {
									if (ProtectVar.m_Notify) {
										if (IsValid()) {
											Menu::GetNotify()->NotifyStackedProtectionEvent(NetGamePlayer->m_PlayerInfo->m_Name, TRANSLATE(T_OwnershipMigrationSpawnedVehicle), false, ProtectVar.m_Block);
										}
									}

									if (ProtectVar.m_Block) {
										return false;
									}
								}
							}
						}
					}

					if (PedSpawnerManageMenuVars::m_Vars.m_SpawnedPeds.size()) {
						for (auto& Veh : PedSpawnerManageMenuVars::m_Vars.m_SpawnedPeds) {
							if (Handle == Veh.m_Handle) {
								if (ProtectVar.m_Notify) {
									if (IsValid()) {
										Menu::GetNotify()->NotifyStackedProtectionEvent(NetGamePlayer->m_PlayerInfo->m_Name, TRANSLATE(T_OwnershipMigrationSpawnedNPC), false, ProtectVar.m_Block);
									}
								}

								if (ProtectVar.m_Block) {
									return false;
								}
							}
						}
					}

					if (ObjectSpawnerManageMenuVars::m_Vars.m_SpawnedObjects.size()) {
						for (auto& Veh : ObjectSpawnerManageMenuVars::m_Vars.m_SpawnedObjects) {
							if (Handle == Veh.m_Handle) {
								if (ProtectVar.m_Notify) {
									if (IsValid()) {
										Menu::GetNotify()->NotifyStackedProtectionEvent(NetGamePlayer->m_PlayerInfo->m_Name, TRANSLATE(T_OwnershipMigrationSpawnedObject), false, ProtectVar.m_Block);
									}
								}

								if (ProtectVar.m_Block) {
									return false;
								}
							}
						}
					}
				}
			}
		}
	}

	std::string Name = "Invalid";
	uint32_t ModelHash = 0;
	Menu::Helpers::GetModelHashFromNode(SyncTree->m_ParentNode, BitBuffer, &ModelHash, &Name);

	if (ModelHash == 0) {
		LOG_WARN("Failed to find entity creation model hash - %i", CloneType);
		return false;
	}

	if (IsCrashModel(ModelHash)) {
		if (NetworkProtectionNotificationsMenuVars::m_Vars.m_Crash) {
			if (IsValid()) {
				Menu::GetPlayer(NetGamePlayer->m_ID).m_BlacklistRequests = GetTickCount() + 15000;
				Menu::GetPlayer(NetGamePlayer->m_ID).m_IsModder = true;
				Menu::GetPlayer(NetGamePlayer->m_ID).m_ModderFlags[Menu::REASON_CRASH_ATTEMPT] = true;
				NetworkProtectionEventReactionsMenuVars::ProcessEvent(NetworkProtectionEventReactionsMenuVars::EVENT_CRASH_ATTEMPT, NetGamePlayer->m_ID);

				Menu::GetNotify()->NotifyStackedProtectionEvent(NetGamePlayer->m_PlayerInfo->m_Name, Utils::VA::VA("%s (%X)", TRANSLATE(T_CrashModel), ModelHash), false, true);
			}
		}

		return false;
	}

	uint64_t ModelInfo = Rage::Engine::GetModelInfo(ModelHash);
	if (ModelInfo) {
		int ModelType = (int)(*(uint8_t*)(ModelInfo + 0x9D) & 0x1F);
		int VehicleType = *(int*)(ModelInfo + 0x340);

		static std::unordered_map<int, uint16_t> TypeTranslations = {
			{ 0, NetObjectAutomobile }, // car
			{ 1, NetObjectPlane },      // plane
			{ 2, NetObjectTrailer },    // trailer
			{ 3, NetObjectAutomobile }, // quadBike
			{ 5, NetObjectAutomobile }, // submarineCar
			{ 6, NetObjectAutomobile }, // amphibiousAutomobile
			{ 7, NetObjectAutomobile }, // amphQuadbike
			{ 8, NetObjectHeli },       // heli
			{ 9, NetObjectHeli },       // blimp
			{ 10, NetObjectHeli },      // autoGyro (perhaps skylift?)
			{ 11, NetObjectBike },      // bike
			{ 12, NetObjectBike },      // bicycle
			{ 13, NetObjectBoat },      // boat
			{ 14, NetObjectTrain },     // train
			{ 15, NetObjectSubmarine }, // submarine
		};

		static std::unordered_map<eNetObjectTypes, std::pair<std::vector<int>, bool>> Translations = {
			{ NetObjectAutomobile, { { 5 }, true } },
			{ NetObjectBike, { { 5 }, true } },
			{ NetObjectBoat, { { 5 }, true } },
			{ NetObjectDoor, { { 1 }, false } },
			{ NetObjectHeli, { { 5 }, true } },
			{ NetObjectObject, { { 1, 4 }, false } },
			{ NetObjectPed, { { 6 }, false } },
			{ NetObjectPickup, { { 1 }, false } },
			{ NetObjectPickupPlacement, { { 1 }, false } },
			{ NetObjectPlane, { { 5 }, true } },
			{ NetObjectSubmarine, { { 5 }, true } },
			{ NetObjectPlayer, { { 6 }, false } },
			{ NetObjectTrailer, { { 5 }, true } },
			{ NetObjectTrain, { { 5 }, true } },
		};

		auto Vit = Translations.find((eNetObjectTypes)CloneType);
		if (Vit == end(Translations)) {
			return false;
		}

		bool ValidType = false;
		for (int Yes : Vit->second.first) {
			if (Yes == ModelType) {
				ValidType = true;
				break;
			}
		}

		if (!ValidType || (Vit->second.second && TypeTranslations[VehicleType] != CloneType)) {
			if (IsValid()) {
				Menu::GetPlayer(NetGamePlayer->m_ID).m_BlacklistRequests = GetTickCount() + 15000;
				Menu::GetPlayer(NetGamePlayer->m_ID).m_IsModder = true;
				Menu::GetPlayer(NetGamePlayer->m_ID).m_ModderFlags[Menu::REASON_CRASH_ATTEMPT] = true;
				NetworkProtectionEventReactionsMenuVars::ProcessEvent(NetworkProtectionEventReactionsMenuVars::EVENT_CRASH_ATTEMPT, NetGamePlayer->m_ID);

				if (NetworkProtectionNotificationsMenuVars::m_Vars.m_Crash) {
					Menu::GetNotify()->NotifyStackedProtectionEvent(NetGamePlayer->m_PlayerInfo->m_Name, TRANSLATE(T_QuantumCrash), false, true);
				}
			}

			return false;
		}
	} else {
		return false;
	}

	if (IsValid()) {
		if (_byteswap_ulong(*(uint32_t*)((uint64_t)BitBuffer + 0x20)) == 0x1DC17987 && CloneType == NetObjectPed) {
			if (NetworkProtectionGameEventsMenuVars::m_Vars.m_Clone.m_Notify) {
				Menu::GetNotify()->NotifyStackedProtectionEvent(NetGamePlayer->m_PlayerInfo->m_Name, TRANSLATE(T_Clone), NetworkProtectionGameEventsMenuVars::m_Vars.m_Clone.m_Redirect, NetworkProtectionGameEventsMenuVars::m_Vars.m_Clone.m_Block);
			}

			if (NetworkProtectionGameEventsMenuVars::m_Vars.m_Clone.m_Redirect) {
				Utils::GetFiberPool()->Push([=] {
					Native::ClonePed(Menu::GetPlayer(NetGamePlayer->m_ID).m_Ped, 0.f, true, true);
				});
			}

			if (NetworkProtectionGameEventsMenuVars::m_Vars.m_Clone.m_Block) {
				return false;
			}
		}

		auto& ProtectVars = NetworkProtectionEntityLimitsMenuVars::m_Vars;
		if (ProtectVars.m_Limits.size()) {
			auto& Vit = std::find_if(ProtectVars.m_Limits.begin(), ProtectVars.m_Limits.end(), [=] (NetworkProtectionEntityLimitsMenuVars::EntityLimit& Limit) {
				return Limit.m_Model == ModelHash;
			});

			if (Vit != ProtectVars.m_Limits.end()) {
				if (Vit->m_Enabled) {
					if ((GetTickCount() - Vit->m_LastSpawned) > (uint32_t)Global::Arrays::g_Timers[Vit->m_Delay].m_Result) {
						Vit->m_LastSpawned = GetTickCount();
					} else {
						if (IsValid()) {
							LOG_INFO("Preventing %s creating %X due to entity limit", NetGamePlayer->m_PlayerInfo->m_Name, ModelHash);
						}

						return false;
					}
				}
			}
		}
	}

	if (CloneType == NetObjectObject) {
		if (NetworkProtectionGameEventsMenuVars::m_Vars.m_Cage) {
			static uint32_t Cages[] = {
				2063962179, 959275690, 1396140175, 2968924808, 2931248133, 1098812088, 379820688, 1692612370,
				2946368461, 1082648418, 3561133533, 3825864590, 3357950520, 4121926986, 109264625, 3955926036, 1181661112,
				joaat("prop_container_01a"), joaat("prop_container_01b"), joaat("prop_container_01c"),
				joaat("prop_container_01d"), joaat("prop_container_01e"), joaat("prop_container_01f"),
				joaat("prop_container_01g"), joaat("prop_container_01h"), joaat("prop_container_01mb"),
				joaat("prop_container_02a"), joaat("prop_container_03a"), joaat("prop_container_03b"),
				joaat("prop_container_03mb"), joaat("prop_container_03_ld"), joaat("prop_container_04a"),
				joaat("prop_container_04mb"), joaat("prop_container_05mb"), joaat("prop_container_door_mb_l"),
				joaat("prop_container_door_mb_r"), joaat("prop_container_hole"), joaat("prop_container_ld"),
				joaat("prop_container_ld2"), joaat("prop_container_old1"), joaat("prop_container_ld"),
				joaat("prop_container_ld2"), joaat("prop_container_old1"), joaat("prop_dog_cage_02"),
				joaat("prop_dog_cage_01"), joaat("prop_dog_cage_02"), joaat("prop_conslift_cage"),
				joaat("prop_rub_cage01a"), joaat("prop_gold_cont_01b"), joaat("prop_gold_trolly"),
				joaat("prop_gold_trolly_full"), joaat("prop_gold_trolly_strap_01"), joaat("prop_cctv_cont_01"),
				joaat("prop_cctv_cont_03"), joaat("prop_cctv_cont_04"), joaat("prop_cctv_cont_05"),
				joaat("prop_cctv_cont_06"), joaat("prop_container_01a"), joaat("prop_container_01b"),
				joaat("prop_container_01c"), joaat("prop_container_01d"), joaat("prop_container_01e"),
				joaat("prop_container_01f"), joaat("prop_container_01g"), joaat("prop_container_01h"),
				joaat("prop_container_01mb"), joaat("prop_container_02a"), joaat("prop_container_03a"),
				joaat("prop_container_03b"), joaat("prop_container_03mb"), joaat("prop_container_03_ld"),
				joaat("prop_container_04a"), joaat("prop_container_04mb"), joaat("prop_container_05mb"),
				joaat("prop_container_door_mb_l"), joaat("prop_container_door_mb_r"), joaat("prop_container_hole"),
				joaat("prop_container_ld"), joaat("prop_container_ld2"), joaat("prop_container_old1"),
				joaat("prop_contnr_pile_01a"), joaat("prop_controller_01"), joaat("prop_control_rm_door_01"),
				joaat("prop_cont_chiller_01"), joaat("prop_container_hole"),joaat("prop_cont_chiller_01"),
				0xC6C3242D, joaat("prop_beach_fire"), 0xA2023E64, 2799454049, 3482883616, 564151899, 710268902,
				3284981583, 3500568834, 1060884015, 4248442390, 4256736313, 2693815128, 1925435073, 682074297,
				3763106032, 621481054, 2908189926, 3763623269
			};

			for (uint32_t Model : Cages) {
				if (ModelHash == Model) {
					return false;
				}
			}
		}
	}

	if (*(uint8_t*)(Global::Vars::g_FunctionPatches[0].m_Address) == Global::Vars::g_FunctionPatches[0].m_Patch[0]) {
		Memory::WriteVector(Global::Vars::g_FunctionPatches[0].m_Address, Global::Vars::g_FunctionPatches[0].m_Original);
		Memory::WriteVector(Global::Vars::g_FunctionPatches[1].m_Address, Global::Vars::g_FunctionPatches[1].m_Original);
	}

	memcpy(BitBuffer, &BitBufferCache, sizeof(BitBufferCache));
	Global::Vars::g_BrokenNodeSenders[0] = NetGamePlayer;
	return OriginalReceiveCloneCreate(This, NetGamePlayer, r8, CloneType, NetworkID, Flag, BitBuffer, Timestamp);
}