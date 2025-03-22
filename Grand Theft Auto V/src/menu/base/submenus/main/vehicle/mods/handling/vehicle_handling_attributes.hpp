#pragma once
#include "menu/base/submenu.hpp"

class VehicleModsHandlingAttributesMenu : public Menu::Submenu {
public:
	static VehicleModsHandlingAttributesMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleModsHandlingAttributesMenu()
		: Menu::Submenu() {}

	~VehicleModsHandlingAttributesMenu();
};

namespace VehicleModsHandlingAttributesMenuVars {
	struct PhysicalRestore {
		float m_Mass;
		float m_DownforceModifier;
		float m_PercentSubmerged;
		Math::Vector3_<float> m_VecCentreOfMassOffset;
		Math::Vector3_<float> m_VecInertiaMultiplier;
	};

	struct TransmissionRestore {
		float m_DriveBiasFront;
		uint8_t m_InitialDriveGears;
		float m_InitialDriveForce;
		float m_DriveInertia;
		float m_InitialDriveMaxFlatVel;
		float m_ClutchChangeRateScaleUpShift;
		float m_ClutchChangeRateScaleDownShift;
		float m_BrakeForce;
		float m_HandBrakeForce;
		float m_SteeringLock;
	};

	struct WheelTractionRestore {
		float m_TractionCurveMax;
		float m_TractionSpringDeltaMax;
		float m_LowSpeedTractionLossMult;
		float m_CamberStiffnesss;
		float m_TractionLossMult;
	};

	struct SuspensionRestore {
		float m_SuspensionForce;
		float m_SuspensionCompDamp;
		float m_SuspensionReboundDamp;
		float m_SuspensionUpperLimit;
		float m_SuspensionLowerLimit;
		float m_AntiRollBarForce;
		float m_RollCentreHeightFront;
		float m_RollCentreHeightRear;
	};

	struct DamageRestore {
		float m_CollisionDamageMult;
		float m_WeaponDamageMult;
		float m_DeformationDamageMult;
		float m_EngineDamageMult;
		float m_PetrolTankVolume;
		float m_OilVolume;
	};

	struct MiscRestore {
		float m_OilVolume;
		Math::Vector3_<float> m_SeatOffset;
		int m_MonetaryValue;
	};

	struct Vars_ {
		int m_Type;
		int m_DriveBiasFront;

		std::unordered_map<uint32_t, PhysicalRestore> m_PhysicalRestore;
		std::unordered_map<uint32_t, TransmissionRestore> m_TransmissionRestore;
		std::unordered_map<uint32_t, WheelTractionRestore> m_WheelTractionRestore;
		std::unordered_map<uint32_t, SuspensionRestore> m_SuspensionRestore;
		std::unordered_map<uint32_t, DamageRestore> m_DamageRestore;
		std::unordered_map<uint32_t, MiscRestore> m_MiscRestore;
	};

	void CachePhysical(uint32_t Model);
	void CacheTransmission(uint32_t Model);
	void CacheWheelTraction(uint32_t Model);
	void CacheSuspension(uint32_t Model);
	void CacheDamage(uint32_t Model);
	void CacheMisc(uint32_t Model);
	extern Vars_ m_Vars;
}