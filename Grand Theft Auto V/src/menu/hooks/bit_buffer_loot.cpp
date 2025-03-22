#include "hooks.hpp"
#include "utils/log.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_notifications.hpp"
#include "utils/fiber_pool.hpp"
#include "utils/memory/memory.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/submenus/main.hpp"
#include "menu/base/util/player_manager.hpp"
#include "menu/base/util/helpers.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_notifications.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_game_events.hpp"
#include "rage/engine.hpp"
#include "utils/utils.hpp"

TranslationString T_CreateMisalign("Create Misalign", true, true);
TranslationString T_SyncMisalign("Sync Misalign", true, true);

bool Menu::Hooks::ReadUnsignedIntFromBitBufferHook(Rage::BitBufferSyncInstance* This, uint32_t* Output, int Count) {
	if (This->m_Buffer->m_Unk1 == 1234) {
		*Output = 0;
		return false;
	}

	bool Return = OriginalReadUnsignedIntFromBitBuffer(This, Output, Count);
	if (!Return) {
		if (Global::Vars::g_WriteArrayThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_WriteArrayThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_WriteArrayThreadInfo.m_Block = true;
			}
		}

		if (Global::Vars::g_ReadNetworkEventThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_ReadNetworkEventThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_ReadNetworkEventThreadInfo.m_Block = true;
			}
		}

		if (This->m_Buffer->m_MaxBit != 0) LOG_DEV("[%s] Failed! %s", __FUNCTION__, Utils::GetStackTrace().c_str());
		This->m_Buffer->m_Unk1 = 1234;
		return false;
	}

	uint64_t ReturnAddress = (uint64_t)_ReturnAddress();

	if (ReturnAddress == Global::Vars::g_NodeFixCDynamicEntityGameStateDataNode && *Output > 11) {
		LOG_CUSTOM_DEV("ReadUintToDataReaderBuffer", "CDynamicEntityGameStateDataNode was bad %i", *Output);
		*Output = 0;
	}

	if (ReturnAddress == Global::Vars::g_NodeFixCVehicleGadgetDataNode && *Output > 2) {
		LOG_CUSTOM_DEV("ReadUintToDataReaderBuffer", "CVehicleGadgetDataNode was bad %i", *Output);
		*Output = 0;
	}

	if (ReturnAddress == Global::Vars::g_NodeFixCVehicleHealthDataNode && *Output > 10) {
		LOG_CUSTOM_DEV("ReadUintToDataReaderBuffer", "CVehicleHealthDataNode was bad %i", *Output);
		*Output = 0;
	}

	if (ReturnAddress == Global::Vars::g_NodeFixCVehicleComponentReservationDataNode && *Output > 32) {
		LOG_CUSTOM_DEV("ReadUintToDataReaderBuffer", "CVehicleComponentReservationDataNode was bad %i", *Output);
		*Output = 0;
	}

	if (ReturnAddress == Global::Vars::g_NodeFixCPedGameStateDataNode && *Output > 11) {
		LOG_CUSTOM_DEV("ReadUintToDataReaderBuffer", "CPedGameStateDataNode was bad %i", *Output);
		*Output = 0;
	}

	if (ReturnAddress == Global::Vars::g_NodeFixCPedGameStateDataNode + 0x5E && *Output > 3) {
		LOG_CUSTOM_DEV("ReadUintToDataReaderBuffer", "CPedGameStateDataNode was bad %i", *Output);
		*Output = 0;
	}

	if (ReturnAddress == Global::Vars::g_NodeFixCPedComponentReservationDataNode && *Output > 32) {
		LOG_CUSTOM_DEV("ReadUintToDataReaderBuffer", "CPedComponentReservationDataNode was bad %i", *Output);
		*Output = 0;
	}

	if (ReturnAddress == Global::Vars::g_NodeFixCPedScriptGameStateDataNode && *Output > 30) {
		LOG_CUSTOM_DEV("ReadUintToDataReaderBuffer", "CPedScriptGameStateDataNode was bad %i", *Output);
		*Output = 0;
	}

	if (ReturnAddress == Global::Vars::g_NodeFixCPedTaskTreeDataNode && *Output > 3) {
		LOG_CUSTOM_DEV("ReadUintToDataReaderBuffer", "CPedTaskTreeDataNode was bad %i", *Output);
		*Output = 0;
	}

	if (ReturnAddress == Global::Vars::g_NodeFixCPedInventoryDataNode && *Output > 105) {
		LOG_CUSTOM_DEV("ReadUintToDataReaderBuffer", "CPedInventoryDataNode was bad %i", *Output);
		*Output = 0;
	}

	if (ReturnAddress == Global::Vars::g_NodeFixCPedInventoryDataNode + 0x4D && *Output > 65) {
		LOG_CUSTOM_DEV("ReadUintToDataReaderBuffer", "CPedInventoryDataNode was bad %i", *Output);
		*Output = 0;
	}

	if (ReturnAddress == Global::Vars::g_NodeFixCPedTaskSequenceDataNode && *Output > 10) {
		LOG_CUSTOM_DEV("ReadUintToDataReaderBuffer", "CPedTaskSequenceDataNode was bad %i", *Output);
		*Output = 0;
	}

	if (ReturnAddress == Global::Vars::g_NodeFixCPlayerGameStateDataNode && *Output > 3) {
		LOG_CUSTOM_DEV("ReadUintToDataReaderBuffer", "CPlayerGameStateDataNode was bad %i", *Output);
		*Output = 0;
	}

	if (ReturnAddress == Global::Vars::g_NodeFixCPickupCreationDataNode && *Output > 11) {
		LOG_CUSTOM_DEV("ReadUintToDataReaderBuffer", "CPickupCreationDataNode was bad %i", *Output);
		*Output = 0;
	}

	if (ReturnAddress == Global::Vars::g_NodeFixCVehicleControlDataNode && *Output > 10) {
		LOG_CUSTOM_DEV("ReadUintToDataReaderBuffer", "CVehicleControlDataNode was bad %i", *Output);
		*Output = 0;
	}

	if (ReturnAddress == Global::Vars::g_NodeFixCVehicleProximityMigrationDataNode && *Output > 16) {
		LOG_CUSTOM_DEV("ReadUintToDataReaderBuffer", "CVehicleProximityMigrationDataNode was bad %i", *Output);
		*Output = 0;
	}

	return Return;
}

