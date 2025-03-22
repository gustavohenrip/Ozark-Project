#include "raycast.hpp"
#include "menu/base/util/player_manager.hpp"
#include "camera.hpp"

namespace Menu {
	Raycast::Raycast(int Cast) {
		Entity HitEntity;
		Native::GetShapeTestResult(Cast, &m_Hit, &m_HitCoords, &m_SurfaceNormal, &HitEntity);
		if (m_Hit) {
			if (Native::DoesEntityExist(HitEntity)) {
				int Type = Native::GetEntityType(HitEntity);
				if (HitEntity != GetLocalPlayer().m_Ped
					&& (Type == 1 || Type == 2 || Type == 3)) {
					m_HitEntity = HitEntity;
				} else m_HitEntity = 0;
			}

			m_DidHitEntity = m_HitEntity != 0;
		}
	}

	bool Raycast::HitEntity() {
		return m_HitEntity;
	}

	bool Raycast::HitSomething() {
		return m_Hit;
	}

	Entity Raycast::GetHitEntity() {
		return m_HitEntity;
	}

	Math::Vector3<float> Raycast::GetHitCoords() {
		return m_HitCoords;
	}

	Raycast RaycastInfront(float Length, Math::Vector3<float> CamCoords, int Flag) {
		auto Infront = Camera::GetCameraDirection();
		Math::Vector3<float> Target = CamCoords + (Infront * Length);
		return Raycast(Native::_StartShapeTestRay(CamCoords.m_X, CamCoords.m_Y, CamCoords.m_Z, Target.m_X, Target.m_Y, Target.m_Z, Flag, Menu::GetLocalPlayer().m_Ped, 0));
	}

	Raycast RaycastPointToPoint(Math::Vector3<float> Points[2], int Flag) {
		return Raycast(Native::_StartShapeTestRay(Points[0].m_X, Points[0].m_Y, Points[0].m_Z, Points[1].m_X, Points[1].m_Y, Points[1].m_Z, Flag, Menu::GetLocalPlayer().m_Ped, 0));
	}
}