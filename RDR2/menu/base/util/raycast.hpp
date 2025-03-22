#pragma once
#include "rage/types/base_types.hpp"
#include "utils/math.hpp"
#include "rage/invoker/natives.hpp"

namespace menu {
	class raycast {
	public:
		raycast(int cast);
		
		bool didHit() { return (bool)didHitEntity; }
		BOOL didHitSomething() { return hit; }
		Entity getEntity() { return hitEntity; }
		math::vector3<float> getCoords() { return endCoords; }
	private:
		Entity hitEntity;
		BOOL hit;
		bool didHitEntity;
		math::vector3<float> endCoords;
		math::vector3<float> surfaceNormal;
	};

	raycast raycastInfront(float length = 25.f, math::vector3<float> camCoord = native::getGameplayCamCoord(), int flag = -1);
	raycast raycastPointToPoint(math::vector3<float> points[2], int flag = -1);
}