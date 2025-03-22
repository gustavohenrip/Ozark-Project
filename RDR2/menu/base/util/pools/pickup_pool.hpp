#pragma once
#include "rage/engine.hpp"
#include "utils/math.hpp"
#include "utils/singleton.hpp"

namespace menu::pools {
	class pickupPool {
	public:
		void update();
		void run(std::function<void(Entity)> callback);
		entityPool* getPoolPointer();
		std::vector<Entity>& getStorage() { return storage; }
	private:
		std::vector<Entity> storage;
	}; pickupPool* getPickupPool();
}