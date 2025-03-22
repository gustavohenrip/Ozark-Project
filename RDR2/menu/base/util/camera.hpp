#pragma once
#include "rage/invoker/natives.hpp"
#include "utils/singleton.hpp"

namespace menu {
	class camera {
	public:
		math::vector3<float> getCameraDirection();
		math::vector3<float> rotateToDirection(math::vector3<float> rotation);
	}; camera* getCamera();
}