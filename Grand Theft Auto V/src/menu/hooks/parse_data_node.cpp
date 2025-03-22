#include "hooks.hpp"
#include "rage/engine.hpp"
#include "menu/base/submenus/main/network/players/network_players_removal.hpp"

bool Menu::Hooks::ParseDataNodeHook(Rage::Network::DataNode* Node, uint32_t Edx, uint64_t R8, Rage::Network::NetObj* Object, Rage::BitBuffer* Buffer, uint32_t BitSize, uint64_t Unk1, bool Unk2, uint64_t Unk3) {
	auto& Vars = NetworkPlayersRemovalMenuVars::m_Vars;

	if (IsValidPtr(Object)) {
		if (Object->m_Type != NetObjectPlayer) {
			if (Vars.m_TargetedCrash) {
				Vars.m_BrokenEntities[Object] = Vars.m_TargetedCrashTarget;

				Vars.m_BreakNodes = true;
				Vars.m_BreakNodeThreads[0] = GetCurrentThreadId();

				bool Return = OriginalParseDataNode(Node, Edx, R8, Object, Buffer, BitSize, Unk1, Unk2, Unk3);

				Vars.m_BreakNodes = false;
				Vars.m_BreakNodeThreads[0] = 0;

				return Return;
			}
		}
	}
	
	return OriginalParseDataNode(Node, Edx, R8, Object, Buffer, BitSize, Unk1, Unk2, Unk3);
}

bool Menu::Hooks::ParseDataNode2Hook(Rage::Network::DataNode* Node, Rage::Network::NetObj* Object) {
	auto& Vars = NetworkPlayersRemovalMenuVars::m_Vars;

	if (IsValidPtr(Object)) {
		if (Object->m_Type != NetObjectPlayer) {
			if (Vars.m_TargetedCrash) {
				Vars.m_BrokenEntities[Object] = Vars.m_TargetedCrashTarget;

				Vars.m_BreakNodes = true;
				Vars.m_BreakNodeThreads[1] = GetCurrentThreadId();

				bool Return = OriginalParseDataNode2(Node, Object);

				Vars.m_BreakNodes = false;
				Vars.m_BreakNodeThreads[1] = 0;

				return Return;
			}
		}
	}

	return OriginalParseDataNode2(Node, Object);
}