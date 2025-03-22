#include "camera.hpp"

namespace menu {
	math::vector3<float> camera::getCameraDirection() {
		math::vector3<float> stack;
		math::vector3<float> rot = native::getGameplayCamRotation(0);
		stack.y = rot.z * 0.0174532924f;
		stack.x = rot.x * 0.0174532924f;
		stack.z = abs(cos(stack.x));
		return math::vector3<float>((-sin(stack.y)) * stack.z, (cos(stack.y)) * stack.z, sin(stack.x));
	}

	math::vector3<float> camera::rotateToDirection(math::vector3<float> rotation) {
		float radiansZ = rotation.z * 0.0174532924f;
		float radiansX = rotation.x * 0.0174532924f;
		float num = abs((float)(cos((double)(radiansX))));

		math::vector3<float> ret_dir = math::vector3<float>(
			(float)((double)((float)(-(float)sin((double)radiansZ))) * (double)num),
			(float)((double)((float)cos((double)radiansZ)) * (double)num),
			(float)sin((double)radiansX)
		);

		return ret_dir;
	}

	camera* getCamera() {
		static camera instance;
		return &instance;
	}
}