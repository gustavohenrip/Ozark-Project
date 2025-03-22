#include "vehicle_handling_flying.hpp"
#include "menu/base/submenu_handler.hpp"
#include "vehicle_handling_main.hpp"

using namespace VehicleHandlingFlyingMenuVars;

namespace VehicleHandlingFlyingMenuVars {
	Vars_ m_Vars;

	Rage::Types::FlyingHandlingData* GetData() {
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

	void Reset() {
		uint32_t Model = Native::GetEntityModel(Menu::GetLocalPlayer().m_Vehicle);

		switch (m_Vars.m_Type) {
			case 0: /* Thrust */
				if (m_Vars.m_ThrustRestore.find(Model) != m_Vars.m_ThrustRestore.end()) {
					auto& Backup = m_Vars.m_ThrustRestore[Model];
					GetData()->m_Thrust = Backup.m_Thrust;
					GetData()->m_ThrustFallOff = Backup.m_ThrustFallOff;
					GetData()->m_ThrustVectoring = Backup.m_ThrustVectoring;
				}
				break;

			case 1: /* Movement */
				if (m_Vars.m_MovementRestore.find(Model) != m_Vars.m_MovementRestore.end()) {
					auto& Backup = m_Vars.m_MovementRestore[Model];
					GetData()->m_SideSlipMult = Backup.m_SideSlipMult;
					GetData()->m_YawMult = Backup.m_YawMult;
					GetData()->m_YawStabilise = Backup.m_YawStabilise;
					GetData()->m_RollMult = Backup.m_RollMult;
					GetData()->m_RollStabilise = Backup.m_RollStabilise;
					GetData()->m_PitchMult = Backup.m_PitchMult;
					GetData()->m_PitchStabilise = Backup.m_PitchStabilise;
					GetData()->m_FormLiftMult = Backup.m_FormLiftMult;
					GetData()->m_AttackLiftMult = Backup.m_AttackLiftMult;
					GetData()->m_AttackDiveMult = Backup.m_AttackDiveMult;
					GetData()->m_EngineOffGlideMulti = Backup.m_EngineOffGlideMulti;
				}
				break;

			case 2: /* Resistance */
				if (m_Vars.m_ResistanceRestore.find(Model) != m_Vars.m_ResistanceRestore.end()) {
					auto& Backup = m_Vars.m_ResistanceRestore[Model];
					GetData()->m_WindMult = Backup.m_WindMult;
					GetData()->m_MoveRes = Backup.m_MoveRes;
					GetData()->m_VecTurnResistance = Backup.m_VecTurnResistance;
					GetData()->m_VecSpeedResistance = Backup.m_VecSpeedResistance;
				}
				break;

			case 3: /* Turbulence */
				if (m_Vars.m_TurbulenceRestore.find(Model) != m_Vars.m_TurbulenceRestore.end()) {
					auto& Backup = m_Vars.m_TurbulenceRestore[Model];
					GetData()->m_TurublenceMagnitudeMax = Backup.m_TurublenceMagnitudeMax;
					GetData()->m_TurublenceForceMulti = Backup.m_TurublenceForceMulti;
					GetData()->m_TurublenceRollTorqueMulti = Backup.m_TurublenceRollTorqueMulti;
					GetData()->m_TurublencePitchTorqueMulti = Backup.m_TurublencePitchTorqueMulti;
				}
				break;

			case 4: /* Gear */
				if (m_Vars.m_GearRestore.find(Model) != m_Vars.m_GearRestore.end()) {
					auto& Backup = m_Vars.m_GearRestore[Model];
					GetData()->m_GearDownDragV = Backup.m_GearDownDragV;
					GetData()->m_GearDownLiftMult = Backup.m_GearDownLiftMult;
				}
				break;

			case 5: /* Ground */
				if (m_Vars.m_GroundRestore.find(Model) != m_Vars.m_GroundRestore.end()) {
					auto& Backup = m_Vars.m_GroundRestore[Model];
					GetData()->m_OnGroundYawBoostSpeedPeak = Backup.m_OnGroundYawBoostSpeedPeak;
					GetData()->m_OnGroundYawBoostSpeedCap = Backup.m_OnGroundYawBoostSpeedCap;
				}
				break;

			case 6: /* Misc */
				if (m_Vars.m_MiscRestore.find(Model) != m_Vars.m_MiscRestore.end()) {
					auto& Backup = m_Vars.m_MiscRestore[Model];
					GetData()->m_BodyDamageControlEffectMult = Backup.m_BodyDamageControlEffectMult;
					GetData()->m_InputSensitivityForDifficulty = Backup.m_InputSensitivityForDifficulty;
				}
				break;
		}
	}

	void CacheThrust(uint32_t Model) {
		if (m_Vars.m_ThrustRestore.find(Model) == m_Vars.m_ThrustRestore.end()) {
			m_Vars.m_ThrustRestore[Model] = {
				GetData()->m_Thrust,
				GetData()->m_ThrustFallOff,
				GetData()->m_ThrustVectoring
			};
		}
	}

	void CacheMovement(uint32_t Model) {
		if (m_Vars.m_MovementRestore.find(Model) == m_Vars.m_MovementRestore.end()) {
			m_Vars.m_MovementRestore[Model] = {
				GetData()->m_SideSlipMult,
				GetData()->m_YawMult,
				GetData()->m_YawStabilise,
				GetData()->m_RollMult,
				GetData()->m_RollStabilise,
				GetData()->m_PitchMult,
				GetData()->m_PitchStabilise,
				GetData()->m_FormLiftMult,
				GetData()->m_AttackLiftMult,
				GetData()->m_AttackDiveMult,
				GetData()->m_EngineOffGlideMulti,
			};
		}
	}

	void CacheResistance(uint32_t Model) {
		if (m_Vars.m_ResistanceRestore.find(Model) == m_Vars.m_ResistanceRestore.end()) {
			m_Vars.m_ResistanceRestore[Model] = {
				GetData()->m_WindMult,
				GetData()->m_MoveRes,
				GetData()->m_VecTurnResistance,
				GetData()->m_VecSpeedResistance
			};
		}
	}

	void CacheTurbulence(uint32_t Model) {
		if (m_Vars.m_TurbulenceRestore.find(Model) == m_Vars.m_TurbulenceRestore.end()) {
			m_Vars.m_TurbulenceRestore[Model] = {
				GetData()->m_TurublenceMagnitudeMax,
				GetData()->m_TurublenceForceMulti,
				GetData()->m_TurublenceRollTorqueMulti,
				GetData()->m_TurublencePitchTorqueMulti
			};
		}
	}

	void CacheGear(uint32_t Model) {
		if (m_Vars.m_GearRestore.find(Model) == m_Vars.m_GearRestore.end()) {
			m_Vars.m_GearRestore[Model] = {
				GetData()->m_GearDownDragV,
				GetData()->m_GearDownLiftMult
			};
		}
	}

	void CacheGround(uint32_t Model) {
		if (m_Vars.m_GroundRestore.find(Model) == m_Vars.m_GroundRestore.end()) {
			m_Vars.m_GroundRestore[Model] = {
				GetData()->m_OnGroundYawBoostSpeedPeak,
				GetData()->m_OnGroundYawBoostSpeedCap
			};
		}
	}

	void CacheMisc(uint32_t Model) {
		if (m_Vars.m_MiscRestore.find(Model) == m_Vars.m_MiscRestore.end()) {
			m_Vars.m_MiscRestore[Model] = {
				GetData()->m_BodyDamageControlEffectMult,
				GetData()->m_InputSensitivityForDifficulty
			};
		}
	}
}

void VehicleHandlingFlyingMenu::Init() {
	SetName("Flying Handling");
	SetParentSubmenu<VehicleModsHandlingMainMenu>();

	addOption(ButtonOption("Reset to Default")
		.addTranslation()
		.addSprite("Ozark", "spinner")
		.addSpriteScale(0.01232f, 0.02184f)
		.addSpriteRotate()
		.addOnClick(Reset));

	addString("Thrust");
	addString("Thrust Falloff");
	addString("Thrust Vectoring");

	addString("Side Slip Multiplier");
	addString("Yaw Multiplier");
	addString("Yaw Stabilize");
	addString("Roll Multiplier");
	addString("Roll Stabilize");
	addString("Pitch Multiplier");
	addString("Pitch Stabilize");
	addString("Form Lift Multiplier");
	addString("Attack Lift Multiplier");
	addString("Attack Dive Multiplier");
	addString("Engine Off Glide Multiplier");

	addString("Wind Multiplier");
	addString("Move Resistance");
	addString("Turn Resistance - Horizontal (X)");
	addString("Turn Resistance - Vertical (Y)");
	addString("Turn Resistance - Height (Z)");
	addString("Speed Resistance - Horizontal (X)");
	addString("Speed Resistance - Vertical (Y)");
	addString("Speed Resistance - Height (Z)");

	addString("Maximum Magnitude");
	addString("Force Multiplier");
	addString("Roll Torque Multiplier");
	addString("Pitch Torque Multiplier");

	addString("Down Drag");
	addString("Down Lift Multiplier");

	addString("Yaw Boost Speed Peak");
	addString("Yaw Boost Speed Cap");

	addString("Body Damage Multiplier");
	addString("Input Sensitivity");
}

void VehicleHandlingFlyingMenu::Update() {
	if (!Menu::GetLocalPlayer().m_InVehicle) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}

