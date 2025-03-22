#pragma once
#include "rage/engine.hpp"
#include "utils/math.hpp"
#include "utils/singleton.hpp"
#include "ped_pool.hpp"
#include "vehicle_pool.hpp"
#include "object_pool.hpp"
#include "pickup_pool.hpp"
#include "attachment_pool.hpp"
#include "net_event_queue_pool.hpp"

namespace menu::pools {
	class poolManager {
	public:
		void initialize();

		uint64_t getPedClass() { return pedClass; }
		uint64_t getObjectClass() { return objectClass; }
		uint64_t getVehicleClass() { return vehicleClass; }
		uint64_t getPickupClass() { return pickupClass; }
	private:
		uint64_t pedClass;
		uint64_t objectClass;
		uint64_t vehicleClass;
		uint64_t pickupClass;
	}; poolManager* getPoolManager();
}