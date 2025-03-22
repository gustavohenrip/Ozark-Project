#include "ped_pool.hpp"
#include "rage/invoker/natives.hpp"
#include "menu/base/util/player_manager.hpp"
#include "menu/base/util/control_manager.hpp"
#include <algorithm>

namespace menu::pools {
	entityPool* pedPool::getPoolPointer() {
		return (entityPool*)rage::engine::decryptPedPool(global::vars::g_encryptedPedPool.table1, global::vars::g_encryptedPedPool.table2, global::vars::g_encryptedPedPool.addition);
	}

	void pedPool::update() {
		std::vector<Ped> peds;

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
					auto pedPtr = pool->pool + (i * pool->size);
					if (pedPtr) {
						auto handle = rage::engine::getEntityHandleFromAddress(pedPtr);
						if (handle) {
							peds.push_back(handle);
						}
					}
				}
			}
		}

		for (std::size_t i = 0; i < peds.size(); i++) {
			if (std::find(storage.begin(), storage.end(), peds[i]) == storage.end()) {
				storage.push_back(peds[i]);
			}
		}

		for (std::size_t i = 0; i < storage.size(); i++) {
			if (!native::doesEntityExist(storage[i])) {
				storage.erase(std::remove(storage.begin(), storage.end(), storage[i]), storage.end());
			}
		}
	}

	void pedPool::run(std::function<void(Ped)> callback) {
		update();

		for (std::size_t i = 0; i < storage.size(); i++) {
			if (storage[i]) {
				if (native::doesEntityExist(storage[i])) {
					if (storage[i] != menu::getLocalPlayer().ped) {
						callback(storage[i]);
					}
				}
			}
		}
	}

	pedPool* getPedPool() {
		static pedPool instance;
		return &instance;
	}
}