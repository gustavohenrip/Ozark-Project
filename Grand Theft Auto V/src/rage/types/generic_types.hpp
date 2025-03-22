#pragma once
#include "stdafx.hpp"
#include "utils/math.hpp"
#include "network_types.hpp"

namespace Rage::Types {
	struct Ped;
	struct AmmoInfo;
	struct AimingInfo;
	struct Weapon;
	struct BaseModelInfo;
	struct VehicleModelInfo;

	struct PedModelInfo {};
	struct PedDrawHandler {};
	struct PedInventory {};

	struct XmlNode {
		const char* m_Name;
		char _0x0008[0x20];
		XmlNode* m_Next;
	};

	struct WaypointData {
		Math::Vector4<float>* m_Points;
		char _0x0008[0x3C];
		uint32_t m_Count;
	};

	struct DictionaryItem { // grcTexture
		char _0x0000[0x28];
		const char* m_Name = nullptr;
		char _0x0030[0x30];
		DictionaryItem* m_Previous;
		DictionaryItem* m_Next;
		char _0x0070[0x20];
	};

	struct Dictionary {
		char _0x0000[0x28];
		short m_Count;
		char _0x002A[0x6];
		DictionaryItem** m_Items;
	};

	struct DictionaryPool {
		Dictionary* m_Pool;
		char _0x0008[0x4];
		uint32_t m_DictionaryHash;
		char _0x0010[0x8];
	};

	struct TxdStore {
		char _0x0000[0x38];
		DictionaryPool* m_Pool;
		uint8_t* m_Mask;
		uint32_t m_Count;
		uint32_t m_Size;

		uint64_t GetTextures() {
			return *(uint64_t*)((uint64_t)this + 0x70);
		}

		uint16_t GetTextureSize() {
			return *(uint16_t*)((uint64_t)this + 0x82);
		}
	};

	struct GenericPool {
		uint64_t m_Address;
		uint64_t m_MaskArray;
		uint32_t m_Count;
		uint32_t m_Size;

		inline uint64_t GetEntity(uint32_t Index) {
			if (IsValidPtr(m_MaskArray) && ~(*(uint8_t*)(m_MaskArray + Index) >> 7) & 1) {
				return (m_Address + (Index * m_Size));
			}

			return 0;
		}
	};

	struct VehicleEntityPool {
		uint64_t* m_Address;
		uint32_t m_Size;
		char _0x000C[0x4];
		uint32_t m_Count;

		inline uint64_t GetEntity(uint32_t Index) {
			if (!m_Address) return 0;
			return m_Address[Index];
		}
	};

	struct EntityPool {
		char _0x0000[0x10];
		uint32_t m_Num1;
		char _0x000C[0xC];
		uint32_t m_Num2;

		inline bool IsFull() {
			return m_Num1 - (m_Num2 & 0x3FFFFFFF) <= 256;
		}
	};

	struct UIElementVector {
		int m_Index;
		char _0x0004[0xC];
		Math::Vector4<float> m_Value;
	};

	struct UIElementVectorConstant {
		Math::Vector4<float> m_Value;
		char _0x0004[0xC];
		int m_Index;
	};

	struct UIIndex {
		int m_Index;
	};

	struct UIElementInteger {
		UIIndex m_Index;
		int m_Value;
	};

	struct UIElementShader {
		uint64_t m_RawElement;
		DictionaryItem* m_Shader;
	};

	struct UIVisualSettings {
		char _0x0000[0x2874];
		float m_RimLight;
		float m_GlobalEnvironmentReflection;
		float m_Gamma;
		float m_MidBlur;
		float m_FarBlur;
		float m_SkyMultiplier;
		float m_Desaturation;
	};

