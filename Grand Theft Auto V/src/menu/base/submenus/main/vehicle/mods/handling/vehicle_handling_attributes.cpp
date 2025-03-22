#include "vehicle_handling_attributes.hpp"
#include "menu/base/submenu_handler.hpp"
#include "vehicle_handling_main.hpp"

using namespace VehicleModsHandlingAttributesMenuVars;

namespace VehicleModsHandlingAttributesMenuVars {
	Vars_ m_Vars;

	ScrollStruct<float> DriveBias[] = {
		{ TranslationString("RWD", true), 0.f },
		{ TranslationString("4WD", true), 0.5f },
		{ TranslationString("FWD", true), 1.f }
	};

	Rage::Types::HandlingData* GetData() {
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

	void Reset() {
		uint32_t Model = Native::GetEntityModel(Menu::GetLocalPlayer().m_Vehicle);

		switch (m_Vars.m_Type) {
			case 0: /* Physical */
				if (m_Vars.m_PhysicalRestore.find(Model) != m_Vars.m_PhysicalRestore.end()) {
					auto& Backup = m_Vars.m_PhysicalRestore[Model];
					GetData()->m_Mass = Backup.m_Mass;
					GetData()->m_DownforceModifier = Backup.m_DownforceModifier;
					GetData()->m_PercentSubmerged = Backup.m_PercentSubmerged;
					GetData()->m_VecCentreOfMassOffset = Backup.m_VecCentreOfMassOffset;
					GetData()->m_VecInertiaMultiplier = Backup.m_VecInertiaMultiplier;
				}
				break;

			case 1: /* Transmission */
				if (m_Vars.m_TransmissionRestore.find(Model) != m_Vars.m_TransmissionRestore.end()) {
					auto& Backup = m_Vars.m_TransmissionRestore[Model];
					GetData()->m_DriveBiasFront = Backup.m_DriveBiasFront;
					GetData()->m_InitialDriveGears = Backup.m_InitialDriveGears;
					GetData()->m_InitialDriveForce = Backup.m_InitialDriveForce;
					GetData()->m_DriveInertia = Backup.m_DriveInertia;
					GetData()->m_InitialDriveMaxFlatVel = Backup.m_InitialDriveMaxFlatVel;
					GetData()->m_ClutchChangeRateScaleUpShift = Backup.m_ClutchChangeRateScaleUpShift;
					GetData()->m_ClutchChangeRateScaleDownShift = Backup.m_ClutchChangeRateScaleDownShift;
					GetData()->m_BrakeForce = Backup.m_BrakeForce;
					GetData()->m_HandBrakeForce = Backup.m_HandBrakeForce;
					GetData()->m_SteeringLock = Backup.m_SteeringLock;

					if (GetData()->m_DriveBiasFront == 0.0f)
						m_Vars.m_DriveBiasFront = 0;
					else if (GetData()->m_DriveBiasFront == 0.5f)
						m_Vars.m_DriveBiasFront = 1;
					else if (GetData()->m_DriveBiasFront == 1.0f)
						m_Vars.m_DriveBiasFront = 2;
				}
				break;

			case 2: /* Wheel Traction */
				if (m_Vars.m_WheelTractionRestore.find(Model) != m_Vars.m_WheelTractionRestore.end()) {
					auto& Backup = m_Vars.m_WheelTractionRestore[Model];
					GetData()->m_TractionCurveMax = Backup.m_TractionCurveMax;
					GetData()->m_TractionSpringDeltaMax = Backup.m_TractionSpringDeltaMax;
					GetData()->m_LowSpeedTractionLossMult = Backup.m_LowSpeedTractionLossMult;
					GetData()->m_CamberStiffnesss = Backup.m_CamberStiffnesss;
					GetData()->m_TractionLossMult = Backup.m_TractionLossMult;
				}
				break;

			case 3: /* Suspension */
				if (m_Vars.m_SuspensionRestore.find(Model) != m_Vars.m_SuspensionRestore.end()) {
					auto& Backup = m_Vars.m_SuspensionRestore[Model];
					GetData()->m_SuspensionForce = Backup.m_SuspensionForce;
					GetData()->m_SuspensionCompDamp = Backup.m_SuspensionCompDamp;
					GetData()->m_SuspensionReboundDamp = Backup.m_SuspensionReboundDamp;
					GetData()->m_SuspensionUpperLimit = Backup.m_SuspensionUpperLimit;
					GetData()->m_SuspensionLowerLimit = Backup.m_SuspensionLowerLimit;
					GetData()->m_AntiRollBarForce = Backup.m_AntiRollBarForce;
					GetData()->m_RollCentreHeightFront = Backup.m_RollCentreHeightFront;
					GetData()->m_RollCentreHeightRear = Backup.m_RollCentreHeightRear;
				}
				break;

			case 4: /* Damage */
				if (m_Vars.m_DamageRestore.find(Model) != m_Vars.m_DamageRestore.end()) {
					auto& Backup = m_Vars.m_DamageRestore[Model];
					GetData()->m_CollisionDamageMult = Backup.m_CollisionDamageMult;
					GetData()->m_WeaponDamageMult = Backup.m_WeaponDamageMult;
					GetData()->m_DeformationDamageMult = Backup.m_DeformationDamageMult;
					GetData()->m_EngineDamageMult = Backup.m_EngineDamageMult;
					GetData()->m_PetrolTankVolume = Backup.m_PetrolTankVolume;
				}
				break;

			case 5: /* Misc */
				if (m_Vars.m_MiscRestore.find(Model) != m_Vars.m_MiscRestore.end()) {
					auto& Backup = m_Vars.m_MiscRestore[Model];
					GetData()->m_OilVolume = Backup.m_OilVolume;
					GetData()->m_SeatOffset = Backup.m_SeatOffset;
					GetData()->m_MonetaryValue = Backup.m_MonetaryValue;
				}
				break;
		}
	}

	void CachePhysical(uint32_t Model) {
		if (m_Vars.m_PhysicalRestore.find(Model) == m_Vars.m_PhysicalRestore.end()) {
			m_Vars.m_PhysicalRestore[Model] = {
				GetData()->m_Mass,
				GetData()->m_DownforceModifier,
				GetData()->m_PercentSubmerged,
				GetData()->m_VecCentreOfMassOffset,
				GetData()->m_VecInertiaMultiplier
			};
		}
	}

	void CacheTransmission(uint32_t Model) {
		if (m_Vars.m_TransmissionRestore.find(Model) == m_Vars.m_TransmissionRestore.end()) {
			m_Vars.m_TransmissionRestore[Model] = {
				GetData()->m_DriveBiasFront,
				GetData()->m_InitialDriveGears,
				GetData()->m_InitialDriveForce,
				GetData()->m_DriveInertia,
				GetData()->m_InitialDriveMaxFlatVel,
				GetData()->m_ClutchChangeRateScaleUpShift,
				GetData()->m_ClutchChangeRateScaleDownShift,
				GetData()->m_BrakeForce,
				GetData()->m_HandBrakeForce,
				GetData()->m_SteeringLock
			};
		}
	}

	void CacheWheelTraction(uint32_t Model) {
		if (m_Vars.m_WheelTractionRestore.find(Model) == m_Vars.m_WheelTractionRestore.end()) {
			m_Vars.m_WheelTractionRestore[Model] = {
				GetData()->m_TractionCurveMax,
				GetData()->m_TractionSpringDeltaMax,
				GetData()->m_LowSpeedTractionLossMult,
				GetData()->m_CamberStiffnesss,
				GetData()->m_TractionLossMult
			};
		}
	}

	void CacheSuspension(uint32_t Model) {
		if (m_Vars.m_SuspensionRestore.find(Model) == m_Vars.m_SuspensionRestore.end()) {
			m_Vars.m_SuspensionRestore[Model] = {
				GetData()->m_SuspensionForce,
				GetData()->m_SuspensionCompDamp,
				GetData()->m_SuspensionReboundDamp,
				GetData()->m_SuspensionUpperLimit,
				GetData()->m_SuspensionLowerLimit,
				GetData()->m_AntiRollBarForce,
				GetData()->m_RollCentreHeightFront,
				GetData()->m_RollCentreHeightRear
			};
		}
	}

	void CacheDamage(uint32_t Model) {
		if (m_Vars.m_DamageRestore.find(Model) == m_Vars.m_DamageRestore.end()) {
			m_Vars.m_DamageRestore[Model] = {
				GetData()->m_CollisionDamageMult,
				GetData()->m_WeaponDamageMult,
				GetData()->m_DeformationDamageMult,
				GetData()->m_EngineDamageMult,
				GetData()->m_PetrolTankVolume,
				GetData()->m_OilVolume
			};
		}
	}

	void CacheMisc(uint32_t Model) {
		if (m_Vars.m_MiscRestore.find(Model) == m_Vars.m_MiscRestore.end()) {
			m_Vars.m_MiscRestore[Model] = {
				GetData()->m_OilVolume,
				GetData()->m_SeatOffset,
				GetData()->m_MonetaryValue
			};
		}
	}
}

void VehicleModsHandlingAttributesMenu::Init() {
	SetName("Handling Attributes");
	SetParentSubmenu<VehicleModsHandlingMainMenu>();

	addOption(ButtonOption("Reset to Default")
		.addTranslation()
		.addSprite("Ozark", "spinner")
		.addSpriteScale(0.01232f, 0.02184f)
		.addSpriteRotate()
		.addOnClick(Reset));

	addString("Mass");
	addString("Downforce Modifier");
	addString("Percent Submerged");
	addString("Center of Mass - Horizontal (X");
	addString("Center of Mass - Vertical (Y)");
	addString("Center of Mass - Height (Z)");
	addString("Inertia Multiplier - Horizontal (X)");
	addString("Inertia Multiplier - Vertical (Y)");
	addString("Inertia Multiplier - Height (Z)");

	addString("Drive Type");
	addString("Drive Gears");
	addString("Drive Force");
	addString("Drive Inertia");
	addString("Drive Max Flat Velocity");
	addString("Clutch Shift Up Multiplier");
	addString("Clutch Shift Down Multiplier");
	addString("Brake Force");
	addString("Handbrake Force");
	addString("Steering Lock");

	addString("Traction Curve Max");
	addString("Traction Spring Delta Max");
	addString("Low Speed Traction Loss Multiplier");
	addString("Camber Stiffnesss");
	addString("Traction Loss Multiplier");

	addString("Suspension Force");
	addString("Suspension Compression Damping");
	addString("Suspension Rebound Damping");
	addString("Suspension Upper Limit");
	addString("Suspension Lower Limit");
	addString("Suspension Anti Rollbar force");
	addString("Roll Centre Height Front");
	addString("Roll Centre Height Rear");

	addString("Collision Damage Multiplier");
	addString("Weapon Damage Multiplier");
	addString("Deformation Damage Multiplier");
	addString("Engine Damage Multiplier");
	addString("Petrol Tank Volume");

	addString("Oil Volume");
	addString("Seat Offset - Horizontal (X)");
	addString("Seat Offset - Vertical (Y)");
	addString("Seat Offset - Height (Z)");
	addString("Monetary Value");
}

void VehicleModsHandlingAttributesMenu::Update() {
	if (!Menu::GetLocalPlayer().m_InVehicle) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}
}