bool Menu::Hooks::ReadUnsignedShortFromBitBufferHook(Rage::BitBufferSyncInstance* This, uint16_t* Output, int Count) {
	if (This->m_Buffer->m_Unk1 == 1234) {
		*Output = 0;
		return false;
	}

	bool Return = OriginalReadUnsignedShortFromBitBuffer(This, Output, Count);
	if (!Return) {
		if (Global::Vars::g_WriteArrayThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_WriteArrayThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_WriteArrayThreadInfo.m_Block = true;
			}
		}

		if (Global::Vars::g_ReadNetworkEventThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_ReadNetworkEventThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_ReadNetworkEventThreadInfo.m_Block = true;
			}
		}

		if (This->m_Buffer->m_MaxBit != 0) LOG_DEV("[%s] Failed! %s", __FUNCTION__, Utils::GetStackTrace().c_str());
		This->m_Buffer->m_Unk1 = 1234;
		return false;
	}

	return Return;
}

bool Menu::Hooks::ReadUnsignedCharFromBitBufferHook(Rage::BitBufferSyncInstance* This, uint8_t* Output, int Count) {
	if (This->m_Buffer->m_Unk1 == 1234) {
		*Output = 0;
		return false;
	}

	bool Return = OriginalReadUnsignedCharFromBitBuffer(This, Output, Count);
	if (!Return) {
		if (Global::Vars::g_WriteArrayThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_WriteArrayThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_WriteArrayThreadInfo.m_Block = true;
			}
		}

		if (Global::Vars::g_ReadNetworkEventThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_ReadNetworkEventThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_ReadNetworkEventThreadInfo.m_Block = true;
			}
		}

		if (This->m_Buffer->m_MaxBit != 0) LOG_DEV("[%s] Failed! %s", __FUNCTION__, Utils::GetStackTrace().c_str());
		This->m_Buffer->m_Unk1 = 1234;
		return false;
	}

	return Return;
}

bool Menu::Hooks::ReadIntFromBitBufferHook(Rage::BitBufferSyncInstance* This, int* Output, int Count) {
	if (This->m_Buffer->m_Unk1 == 1234) {
		*Output = 0;
		return false;
	}

	bool Return = OriginalReadIntFromBitBuffer(This, Output, Count);
	if (!Return) {
		if (Global::Vars::g_WriteArrayThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_WriteArrayThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_WriteArrayThreadInfo.m_Block = true;
			}
		}

		if (Global::Vars::g_ReadNetworkEventThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_ReadNetworkEventThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_ReadNetworkEventThreadInfo.m_Block = true;
			}
		}

		if (This->m_Buffer->m_MaxBit != 0) LOG_DEV("[%s] Failed! %s", __FUNCTION__, Utils::GetStackTrace().c_str());
		This->m_Buffer->m_Unk1 = 1234;
		return false;
	}

	return Return;
}