	struct UIWeather {
		char _0x0000[0x10];
		UIElementVector m_AzimuthEastColor;
		char _0x0030[0x10];
		UIElementVector m_AzimuthWestColor;
		char _0x0060[0x10];
		UIElementVector m_AzimuthTransitionColor;
		char _0x0090[0x4];
		UIElementInteger m_AzimuthTransitionPosition;
		char _0x009c[0x4];
		UIElementVector m_ZenithColor;
		char _0x00c0[0x10];
		UIElementVector m_ZenithTransitionColor;
		char _0x00f0[0x4];
		UIElementVectorConstant m_ZenithConstants;
		char _0x0114[0x1C];
		UIElementVector m_SkyPlaneColor;
		char _0x0150[0x10];
		UIIndex m_SkyPlaneParams;
		char _0x0164[0x34];
		UIIndex m_SunDirection;
		UIIndex m_SunPosition;
		char _0x01a0[0x10];
		UIElementVector m_SunColor;
		UIElementVector m_SunColorHdr;
		char _0x01f0[0x10];
		UIElementVector m_SunDiscColorHdr;
		char _0x0220[0x20];
		UIElementVector m_SunConstants;
		UIElementShader m_HighDetailNoise;
		UIElementVectorConstant m_CloudConstant1;
		char _0x0290[0x10];
		UIElementVectorConstant m_CloudConstant2;
		char _0x02c0[0x10];
		UIElementVectorConstant m_CloudConstant3;
		char _0x02f0[0x10];
		UIElementVectorConstant m_CloudDetailConstants;
		char _0x0320[0x30];
		UIElementVector m_CloudBaseMinusMidColour;
		UIElementVector m_CloudMidColor;
		UIElementVector m_CloudShadowMinusBaseColourTimesShadowStrength;
		UIElementVectorConstant m_SmallCloudConstants;
		char _0x03d0[0x20];
		UIElementVector m_SmallCloudColorHdr;
		char _0x0410[0x10];
		UIIndex m_CloudgenFrequency;
		char _0x0424[0x1C];
		UIElementVector m_NoisePhase;
		char _0x0460[0x20];
		UIIndex m_SpeedConstants;
		char _0x0484[0x1C];
		UIElementInteger m_HorizonLevel;
		char _0x04a8[0x10];
		UIElementShader m_Dither;
		char _0x04c8[0x18];
		UIElementShader m_StarField;
		UIElementInteger m_StarfieldIntensity;
		UIElementShader m_Moon;
		char _0x0508[0x28];
		UIElementInteger m_MoonIntensity;
		char _0x0538[0x8];
		UIElementVector m_MoonColor;
		char _0x0560[0x10];
		UIIndex m_LunarCycle;
		char _0x0574[0x1C];
		UIIndex m_MoonDirection;
		UIIndex m_MoonPosition;
		char _0x0598[0x18];
		UIElementInteger m_NoiseFrequency;
		UIElementInteger m_NoiseScale;
		UIElementInteger m_NoiseThreshold;
		UIElementInteger m_NoiseSoftness;
		UIElementInteger m_NoiseDensityOffset;
		UIElementShader m_Noise;

		void SetSkyEnabled(bool Enabled) {
			*(bool*)((uint64_t)this - 0x60) = Enabled;
		}
	};

	struct UIPuddleRipple {
		char _0x0000[0xF50];
		float m_RippleMaxSize;
		float m_RippleMinSize;
		float m_RippleMinDuration;
		float m_RippleMaxDuration;
		float m_RippleSpeed;
		float m_RippleDropFactor;
		float m_RippleWindStrength;
		char _0x0f6c[0x4];
		float m_RippleStrength;
		char _0x0f74[0x14];
		float m_RipplePlayerMapSize;
		float m_RipplePlayerSpeed;
		char _0x0f90[0x4];
		float m_RippleWindDamping;
	};

	struct UIPuddle {
		char _0x0000[0x70];
		float m_Scale;
		float m_Range;
		float m_Depth;
		float m_DepthTest;
		float m_Reflectivity;
		char _0x0084[0x2C];
		UIPuddleRipple* m_Ripples;
	};

	struct UIElementColorStruct {
		float m_Intensity;
		float m_Radius;
		float m_FalloffExp;
		float m_InnerConeAngle;
		float m_OuterConeAngle;
		float m_CoronaHDR;
		float m_CoronaSize;
		float m_ShadowBlur;
		float m_CapsuleLength;
		char _0x0024[0xC];
		Math::Vector3_<float> m_Color;
		char _0x003C[0x4];
		uint32_t m_Flags; // 0x200 = using sun, 0x140 = using dynamic shadows
	};

	struct VehicleStreamRender {
		char _0x0000[0x8];
		float m_TireSize;
		char _0x000C[0xB94];
		float m_TireWidth;
	};

	struct VehicleDrawHandler {
		char _0x0000[0x370]; //0x0000
		VehicleStreamRender* m_VehicleStreamRender; //0x0370 
		char _0x0378[0x8B0]; //0x0378
	};

	struct FlyingHandlingData {
		char _0x0000[0x8]; //0x0000
		float m_Thrust; //0x0008 
		float m_ThrustFallOff; //0x000C 
		float m_ThrustVectoring; //0x0010 
		char _0x0014[0x8]; //0x0014
		float m_YawMult; //0x001C 
		float m_YawStabilise; //0x0020 
		float m_SideSlipMult; //0x0024 
		char _0x0028[0x4]; //0x0028
		float m_RollMult; //0x002C 
		float m_RollStabilise; //0x0030 
		float N000026D3; //0x0034 
		float m_PitchMult; //0x0038 
		float m_PitchStabilise; //0x003C 
		char _0x0040[0x4]; //0x0040
		float m_FormLiftMult; //0x0044 
		float m_AttackLiftMult; //0x0048 
		float m_AttackDiveMult; //0x004C 
		float m_GearDownDragV; //0x0050 
		float m_GearDownLiftMult; //0x0054 
		float m_WindMult; //0x0058 
		float m_MoveRes; //0x005C 
		Math::Vector3_<float> m_VecTurnResistance; //0x0060 
		char _0x006C[0x4]; //0x006C
		Math::Vector3_<float> m_VecSpeedResistance; //0x0070 
		char _0x007C[0x4]; //0x007C
		float m_GearDoorFrontOpen; //0x0080 
		float m_GearDoorRearOpen; //0x0084 
		float m_GearDoorRearOpen2; //0x0088 
		float m_GearDoorRearMOpen; //0x008C 
		float m_TurublenceMagnitudeMax; //0x0090 
		float m_TurublenceForceMulti; //0x0094 
		float m_TurublenceRollTorqueMulti; //0x0098 
		float m_TurublencePitchTorqueMulti; //0x009C 
		float m_BodyDamageControlEffectMult; //0x00A0 
		float m_InputSensitivityForDifficulty; //0x00A4 
		float m_OnGroundYawBoostSpeedPeak; //0x00A8 
		float m_OnGroundYawBoostSpeedCap; //0x00AC 
		float m_EngineOffGlideMulti; //0x00B0 
	}; //Size=0x00B4

