#include "net_event_queue_pool.hpp"
#include <algorithm>

namespace menu::pools {
	entityPool* netEventQueuePool::getPoolPointer() {
		return (entityPool*)rage::engine::decryptNetworkQueuePool(global::vars::g_encryptedNetworkEventQueuePool.table1, global::vars::g_encryptedNetworkEventQueuePool.table2, global::vars::g_encryptedNetworkEventQueuePool.addition);
	}

	netEventQueuePool* getNetEventQueuePool() {
		static netEventQueuePool instance;
		return &instance;
	}
}