#pragma once
#include "menu/base/submenu.hpp"

class VehicleHandlingBikeMenu : public Menu::Submenu {
public:
	static VehicleHandlingBikeMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleHandlingBikeMenu()
		: Menu::Submenu() {}

	~VehicleHandlingBikeMenu();
};

namespace VehicleHandlingBikeMenuVars {
	struct PhysicalRestore {
		float m_LeanFwdCOMMult;
		float m_LeanFwdForceMult;
		float m_LeanBakCOMMult;
		float m_LeanBakForceMult;
		float m_MaxBankAngle;
		float m_FullAnimAngle;
		float m_DesLeanReturnFrac;
		float m_StickLeanMult;
		float m_BrakingStabilityMult;
		float m_InAirSteerMult;
		float m_JumpForce;
	};

	struct BalanceRestore {
		float m_BikeOnStandSteerAngle;
		float m_BikeOnStandLeanAngle;
		float m_FrontBalanceMult;
		float m_RearBalanceMult;
		float m_WheelieBalancePoint;
		float m_StoppieBalancePoint;
		float m_WheelieSteerMult;
	};

	struct FrictionRestore {
		float m_BikeGroundSideFrictionMult;
		float m_BikeWheelGroundSideFrictionMult;
	};

	struct Vars_ {
		int m_Type;

		std::unordered_map<uint32_t, PhysicalRestore> m_PhysicalRestore;
		std::unordered_map<uint32_t, BalanceRestore> m_BalanceRestore;
		std::unordered_map<uint32_t, FrictionRestore> m_FrictionRestore;
	};

	void CachePhysical(uint32_t Model);
	void CacheBalance(uint32_t Model);
	void CacheFriction(uint32_t Model);
	extern Vars_ m_Vars;
}