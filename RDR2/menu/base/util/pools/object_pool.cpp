#include "object_pool.hpp"
#include "rage/invoker/natives.hpp"
#include "menu/base/util/player_manager.hpp"
#include <algorithm>

namespace menu::pools {
	entityPool* objectPool::getPoolPointer() {
		return (entityPool*)rage::engine::decryptObjectPool(global::vars::g_encryptedObjectPool.table1, global::vars::g_encryptedObjectPool.table2, global::vars::g_encryptedObjectPool.addition);
	}

	void objectPool::update() {
		std::vector<Object> objects;

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
					auto objectPtr = pool->pool + (i * pool->size);
					if (objectPtr) {
						auto handle = rage::engine::getEntityHandleFromAddress(objectPtr);
						if (handle) {
							objects.push_back(handle);
						}
					}
				}
			}
		}

		for (std::size_t i = 0; i < objects.size(); i++) {
			if (std::find(storage.begin(), storage.end(), objects[i]) == storage.end()) {
				storage.push_back(objects[i]);
			}
		}

		for (std::size_t i = 0; i < storage.size(); i++) {
			if (!native::doesEntityExist(storage[i])) {
				storage.erase(std::remove(storage.begin(), storage.end(), storage[i]), storage.end());
			}
		}
	}

	void objectPool::run(std::function<void(Object)> callback) {
		update();

		for (std::size_t i = 0; i < storage.size(); i++) {
			if (storage[i]) {
				if (native::doesEntityExist(storage[i])) {
					callback(storage[i]);
				}
			}
		}
	}

	objectPool* getObjectPool() {
		static objectPool instance;
		return &instance;
	}
}