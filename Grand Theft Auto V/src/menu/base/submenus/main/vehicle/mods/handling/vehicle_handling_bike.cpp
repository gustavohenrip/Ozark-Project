#include "vehicle_handling_bike.hpp"
#include "menu/base/submenu_handler.hpp"
#include "vehicle_handling_main.hpp"

using namespace VehicleHandlingBikeMenuVars;

namespace VehicleHandlingBikeMenuVars {
	Vars_ m_Vars;

	Rage::Types::BikeHandlingData* GetData() {
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

	void Reset() {
		uint32_t Model = Native::GetEntityModel(Menu::GetLocalPlayer().m_Vehicle);

		switch (m_Vars.m_Type) {
			case 0: /* Physical */
				if (m_Vars.m_PhysicalRestore.find(Model) != m_Vars.m_PhysicalRestore.end()) {
					auto& Backup = m_Vars.m_PhysicalRestore[Model];
					GetData()->m_LeanFwdCOMMult = Backup.m_LeanFwdCOMMult;
					GetData()->m_LeanFwdForceMult = Backup.m_LeanFwdForceMult;
					GetData()->m_LeanBakCOMMult = Backup.m_LeanBakCOMMult;
					GetData()->m_LeanBakForceMult = Backup.m_LeanBakForceMult;
					GetData()->m_MaxBankAngle = Backup.m_MaxBankAngle;
					GetData()->m_FullAnimAngle = Backup.m_FullAnimAngle;
					GetData()->m_DesLeanReturnFrac = Backup.m_DesLeanReturnFrac;
					GetData()->m_StickLeanMult = Backup.m_StickLeanMult;
					GetData()->m_BrakingStabilityMult = Backup.m_BrakingStabilityMult;
					GetData()->m_InAirSteerMult = Backup.m_InAirSteerMult;
					GetData()->m_JumpForce = Backup.m_JumpForce;
				}
				break;

			case 1: /* Balance */
				if (m_Vars.m_BalanceRestore.find(Model) != m_Vars.m_BalanceRestore.end()) {
					auto& Backup = m_Vars.m_BalanceRestore[Model];
					GetData()->m_BikeOnStandSteerAngle = Backup.m_BikeOnStandSteerAngle;
					GetData()->m_BikeOnStandLeanAngle = Backup.m_BikeOnStandLeanAngle;
					GetData()->m_FrontBalanceMult = Backup.m_FrontBalanceMult;
					GetData()->m_RearBalanceMult = Backup.m_RearBalanceMult;
					GetData()->m_WheelieBalancePoint = Backup.m_WheelieBalancePoint;
					GetData()->m_StoppieBalancePoint = Backup.m_StoppieBalancePoint;
					GetData()->m_WheelieSteerMult = Backup.m_WheelieSteerMult;
				}
				break;

			case 2: /* Friction */
				if (m_Vars.m_FrictionRestore.find(Model) != m_Vars.m_FrictionRestore.end()) {
					auto& Backup = m_Vars.m_FrictionRestore[Model];
					GetData()->m_BikeGroundSideFrictionMult = Backup.m_BikeGroundSideFrictionMult;
					GetData()->m_BikeWheelGroundSideFrictionMult = Backup.m_BikeWheelGroundSideFrictionMult;
				}
				break;
		}
	}

	void CachePhysical(uint32_t Model) {
		if (m_Vars.m_PhysicalRestore.find(Model) == m_Vars.m_PhysicalRestore.end()) {
			m_Vars.m_PhysicalRestore[Model] = {
				GetData()->m_LeanFwdCOMMult,
				GetData()->m_LeanFwdForceMult,
				GetData()->m_LeanBakCOMMult,
				GetData()->m_LeanBakForceMult,
				GetData()->m_MaxBankAngle,
				GetData()->m_FullAnimAngle,
				GetData()->m_DesLeanReturnFrac,
				GetData()->m_StickLeanMult,
				GetData()->m_BrakingStabilityMult,
				GetData()->m_InAirSteerMult,
				GetData()->m_JumpForce,
			};
		}
	}

	void CacheBalance(uint32_t Model) {
		if (m_Vars.m_BalanceRestore.find(Model) == m_Vars.m_BalanceRestore.end()) {
			m_Vars.m_BalanceRestore[Model] = {
				GetData()->m_BikeOnStandSteerAngle,
				GetData()->m_BikeOnStandLeanAngle,
				GetData()->m_FrontBalanceMult,
				GetData()->m_RearBalanceMult,
				GetData()->m_WheelieBalancePoint,
				GetData()->m_StoppieBalancePoint,
				GetData()->m_WheelieSteerMult
			};
		}
	}

	void CacheFriction(uint32_t Model) {
		if (m_Vars.m_FrictionRestore.find(Model) == m_Vars.m_FrictionRestore.end()) {
			m_Vars.m_FrictionRestore[Model] = {
				GetData()->m_BikeGroundSideFrictionMult,
				GetData()->m_BikeWheelGroundSideFrictionMult
			};
		}
	}
}

void VehicleHandlingBikeMenu::Init() {
	SetName("Bike Handling");
	SetParentSubmenu<VehicleModsHandlingMainMenu>();

	addOption(ButtonOption("Reset to Default")
		.addTranslation()
		.addSprite("Ozark", "spinner")
		.addSpriteScale(0.01232f, 0.02184f)
		.addSpriteRotate()
		.addOnClick(Reset));

	addString("Lean Forward COM Multiplier");
	addString("Lean Forward Force Multiplier");
	addString("Lean Backward COM Multiplier");
	addString("Lean Backward Force Multiplier");
	addString("Max Bank Angle");
	addString("Full Animation Angle");
	addString("Lean Return Fraction");
	addString("Stick Lean Multiplier");
	addString("Braking Stability Multiplier");
	addString("In Air Steer Multiplier");
	addString("Jump Force");

	addString("On Stand Steer Angle");
	addString("On Stand Lean Angle");
	addString("Front Balance Multiplier");
	addString("Rear Balance Multiplier");
	addString("Wheelie Balance Point");
	addString("Stoppie Balance Point");
	addString("Wheelie Steer Multiplier");

	addString("Ground Side Friction Multiplier");
	addString("Wheel Ground Side Friction Multiplier");
}

void VehicleHandlingBikeMenu::Update() {
	if (!Menu::GetLocalPlayer().m_InVehicle) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}

