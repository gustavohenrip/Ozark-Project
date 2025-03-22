#include "spawn_manager.hpp"
#include "rage/invoker/natives.hpp"
#include "src/menu/base/util/fiber_manager.hpp"
#include "src/menu/base/util/fiber_pool.hpp"

namespace menu::util {
	Object spawnManager::spawnObject(Hash hash, math::vector3<float> coords) {
		if (!native::hasModelLoaded(hash)) {
			native::requestModel(hash);
		}

		return native::createObject(hash, coords, TRUE, FALSE, TRUE, FALSE, FALSE);
	}

	Ped spawnManager::spawnPed(Hash hash, math::vector3<float> coords, float heading) {
		if (!native::hasModelLoaded(hash)) {
			native::requestModel(hash);
		}
		global::vars::g_blockPedMetric = true;
		Ped ped = native::createPed(hash, coords, heading, TRUE, TRUE, TRUE, TRUE);

		if (native::doesEntityExist(ped)) {
			native::setPedDefaultOutfit(ped, TRUE);
			if (native::getMount(ped)) {
				native::decorSetBool(native::getMount(ped), "wagon_block_honor", TRUE);
			}
		}

		return ped;
	}

	Vehicle spawnManager::spawnVehicle(Hash hash, math::vector3<float> coords, float heading) {
		if (!native::hasModelLoaded(hash)) {
			native::requestModel(hash);
		}

		Vehicle vehicle = native::createVehicle(hash, coords, heading, TRUE, TRUE, 0, TRUE);
		if (native::doesEntityExist(vehicle)) {
			native::decorSetBool(vehicle, "wagon_block_honor", TRUE);
		}

		return vehicle;
	}

	spawnManager* getSpawnManager() {
		static spawnManager instance;
		return &instance;
	}
}