	struct BoatHandlingData {
		char _0x0000[0x8]; //0x0000
		float m_BoxFrontMult; //0x0008 
		float m_BoxRearMult; //0x000C 
		float m_BoxSideMult; //0x0010 
		float m_SampleTop; //0x0014 
		float m_SampleBottom; //0x0018 
		float N000028C4; //0x001C 
		float m_AquaplaneForce; //0x0020 
		float m_AquaplanePushWaterMult; //0x0024 
		float m_AquaplanePushWaterCap; //0x0028 
		float m_AquaplanePushWaterApply; //0x002C 
		float m_RudderForce; //0x0030 
		float m_RudderOffsetSubmerge; //0x0034 
		float m_RudderOffsetForce; //0x0038 
		float m_RudderOffsetForceZMult; //0x003C 
		float m_WaveAudioMult; //0x0040 
		float N000028CE; //0x0044 
		float N0000283E; //0x0048 
		float N000028D0; //0x004C 
		Math::Vector3_<float> m_VecMoveResistance; //0x0050 
		float N000028D4; //0x005C 
		Math::Vector3_<float> m_VecTurnResistance; //0x0060 
		float N000028D8; //0x006C 
		float m_Look_L_R_CamHeight; //0x0070 
		float m_DragCoefficient; //0x0074 
		float m_KeelSphereSize; //0x0078 
		float m_PropRadius; //0x007C 
		float m_LowLodAngOffset; //0x0080 
		float m_LowLodDraughtOffset; //0x0084 
		float m_ImpellerOffset; //0x0088 
		float m_ImpellerForceMult; //0x008C 
		float m_DinghySphereBuoyConst; //0x0090 
		float m_ProwRaiseMult; //0x0094 
	}; //Size=0x0098

	struct BikeHandlingData {
		char _0x0000[0x8]; //0x0000
		float m_LeanFwdCOMMult; //0x0008 
		float m_LeanFwdForceMult; //0x000C 
		float m_LeanBakCOMMult; //0x0010 
		float m_LeanBakForceMult; //0x0014 
		float m_MaxBankAngle; //0x0018 
		char _0x001C[0x4]; //0x001C
		float m_FullAnimAngle; //0x0020 
		float m_DesLeanReturnFrac; //0x0024 
		float m_StickLeanMult; //0x0028 
		float m_BrakingStabilityMult; //0x002C 
		float m_InAirSteerMult; //0x0030 
		float m_WheelieBalancePoint; //0x0034 
		float m_StoppieBalancePoint; //0x0038 
		float m_WheelieSteerMult; //0x003C 
		float m_RearBalanceMult; //0x0040 
		float m_FrontBalanceMult; //0x0044 
		float m_BikeGroundSideFrictionMult; //0x0048 
		float m_BikeWheelGroundSideFrictionMult; //0x004C 
		float m_BikeOnStandLeanAngle; //0x0050 c
		float m_BikeOnStandSteerAngle; //0x0054 
		float m_JumpForce; //0x0058 
	}; //Size=0x005C

	typedef union {
		FlyingHandlingData m_FlyingHandlingData;
		BikeHandlingData m_BikeHandlingData;
		BoatHandlingData m_BoatHandlingData;
	} CSubHandlingDataType;

	struct SubHandlingData {
		CSubHandlingDataType* m_SubHandlingData; //0x0000 
		void* m_VehicleWeaponHandlingData; //0x0008 
		char _0x0010[0x8]; //0x0010
	};

