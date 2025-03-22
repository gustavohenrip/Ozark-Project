#include "vehicle_handling_save_load.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_handling.hpp"
#include "utils/va.hpp"
#include "utils/json.hpp"
#include "menu/base/util/notify.hpp"
#include "vehicle_handling_attributes.hpp"
#include "vehicle_handling_bike.hpp"
#include "vehicle_handling_boat.hpp"
#include "vehicle_handling_flying.hpp"
#include "utils/log.hpp"

using namespace VehicleModsHandlingSaveLoadMenuVars;

namespace VehicleModsHandlingSaveLoadMenuVars {
	Vars_ m_Vars;

	Rage::Types::HandlingData* GetDataMain() {
		static Rage::Types::HandlingData hd;

		if (Global::Vars::g_PedFactory) {
			if (Global::Vars::g_PedFactory->m_LocalPed) {
				if (Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle) {
					if (Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle->m_HandlingData) {
						return Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle->m_HandlingData;
					}
				}
			}
		}

		return &hd;
	}

	Rage::Types::BoatHandlingData* GetDataBoat() {
		static Rage::Types::BoatHandlingData hd;

		if (Global::Vars::g_PedFactory) {
			if (Global::Vars::g_PedFactory->m_LocalPed) {
				if (Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle) {
					if (Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle->m_HandlingData) {
						if (Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle->m_HandlingData->m_SubHandlingData) {
							if (Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle->m_HandlingData->m_SubHandlingData->m_SubHandlingData) {
								return &Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle->m_HandlingData->m_SubHandlingData->m_SubHandlingData->m_BoatHandlingData;
							}
						}
					}
				}
			}
		}

		return &hd;
	}

	Rage::Types::BikeHandlingData* GetDataBike() {
		static Rage::Types::BikeHandlingData hd;

		if (Global::Vars::g_PedFactory) {
			if (Global::Vars::g_PedFactory->m_LocalPed) {
				if (Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle) {
					if (Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle->m_HandlingData) {
						if (Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle->m_HandlingData->m_SubHandlingData) {
							if (Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle->m_HandlingData->m_SubHandlingData->m_SubHandlingData) {
								return &Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle->m_HandlingData->m_SubHandlingData->m_SubHandlingData->m_BikeHandlingData;
							}
						}
					}
				}
			}
		}

		return &hd;
	}

	Rage::Types::FlyingHandlingData* GetDataFlying() {
		static Rage::Types::FlyingHandlingData hd;

		if (Global::Vars::g_PedFactory) {
			if (Global::Vars::g_PedFactory->m_LocalPed) {
				if (Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle) {
					if (Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle->m_HandlingData) {
						if (Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle->m_HandlingData->m_SubHandlingData) {
							if (Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle->m_HandlingData->m_SubHandlingData->m_SubHandlingData) {
								return &Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle->m_HandlingData->m_SubHandlingData->m_SubHandlingData->m_FlyingHandlingData;
							}
						}
					}
				}
			}
		}

		return &hd;
	}

