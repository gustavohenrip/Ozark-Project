#pragma once
#include "menu/base/submenu.hpp"

class VehicleHandlingFlyingMenu : public Menu::Submenu {
public:
	static VehicleHandlingFlyingMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleHandlingFlyingMenu()
		: Menu::Submenu() {}

	~VehicleHandlingFlyingMenu();
};

namespace VehicleHandlingFlyingMenuVars {
	struct ThrustRestore {
		float m_Thrust;
		float m_ThrustFallOff;
		float m_ThrustVectoring;
	};

	struct MovementRestore {
		float m_SideSlipMult;
		float m_YawMult;
		float m_YawStabilise;
		float m_RollMult;
		float m_RollStabilise;
		float m_PitchMult;
		float m_PitchStabilise;
		float m_FormLiftMult;
		float m_AttackLiftMult;
		float m_AttackDiveMult;
		float m_EngineOffGlideMulti;
	};

	struct ResistanceRestore {
		float m_WindMult;
		float m_MoveRes;
		Math::Vector3_<float> m_VecTurnResistance;
		Math::Vector3_<float> m_VecSpeedResistance;
	};

	struct TurbulenceRestore {
		float m_TurublenceMagnitudeMax;
		float m_TurublenceForceMulti;
		float m_TurublenceRollTorqueMulti;
		float m_TurublencePitchTorqueMulti;
	};

	struct GearRestore {
		float m_GearDownDragV;
		float m_GearDownLiftMult;
	};

	struct GroundRestore {
		float m_OnGroundYawBoostSpeedPeak;
		float m_OnGroundYawBoostSpeedCap;
	};

	struct MiscRestore {
		float m_BodyDamageControlEffectMult;
		float m_InputSensitivityForDifficulty;
	};

	struct Vars_ {
		int m_Type;

		std::unordered_map<uint32_t, ThrustRestore> m_ThrustRestore;
		std::unordered_map<uint32_t, MovementRestore> m_MovementRestore;
		std::unordered_map<uint32_t, ResistanceRestore> m_ResistanceRestore;
		std::unordered_map<uint32_t, TurbulenceRestore> m_TurbulenceRestore;
		std::unordered_map<uint32_t, GearRestore> m_GearRestore;
		std::unordered_map<uint32_t, GroundRestore> m_GroundRestore;
		std::unordered_map<uint32_t, MiscRestore> m_MiscRestore;
	};

	void CacheThrust(uint32_t Model);
	void CacheMovement(uint32_t Model);
	void CacheResistance(uint32_t Model);
	void CacheTurbulence(uint32_t Model);
	void CacheGear(uint32_t Model);
	void CacheGround(uint32_t Model);
	void CacheMisc(uint32_t Model);
	extern Vars_ m_Vars;
}