	struct HandlingData {
		char _0x0000[0x8]; //0x0000
		int m_Hash; //0x0008 
		float m_Mass; //0x000C 
		char _0x0010[0x4]; //0x0010
		float m_DownforceModifier; //0x0014 
		char _0x0018[0x8]; //0x0018
		Math::Vector3_<float> m_VecCentreOfMassOffset; //0x0020 
		char _0x002C[0x4]; //0x002C
		Math::Vector3_<float> m_VecInertiaMultiplier; //0x0030 
		char _0x003C[0x4]; //0x003C
		float m_PercentSubmerged; //0x0040 
		char _0x0044[0x8]; //0x0044
		float m_DriveBiasFront; //0x004C 
		unsigned char m_InitialDriveGears; //0x0050 
		char _0x0051[0x3]; //0x0051
		float m_DriveInertia; //0x0054 
		float m_ClutchChangeRateScaleUpShift; //0x0058 
		float m_ClutchChangeRateScaleDownShift; //0x005C 
		float m_InitialDriveForce; //0x0060 
		char _0x0064[0x8]; //0x0064
		float m_BrakeForce; //0x006C 
		char _0x0070[0x4]; //0x0070
		float m_InitialDriveMaxFlatVel; //0x0074 
		char _0x0078[0x4]; //0x0078
		float m_HandBrakeForce; //0x007C 
		float m_SteeringLock; //0x0080 
		char _0x0084[0x4]; //0x0084
		float m_TractionCurveMax; //0x0088 
		char _0x008C[0x14]; //0x008C
		float m_TractionSpringDeltaMax; //0x00A0 
		char _0x00A4[0x4]; //0x00A4
		float m_LowSpeedTractionLossMult; //0x00A8 
		float m_CamberStiffnesss; //0x00AC 
		char _0x00B0[0x8]; //0x00B0
		float m_TractionLossMult; //0x00B8 
		float m_SuspensionForce; //0x00BC 
		float m_SuspensionCompDamp; //0x00C0 
		float m_SuspensionReboundDamp; //0x00C4 
		float m_SuspensionUpperLimit; //0x00C8 
		float m_SuspensionLowerLimit; //0x00CC 
		char _0x00D0[0xC]; //0x00D0
		float m_AntiRollBarForce; //0x00DC 
		char _0x00E0[0x8]; //0x00E0
		float m_RollCentreHeightFront; //0x00E8 
		float m_RollCentreHeightRear; //0x00EC 
		float m_CollisionDamageMult; //0x00F0 
		float m_WeaponDamageMult; //0x00F4 
		float m_DeformationDamageMult; //0x00F8 
		float m_EngineDamageMult; //0x00FC 
		float m_PetrolTankVolume; //0x0100 
		float m_OilVolume; //0x0104 
		char _0x0108[0x4]; //0x0108
		Math::Vector3_<float> m_SeatOffset; //0x010C 
		int m_MonetaryValue; //0x0118 
		char _0x011C[0x30]; //0x011C
		unsigned char N00002337; //0x014C 
		unsigned char N0000239F; //0x014D 
		char _0x014E[0x2]; //0x014E
		void* m_AIHandlingInfo; //0x0150 
		SubHandlingData* m_SubHandlingData; //0x0158 
	}; //Size=0x0160

	struct Vehicle {
		char _0x0000[0x20]; //0x0000
		VehicleModelInfo* m_ModelInfo;
		char _0x0028[0x8];
		void* m_FragInst; //0x0030 
		char _0x0038[0x10]; //0x0038
		VehicleDrawHandler* m_VehicleDrawHandler; //0x0048 
		char _0x0050[0x230]; //0x0050
		float m_Health; //0x0280 
		char _0x0284[0x1C]; //0x0284
		float m_MaxHealth; //0x02A0 
		char _0x02A4[0x77]; //0x02A4
		unsigned char m_RocketState; //0x031B 
		char _0x031C[0x1]; //0x031C
		unsigned char m_RocketEnergy; //0x031D 
		char _0x031E[0x5FA + 0x20]; //0x031E
		HandlingData* m_HandlingData; //0x0918 - NOW 0x938
		char _0x0920[0x48]; //0x0920
		Vehicle* m_Vehicle; //0x0968 
		char _0x0970[0x8]; //0x0970
		void* m_VehicleDeformation; //0x0978 
		char _0x0980[0x101C]; //0x0980
		float m_VisualSuspension; //0x199C 
		char _0x19A0[0x240]; //0x19A0

		Math::Matrix* GetMatrix() {
			return (Math::Matrix*)((uint64_t)this + 0x60);
		}
	};

	struct Object {
		char _0x0000[0x7C];
		float m_Width;
		char _0x0080[0xC];
		float m_Height;
	};

	struct PedWeaponManager {
		char _0x0000[0x20]; //0x0000
		Rage::Types::Weapon* m_WeaponInfo; //0x0020 
		char _0x0028[0x178]; //0x0028
		Math::Vector3_<float> m_LastAimedPosition; //0x01A0 
		char _0x01AC[0x4]; //0x01AC
		Math::Vector3_<float> m_ImpactPosition; //0x01B0 
		char _0x01BC[0x2E4]; //0x01BC
	};

	struct Navigation {
		char _0x0000[0x20];
		float m_LastHeading;
		float m_Heading;
		char _0x0028[0x8];
		Math::Vector3_<float> m_Rotation;
		char _0x003C[0x14];
		Math::Vector3_<float> m_Position;
	};