	void SaveHandling(const char* OutfitName) {
		VehicleModsHandlingMenu::GetInstance()->UpdateOnce(); // re-cache model check

		auto Location = Utils::VA::VA("%s%s.json", Utils::GetConfig()->GetHandlingPath().c_str(), OutfitName);

		try {
			std::ofstream Output(Location);
			nlohmann::json Object;
			
			Object["Type"] = 0;
			auto& Main = Object["Main"];
			auto& Physical = Main["Physical"];
			auto& Transmission = Main["Transmission"];
			auto& WheelTraction = Main["Wheel Traction"];
			auto& Suspension = Main["Suspension"];
			auto& Damage = Main["Damage"];
			auto& Miscellaneous = Main["Miscellaneous"];

			Physical["Mass"] = GetDataMain()->m_Mass;
			Physical["DownforceModifier"] = GetDataMain()->m_DownforceModifier;
			Physical["PercentSubmerged"] = GetDataMain()->m_PercentSubmerged;
			Physical["VecCentreOfMassOffset.X"] = GetDataMain()->m_VecCentreOfMassOffset.m_X;
			Physical["VecCentreOfMassOffset.Y"] = GetDataMain()->m_VecCentreOfMassOffset.m_Y;
			Physical["VecCentreOfMassOffset.Z"] = GetDataMain()->m_VecCentreOfMassOffset.m_Z;
			Physical["VecInertiaMultiplier.X"] = GetDataMain()->m_VecInertiaMultiplier.m_X;
			Physical["VecInertiaMultiplier.Y"] = GetDataMain()->m_VecInertiaMultiplier.m_Y;
			Physical["VecInertiaMultiplier.Z"] = GetDataMain()->m_VecInertiaMultiplier.m_Z;

			Transmission["DriveBiasFront"] = GetDataMain()->m_DriveBiasFront;
			Transmission["InitialDriveGears"] = GetDataMain()->m_InitialDriveGears;
			Transmission["InitialDriveForce"] = GetDataMain()->m_InitialDriveForce;
			Transmission["DriveInertia"] = GetDataMain()->m_DriveInertia;
			Transmission["InitialDriveMaxFlatVel"] = GetDataMain()->m_InitialDriveMaxFlatVel;
			Transmission["ClutchChangeRateScaleUpShift"] = GetDataMain()->m_ClutchChangeRateScaleUpShift;
			Transmission["ClutchChangeRateScaleDownShift"] = GetDataMain()->m_ClutchChangeRateScaleDownShift;
			Transmission["BrakeForce"] = GetDataMain()->m_BrakeForce;
			Transmission["HandBrakeForce"] = GetDataMain()->m_HandBrakeForce;
			Transmission["SteeringLock"] = GetDataMain()->m_SteeringLock;

			WheelTraction["TractionCurveMax"] = GetDataMain()->m_TractionCurveMax;
			WheelTraction["TractionSpringDeltaMax"] = GetDataMain()->m_TractionSpringDeltaMax;
			WheelTraction["LowSpeedTractionLossMult"] = GetDataMain()->m_LowSpeedTractionLossMult;
			WheelTraction["CamberStiffnesss"] = GetDataMain()->m_CamberStiffnesss;
			WheelTraction["TractionLossMult"] = GetDataMain()->m_TractionLossMult;

			Suspension["SuspensionForce"] = GetDataMain()->m_SuspensionForce;
			Suspension["SuspensionCompDamp"] = GetDataMain()->m_SuspensionCompDamp;
			Suspension["SuspensionReboundDamp"] = GetDataMain()->m_SuspensionReboundDamp;
			Suspension["SuspensionUpperLimit"] = GetDataMain()->m_SuspensionUpperLimit;
			Suspension["SuspensionLowerLimit"] = GetDataMain()->m_SuspensionLowerLimit;
			Suspension["AntiRollBarForce"] = GetDataMain()->m_AntiRollBarForce;
			Suspension["RollCentreHeightFront"] = GetDataMain()->m_RollCentreHeightFront;
			Suspension["RollCentreHeightRear"] = GetDataMain()->m_RollCentreHeightRear;

			Damage["CollisionDamageMult"] = GetDataMain()->m_CollisionDamageMult;
			Damage["WeaponDamageMult"] = GetDataMain()->m_WeaponDamageMult;
			Damage["DeformationDamageMult"] = GetDataMain()->m_DeformationDamageMult;
			Damage["EngineDamageMult"] = GetDataMain()->m_EngineDamageMult;
			Damage["PetrolTankVolume"] = GetDataMain()->m_PetrolTankVolume;

			Miscellaneous["OilVolume"] = GetDataMain()->m_OilVolume;
			Miscellaneous["SeatOffset.X"] = GetDataMain()->m_SeatOffset.m_X;
			Miscellaneous["SeatOffset.Y"] = GetDataMain()->m_SeatOffset.m_Y;
			Miscellaneous["SeatOffset.Z"] = GetDataMain()->m_SeatOffset.m_Z;
			Miscellaneous["MonetaryValue"] = GetDataMain()->m_MonetaryValue;

			if (VehicleModsHandlingMenuVars::m_Vars.m_InBoat) {
				Object["Type"] = 1;
				auto& Boat = Object["Boat"];
				auto& Physical = Boat["Physical"];
				auto& Aquaplane = Boat["Aquaplane"];
				auto& Rudder = Boat["Rudder"];
				auto& Resistance = Boat["Resistance"];
				auto& Miscellaneous = Boat["Miscellaneous"];

				Physical["BoxFrontMult"] = GetDataBoat()->m_BoxFrontMult;
				Physical["BoxRearMult"] = GetDataBoat()->m_BoxRearMult;
				Physical["BoxSideMult"] = GetDataBoat()->m_BoxSideMult;
				Physical["SampleTop"] = GetDataBoat()->m_SampleTop;
				Physical["SampleBottom"] = GetDataBoat()->m_SampleBottom;

				Aquaplane["AquaplaneForce"] = GetDataBoat()->m_AquaplaneForce;
				Aquaplane["AquaplanePushWaterMult"] = GetDataBoat()->m_AquaplanePushWaterMult;
				Aquaplane["AquaplanePushWaterCap"] = GetDataBoat()->m_AquaplanePushWaterCap;
				Aquaplane["AquaplanePushWaterApply"] = GetDataBoat()->m_AquaplanePushWaterApply;
				Aquaplane["KeelSphereSize"] = GetDataBoat()->m_KeelSphereSize;
				Aquaplane["PropRadius"] = GetDataBoat()->m_PropRadius;
				Aquaplane["ImpellerOffset"] = GetDataBoat()->m_ImpellerOffset;
				Aquaplane["ImpellerForceMult"] = GetDataBoat()->m_ImpellerForceMult;
				Aquaplane["ProwRaiseMult"] = GetDataBoat()->m_ProwRaiseMult;

				Rudder["RudderForce"] = GetDataBoat()->m_RudderForce;
				Rudder["RudderOffsetSubmerge"] = GetDataBoat()->m_RudderOffsetSubmerge;
				Rudder["RudderOffsetForce"] = GetDataBoat()->m_RudderOffsetForce;
				Rudder["RudderOffsetForceZMult"] = GetDataBoat()->m_RudderOffsetForceZMult;

				Resistance["DragCoefficient"] = GetDataBoat()->m_DragCoefficient;
				Resistance["VecMoveResistance.X"] = GetDataBoat()->m_VecMoveResistance.m_X;
				Resistance["VecMoveResistance.Y"] = GetDataBoat()->m_VecMoveResistance.m_Y;
				Resistance["VecMoveResistance.Z"] = GetDataBoat()->m_VecMoveResistance.m_Z;
				Resistance["VecTurnResistance.X"] = GetDataBoat()->m_VecTurnResistance.m_X;
				Resistance["VecTurnResistance.Y"] = GetDataBoat()->m_VecTurnResistance.m_Y;
				Resistance["VecTurnResistance.Z"] = GetDataBoat()->m_VecTurnResistance.m_Z;

				Miscellaneous["WaveAudioMult"] = GetDataBoat()->m_WaveAudioMult;
				Miscellaneous["Look_L_R_CamHeight"] = GetDataBoat()->m_Look_L_R_CamHeight;
				Miscellaneous["LowLodAngOffset"] = GetDataBoat()->m_LowLodAngOffset;
				Miscellaneous["LowLodDraughtOffset"] = GetDataBoat()->m_LowLodDraughtOffset;
				Miscellaneous["DinghySphereBuoyConst"] = GetDataBoat()->m_DinghySphereBuoyConst;
			} else if (VehicleModsHandlingMenuVars::m_Vars.m_InBike) {
				Object["Type"] = 2;
				auto& Bike = Object["Bike"];
				auto& Physical = Bike["Physical"];
				auto& Balance = Bike["Balance"];
				auto& Friction = Bike["Friction"];

				Physical["LeanFwdCOMMult"] = GetDataBike()->m_LeanFwdCOMMult;
				Physical["LeanFwdForceMult"] = GetDataBike()->m_LeanFwdForceMult;
				Physical["LeanBakCOMMult"] = GetDataBike()->m_LeanBakCOMMult;
				Physical["LeanBakForceMult"] = GetDataBike()->m_LeanBakForceMult;
				Physical["MaxBankAngle"] = GetDataBike()->m_MaxBankAngle;
				Physical["FullAnimAngle"] = GetDataBike()->m_FullAnimAngle;
				Physical["DesLeanReturnFrac"] = GetDataBike()->m_DesLeanReturnFrac;
				Physical["StickLeanMult"] = GetDataBike()->m_StickLeanMult;
				Physical["BrakingStabilityMult"] = GetDataBike()->m_BrakingStabilityMult;
				Physical["InAirSteerMult"] = GetDataBike()->m_InAirSteerMult;
				Physical["JumpForce"] = GetDataBike()->m_JumpForce;

				Balance["BikeOnStandSteerAngle"] = GetDataBike()->m_BikeOnStandSteerAngle;
				Balance["BikeOnStandLeanAngle"] = GetDataBike()->m_BikeOnStandLeanAngle;
				Balance["FrontBalanceMult"] = GetDataBike()->m_FrontBalanceMult;
				Balance["RearBalanceMult"] = GetDataBike()->m_RearBalanceMult;
				Balance["WheelieBalancePoint"] = GetDataBike()->m_WheelieBalancePoint;
				Balance["StoppieBalancePoint"] = GetDataBike()->m_StoppieBalancePoint;
				Balance["WheelieSteerMult"] = GetDataBike()->m_WheelieSteerMult;

				Friction["BikeGroundSideFrictionMult"] = GetDataBike()->m_BikeGroundSideFrictionMult;
				Friction["BikeWheelGroundSideFrictionMult"] = GetDataBike()->m_BikeWheelGroundSideFrictionMult;
			} else if (VehicleModsHandlingMenuVars::m_Vars.m_InFlying) {
				Object["Type"] = 3;
				auto& Flying = Object["Flying"];
				auto& Thrust = Flying["Thrust"];
				auto& Movement = Flying["Movement"];
				auto& Resistance = Flying["Resistance"];
				auto& Turbulence = Flying["Turbulence"];
				auto& Gear = Flying["Gear"];
				auto& Ground = Flying["Ground"];
				auto& Miscellaneous = Flying["Miscellaneous"];

				Thrust["Thrust"] = GetDataFlying()->m_Thrust;
				Thrust["ThrustFallOff"] = GetDataFlying()->m_ThrustFallOff;
				Thrust["ThrustVectoring"] = GetDataFlying()->m_ThrustVectoring;

				Movement["SideSlipMult"] = GetDataFlying()->m_SideSlipMult;
				Movement["YawMult"] = GetDataFlying()->m_YawMult;
				Movement["YawStabilise"] = GetDataFlying()->m_YawStabilise;
				Movement["RollMult"] = GetDataFlying()->m_RollMult;
				Movement["RollStabilise"] = GetDataFlying()->m_RollStabilise;
				Movement["PitchMult"] = GetDataFlying()->m_PitchMult;
				Movement["PitchStabilise"] = GetDataFlying()->m_PitchStabilise;
				Movement["FormLiftMult"] = GetDataFlying()->m_FormLiftMult;
				Movement["AttackLiftMult"] = GetDataFlying()->m_AttackLiftMult;
				Movement["AttackDiveMult"] = GetDataFlying()->m_AttackDiveMult;
				Movement["EngineOffGlideMulti"] = GetDataFlying()->m_EngineOffGlideMulti;

				Resistance["WindMult"] = GetDataFlying()->m_WindMult;
				Resistance["MoveRes"] = GetDataFlying()->m_MoveRes;
				Resistance["VecTurnResistance.X"] = GetDataFlying()->m_VecTurnResistance.m_X;
				Resistance["VecTurnResistance.Y"] = GetDataFlying()->m_VecTurnResistance.m_Y;
				Resistance["VecTurnResistance.Z"] = GetDataFlying()->m_VecTurnResistance.m_Z;
				Resistance["VecSpeedResistance.X"] = GetDataFlying()->m_VecSpeedResistance.m_X;
				Resistance["VecSpeedResistance.Y"] = GetDataFlying()->m_VecSpeedResistance.m_Y;
				Resistance["VecSpeedResistance.Z"] = GetDataFlying()->m_VecSpeedResistance.m_Z;

				Turbulence["TurublenceMagnitudeMax"] = GetDataFlying()->m_TurublenceMagnitudeMax;
				Turbulence["TurublenceForceMulti"] = GetDataFlying()->m_TurublenceForceMulti;
				Turbulence["TurublenceRollTorqueMulti"] = GetDataFlying()->m_TurublenceRollTorqueMulti;
				Turbulence["TurublencePitchTorqueMulti"] = GetDataFlying()->m_TurublencePitchTorqueMulti;

				Gear["GearDownDragV"] = GetDataFlying()->m_GearDownDragV;
				Gear["GearDownLiftMult"] = GetDataFlying()->m_GearDownLiftMult;

				Ground["OnGroundYawBoostSpeedPeak"] = GetDataFlying()->m_OnGroundYawBoostSpeedPeak;
				Ground["OnGroundYawBoostSpeedCap"] = GetDataFlying()->m_OnGroundYawBoostSpeedCap;

				Miscellaneous["BodyDamageControlEffectMult"] = GetDataFlying()->m_BodyDamageControlEffectMult;
				Miscellaneous["InputSensitivityForDifficulty"] = GetDataFlying()->m_InputSensitivityForDifficulty;
			}

			Output << Object.dump(4);
			Output.close();
			Menu::GetSubmenuHandler()->GetCurrentSubmenu()->UpdateOnce();
		} catch (std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}
	}

