#include "camera.hpp"
#include "global/variables.hpp"
#include "utils/caller.hpp"

namespace Menu::Camera {
	Math::Vector3<float> GetCameraDirection(Math::Vector3<float> Rotation) {
		Math::Vector3<float> Tmp;
		Tmp.m_Y = Rotation.m_Z * 0.0174532924f;
		Tmp.m_X = Rotation.m_X * 0.0174532924f;
		Tmp.m_Z = abs(cos(Tmp.m_X));

		return Math::Vector3<float>((-sin(Tmp.m_Y)) * Tmp.m_Z, (cos(Tmp.m_Y)) * Tmp.m_Z, sin(Tmp.m_X));
	}

	Rage::Network::PlayerAngles* GetPlayerAngles() {
		if (IsValidPtr(Global::Vars::g_GameCameraAngles)) {
			Rage::Network::GameCameraAngles* CameraAngles = Global::Vars::g_GameCameraAngles;
			if (IsValidPtr(CameraAngles)) {
				Rage::Network::CameraManagerAngles* CameraManagerAngles = CameraAngles->m_CameraManagerAngles;
				if (IsValidPtr(CameraManagerAngles)) {
					Rage::Network::CameraAngles* CameraAngles = CameraManagerAngles->m_CameraAngles;
					if (IsValidPtr(CameraAngles)) {
						Rage::Network::PlayerAngles* PlayerAngles = Caller::Call<Rage::Network::PlayerAngles*>(Global::Vars::g_GetThirdPersonCamera, *(uint64_t*)(Global::Vars::g_ThirdPersonCamera), CameraAngles->m_Unknown);
						if (IsValidPtr(PlayerAngles)) {
							return (Rage::Network::PlayerAngles*)((uint64_t)PlayerAngles + 0x390);
						}

						if (IsValidPtr(CameraAngles)) {
							if (IsValidPtr(CameraAngles->m_FPSPlayerAngles)) {
								return CameraAngles->m_FPSPlayerAngles;
							}
						}
					}
				}
			}
		}

		return nullptr;
	}
}