	struct Ped {
		char _0x0000[0x10];
		uint64_t m_DecorInfo;
		char _0x0018[0x8];
		PedModelInfo* m_PedModelInfo;
		char _0x0028[0x8];
		Navigation* m_Navigation;
		char _0x0038[0x10];
		PedDrawHandler* m_PedDrawHandler; // 0x48
		char _0x0050[0x10];
		Math::Matrix m_Transform;
		char _0x00A0[0x1E0];
		float m_Health;
		char _0x0284[0x1C];
		float m_MaxHealth;
		char _0x02A4[0xA8C]; // 0xA8C
		Vehicle* m_Vehicle;
		char _0x0D30[0x390];
		Rage::Network::PlayerInfo* m_PlayerInfo;
		PedInventory* m_PedInventory;
		PedWeaponManager* m_PedWeaponManager;

		Rage::Network::NetObj* GetNetObj() {
			return *(Rage::Network::NetObj**)((uint64_t)this + 0xD0);
		}

		void SetFlags(uint32_t Flag) {
			*(uint32_t*)((uint64_t)this + 0x141C) = Flag;
		}
	};

	struct PedFactory {
		char _0x0000[8];
		Ped* m_LocalPed;
	};

	struct AmmoInfo {
		char _0x0008[0x10]; //0x0008
		uint32_t m_AmmoHash; //0x0010
		uint32_t m_Name; //0x0014
		uint32_t m_Model; //0x0018
		uint32_t m_Slot; //0x001C
		unsigned char m_AmmoMax; //0x0020 
		char _0x0021[0x3]; //0x0021
		unsigned char m_AmmoMax50; //0x0024 
		char _0x0025[0x3]; //0x0025
		unsigned char m_AmmoMax100; //0x0028 
		char _0x0029[0x3]; //0x0029
		unsigned char m_AmmoMaxMP; //0x002C 
		char _0x002D[0x3]; //0x002D
		unsigned char m_AmmoMax50MP; //0x0030 
		char _0x0031[0x3]; //0x0031
		unsigned char m_AmmoMax100MP; //0x0034 
		char _0x0035[0xB]; //0x0035
		float m_Damage; //0x0040 
		float m_LifeTime; //0x0044 
		float m_FromVehicleLifeTime; //0x0048 
		float m_LifeTimeAfterImpact; //0x004C 
		float m_LifeTimeAfterExplosion; //0x0050 
		float m_ExplosionTime; //0x0054 
		float m_LaunchSpeed; //0x0058 
		float m_SeparationTime; //0x005C 
		float m_TimeToReachTarget; //0x0060 
		float m_Damping; //0x0064 
		float m_GravityFactor; //0x0068 
		float m_RicochetTolerance; //0x006C 
		float m_PedRicochetTolerance; //0x0070 
		float m_VehicleRicochetTolerance; //0x0074 
		float m_FrictionMultiplier; //0x0078 
		char _0x007C[0x64]; //0x007C
		Math::Vector3_<float> m_LightColor; //0x00E0 
		char _0x00EC[0x4]; //0x00EC
		float m_LightIntensity; //0x00F0 
		float m_LightRange; //0x00F4 
		float m_LightFalloffExp; //0x00F8 
		float m_LightFrequency; //0x00FC 
		float m_LightPower; //0x0100 
		float m_CoronaSize; //0x0104 
		float m_CoronaIntensity; //0x0108 
		float m_CoronaZBias; //0x010C 
		char _0x0110[0x40]; //0x0110
		unsigned char m_AmmoMaxMPBonus; //0x0150 
		char _0x0151[0x7]; //0x0151
	};

	struct AimingInfo {
		int m_Hash; //0x0000 
		float m_HeadingLimit; //0x0004 
		float m_SweepPitchMin; //0x0008 
		float m_SweepPitchMax; //0x000C 
	};