	void LoadHandling(const char* OutfitName) {
		VehicleModsHandlingMenu::GetInstance()->UpdateOnce(); // re-cache model check

		auto Location = Utils::VA::VA("%s%s.json", Utils::GetConfig()->GetHandlingPath().c_str(), OutfitName);

		try {
			std::ifstream Input(Location);
			if (!Input.good()) return;

			nlohmann::json Object;
			Input >> Object;

			if ((Object["Type"].get<int>() == 1 && !VehicleModsHandlingMenuVars::m_Vars.m_InBoat)
				|| (Object["Type"].get<int>() == 2 && !VehicleModsHandlingMenuVars::m_Vars.m_InBike)
				|| (Object["Type"].get<int>() == 3 && !VehicleModsHandlingMenuVars::m_Vars.m_InFlying)) {
				Menu::GetNotify()->NotifyStacked(VehicleModsHandlingSaveLoadMenu::GetInstance()->getString("You're not in the correct vehicle type"), Global::UIVars::g_NotificationError);
				return;
			}

			uint32_t Model = Native::GetEntityModel(Menu::GetLocalPlayer().m_Vehicle);
			VehicleModsHandlingAttributesMenuVars::CachePhysical(Model);
			VehicleModsHandlingAttributesMenuVars::CacheTransmission(Model);
			VehicleModsHandlingAttributesMenuVars::CacheWheelTraction(Model);
			VehicleModsHandlingAttributesMenuVars::CacheSuspension(Model);
			VehicleModsHandlingAttributesMenuVars::CacheDamage(Model);
			VehicleModsHandlingAttributesMenuVars::CacheMisc(Model);

			// Main handling
			auto& Main = Object["Main"];
			auto& Physical = Main["Physical"];
			auto& Transmission = Main["Transmission"];
			auto& WheelTraction = Main["Wheel Traction"];
			auto& Suspension = Main["Suspension"];
			auto& Damage = Main["Damage"];
			auto& Miscellaneous = Main["Miscellaneous"];

			GetDataMain()->m_Mass = Physical["Mass"];
			GetDataMain()->m_DownforceModifier = Physical["DownforceModifier"];
			GetDataMain()->m_PercentSubmerged = Physical["PercentSubmerged"];
			GetDataMain()->m_VecCentreOfMassOffset.m_X = Physical["VecCentreOfMassOffset.X"];
			GetDataMain()->m_VecCentreOfMassOffset.m_Y = Physical["VecCentreOfMassOffset.Y"];
			GetDataMain()->m_VecCentreOfMassOffset.m_Z = Physical["VecCentreOfMassOffset.Z"];
			GetDataMain()->m_VecInertiaMultiplier.m_X = Physical["VecInertiaMultiplier.X"];
			GetDataMain()->m_VecInertiaMultiplier.m_Y = Physical["VecInertiaMultiplier.Y"];
			GetDataMain()->m_VecInertiaMultiplier.m_Z = Physical["VecInertiaMultiplier.Z"];

			GetDataMain()->m_DriveBiasFront = Transmission["DriveBiasFront"];
			GetDataMain()->m_InitialDriveGears = Transmission["InitialDriveGears"];
			GetDataMain()->m_InitialDriveForce = Transmission["InitialDriveForce"];
			GetDataMain()->m_DriveInertia = Transmission["DriveInertia"];
			GetDataMain()->m_InitialDriveMaxFlatVel = Transmission["InitialDriveMaxFlatVel"];
			GetDataMain()->m_ClutchChangeRateScaleUpShift = Transmission["ClutchChangeRateScaleUpShift"];
			GetDataMain()->m_ClutchChangeRateScaleDownShift = Transmission["ClutchChangeRateScaleDownShift"];
			GetDataMain()->m_BrakeForce = Transmission["BrakeForce"];
			GetDataMain()->m_HandBrakeForce = Transmission["HandBrakeForce"];
			GetDataMain()->m_SteeringLock = Transmission["SteeringLock"];

			GetDataMain()->m_TractionCurveMax = WheelTraction["TractionCurveMax"];
			GetDataMain()->m_TractionSpringDeltaMax = WheelTraction["TractionSpringDeltaMax"];
			GetDataMain()->m_LowSpeedTractionLossMult = WheelTraction["LowSpeedTractionLossMult"];
			GetDataMain()->m_CamberStiffnesss = WheelTraction["CamberStiffnesss"];
			GetDataMain()->m_TractionLossMult = WheelTraction["TractionLossMult"];

			GetDataMain()->m_SuspensionForce = Suspension["SuspensionForce"];
			GetDataMain()->m_SuspensionCompDamp = Suspension["SuspensionCompDamp"];
			GetDataMain()->m_SuspensionReboundDamp = Suspension["SuspensionReboundDamp"];
			GetDataMain()->m_SuspensionUpperLimit = Suspension["SuspensionUpperLimit"];
			GetDataMain()->m_SuspensionLowerLimit = Suspension["SuspensionLowerLimit"];
			GetDataMain()->m_AntiRollBarForce = Suspension["AntiRollBarForce"];
			GetDataMain()->m_RollCentreHeightFront = Suspension["RollCentreHeightFront"];
			GetDataMain()->m_RollCentreHeightRear = Suspension["RollCentreHeightRear"];

			GetDataMain()->m_CollisionDamageMult = Damage["CollisionDamageMult"];
			GetDataMain()->m_WeaponDamageMult =	Damage["WeaponDamageMult"];
			GetDataMain()->m_DeformationDamageMult = Damage["DeformationDamageMult"];
			GetDataMain()->m_EngineDamageMult = Damage["EngineDamageMult"];
			GetDataMain()->m_PetrolTankVolume = Damage["PetrolTankVolume"];

			GetDataMain()->m_OilVolume = Miscellaneous["OilVolume"];
			GetDataMain()->m_SeatOffset.m_X = Miscellaneous["SeatOffset.X"];
			GetDataMain()->m_SeatOffset.m_Y = Miscellaneous["SeatOffset.Y"];
			GetDataMain()->m_SeatOffset.m_Z = Miscellaneous["SeatOffset.Z"];
			GetDataMain()->m_MonetaryValue = Miscellaneous["MonetaryValue"];

			// Boat
			if (Object["Type"].get<int>() == 1) {
				VehicleHandlingBoatMenuVars::CachePhysical(Model);
				VehicleHandlingBoatMenuVars::CacheAquaplane(Model);
				VehicleHandlingBoatMenuVars::CacheRudder(Model);
				VehicleHandlingBoatMenuVars::CacheResistance(Model);
				VehicleHandlingBoatMenuVars::CacheMisc(Model);

				auto& Boat = Object["Boat"];
				auto& Physical = Boat["Physical"];
				auto& Aquaplane = Boat["Aquaplane"];
				auto& Rudder = Boat["Rudder"];
				auto& Resistance = Boat["Resistance"];
				auto& Miscellaneous = Boat["Miscellaneous"];

				GetDataBoat()->m_BoxFrontMult = Physical["BoxFrontMult"];
				GetDataBoat()->m_BoxRearMult = Physical["BoxRearMult"];
				GetDataBoat()->m_BoxSideMult = Physical["BoxSideMult"];
				GetDataBoat()->m_SampleTop = Physical["SampleTop"];
				GetDataBoat()->m_SampleBottom = Physical["SampleBottom"];

				GetDataBoat()->m_AquaplaneForce = Aquaplane["AquaplaneForce"];
				GetDataBoat()->m_AquaplanePushWaterMult = Aquaplane["AquaplanePushWaterMult"];
				GetDataBoat()->m_AquaplanePushWaterCap = Aquaplane["AquaplanePushWaterCap"];
				GetDataBoat()->m_AquaplanePushWaterApply = Aquaplane["AquaplanePushWaterApply"];
				GetDataBoat()->m_KeelSphereSize = Aquaplane["KeelSphereSize"];
				GetDataBoat()->m_PropRadius = Aquaplane["PropRadius"];
				GetDataBoat()->m_ImpellerOffset = Aquaplane["ImpellerOffset"];
				GetDataBoat()->m_ImpellerForceMult = Aquaplane["ImpellerForceMult"];
				GetDataBoat()->m_ProwRaiseMult = Aquaplane["ProwRaiseMult"];

				GetDataBoat()->m_RudderForce = Rudder["RudderForce"];
				GetDataBoat()->m_RudderOffsetSubmerge = Rudder["RudderOffsetSubmerge"];
				GetDataBoat()->m_RudderOffsetForce = Rudder["RudderOffsetForce"];
				GetDataBoat()->m_RudderOffsetForceZMult = Rudder["RudderOffsetForceZMult"];

				GetDataBoat()->m_DragCoefficient = Resistance["DragCoefficient"];
				GetDataBoat()->m_VecMoveResistance.m_X = Resistance["VecMoveResistance.X"];
				GetDataBoat()->m_VecMoveResistance.m_Y = Resistance["VecMoveResistance.Y"];
				GetDataBoat()->m_VecMoveResistance.m_Z = Resistance["VecMoveResistance.Z"];
				GetDataBoat()->m_VecTurnResistance.m_X = Resistance["VecTurnResistance.X"];
				GetDataBoat()->m_VecTurnResistance.m_Y = Resistance["VecTurnResistance.Y"];
				GetDataBoat()->m_VecTurnResistance.m_Z = Resistance["VecTurnResistance.Z"];

				GetDataBoat()->m_WaveAudioMult = Miscellaneous["WaveAudioMult"];
				GetDataBoat()->m_Look_L_R_CamHeight = Miscellaneous["Look_L_R_CamHeight"];
				GetDataBoat()->m_LowLodAngOffset = Miscellaneous["LowLodAngOffset"];
				GetDataBoat()->m_LowLodDraughtOffset = Miscellaneous["LowLodDraughtOffset"];
				GetDataBoat()->m_DinghySphereBuoyConst = Miscellaneous["DinghySphereBuoyConst"];
			}

			// Bike
			if (Object["Type"].get<int>() == 2) {
				VehicleHandlingBikeMenuVars::CachePhysical(Model);
				VehicleHandlingBikeMenuVars::CacheBalance(Model);
				VehicleHandlingBikeMenuVars::CacheFriction(Model);

				auto& Bike = Object["Bike"];
				auto& Physical = Bike["Physical"];
				auto& Balance = Bike["Balance"];
				auto& Friction = Bike["Friction"];

				GetDataBike()->m_LeanFwdCOMMult = Physical["LeanFwdCOMMult"];
				GetDataBike()->m_LeanFwdForceMult = Physical["LeanFwdForceMult"];
				GetDataBike()->m_LeanBakCOMMult = Physical["LeanBakCOMMult"];
				GetDataBike()->m_LeanBakForceMult = Physical["LeanBakForceMult"];
				GetDataBike()->m_MaxBankAngle = Physical["MaxBankAngle"];
				GetDataBike()->m_FullAnimAngle = Physical["FullAnimAngle"];
				GetDataBike()->m_DesLeanReturnFrac = Physical["DesLeanReturnFrac"];
				GetDataBike()->m_StickLeanMult = Physical["StickLeanMult"];
				GetDataBike()->m_BrakingStabilityMult = Physical["BrakingStabilityMult"];
				GetDataBike()->m_InAirSteerMult = Physical["InAirSteerMult"];
				GetDataBike()->m_JumpForce = Physical["JumpForce"];

				GetDataBike()->m_BikeOnStandSteerAngle = Balance["BikeOnStandSteerAngle"];
				GetDataBike()->m_BikeOnStandLeanAngle = Balance["BikeOnStandLeanAngle"];
				GetDataBike()->m_FrontBalanceMult =	Balance["FrontBalanceMult"];
				GetDataBike()->m_RearBalanceMult = Balance["RearBalanceMult"];
				GetDataBike()->m_WheelieBalancePoint = Balance["WheelieBalancePoint"];
				GetDataBike()->m_StoppieBalancePoint = Balance["StoppieBalancePoint"];
				GetDataBike()->m_WheelieSteerMult = Balance["WheelieSteerMult"];

				GetDataBike()->m_BikeGroundSideFrictionMult = Friction["BikeGroundSideFrictionMult"];
				GetDataBike()->m_BikeWheelGroundSideFrictionMult = Friction["BikeWheelGroundSideFrictionMult"];
			}

			// Flying
			if (Object["Type"].get<int>() == 3) {
				VehicleHandlingFlyingMenuVars::CacheThrust(Model);
				VehicleHandlingFlyingMenuVars::CacheMovement(Model);
				VehicleHandlingFlyingMenuVars::CacheResistance(Model);
				VehicleHandlingFlyingMenuVars::CacheTurbulence(Model);
				VehicleHandlingFlyingMenuVars::CacheGear(Model);
				VehicleHandlingFlyingMenuVars::CacheGround(Model);
				VehicleHandlingFlyingMenuVars::CacheMisc(Model);

				auto& Flying = Object["Flying"];
				auto& Thrust = Flying["Thrust"];
				auto& Movement = Flying["Movement"];
				auto& Resistance = Flying["Resistance"];
				auto& Turbulence = Flying["Turbulence"];
				auto& Gear = Flying["Gear"];
				auto& Ground = Flying["Ground"];
				auto& Miscellaneous = Flying["Miscellaneous"];

				GetDataFlying()->m_Thrust = Thrust["Thrust"];
				GetDataFlying()->m_ThrustFallOff = Thrust["ThrustFallOff"];
				GetDataFlying()->m_ThrustVectoring = Thrust["ThrustVectoring"];

				GetDataFlying()->m_SideSlipMult = Movement["SideSlipMult"];
				GetDataFlying()->m_YawMult = Movement["YawMult"];
				GetDataFlying()->m_YawStabilise = Movement["YawStabilise"];
				GetDataFlying()->m_RollMult = Movement["RollMult"];
				GetDataFlying()->m_RollStabilise = Movement["RollStabilise"];
				GetDataFlying()->m_PitchMult = Movement["PitchMult"];
				GetDataFlying()->m_PitchStabilise = Movement["PitchStabilise"];
				GetDataFlying()->m_FormLiftMult = Movement["FormLiftMult"];
				GetDataFlying()->m_AttackLiftMult = Movement["AttackLiftMult"];
				GetDataFlying()->m_AttackDiveMult = Movement["AttackDiveMult"];
				GetDataFlying()->m_EngineOffGlideMulti = Movement["EngineOffGlideMulti"];

				GetDataFlying()->m_WindMult = Resistance["WindMult"];
				GetDataFlying()->m_MoveRes = Resistance["MoveRes"];
				GetDataFlying()->m_VecTurnResistance.m_X = Resistance["VecTurnResistance.X"];
				GetDataFlying()->m_VecTurnResistance.m_Y = Resistance["VecTurnResistance.Y"];
				GetDataFlying()->m_VecTurnResistance.m_Z = Resistance["VecTurnResistance.Z"];
				GetDataFlying()->m_VecSpeedResistance.m_X = Resistance["VecSpeedResistance.X"];
				GetDataFlying()->m_VecSpeedResistance.m_Y =	Resistance["VecSpeedResistance.Y"];
				GetDataFlying()->m_VecSpeedResistance.m_Z = Resistance["VecSpeedResistance.Z"];

				GetDataFlying()->m_TurublenceMagnitudeMax = Turbulence["TurublenceMagnitudeMax"];
				GetDataFlying()->m_TurublenceForceMulti = Turbulence["TurublenceForceMulti"];
				GetDataFlying()->m_TurublenceRollTorqueMulti = Turbulence["TurublenceRollTorqueMulti"];
				GetDataFlying()->m_TurublencePitchTorqueMulti = Turbulence["TurublencePitchTorqueMulti"];

				GetDataFlying()->m_GearDownDragV = Gear["GearDownDragV"];
				GetDataFlying()->m_GearDownLiftMult = Gear["GearDownLiftMult"];

				GetDataFlying()->m_OnGroundYawBoostSpeedPeak = Ground["OnGroundYawBoostSpeedPeak"];
				GetDataFlying()->m_OnGroundYawBoostSpeedCap = Ground["OnGroundYawBoostSpeedCap"];

				GetDataFlying()->m_BodyDamageControlEffectMult = Miscellaneous["BodyDamageControlEffectMult"];
				GetDataFlying()->m_InputSensitivityForDifficulty = Miscellaneous["InputSensitivityForDifficulty"];
			}

			Input.close();
		} catch (std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}
	}
}

