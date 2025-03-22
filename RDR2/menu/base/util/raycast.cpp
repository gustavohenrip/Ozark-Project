#include "raycast.hpp"
#include "menu/base/util/player_manager.hpp"
#include "camera.hpp"

namespace menu {
	raycast::raycast(int cast) {
		Entity hitEnt;
		native::getShapeTestResult(cast, &hit, &endCoords, &surfaceNormal, &hitEnt);
		if (hit) {
			if (native::doesEntityExist(hitEnt)) {
				auto type = native::getEntityType(hitEnt);
				if (hitEnt != getLocalPlayer().ped && (type == 1 || type == 2 || type == 3)) { // 1 = peds, 3 = ground shit
					hitEntity = hitEnt;
				} else hitEntity = NULL;
			}
		}

		didHitEntity = hitEntity != NULL;
	}

	raycast raycastInfront(float length, math::vector3<float> camCoord, int flag) {
		auto infront = getCamera()->getCameraDirection();
		math::vector3<float> target = camCoord + (infront * length);
		return raycast(native::startShapeTestRay(camCoord, target, flag, getLocalPlayer().ped, 0));
	}

	raycast raycastPointToPoint(math::vector3<float> points[2], int flag) {
		return raycast(native::startShapeTestRay(points[0], points[1], flag, getLocalPlayer().ped, 0));
	}
}