	struct Weapon {
		uint64_t m_Vmt; //0x0000
		char pad_0008[8]; //0x0008
		uint32_t m_NameHash; //0x0010
		uint32_t m_ModelHash; //0x0014
		uint32_t m_Audio; //0x0018
		uint32_t m_Slot; //0x001C
		int32_t m_DamageType; //0x0020
		uint32_t m_ExplosionDefault; //0x0024
		uint32_t m_ExplosionHitCar; //0x0028
		uint32_t m_ExplosionHitTruck; //0x002C
		uint32_t m_ExplosionHitBike; //0x0030
		uint32_t m_ExplosionHitBoat; //0x0034
		uint32_t m_ExplosionHitPlane; //0x0038
		char pad_003C[24]; //0x003C
		int32_t m_FireType; //0x0054
		int32_t m_WheelSlot; //0x0058
		uint32_t m_GroupHash; //0x005C
		Rage::Types::AmmoInfo* m_AmmoInfo; //0x0060
		Rage::Types::AimingInfo* m_AimingInfo; //0x0068
		uint32_t m_ClipSize; //0x0070
		float m_AccuracySpread; //0x0074
		float m_AccurateModeAccuracyModifier; //0x0078
		float m_RunAndGunAccuracyModifier; //0x007C
		float m_RunAndGunAccuracyMaxModifier; //0x0080
		float m_RecoilAccuracyMax; //0x0084
		float m_RecoilErrorTime; //0x0088
		float m_RecoilRecoveryRate; //0x008C
		float m_RecoilAccuracyToAllowHeadShotAI; //0x0090
		float m_MinHeadShotDistanceAI; //0x0094
		float m_MaxHeadShotDistanceAI; //0x0098
		float m_HeadShotDamageModifierAI; //0x009C
		float m_RecoilAccuracyToAllowHeadShotPlayer; //0x00A0
		float m_MinHeadShotDistancePlayer; //0x00A4
		float m_MaxHeadShotDistancePlayer; //0x00A8
		float m_HeadShotDamageModifierPlayer; //0x00AC
		float m_Damage; //0x00B0
		float m_DamageTime; //0x00B4
		float m_DamageTimeInVehicle; //0x00B8
		float m_DamageTimeInVehicleHeadShot; //0x00BC
		char pad_00C0[8]; //0x00C0
		float m_HitLimbsDamageModifier; //0x00C8
		float m_NetworkHitLimbsDamageModifier; //0x00CC
		float m_LightlyArmouredDamageModifier; //0x00D0
		float m_VehicleDamageModifier; //0x00D4
		float m_Force; //0x00D8
		float m_ForceHitPed; //0x00DC
		float m_ForceHitVehicle; //0x00E0
		float m_ForceHitFlyingHeli; //0x00E4
		uint64_t m_OverrideForces; //0x00E8
		int16_t m_OverrideForcesCount; //0x00F0
		int16_t m_OverrideForcesCapacity; //0x00F2
		char pad_00F4[4]; //0x00F4
		float m_ForceMaxStrengthMult; //0x00F8
		float m_ForceFalloffRangeStart; //0x00FC
		float m_ForceFalloffRangeEnd; //0x0100
		float m_ForceFalloffMin; //0x0104
		float m_ProjectileForce; //0x0108
		float m_FragImpulse; //0x010C
		float m_Penetration; //0x0110
		float m_VerticalLaunchAdjustment; //0x0114
		float m_DropForwardVelocity; //0x0118
		float m_Speed; //0x011C
		int32_t m_BulletsInBatch; //0x0120
		float m_BatchSpread; //0x0124
		float m_ReloadTimeMP; //0x0128
		float m_ReloadTimeSP; //0x012C
		float m_VehicleReloadTime; //0x0130
		float m_AnimReloadRate; //0x0134
		int32_t m_BulletsPerAnimLoop; //0x0138
		float m_TimeBetweenShots; //0x013C
		float m_TimeLeftBetweenShotsWhereShouldFireIsCached; //0x0140
		float m_SpinUpTime; //0x0144
		float m_SpinTime; //0x0148
		float m_SpinDownTime; //0x014C
		float m_AlternateWaitTime; //0x0150
		float m_BulletBendingNearRadius; //0x0154
		float m_BulletBendingFarRadius; //0x0158
		float m_BulletBendingZoomedRadius; //0x015C
		float m_FirstPersonBulletBendingNearRadius; //0x0160
		float m_FirstPersonBulletBendingFarRadius; //0x0164
		float m_FirstPersonBulletBendingZoomedRadius; //0x0168
		char pad_016C[4]; //0x016C
		int32_t m_EffectGroup; //0x0170
		uint32_t m_FlashFx; //0x0174
		uint32_t m_FlashFxAlt; //0x0178
		uint32_t m_FlashFxFP; //0x017C
		uint32_t m_FlashFxAltFP; //0x0180
		uint32_t m_MuzzleSmokeFx; //0x0184
		uint32_t m_MuzzleSmokeFxFP; //0x0188
		float m_MuzzleSmokeFxMinLevel; //0x018C
		float m_MuzzleSmokeFxIncPerShot; //0x0190
		float m_MuzzleSmokeFxDecPerSec; //0x0194
		char pad_0198[28]; //0x0198
		uint32_t m_ShellFx; //0x01B4
		uint32_t m_ShellFxFP; //0x01B8
		uint32_t m_TracerFx; //0x01BC
		uint32_t m_PedDamageHash; //0x01C0
		float m_TracerFxChanceSP; //0x01C4
		float m_TracerFxChanceMP; //0x01C8
		char pad_01CC[4]; //0x01CC
		float m_FlashFxChanceSP; //0x01D0
		float m_FlashFxChanceMP; //0x01D4
		float m_FlashFxAltChance; //0x01D8
		float m_FlashFxScale; //0x01DC
		char pad_01E0[76]; //0x01E0
		float m_GroundDisturbFxDist; //0x022C
		char pad_0230[32]; //0x0230
		int32_t m_InitialRumbleDuration; //0x0250
		float m_InitialRumbleIntensity; //0x0254
		float m_InitialRumbleIntensityTrigger; //0x0258
		int32_t m_RumbleDuration; //0x025C
		float m_RumbleIntensity; //0x0260
		float m_RumbleIntensityTrigger; //0x0264
		float m_RumbleDamageIntensity; //0x0268
		int32_t m_InitialRumbleDurationFps; //0x026C
		float m_InitialRumbleIntensityFps; //0x0270
		int32_t m_RumbleDurationFps; //0x0274
		float m_RumbleIntensityFps; //0x0278
		float m_NetworkPlayerDamageModifier; //0x027C
		float m_NetworkPedDamageModifier; //0x0280
		float m_NetworkHeadShotPlayerDamageModifier; //0x0284
		float m_LockOnRange; //0x0288
		float m_WeaponRange; //0x028C
		float m_AiSoundRange; //0x0290
		float m_AiPotentialBlastEventRange; //0x0294
		float m_DamageFallOffRangeMin; //0x0298
		float m_DamageFallOffRangeMax; //0x029C
		char pad_02A0[8]; //0x02A0
		float m_DamageFallOffModifier; //0x02A8
		char pad_02AC[4]; //0x02AC
		float m_DamageFallOffModifier1; //0x02B0
		char pad_02B4[60]; //0x02B4
		int32_t m_MinTimeBetweenRecoilShakes; //0x02F0
		float m_RecoilShakeAmptitude; //0x02F4
		float m_ExplosionShakeAmplitude; //0x02F8
		float m_CameraFov; //0x02FC
		char pad_0300[8]; //0x0300
		float m_FirstPersonScopeFov; //0x0308
		float m_FirstPersonScopeAttachmentFov; //0x030C
		char pad_0310[736]; //0x0310
		char* m_LabelText; //0x05F0
		char pad_05F8[872]; //0x05F8
	};

