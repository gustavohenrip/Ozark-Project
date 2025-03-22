#include "pool_manager.hpp"
#include "utils/memory/pattern.hpp"

namespace menu::pools {
	void poolManager::initialize() {
		if (!memory::getPattern()->serverScanProcess("PEDP"_Protect, [&] (uint64_t address) {
			pedClass = memory::getMemory()->getAddressFromInstruction(address);
			if (pedClass) {
				uint64_t constructor = *(uint64_t*)pedClass;
				if (constructor) {
					global::vars::g_encryptedPedPool.table1 = memory::getMemory()->getAddressFromInstruction(constructor + 0x2D);
					global::vars::g_encryptedPedPool.table2 = memory::getMemory()->getAddressFromInstruction(constructor + 0x34);
					global::vars::g_encryptedPedPool.addition = *(uint8_t*)(constructor + 0x4D);
				}
			}
		})) return;

		if (!memory::getPattern()->serverScanProcess("VEHP"_Protect, [&] (uint64_t address) {
			vehicleClass = memory::getMemory()->getAddressFromInstruction(address);
			if (vehicleClass) {
				uint64_t constructor = *(uint64_t*)vehicleClass;
				if (constructor) {
					global::vars::g_encryptedVehiclePool.table1 = memory::getMemory()->getAddressFromInstruction(constructor + 0x2D);
					global::vars::g_encryptedVehiclePool.table2 = memory::getMemory()->getAddressFromInstruction(constructor + 0x34);
					global::vars::g_encryptedVehiclePool.addition = *(uint8_t*)(constructor + 0x50);
				}
			}
		})) return;

		if (!memory::getPattern()->serverScanProcess("PICKP"_Protect, [&] (uint64_t address) {
			pickupClass = memory::getMemory()->getAddressFromInstruction(address);
			if (pickupClass) {
				uint64_t constructor = *(uint64_t*)pickupClass;
				if (constructor) {
					global::vars::g_encryptedPickupPool.table1 = memory::getMemory()->getAddressFromInstruction(constructor + 0x2D);
					global::vars::g_encryptedPickupPool.table2 = memory::getMemory()->getAddressFromInstruction(constructor + 0x34);
					global::vars::g_encryptedPickupPool.addition = 1;
				}
			}
		})) return;

		if (!memory::getPattern()->serverScanProcess("OBJP"_Protect, [&] (uint64_t address) {
			objectClass = memory::getMemory()->getAddressFromInstruction(address);
			if (objectClass) {
				uint64_t constructor = *(uint64_t*)objectClass;
				if (constructor) {
					global::vars::g_encryptedObjectPool.table1 = memory::getMemory()->getAddressFromInstruction(constructor + 0x2D);
					global::vars::g_encryptedObjectPool.table2 = memory::getMemory()->getAddressFromInstruction(constructor + 0x34);
					global::vars::g_encryptedObjectPool.addition = *(uint8_t*)(constructor + 0x51);
				}
			}
		})) return;

		if (!memory::getPattern()->serverScanProcess("ATTP"_Protect, [&] (uint64_t address) {
			uint64_t func = memory::getMemory()->getAddressFromInstruction(address, 1, 5);
			if (func) {
				global::vars::g_encryptedAttachmentPool.table1 = memory::getMemory()->getAddressFromInstruction(func + 0x2D);
				global::vars::g_encryptedAttachmentPool.table2 = memory::getMemory()->getAddressFromInstruction(func + 0x34);
				global::vars::g_encryptedAttachmentPool.addition = *(uint8_t*)(func + 0x51);
			}
		})) return;

		if (!memory::getPattern()->serverScanProcess("NETQP"_Protect, [] (uint64_t address) {
			uint64_t func = memory::getMemory()->getAddressFromInstruction(address, 1, 5);
			if (func) {
				global::vars::g_encryptedNetworkEventQueuePool.table1 = memory::getMemory()->getAddressFromInstruction(func + 0x22);
				global::vars::g_encryptedNetworkEventQueuePool.table2 = memory::getMemory()->getAddressFromInstruction(func + 0x29);
				global::vars::g_encryptedNetworkEventQueuePool.addition = *(uint8_t*)(func + 0x42);
			}
		})) return;

		if (global::vars::g_getEntityAddress) {
			global::vars::g_encryptedEntityPool.table1 = memory::getMemory()->getAddressFromInstruction(global::vars::g_getEntityAddress + 0x16);
			global::vars::g_encryptedEntityPool.table2 = memory::getMemory()->getAddressFromInstruction(global::vars::g_getEntityAddress + 0x1D);
			global::vars::g_encryptedEntityPool.addition = *(uint8_t*)(global::vars::g_getEntityAddress + 0x36);
		}
	}

	poolManager* getPoolManager() {
		static poolManager instance;
		return &instance;
	}
}