bool Menu::Hooks::ReadShortFromBitBufferHook(Rage::BitBufferSyncInstance* This, short* Output, int Count) {
	if (This->m_Buffer->m_Unk1 == 1234) {
		*Output = 0;
		return false;
	}

	bool Return = OriginalReadShortFromBitBuffer(This, Output, Count);
	if (!Return) {
		if (Global::Vars::g_WriteArrayThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_WriteArrayThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_WriteArrayThreadInfo.m_Block = true;
			}
		}

		if (Global::Vars::g_ReadNetworkEventThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_ReadNetworkEventThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_ReadNetworkEventThreadInfo.m_Block = true;
			}
		}

		if (This->m_Buffer->m_MaxBit != 0) LOG_DEV("[%s] Failed! %s", __FUNCTION__, Utils::GetStackTrace().c_str());
		This->m_Buffer->m_Unk1 = 1234;
		return false;
	}

	return Return;
}

bool Menu::Hooks::ReadCharFromBitBufferHook(Rage::BitBufferSyncInstance* This, char* Output, int Count) {
	if (This->m_Buffer->m_Unk1 == 1234) {
		*Output = 0;
		return false;
	}

	bool Return = OriginalReadCharFromBitBuffer(This, Output, Count);
	if (!Return) {
		if (Global::Vars::g_WriteArrayThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_WriteArrayThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_WriteArrayThreadInfo.m_Block = true;
			}
		}

		if (Global::Vars::g_ReadNetworkEventThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_ReadNetworkEventThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_ReadNetworkEventThreadInfo.m_Block = true;
			}
		}

		if (This->m_Buffer->m_MaxBit != 0) LOG_DEV("[%s] Failed! %s", __FUNCTION__, Utils::GetStackTrace().c_str());
		This->m_Buffer->m_Unk1 = 1234;
		return false;
	}

	return Return;
}

bool Menu::Hooks::ReadBoolFromBitBufferHook(Rage::BitBufferSyncInstance* This, bool* Output) {
	if (This->m_Buffer->m_Unk1 == 1234) {
		*Output = 0;
		return false;
	}

	bool Return = OriginalReadBoolFromBitBuffer(This, Output);
	if (!Return) {
		if (Global::Vars::g_WriteArrayThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_WriteArrayThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_WriteArrayThreadInfo.m_Block = true;
			}
		}

		if (Global::Vars::g_ReadNetworkEventThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_ReadNetworkEventThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_ReadNetworkEventThreadInfo.m_Block = true;
			}
		}

		if (This->m_Buffer->m_MaxBit != 0) LOG_DEV("[%s] Failed! %s", __FUNCTION__, Utils::GetStackTrace().c_str());
		This->m_Buffer->m_Unk1 = 1234;
		return false;
	}

	return Return;
}

bool Menu::Hooks::ReadLongLongFromBitBufferHook(Rage::BitBufferSyncInstance* This, long long* Output, int Count) {
	if (This->m_Buffer->m_Unk1 == 1234) {
		*Output = 0;
		return false;
	}

	bool Return = OriginalReadLongLongFromBitBuffer(This, Output, Count);
	if (!Return) {
		if (Global::Vars::g_WriteArrayThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_WriteArrayThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_WriteArrayThreadInfo.m_Block = true;
			}
		}

		if (Global::Vars::g_ReadNetworkEventThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_ReadNetworkEventThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_ReadNetworkEventThreadInfo.m_Block = true;
			}
		}

		if (This->m_Buffer->m_MaxBit != 0) LOG_DEV("[%s] Failed! %s", __FUNCTION__, Utils::GetStackTrace().c_str());
		This->m_Buffer->m_Unk1 = 1234;
		return false;
	}

	return Return;
}

bool Menu::Hooks::ReadUnsignedLongLongFromBitBufferHook(Rage::BitBufferSyncInstance* This, uint64_t* Output, int Count) {
	if (This->m_Buffer->m_Unk1 == 1234) {
		*Output = 0;
		return false;
	}

	bool Return = OriginalReadUnsignedLongLongFromBitBuffer(This, Output, Count);
	if (!Return) {
		if (Global::Vars::g_WriteArrayThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_WriteArrayThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_WriteArrayThreadInfo.m_Block = true;
			}
		}

		if (Global::Vars::g_ReadNetworkEventThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_ReadNetworkEventThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_ReadNetworkEventThreadInfo.m_Block = true;
			}
		}

		if (This->m_Buffer->m_MaxBit != 0) LOG_DEV("[%s] Failed! %s", __FUNCTION__, Utils::GetStackTrace().c_str());
		This->m_Buffer->m_Unk1 = 1234;
		return false;
	}

	return Return;
}

