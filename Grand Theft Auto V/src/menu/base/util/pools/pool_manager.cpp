#include "pool_manager.hpp"
#include "utils/memory/pattern.hpp"
#include "global/variables.hpp"
#include "utils/memory/memory.hpp"

namespace Menu::Pools {
	void PoolManager::Initialize() {
		if (!Memory::ServerScanProcess("POOL_P"_Protect, [](uint64_t Address) {
			Global::Vars::g_PedPool = *(Rage::Types::GenericPool**)Memory::GetAddressFromInstruction(Address);
		})) return;

		if (!Memory::ServerScanProcess("POOL_O"_Protect, [](uint64_t Address) {
			Global::Vars::g_ObjectPool = *(Rage::Types::GenericPool**)Memory::GetAddressFromInstruction(Address);
		})) return;

		if (!Memory::ServerScanProcess("POOL_PI"_Protect, [](uint64_t Address) {
			Global::Vars::g_PickupPool = *(Rage::Types::GenericPool**)Memory::GetAddressFromInstruction(Address);
		})) return;
		
		if (!Memory::ServerScanProcess("POOL_A"_Protect, [](uint64_t Address) {
			Global::Vars::g_AttachmentPool = *(Rage::Types::GenericPool**)Memory::GetAddressFromInstruction(Address);
		})) return;

		if (!Memory::ServerScanProcess("POOL_NEQ"_Protect, [] (uint64_t Address) {
			Global::Vars::g_NetworkEventQueuePool = *(Rage::Types::GenericPool**)Memory::GetAddressFromInstruction(Address);
		})) return;

		if (!Memory::ServerScanProcess("POOL_V"_Protect, [](uint64_t Address) {
			Global::Vars::g_VehiclePool = *(Rage::Types::VehicleEntityPool**)Memory::GetAddressFromInstruction(Address);
		})) return;

		if (!Memory::ServerScanProcess("POOL_E"_Protect, [](uint64_t Address) {
			Global::Vars::g_EntityPool = *(Rage::Types::EntityPool**)Memory::GetAddressFromInstruction(Address);
		})) return;

		if (!Memory::ServerScanProcess("POOL_E2"_Protect, [] (uint64_t Address) {
			Global::Vars::g_MainEntityPool = (uint64_t*)Memory::GetAddressFromInstruction(Address);
		})) return;
	}

	PoolManager* GetPoolManager() {
		static PoolManager Instance;
		return &Instance;
	}
}