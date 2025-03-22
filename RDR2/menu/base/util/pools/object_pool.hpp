#pragma once
#include "rage/engine.hpp"
#include "utils/math.hpp"
#include "utils/singleton.hpp"

namespace menu::pools {
	class objectPool {
	public:
		void update();
		void run(std::function<void(Object)> callback);
		entityPool* getPoolPointer();
		std::vector<Object>& getStorage() { return storage; }
	private:
		std::vector<Object> storage;
	}; objectPool* getObjectPool();
}