void VehicleModsHandlingSaveLoadMenu::Init() {
	SetName("Save and Load");
	SetParentSubmenu<VehicleModsHandlingMenu>();

	addOption(ButtonOption("Save Handling")
		.addTranslation()
		.addKeyboard("Enter the handling name!", 30, SaveHandling));

	addOption(BreakOption("Handling")
		.addTranslation());

	addString("~m~None");
	addString("You're not in the correct vehicle type");
}

void VehicleModsHandlingSaveLoadMenu::Update() {
	if (!Menu::GetLocalPlayer().m_InVehicle) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}
}

/*Called once on submenu open*/
void VehicleModsHandlingSaveLoadMenu::UpdateOnce() {
	ClearOptionsOffset(2);

	auto Handling = Utils::GetConfig()->GetFilesInDirectory(Utils::GetConfig()->GetHandlingPath(), ".json");
	if (Handling.size()) {
		for (auto& Item : Handling) {
			addOption(ButtonOption(Item)
				.addOnClick([=] { LoadHandling(Item.c_str()); }));
		}
	} else {
		addOption(ButtonOption(getString("~m~None")));
	}
}

/*Called always*/
void VehicleModsHandlingSaveLoadMenu::FeatureUpdate() {}

VehicleModsHandlingSaveLoadMenu* _instance;
VehicleModsHandlingSaveLoadMenu* VehicleModsHandlingSaveLoadMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleModsHandlingSaveLoadMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleModsHandlingSaveLoadMenu::~VehicleModsHandlingSaveLoadMenu() { delete _instance; }