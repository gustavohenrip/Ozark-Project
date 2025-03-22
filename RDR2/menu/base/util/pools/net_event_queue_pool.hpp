#pragma once
#include "rage/engine.hpp"
#include "utils/math.hpp"

namespace menu::pools {
	class netEventQueuePool {
	public:
		entityPool* getPoolPointer();
	}; netEventQueuePool* getNetEventQueuePool();
}