	uint32_t Model = Native::GetEntityModel(Menu::GetLocalPlayer().m_Vehicle);
	if (!(Native::IsThisModelABicycle(Model) || Native::IsThisModelABike(Model))) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}
}

/*Called once on submenu open*/
void VehicleHandlingBikeMenu::UpdateOnce() {
	ClearOptionsOffset(1);

	uint32_t Model = Native::GetEntityModel(Menu::GetLocalPlayer().m_Vehicle);

	/* Physical */
	if (m_Vars.m_Type == 0) {
		CachePhysical(Model);

		addOption(NumberOption<float>(SCROLL, getString("Lean Forward COM Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_LeanFwdCOMMult, "%.1f", 0.1f); })
			.addMin(-500.f).addMax(500.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Lean Forward Force Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_LeanFwdForceMult, "%.1f", 0.1f); })
			.addMin(-500.f).addMax(500.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Lean Backward COM Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_LeanBakCOMMult, "%.1f", 0.1f); })
			.addMin(-500.f).addMax(500.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Lean Backward Force Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_LeanBakForceMult, "%.1f", 0.1f); })
			.addMin(-500.f).addMax(500.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Max Bank Angle"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_MaxBankAngle, "%.2f", 0.01f); })
			.addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Full Animation Angle"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_FullAnimAngle, "%.2f", 0.01f); })
			.addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Lean Return Fraction"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_DesLeanReturnFrac, "%.2f", 0.01f); })
			.addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Stick Lean Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_StickLeanMult, "%.1f", 0.1f); })
			.addMin(-500.f).addMax(500.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Braking Stability Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_BrakingStabilityMult, "%.1f", 0.1f); })
			.addMin(-500.f).addMax(500.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("In Air Steer Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_InAirSteerMult, "%.1f", 0.1f); })
			.addMin(-500.f).addMax(500.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Jump Force"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_JumpForce, "%.1f", 0.1f); })
			.addMin(-500.f).addMax(500.f).setScrollSpeed(10).showMax());
	}

	/* Balance */
	if (m_Vars.m_Type == 1) {
		CacheBalance(Model);

		addOption(NumberOption<float>(SCROLL, getString("On Stand Steer Angle"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_BikeOnStandSteerAngle, "%.1f", 0.1f); })
			.addMin(-500.f).addMax(500.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("On Stand Lean Angle"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_BikeOnStandLeanAngle, "%.1f", 0.1f); })
			.addMin(-500.f).addMax(500.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Front Balance Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_FrontBalanceMult, "%.1f", 0.1f); })
			.addMin(-500.f).addMax(500.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Rear Balance Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_RearBalanceMult, "%.1f", 0.1f); })
			.addMin(-500.f).addMax(500.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Wheelie Balance Point"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_WheelieBalancePoint, "%.1f", 0.1f); })
			.addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Stoppie Balance Point"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_StoppieBalancePoint, "%.1f", 0.1f); })
			.addMin(-100.f).addMax(500.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Wheelie Steer Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_WheelieSteerMult, "%.1f", 0.1f); })
			.addMin(-500.f).addMax(500.f).setScrollSpeed(10).showMax());
	}

	/* Friction */
	if (m_Vars.m_Type == 2) {
		CacheFriction(Model);

		addOption(NumberOption<float>(SCROLL, getString("Ground Side Friction Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_BikeGroundSideFrictionMult, "%.1f", 0.1f); })
			.addMin(-500.f).addMax(500.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Wheel Ground Side Friction Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_BikeWheelGroundSideFrictionMult, "%.1f", 0.1f); })
			.addMin(-500.f).addMax(500.f).setScrollSpeed(10).showMax());
	}
}

/*Called always*/
void VehicleHandlingBikeMenu::FeatureUpdate() {}

VehicleHandlingBikeMenu* _instance;
VehicleHandlingBikeMenu* VehicleHandlingBikeMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleHandlingBikeMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleHandlingBikeMenu::~VehicleHandlingBikeMenu() { delete _instance; }