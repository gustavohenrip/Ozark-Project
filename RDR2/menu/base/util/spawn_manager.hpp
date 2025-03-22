#pragma once
#include "rage/types/base_types.hpp"
#include "utils/singleton.hpp"
#include "utils/math.hpp"

namespace menu::util {
	class spawnManager {
	public:
		Object spawnObject(Hash hash, math::vector3<float> coords);
		Ped spawnPed(Hash hash, math::vector3<float> coords, float heading);
		Vehicle spawnVehicle(Hash hash, math::vector3<float> coords, float heading);
	}; spawnManager* getSpawnManager();
}