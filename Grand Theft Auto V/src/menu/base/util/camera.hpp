#pragma once
#include "rage/invoker/natives.hpp"
#include "utils/math.hpp"
#include "rage/types/network_types.hpp"

namespace Menu::Camera {
	__declspec(noinline) Rage::Network::PlayerAngles* GetPlayerAngles();
	Math::Vector3<float> GetCameraDirection(Math::Vector3<float> Rotation = Native::GetGameplayCamRot(0));
}