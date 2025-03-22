#pragma once
#include "rage/engine.hpp"
#include "utils/math.hpp"
#include "utils/singleton.hpp"

namespace menu::pools {
	class vehiclePool {
	public:
		void update();
		void run(std::function<void(Vehicle)> callback);
		entityPool* getPoolPointer();
		std::vector<Vehicle>& getStorage() { return storage; }
	private:
		std::vector<Vehicle> storage;
	}; vehiclePool* getVehiclePool();
}