	struct ExplosionData {
		const char* m_ExplosionName;
		int m_Unk1;
		int m_Unk2;
		float m_DamageAtCentre;
		float m_DamageAtEdge;
		float m_NetworkPlayerModifier;
		float m_NetworkPedModifier;
		float m_EndRadius;
		float m_InitSpeed;
		float m_DecayFactor;
		float m_ForceFactor;
		float m_RagdollForceModifier;
		float m_SelfForceModifier;
		float m_DirectedWidth;
		float m_DirectedLifeTime;
		float m_CamShake;
		float m_CamShakeRollOffScaling;
		float m_ShockingEventVisualRangeOverride;
		float m_ShockingEventAudioRangeOverride;
		float m_FragDamage;
		bool m_MinorExplosion;
		bool m_AppliesContinuousDamage;
		bool m_PostProcessCollisionsWithNoForce;
		bool m_DamageVehicles;
		bool m_DamageObjects;
		bool m_OnlyAffectsLivePeds;
		bool m_IgnoreExplodingEntity;
		bool m_NoOcclusion;
		bool m_ExplodeAttachEntityWhenFinished;
		bool m_CanSetPedOnFire;
		bool m_CanSetPlayerOnFire;
		bool m_SuppressCrime;
		bool m_UseDistanceDamageCalc;
		bool m_PreventWaterExplosionVFX;
		bool m_IgnoreRatioCheckForFire;
		bool m_AllowUnderwaterExplosion;
		int m_Unk3;
		float m_Unk4;
		float m_Unk5;
		char m_Unk6[12];
		int m_CamShakeNameHash;
		int m_VFXTagNameHash;
		int m_UnkSize;
	};

	struct ExplosionFX {
		uint32_t m_NameHash;
		char Unk[0x20];
		float m_Scale;
		char Unk2[0x38];
	};

	struct Blip {
		char _0x0000[0x4];
		int m_Handle;
		char _0x0008[0x8];
		Math::Vector3_<float> m_Coords;
		char _0x001C[0x24];
		int m_Sprite;
		char _0x0044[0x4];
		int m_Color;
		int m_Color2;
		Math::Vector2<float> m_Scale;
		float m_Rotation;
	};

	struct BlipList {
		Blip* m_Blips[1500];
	};

	struct OceanWaveQuad {
		Math::Vector2<short> m_Min;
		Math::Vector2<short> m_Max;
		short m_Amplitude;
		Math::Vector2<uint8_t> m_Direction;
	};

	struct OceanCalmingQuad {
		Math::Vector2<short> m_Min;
		Math::Vector2<short> m_Max;
		float m_Dampening;
	};

	struct OceanQuad {
		Math::Vector2<short> m_Min;
		Math::Vector2<short> m_Max;
		uint32_t m_Alpha;
		char _0x000C[0x8];
		float m_Height;
		char _0x0024[0x4];
	};

	struct OceanQuadInfo {
		uint64_t m_QuadPool;
		short m_QuadCount;
	};