bool Menu::Hooks::ReadFloatFromBitBufferHook(Rage::BitBufferSyncInstance* This, float* Output, float Mod, int Count) {
	if (This->m_Buffer->m_Unk1 == 1234) {
		*Output = 0.f;
		return false;
	}

	bool Return = OriginalReadFloatFromBitBuffer(This, Output, Mod, Count);
	if (!Return) {
		if (Global::Vars::g_WriteArrayThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_WriteArrayThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_WriteArrayThreadInfo.m_Block = true;
			}
		}

		if (Global::Vars::g_ReadNetworkEventThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_ReadNetworkEventThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_ReadNetworkEventThreadInfo.m_Block = true;
			}
		}

		if (This->m_Buffer->m_MaxBit != 0) LOG_DEV("[%s] Failed! %s", __FUNCTION__, Utils::GetStackTrace().c_str());
		This->m_Buffer->m_Unk1 = 1234;
		return false;
	}

	return Return;
}

bool Menu::Hooks::ReadUnsignedFloatFromBitBufferHook(Rage::BitBufferSyncInstance* This, float* Output, float Mod, int Count) {
	if (This->m_Buffer->m_Unk1 == 1234) {
		*Output = 0.f;
		return false;
	}

	bool Return = OriginalReadUnsignedFloatFromBitBuffer(This, Output, Mod, Count);
	if (!Return) {
		if (Global::Vars::g_WriteArrayThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_WriteArrayThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_WriteArrayThreadInfo.m_Block = true;
			}
		}

		if (Global::Vars::g_ReadNetworkEventThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_ReadNetworkEventThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_ReadNetworkEventThreadInfo.m_Block = true;
			}
		}

		if (This->m_Buffer->m_MaxBit != 0) LOG_DEV("[%s] Failed! %s", __FUNCTION__, Utils::GetStackTrace().c_str());
		This->m_Buffer->m_Unk1 = 1234;
		return false;
	}

	return Return;
}

bool Menu::Hooks::ReadNetworkIdFromBitBufferHook(Rage::BitBufferSyncInstance* This, short* Output) {
	if (This->m_Buffer->m_Unk1 == 1234) {
		*Output = 0;
		return false;
	}

	bool Return = OriginalReadNetworkIdFromBitBuffer(This, Output);
	if (!Return) {
		if (Global::Vars::g_WriteArrayThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_WriteArrayThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_WriteArrayThreadInfo.m_Block = true;
			}
		}

		if (Global::Vars::g_ReadNetworkEventThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_ReadNetworkEventThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_ReadNetworkEventThreadInfo.m_Block = true;
			}
		}

		if (This->m_Buffer->m_MaxBit != 0) LOG_DEV("[%s] Failed! %s", __FUNCTION__, Utils::GetStackTrace().c_str());
		This->m_Buffer->m_Unk1 = 1234;
		return false;
	}

	return Return;
}

bool Menu::Hooks::ReadArrayFromBitBufferHook(Rage::BitBufferSyncInstance* This, int* Output, int Count, int Unk) {
	if (This->m_Buffer->m_Unk1 == 1234) {
		*Output = 0;
		return false;
	}

	bool Return = OriginalReadArrayFromBitBuffer(This, Output, Count, Unk);
	if (!Return) {
		if (Global::Vars::g_WriteArrayThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_WriteArrayThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_WriteArrayThreadInfo.m_Block = true;
			}
		}

		if (Global::Vars::g_ReadNetworkEventThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_ReadNetworkEventThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_ReadNetworkEventThreadInfo.m_Block = true;
			}
		}

		if (This->m_Buffer->m_MaxBit != 0) LOG_DEV("[%s] Failed! %s", __FUNCTION__, Utils::GetStackTrace().c_str());
		This->m_Buffer->m_Unk1 = 1234;
		return false;
	}

	return Return;
}

