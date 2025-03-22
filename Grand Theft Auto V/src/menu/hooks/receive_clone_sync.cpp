#include "hooks.hpp"
#include "menu/base/util/player_manager.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/helpers.hpp"
#include "utils/log.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_entity_blocking.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_entity_events.hpp"
#include "utils/caller.hpp"
#include "utils/memory/memory.hpp"

uint64_t Menu::Hooks::ReceiveCloneSyncHook(uint64_t This, Rage::Network::NetGamePlayer* NetGamePlayer, uint64_t r8, uint16_t CloneType, uint16_t NetworkID, Rage::BitBuffer* BitBuffer, uint16_t SequenceID, uint32_t Timestamp) {
	if (CloneType == NetObjectPickupPlacement) return 6;

	auto IsValid = [=] {
		return NetGamePlayer && (NetGamePlayer->m_ID >= 0 && NetGamePlayer->m_ID <= 34);
	};
	
	if (IsValid()) {
		if (Menu::GetPlayer(NetGamePlayer->m_ID).m_BlacklistRequests > GetTickCount() || Menu::GetPlayer(NetGamePlayer->m_ID).m_BlockEntities[CloneType]) {
			return 6;
		}
	}

	if (CloneType < NetObjectAutomobile || CloneType > NetObjectTrain || NetworkProtectionEntityBlockingMenuVars::m_Vars.m_Toggled[CloneType]) {
		return 6;
	}

	Rage::Network::SyncTree* SyncTree = Rage::Engine::GetSyncTreeFromCloneType(CloneType);
	if (!SyncTree) {
		return 6;
	}

	if (SyncTree->m_DataNodeCount == 0 || SyncTree->m_DataNodeCount > 100) return 6;

	Rage::Network::NetObj* Object = Rage::Engine::GetNetworkObjectFromNetworkID(This, NetworkID, true);
	if (!Object) {
		return 6;
	}

	if (Object->m_Type != CloneType) {
		return 6;
	}

	if (NetworkProtectionEntityEventsMenuVars::m_Vars.m_DisableModderSync) {
		if (Menu::GetPlayer(NetGamePlayer->m_ID).m_IsModder) {
			return 6;
		}
	}

	if (*(uint8_t*)(Global::Vars::g_FunctionPatches[2].m_Address) == Global::Vars::g_FunctionPatches[2].m_Patch[0]) {
		Memory::WriteVector(Global::Vars::g_FunctionPatches[2].m_Address, Global::Vars::g_FunctionPatches[2].m_Original);
	}

	Global::Vars::g_BrokenNodeSenders[1] = NetGamePlayer;
	return OriginalReceiveCloneSync(This, NetGamePlayer, r8, CloneType, NetworkID, BitBuffer, SequenceID, Timestamp);
}