	uint32_t Model = Native::GetEntityModel(Menu::GetLocalPlayer().m_Vehicle);
	if (!(Native::IsThisModelAPlane(Model) || Native::IsThisModelAHeli(Model))) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}
}

/*Called once on submenu open*/
void VehicleHandlingFlyingMenu::UpdateOnce() {
	ClearOptionsOffset(1);

	uint32_t Model = Native::GetEntityModel(Menu::GetLocalPlayer().m_Vehicle);

	/* Thrust */
	if (m_Vars.m_Type == 0) {
		CacheThrust(Model);

		addOption(NumberOption<float>(SCROLL, getString("Thrust"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_Thrust, "%.1f", 0.1f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Thrust Falloff"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_ThrustFallOff, "%.1f", 0.1f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Thrust Vectoring"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_ThrustVectoring, "%.1f", 0.1f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());
	}

	/* Movement */
	if (m_Vars.m_Type == 1) {
		CacheMovement(Model);

		addOption(NumberOption<float>(SCROLL, getString("Side Slip Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_SideSlipMult, "%.2f", 0.01f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Yaw Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_YawMult, "%.3f", 0.001f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Yaw Stabilize"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_YawStabilise, "%.3f", 0.001f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Roll Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_RollMult, "%.3f", 0.001f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Roll Stabilize"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_RollStabilise, "%.3f", 0.001f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Pitch Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_PitchMult, "%.3f", 0.001f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Pitch Stabilize"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_PitchStabilise, "%.3f", 0.001f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Form Lift Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_FormLiftMult, "%.2f", 0.01f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Attack Lift Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_AttackLiftMult, "%.2f", 0.01f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Attack Dive Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_AttackDiveMult, "%.3f", 0.001f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Engine Off Glide Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_EngineOffGlideMulti, "%.0f", 1.f); })
			.addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax());
	}

	/* Resistance */
	if (m_Vars.m_Type == 2) {
		CacheResistance(Model);

		addOption(NumberOption<float>(SCROLL, getString("Wind Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_WindMult, "%.1f", 0.1f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Move Resistance"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_MoveRes, "%.3f", 0.001f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Turn Resistance - Horizontal (X)"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_VecTurnResistance.m_X, "%.2f", 0.01f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Turn Resistance - Vertical (Y)"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_VecTurnResistance.m_Y, "%.2f", 0.01f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Turn Resistance - Height (Z)"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_VecTurnResistance.m_Z, "%.2f", 0.01f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Speed Resistance - Horizontal (X)"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_VecSpeedResistance.m_X, "%.2f", 0.01f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Speed Resistance - Vertical (Y)"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_VecSpeedResistance.m_Y, "%.2f", 0.01f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Speed Resistance - Height (Z)"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_VecSpeedResistance.m_Z, "%.2f", 0.01f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());
	}

	/* Turbulence */
	if (m_Vars.m_Type == 3) {
		CacheTurbulence(Model);

		addOption(NumberOption<float>(SCROLL, getString("Maximum Magnitude"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_TurublenceMagnitudeMax, "%.0f", 1.f); })
			.addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Force Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_TurublenceForceMulti, "%.1f", 0.1f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Roll Torque Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_TurublenceRollTorqueMulti, "%.2f", 0.01f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Pitch Torque Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_TurublencePitchTorqueMulti, "%.2f", 0.01f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());
	}

	/* Gear */
	if (m_Vars.m_Type == 4) {
		CacheGear(Model);

		addOption(NumberOption<float>(SCROLL, getString("Down Drag"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_GearDownDragV, "%.2f", 0.01f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Down Lift Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_GearDownLiftMult, "%.1f", 0.1f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());
	}

	/* Ground */
	if (m_Vars.m_Type == 5) {
		CacheGround(Model);

		addOption(NumberOption<float>(SCROLL, getString("Yaw Boost Speed Peak"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_OnGroundYawBoostSpeedPeak, "%.0f", 1.f); })
			.addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Yaw Boost Speed Cap"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_OnGroundYawBoostSpeedCap, "%.0f", 1.f); })
			.addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax());
	}

	/* Miscellaneous */
	if (m_Vars.m_Type == 6) {
		CacheMisc(Model);

		addOption(NumberOption<float>(SCROLL, getString("Body Damage Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_BodyDamageControlEffectMult, "%.1f", 0.1f); })
			.addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Input Sensitivity"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_InputSensitivityForDifficulty, "%.1f", 0.1f); })
			.addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax());
	}
}

/*Called always*/
void VehicleHandlingFlyingMenu::FeatureUpdate() {}

VehicleHandlingFlyingMenu* _instance;
VehicleHandlingFlyingMenu* VehicleHandlingFlyingMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleHandlingFlyingMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleHandlingFlyingMenu::~VehicleHandlingFlyingMenu() { delete _instance; }