bool Menu::Hooks::ReadStringFromBitBufferHook(Rage::BitBufferSyncInstance* This, char* Output, int Length) {
	if (This->m_Buffer->m_Unk1 == 1234) {
		*Output = 0;
		return false;
	}

	bool Return = OriginalReadStringFromBitBuffer(This, Output, Length);
	if (!Return) {
		if (Global::Vars::g_WriteArrayThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_WriteArrayThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_WriteArrayThreadInfo.m_Block = true;
			}
		}

		if (Global::Vars::g_ReadNetworkEventThreadInfo.m_CameFromThread) {
			if (Global::Vars::g_ReadNetworkEventThreadInfo.m_ThreadID == GetCurrentThreadId()) {
				Global::Vars::g_ReadNetworkEventThreadInfo.m_Block = true;
			}
		}

		if (This->m_Buffer->m_MaxBit != 0) LOG_DEV("[%s] Failed! %s", __FUNCTION__, Utils::GetStackTrace().c_str());
		This->m_Buffer->m_Unk1 = 1234;
		return false;
	}

	return Return;
}

void Menu::Hooks::ReadNodeDataHook(Rage::Network::SyncTree* Tree, uint32_t Bit, uint32_t R8d, Rage::BitBuffer* BitBuffer, uint64_t LoggingInterface) {
	if (!Tree) {
		return;
	}

	if (Tree->m_DataNodeCount > 0) {
		OriginalReadNodeData(Tree, Bit, R8d, BitBuffer, LoggingInterface);

		if (BitBuffer->m_Unk1 == 1234) {
			LOG_DEV("Crash detected lolz");
			
			// RECEIVED_CLONE_CREATE
			if (Bit == 1) {
				if (IsValidPtr(Global::Vars::g_BrokenNodeSenders[0])) {
					Menu::GetPlayer(Global::Vars::g_BrokenNodeSenders[0]->m_ID).m_BlacklistRequests = GetTickCount() + 15000;

					if (NetworkProtectionNotificationsMenuVars::m_Vars.m_Crash) {
						if (IsValidPtr(Global::Vars::g_BrokenNodeSenders[0]->m_PlayerInfo)) {
							std::string SenderName = Global::Vars::g_BrokenNodeSenders[0]->m_PlayerInfo->m_Name;

							Utils::GetFiberPool()->Push([=] {
								Menu::GetNotify()->NotifyStackedProtectionEvent(SenderName, TRANSLATE(T_CreateMisalign), false, true);
							});
						}
					}

					Global::Vars::g_BrokenNodeSenders[0] = nullptr;
				}

				Memory::WriteVector(Global::Vars::g_FunctionPatches[0].m_Address, Global::Vars::g_FunctionPatches[0].m_Patch);
				Memory::WriteVector(Global::Vars::g_FunctionPatches[1].m_Address, Global::Vars::g_FunctionPatches[1].m_Patch);
			}

			// RECEIVED_CLONE_SYNC
			if (Bit == 2) {
				if (IsValidPtr(Global::Vars::g_BrokenNodeSenders[1])) {
					Menu::GetPlayer(Global::Vars::g_BrokenNodeSenders[1]->m_ID).m_BlacklistRequests = GetTickCount() + 15000;

					if (NetworkProtectionNotificationsMenuVars::m_Vars.m_Crash) {
						if (IsValidPtr(Global::Vars::g_BrokenNodeSenders[1]->m_PlayerInfo)) {
							std::string SenderName = Global::Vars::g_BrokenNodeSenders[1]->m_PlayerInfo->m_Name;

							Utils::GetFiberPool()->Push([=] {
								Menu::GetNotify()->NotifyStackedProtectionEvent(SenderName, TRANSLATE(T_SyncMisalign), false, true);
							});
						}
					}

					Global::Vars::g_BrokenNodeSenders[1] = nullptr;
				}

				Memory::WriteVector(Global::Vars::g_FunctionPatches[2].m_Address, Global::Vars::g_FunctionPatches[2].m_Patch);
			}

			// GIVE_CONTROL_EVENT
			if (Bit == 4) {
				if (IsValidPtr(Global::Vars::g_BrokenNodeSenders[2])) {
					Menu::GetPlayer(Global::Vars::g_BrokenNodeSenders[2]->m_ID).m_BlacklistRequests = GetTickCount() + 15000;

					if (NetworkProtectionNotificationsMenuVars::m_Vars.m_Crash) {
						if (IsValidPtr(Global::Vars::g_BrokenNodeSenders[2]->m_PlayerInfo)) {
							std::string SenderName = Global::Vars::g_BrokenNodeSenders[2]->m_PlayerInfo->m_Name;

							Utils::GetFiberPool()->Push([=] {
								Menu::GetNotify()->NotifyStackedProtectionEvent(SenderName, TRANSLATE(T_SyncMisalign), false, true);
							});
						}
					}

					Global::Vars::g_BrokenNodeSenders[2] = nullptr;
				}

				Memory::WriteVector(Global::Vars::g_FunctionPatches[3].m_Address, Global::Vars::g_FunctionPatches[3].m_Patch);
			}

			return;
		}
	}
}