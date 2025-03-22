#include "entity.hpp"
#include "rage/engine.hpp"
#include "utils/caller.hpp"
#include "global/variables.hpp"

namespace Rage::Entity {
	uint64_t GetNetworkObject(::Entity entity) {
		uint64_t EntityAddress = Rage::Engine::GetEntityAddress(entity);
		if (EntityAddress) {
			return *(uint64_t*)(EntityAddress + 0xD0);
		}

		return 0;
	}

	int GetNetworkObjectOwner(::Entity entity) {
		uint64_t NetObject = GetNetworkObject(entity);
		if (NetObject) {
			return (int)*(uint8_t*)(NetObject + 0x49);
		}

		return 0;
	}

	bool UpdateNetworkObjectOwner(::Entity entity, int NewOwner) {
		uint64_t NetObject = GetNetworkObject(entity);
		if (NetObject) {
			return Caller::Call<bool>(Global::Vars::g_UpdateNetObjectOwner, *(uint64_t*)Global::Vars::g_NetworkObjectManager, NetObject, Rage::Engine::GetNetGamePlayerFromIndex(NewOwner), 0);
		}

		return false;
	}
}