#include "vehicle_pool.hpp"
#include "rage/invoker/natives.hpp"
#include "menu/base/util/player_manager.hpp"
#include <algorithm>

namespace menu::pools {
	entityPool* vehiclePool::getPoolPointer() {
		return (entityPool*)rage::engine::decryptVehiclePool(global::vars::g_encryptedVehiclePool.table1, global::vars::g_encryptedVehiclePool.table2, global::vars::g_encryptedVehiclePool.addition);
	}

	void vehiclePool::update() {
		std::vector<Vehicle> vehicles;

		auto pool = getPoolPointer();
		if (pool) {
			if (pool->count > 4000) return;
			if (pool->unk <= 0) return;
			if (!pool->pool) return;

			auto entityPool = rage::engine::decryptEntityPool(global::vars::g_encryptedEntityPool.table1, global::vars::g_encryptedEntityPool.table2, global::vars::g_encryptedEntityPool.addition);

			for (int i = 0; i < pool->count; i++) {
				if (entityPool) {
					if (*(uint32_t*)(entityPool + 0x18) - (*(uint32_t*)(entityPool + 0x28) & 0x3FFFFFFF) <= 256)
						break;
				}

				if (~(*(uint8_t*)(pool->unk + i) >> 7) & 1) {
					auto vehiclePtr = pool->pool + (i * pool->size);
					if (vehiclePtr) {
						auto handle = rage::engine::getEntityHandleFromAddress(vehiclePtr);
						if (handle) {
							vehicles.push_back(handle);
						}
					}
				}
			}
		}

		for (std::size_t i = 0; i < vehicles.size(); i++) {
			if (std::find(storage.begin(), storage.end(), vehicles[i]) == storage.end()) {
				storage.push_back(vehicles[i]);
			}
		}

		for (std::size_t i = 0; i < storage.size(); i++) {
			if (!native::doesEntityExist(storage[i])) {
				storage.erase(std::remove(storage.begin(), storage.end(), storage[i]), storage.end());
			}
		}
	}

	void vehiclePool::run(std::function<void(Vehicle)> callback) {
		update();

		for (std::size_t i = 0; i < storage.size(); i++) {
			if (storage[i]) {
				if (native::doesEntityExist(storage[i])) {
					if (storage[i] != menu::getLocalPlayer().entity) {
						callback(storage[i]);
					}
				}
			}
		}
	}

	vehiclePool* getVehiclePool() {
		static vehiclePool instance;
		return &instance;
	}
}