/*Called once on submenu open*/
void VehicleModsHandlingAttributesMenu::UpdateOnce() {
	ClearOptionsOffset(1);

	uint32_t Model = Native::GetEntityModel(Menu::GetLocalPlayer().m_Vehicle);

	/* Physical */
	if (m_Vars.m_Type == 0) {
		CachePhysical(Model);

		addOption(NumberOption<float>(SCROLL, getString("Mass"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_Mass, "%.0f", 1.f); })
			.addMin(0.0f).addMax(10000.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Downforce Modifier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_DownforceModifier, "%.0f", 1.f); })
			.addMin(0.0f).addMax(10000.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Percent Submerged"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_PercentSubmerged, "%.1f", 0.1f); })
			.addMin(0.0f).addMax(500.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Center of Mass - Horizontal (X)"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_VecCentreOfMassOffset.m_X, "%.2f", 0.01f); })
			.addMin(-5.f).addMax(5.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Center of Mass - Vertical (Y)"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_VecCentreOfMassOffset.m_Y, "%.2f", 0.01f); })
			.addMin(-5.f).addMax(5.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Center of Mass - Height (Z)"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_VecCentreOfMassOffset.m_Z, "%.2f", 0.01f); })
			.addMin(-5.f).addMax(5.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Inertia Multiplier - Horizontal (X)"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_VecInertiaMultiplier.m_X, "%.2f", 0.01f); })
			.addMin(-5.f).addMax(5.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Inertia Multiplier - Vertical (Y)"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_VecInertiaMultiplier.m_Y, "%.2f", 0.01f); })
			.addMin(-5.f).addMax(5.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Inertia Multiplier - Height (Z)"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_VecInertiaMultiplier.m_Z, "%.2f", 0.01f); })
			.addMin(-5.f).addMax(5.f).setScrollSpeed(10).showMax());

		return;
	}

	/* Transmission */
	if (m_Vars.m_Type == 1) {
		CacheTransmission(Model);

		if (GetData()->m_DriveBiasFront == 0.0f)
			m_Vars.m_DriveBiasFront = 0;
		else if (GetData()->m_DriveBiasFront == 0.5f)
			m_Vars.m_DriveBiasFront = 1;
		else if (GetData()->m_DriveBiasFront == 1.0f)
			m_Vars.m_DriveBiasFront = 2;

		addOption(ScrollOption<float>(SCROLL, getString("Drive Type"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addScroll(m_Vars.m_DriveBiasFront, 0, NUMOF(DriveBias), DriveBias)
			.addOnUpdate([] (ScrollOption<float>* Option) { GetData()->m_DriveBiasFront = DriveBias[m_Vars.m_DriveBiasFront].m_Result; }));

		addOption(NumberOption<uint8_t>(SCROLL, getString("Drive Gears"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<uint8_t>* Option) { Option->addNumber(GetData()->m_InitialDriveGears, "%i", 1); })
			.addMin(1).addMax(16).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Drive Force"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_InitialDriveForce, "%.2f", 0.01f); })
			.addMin(0.0f).addMax(100.f).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Drive Inertia"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_DriveInertia, "%.2f", 0.01f); })
			.addMin(0.0f).addMax(100.f).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Drive Max Flat Velocity"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_InitialDriveMaxFlatVel, "%.1f", 0.1f); })
			.addMin(0.0f).addMax(500.f).showMax().setScrollSpeed(10));

		addOption(NumberOption<float>(SCROLL, getString("Clutch Shift Up Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_ClutchChangeRateScaleUpShift, "%.1f", 0.1f); })
			.addMin(0.0f).addMax(100.f).showMax().setScrollSpeed(10));

		addOption(NumberOption<float>(SCROLL, getString("Clutch Shift Down Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_ClutchChangeRateScaleDownShift, "%.1f", 0.1f); })
			.addMin(0.0f).addMax(100.f).showMax().setScrollSpeed(10));

		addOption(NumberOption<float>(SCROLL, getString("Brake Force"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_BrakeForce, "%.1f", 0.1f); })
			.addMin(0.f).addMax(100.f).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Handbrake Force"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_HandBrakeForce, "%.1f", 0.1f); })
			.addMin(0.f).addMax(100.f).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Steering Lock"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_SteeringLock, "%.1f", 0.1f); })
			.addMin(0.f).addMax(100.f).showMax());

		return;
	}

	/* Wheel Traction */
	if (m_Vars.m_Type == 2) {
		CacheWheelTraction(Model);

		addOption(NumberOption<float>(SCROLL, getString("Traction Curve Max"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_TractionCurveMax, "%.1f", 0.1f); })
			.addMin(0.f).addMax(100.f).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Traction Spring Delta Max"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_TractionSpringDeltaMax, "%.1f", 0.1f); })
			.addMin(0.f).addMax(100.f).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Low Speed Traction Loss Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_LowSpeedTractionLossMult, "%.1f", 0.1f); })
			.addMin(0.f).addMax(100.f).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Camber Stiffnesss"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_CamberStiffnesss, "%.1f", 0.1f); })
			.addMin(0.f).addMax(100.f).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Traction Loss Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_TractionLossMult, "%.1f", 0.1f); })
			.addMin(0.f).addMax(100.f).showMax());

		return;
	}

	/* Suspension */
	if (m_Vars.m_Type == 3) {
		CacheSuspension(Model);

		addOption(NumberOption<float>(SCROLL, getString("Suspension Force"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_SuspensionForce, "%.1f", 0.1f); })
			.addMin(0.1f).addMax(100.f).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Suspension Compression Damping"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_SuspensionCompDamp, "%.1f", 0.1f); })
			.addMin(0.1f).addMax(100.f).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Suspension Rebound Damping"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_SuspensionReboundDamp, "%.1f", 0.1f); })
			.addMin(0.1f).addMax(100.f).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Suspension Upper Limit"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_SuspensionUpperLimit, "%.1f", 0.1f); })
			.addMin(-10.f).addMax(100.f).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Suspension Lower Limit"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_SuspensionLowerLimit, "%.1f", 0.1f); })
			.addMin(-10.f).addMax(100.f).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Suspension Anti Rollbar force"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_AntiRollBarForce, "%.1f", 0.1f); })
			.addMin(0.1f).addMax(100.f).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Roll Centre Height Front"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_RollCentreHeightFront, "%.1f", 0.1f); })
			.addMin(0.1f).addMax(100.f).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Roll Centre Height Rear"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_RollCentreHeightRear, "%.1f", 0.1f); })
			.addMin(0.1f).addMax(100.f).showMax());

		return;
	}

	/* Damage */
	if (m_Vars.m_Type == 4) {
		CacheDamage(Model);

		addOption(NumberOption<float>(SCROLL, getString("Collision Damage Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_CollisionDamageMult, "%.1f", 0.1f); })
			.addMin(0.0f).addMax(100.f).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Weapon Damage Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_WeaponDamageMult, "%.1f", 0.1f); })
			.addMin(0.0f).addMax(100.f).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Deformation Damage Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_DeformationDamageMult, "%.1f", 0.1f); })
			.addMin(0.0f).addMax(100.f).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Engine Damage Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_EngineDamageMult, "%.1f", 0.1f); })
			.addMin(0.0f).addMax(100.f).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Petrol Tank Volume"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_PetrolTankVolume, "%.1f", 0.1f); })
			.addMin(0.0f).addMax(100.f).showMax());

		return;
	}

	/* Misc */
	if (m_Vars.m_Type == 5) {
		CacheMisc(Model);

		addOption(NumberOption<float>(SCROLL, getString("Oil Volume"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_OilVolume, "%.1f", 0.1f); })
			.addMin(0.0f).addMax(100.f).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Seat Offset - Horizontal (X)"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_SeatOffset.m_X, "%.2f", 0.01f); })
			.addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Seat Offset - Vertical (Y)"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_SeatOffset.m_Y, "%.2f", 0.01f); })
			.addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Seat Offset - Height (Z)"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_SeatOffset.m_Z, "%.2f", 0.01f); })
			.addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<int>(SCROLL, getString("Monetary Value"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<int>* Option) { Option->addNumber(GetData()->m_MonetaryValue, "%i", 1000); })
			.addMin(0).addMax(100000000).setScrollSpeed(0));

		return;
	}
}

/*Called always*/
void VehicleModsHandlingAttributesMenu::FeatureUpdate() {}

VehicleModsHandlingAttributesMenu* _instance;
VehicleModsHandlingAttributesMenu* VehicleModsHandlingAttributesMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleModsHandlingAttributesMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleModsHandlingAttributesMenu::~VehicleModsHandlingAttributesMenu() { delete _instance; }