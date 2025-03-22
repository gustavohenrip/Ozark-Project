#include "hooks.hpp"
#include "utils/log.hpp"
#include "menu/base/submenus/main/network/players/network_players_removal.hpp"

bool Menu::Hooks::WriteUintToDataWriterBufferHook(Rage::BitBufferSyncInstance* This, uint32_t* Input, uint32_t BitsToWrite, uint32_t R9d) {
	auto& Vars = NetworkPlayersRemovalMenuVars::m_Vars;

	if (Vars.m_BreakNodes || Vars.m_SessionCrash) {
		if (Vars.m_BreakNodeThreads[0] == GetCurrentThreadId() || Vars.m_BreakNodeThreads[1] == GetCurrentThreadId()) {
			bool FuckUp = false;

			if ((Vars.m_SessionCrash && Vars.m_SessionCrashID == 1) || (Vars.m_TargetedCrash && Vars.m_TargetedCrashID == 1)) {
				FuckUp = true;
			}

			if (FuckUp) {
				uint64_t ReturnAddress = (uint64_t)_ReturnAddress();
				uint32_t NicholasDavidChant = 0xFFFFFFFF;

				if (ReturnAddress == Global::Vars::g_NodeFixCDynamicEntityGameStateDataNode) {
					LOG_CUSTOM_DEV("WriteToBuffer", "Breaking CDynamicEntityGameStateDataNode");
					return OriginalWriteUintToDataWriterBuffer(This, &NicholasDavidChant, BitsToWrite, R9d);
				}

				if (ReturnAddress == Global::Vars::g_NodeFixCVehicleGadgetDataNode) {
					LOG_CUSTOM_DEV("WriteToBuffer", "Breaking CVehicleGadgetDataNode");
					return OriginalWriteUintToDataWriterBuffer(This, &NicholasDavidChant, BitsToWrite, R9d);
				}

				if (ReturnAddress == Global::Vars::g_NodeFixCVehicleHealthDataNode) {
					LOG_CUSTOM_DEV("WriteToBuffer", "Breaking CVehicleHealthDataNode");
					return OriginalWriteUintToDataWriterBuffer(This, &NicholasDavidChant, BitsToWrite, R9d);
				}

				if (ReturnAddress == Global::Vars::g_NodeFixCVehicleComponentReservationDataNode) {
					LOG_CUSTOM_DEV("WriteToBuffer", "Breaking CVehicleComponentReservationDataNode");
					return OriginalWriteUintToDataWriterBuffer(This, &NicholasDavidChant, BitsToWrite, R9d);
				}

				if (ReturnAddress == Global::Vars::g_NodeFixCPedGameStateDataNode) {
					LOG_CUSTOM_DEV("WriteToBuffer", "Breaking CPedGameStateDataNode");
					return OriginalWriteUintToDataWriterBuffer(This, &NicholasDavidChant, BitsToWrite, R9d);
				}

				if (ReturnAddress == Global::Vars::g_NodeFixCPedGameStateDataNode + 0x5E) {
					LOG_CUSTOM_DEV("WriteToBuffer", "Breaking CPedGameStateDataNode");
					return OriginalWriteUintToDataWriterBuffer(This, &NicholasDavidChant, BitsToWrite, R9d);
				}

				if (ReturnAddress == Global::Vars::g_NodeFixCPedComponentReservationDataNode) {
					LOG_CUSTOM_DEV("WriteToBuffer", "Breaking CPedComponentReservationDataNode");
					return OriginalWriteUintToDataWriterBuffer(This, &NicholasDavidChant, BitsToWrite, R9d);
				}

				if (ReturnAddress == Global::Vars::g_NodeFixCPedScriptGameStateDataNode) {
					LOG_CUSTOM_DEV("WriteToBuffer", "Breaking CPedScriptGameStateDataNode");
					return OriginalWriteUintToDataWriterBuffer(This, &NicholasDavidChant, BitsToWrite, R9d);
				}

				if (ReturnAddress == Global::Vars::g_NodeFixCPedTaskTreeDataNode) {
					LOG_CUSTOM_DEV("WriteToBuffer", "Breaking CPedTaskTreeDataNode");
					return OriginalWriteUintToDataWriterBuffer(This, &NicholasDavidChant, BitsToWrite, R9d);
				}

				if (ReturnAddress == Global::Vars::g_NodeFixCPedInventoryDataNode) {
					LOG_CUSTOM_DEV("WriteToBuffer", "Breaking CPedInventoryDataNode");
					return OriginalWriteUintToDataWriterBuffer(This, &NicholasDavidChant, BitsToWrite, R9d);
				}

				if (ReturnAddress == Global::Vars::g_NodeFixCPedInventoryDataNode + 0x4D) {
					LOG_CUSTOM_DEV("WriteToBuffer", "Breaking CPedInventoryDataNode");
					return OriginalWriteUintToDataWriterBuffer(This, &NicholasDavidChant, BitsToWrite, R9d);
				}

				if (ReturnAddress == Global::Vars::g_NodeFixCPedTaskSequenceDataNode) {
					LOG_CUSTOM_DEV("WriteToBuffer", "Breaking CPedTaskSequenceDataNode");
					return OriginalWriteUintToDataWriterBuffer(This, &NicholasDavidChant, BitsToWrite, R9d);
				}

				if (ReturnAddress == Global::Vars::g_NodeFixCPlayerGameStateDataNode) {
					LOG_CUSTOM_DEV("WriteToBuffer", "Breaking CPlayerGameStateDataNode");
					return OriginalWriteUintToDataWriterBuffer(This, &NicholasDavidChant, BitsToWrite, R9d);
				}

				if (ReturnAddress == Global::Vars::g_NodeFixCPickupCreationDataNode) {
					LOG_CUSTOM_DEV("WriteToBuffer", "Breaking CPickupCreationDataNode");
					return OriginalWriteUintToDataWriterBuffer(This, &NicholasDavidChant, BitsToWrite, R9d);
				}

				if (ReturnAddress == Global::Vars::g_NodeFixCVehicleControlDataNode) {
					LOG_CUSTOM_DEV("WriteToBuffer", "Breaking CVehicleControlDataNode");
					return OriginalWriteUintToDataWriterBuffer(This, &NicholasDavidChant, BitsToWrite, R9d);
				}

				if (ReturnAddress == Global::Vars::g_NodeFixCVehicleProximityMigrationDataNode) {
					LOG_CUSTOM_DEV("WriteToBuffer", "Breaking CVehicleProximityMigrationDataNode");
					return OriginalWriteUintToDataWriterBuffer(This, &NicholasDavidChant, BitsToWrite, R9d);
				}
			}

			if ((Vars.m_SessionCrash && Vars.m_SessionCrashID == 3) || (Vars.m_TargetedCrash && Vars.m_TargetedCrashID == 3)) {
				return true;
			}
		}
	}

	return OriginalWriteUintToDataWriterBuffer(This, Input, BitsToWrite, R9d);
}