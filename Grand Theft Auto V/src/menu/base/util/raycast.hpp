#pragma once
#include "rage/types/base_types.hpp"
#include "utils/math.hpp"
#include "rage/invoker/natives.hpp"

namespace Menu {
	class Raycast {
	public:
		Raycast(int Cast);

		bool HitEntity();
		bool HitSomething();
		Entity GetHitEntity();
		Math::Vector3<float> GetHitCoords();
	private:
		Entity m_HitEntity = 0;
		bool m_Hit = false;
		bool m_DidHitEntity  = false;
		Math::Vector3<float> m_HitCoords;
		Math::Vector3<float> m_SurfaceNormal;
	};

	Raycast RaycastInfront(float Length = 25.f, Math::Vector3<float> CamCoords = Native::GetGameplayCamCoord(), int Flag = -1);
	Raycast RaycastPointToPoint(Math::Vector3<float> Points[2], int Flag = -1);
}