	struct OceanQuads {
		OceanQuadInfo* m_Ocean;
		OceanQuadInfo* m_Wave;
		OceanQuadInfo* m_Calming;
	};

	struct WaterTune {
		int m_WaterColor;
		float m_RippleScale;
		float m_OceanFoamScale;
		float m_SpecularFalloff;
		float m_FogPierceIntensity;
		float m_RefractionBlend;
		float m_RefractionExponent;
		float m_WaterCycleDepth;
		float m_WaterCycleFade;
		float m_WaterLightningDepth;
		float m_WaterLightningFade;
		float m_DeepWaterModDepth;
		float m_DeepWaterModFade;
		float m_GodRaysLerpStart;
		float m_GodRaysLerpEnd;
		float m_DisturbFoamScale;
		Math::Vector2<float> m_FogMin;
		Math::Vector2<float> m_FogMax;
	};

	struct BaseModelInfo {
		char _0x0000[24];
		uint32_t m_Model;
	};
	struct Building {
		char _0x0000[32];
		BaseModelInfo* m_ModelInfo;
		uint8_t m_Type; // 1
		char _0x0029[55];
		Math::Matrix m_Matrix;
		char _0x00A0[16];
		int m_Lod;
	};

	struct PtxFxList {
		char _0x0000[0x10];
		const char* m_Name;
	};

	struct PtxEffectRule {
		char _0x0000[0x20];
		const char* m_DictName;
		PtxFxList* m_FxList;
	};

	struct ParticleFX {
		PtxEffectRule* m_Rule;
	};

	struct VfxLiquid {
		float m_DurationFoot;
		float m_DurationWheelIn;
		float m_LifeFoot;
		float m_LifeWheel;
		char _0x0010[0x48];
		uint8_t m_Red;
		uint8_t m_Green;
		uint8_t m_Blue;
		uint8_t m_Alpha;
		char _0x005C[0x4];
	};

	struct VfxWheel {
		float m_SlipMin; //0x0004
		float m_SlipMax; //0x0008
		float m_PressureMin; //0x000C
		float m_PressureMax; //0x0010
		int m_Type1; //0x0014
		BOOL m_Unk1; //0x0018
		int m_Type2; //0x001c
		BOOL m_Unk2; //0x0010
		int m_Type3; //0x0024
		BOOL m_Unk3; //0x0028
		int m_Type4; //0x002c
		BOOL m_Unk4; //0x0030
		uint8_t m_R; //0x0034
		uint8_t m_G; //0x0035
		uint8_t m_B; //0x0036
		char _0x0037[1]; //0x0037
		float m_FrictionThreshMin; //0x0038
		float m_FrictionThreshMax; //0x003C
		uint32_t m_FrictionFx1; //0x0040
		uint32_t m_FrictionFx2; //0x0044
		uint32_t m_FrictionFx3; //0x0048
		float m_DispThreshMin; //0x004C
		float m_DispThreshMax; //0x0050
		uint32_t m_DisplacementFx1; //0x0054
		uint32_t m_DisplacementFx2; //0x0058
		uint32_t m_DisplacementFx3; //0x005C
		uint32_t m_DisplacementFxLod; //0x0060
		float m_BurnFrictionEvoMin; //0x0064
		float m_BurnFrictionEvoMax; //0x0068
		float m_BurnTempEvoMin; //0x006C
		float m_BurnTempEvoMax; //0x0070
		uint32_t m_BurnoutFx1; //0x0074
		uint32_t m_BurnoutFx2; //0x0078
		uint32_t m_BurnoutFx3; //0x007C
		bool m_LightsOn; //0x0080
		uint8_t m_LightsColMinR; //0x0081
		uint8_t m_LightsColMinG; //0x0082
		uint8_t m_LightsColMinB; //0x0083
		uint8_t m_LightsColMaxR; //0x0084
		uint8_t m_LightsColMaxG; //0x0085
		uint8_t m_LightsColMaxB; //0x0086
		char _0x0087[1]; //0x0087
		float m_LightsIntensityMin; //0x0088
		float m_LightsIntensityMax; //0x008C
		float m_LightsRangeMin; //0x0090
		float m_LightsRangeMax; //0x0094
		float m_LightsFalloffMin; //0x0098
		float m_LightsFalloffMax; //0x009C
	};

	struct VehicleModelInfo {
		char _0x0000[0x298];
		char m_ModelName[0xC];
		char m_ManufacturerName[0xC];
		uint16_t* m_ModKits;
		uint16_t m_ModKitCount;

		uint32_t* GetCameraName() {
			return (uint32_t*)((uint64_t)this + 0x4D0);
		}

		uint32_t* GetAimCameraName() {
			return (uint32_t*)((uint64_t)this + 0x4D4);
		}

		uint32_t* GetBonnetCameraName() {
			return (uint32_t*)((uint64_t)this + 0x4D8);
		}
		uint32_t* GetPOVCameraName() {
			return (uint32_t*)((uint64_t)this + 0x4DC);
		}
	};
}