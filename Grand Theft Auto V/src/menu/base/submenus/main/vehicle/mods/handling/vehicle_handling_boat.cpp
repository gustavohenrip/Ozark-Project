#include "vehicle_handling_boat.hpp"
#include "menu/base/submenu_handler.hpp"
#include "vehicle_handling_main.hpp"

using namespace VehicleHandlingBoatMenuVars;

namespace VehicleHandlingBoatMenuVars {
	Vars_ m_Vars;

	Rage::Types::BoatHandlingData* GetData() {
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

	void Reset() {
		uint32_t Model = Native::GetEntityModel(Menu::GetLocalPlayer().m_Vehicle);

		switch (m_Vars.m_Type) {
			case 0: /* Physical */
				if (m_Vars.m_PhysicalRestore.find(Model) != m_Vars.m_PhysicalRestore.end()) {
					auto& Backup = m_Vars.m_PhysicalRestore[Model];
					GetData()->m_BoxFrontMult = Backup.m_BoxFrontMult;
					GetData()->m_BoxRearMult = Backup.m_BoxRearMult;
					GetData()->m_BoxSideMult = Backup.m_BoxSideMult;
					GetData()->m_SampleTop = Backup.m_SampleTop;
					GetData()->m_SampleBottom = Backup.m_SampleBottom;
				}
				break;

			case 1: /* Aquaplane */
				if (m_Vars.m_AquaplaneRestore.find(Model) != m_Vars.m_AquaplaneRestore.end()) {
					auto& Backup = m_Vars.m_AquaplaneRestore[Model];
					GetData()->m_AquaplaneForce = Backup.m_AquaplaneForce;
					GetData()->m_AquaplanePushWaterMult = Backup.m_AquaplanePushWaterMult;
					GetData()->m_AquaplanePushWaterCap = Backup.m_AquaplanePushWaterCap;
					GetData()->m_AquaplanePushWaterApply = Backup.m_AquaplanePushWaterApply;
					GetData()->m_KeelSphereSize = Backup.m_KeelSphereSize;
					GetData()->m_PropRadius = Backup.m_PropRadius;
					GetData()->m_ImpellerOffset = Backup.m_ImpellerOffset;
					GetData()->m_ImpellerForceMult = Backup.m_ImpellerForceMult;
					GetData()->m_ProwRaiseMult = Backup.m_ProwRaiseMult;
				}
				break;

			case 2: /* Rudder */
				if (m_Vars.m_RudderRestore.find(Model) != m_Vars.m_RudderRestore.end()) {
					auto& Backup = m_Vars.m_RudderRestore[Model];
					GetData()->m_RudderForce = Backup.m_RudderForce;
					GetData()->m_RudderOffsetSubmerge = Backup.m_RudderOffsetSubmerge;
					GetData()->m_RudderOffsetForce = Backup.m_RudderOffsetForce;
					GetData()->m_RudderOffsetForceZMult = Backup.m_RudderOffsetForceZMult;
				}
				break;

			case 3: /* Resistance */
				if (m_Vars.m_ResistanceRestore.find(Model) != m_Vars.m_ResistanceRestore.end()) {
					auto& Backup = m_Vars.m_ResistanceRestore[Model];
					GetData()->m_DragCoefficient = Backup.m_DragCoefficient;
					GetData()->m_VecMoveResistance = Backup.m_VecMoveResistance;
					GetData()->m_VecTurnResistance = Backup.m_VecTurnResistance;
				}
				break;

			case 4: /* Misc */
				if (m_Vars.m_MiscRestore.find(Model) != m_Vars.m_MiscRestore.end()) {
					auto& Backup = m_Vars.m_MiscRestore[Model];
					GetData()->m_WaveAudioMult = Backup.m_WaveAudioMult;
					GetData()->m_Look_L_R_CamHeight = Backup.m_Look_L_R_CamHeight;
					GetData()->m_LowLodAngOffset = Backup.m_LowLodAngOffset;
					GetData()->m_LowLodDraughtOffset = Backup.m_LowLodDraughtOffset;
					GetData()->m_DinghySphereBuoyConst = Backup.m_DinghySphereBuoyConst;
				}
				break;
		}
	}

	void CachePhysical(uint32_t Model) {
		if (m_Vars.m_PhysicalRestore.find(Model) == m_Vars.m_PhysicalRestore.end()) {
			m_Vars.m_PhysicalRestore[Model] = {
				GetData()->m_BoxFrontMult,
				GetData()->m_BoxRearMult,
				GetData()->m_BoxSideMult,
				GetData()->m_SampleTop,
				GetData()->m_SampleBottom
			};
		}
	}

	void CacheAquaplane(uint32_t Model) {
		if (m_Vars.m_AquaplaneRestore.find(Model) == m_Vars.m_AquaplaneRestore.end()) {
			m_Vars.m_AquaplaneRestore[Model] = {
				GetData()->m_AquaplaneForce,
				GetData()->m_AquaplanePushWaterMult,
				GetData()->m_AquaplanePushWaterCap,
				GetData()->m_AquaplanePushWaterApply,
				GetData()->m_KeelSphereSize,
				GetData()->m_PropRadius,
				GetData()->m_ImpellerOffset,
				GetData()->m_ImpellerForceMult,
				GetData()->m_ProwRaiseMult
			};
		}
	}

	void CacheRudder(uint32_t Model) {
		if (m_Vars.m_RudderRestore.find(Model) == m_Vars.m_RudderRestore.end()) {
			m_Vars.m_RudderRestore[Model] = {
				GetData()->m_RudderForce,
				GetData()->m_RudderOffsetSubmerge,
				GetData()->m_RudderOffsetForce,
				GetData()->m_RudderOffsetForceZMult
			};
		}
	}

	void CacheResistance(uint32_t Model) {
		if (m_Vars.m_ResistanceRestore.find(Model) == m_Vars.m_ResistanceRestore.end()) {
			m_Vars.m_ResistanceRestore[Model] = {
				GetData()->m_DragCoefficient,
				GetData()->m_VecMoveResistance,
				GetData()->m_VecTurnResistance
			};
		}
	}

	void CacheMisc(uint32_t Model) {
		if (m_Vars.m_MiscRestore.find(Model) == m_Vars.m_MiscRestore.end()) {
			m_Vars.m_MiscRestore[Model] = {
				GetData()->m_WaveAudioMult,
				GetData()->m_Look_L_R_CamHeight,
				GetData()->m_LowLodAngOffset,
				GetData()->m_LowLodDraughtOffset,
				GetData()->m_DinghySphereBuoyConst
			};
		}
	}
}

void VehicleHandlingBoatMenu::Init() {
	SetName("Boat Handling");
	SetParentSubmenu<VehicleModsHandlingMainMenu>();

	addOption(ButtonOption("Reset to Default")
		.addTranslation()
		.addSprite("Ozark", "spinner")
		.addSpriteScale(0.01232f, 0.02184f)
		.addSpriteRotate()
		.addOnClick(Reset));

	addString("Box Front Multiplier");
	addString("Box Rear Multiplier");
	addString("Box Side Multiplier");
	addString("Sample Top");
	addString("Sample Bottom");

	addString("Aquaplane Force");
	addString("Aquaplane Push Water Multiplier");
	addString("Aquaplane Push Water Cap");
	addString("Aquaplane Push Water Apply");
	addString("Keel Sphere Size");
	addString("Propeller Radius");
	addString("Impeller Offset");
	addString("Impeller Force Multiplier");
	addString("Prow Raise Multiplier");

	addString("Rudder Force");
	addString("Rudder Offset Submerge");
	addString("Rudder Offset Force");
	addString("Rudder Offset Force Z Multiplier");

	addString("Drag Coefficient");
	addString("Move Resistance - Horizontal (X)");
	addString("Move Resistance - Vertical (Y)");
	addString("Move Resistance - Height (Z)");
	addString("Turn Resistance - Horizontal (X)");
	addString("Turn Resistance - Vertical (Y)");
	addString("Turn Resistance - Height (Z)");

	addString("Wave Audio Multiplier");
	addString("Look L/R Cam Height");
	addString("Low Lod Angular Offset");
	addString("Low Lod Draught Offset");
	addString("Dinghy Sphere Buoy Const");
}

void VehicleHandlingBoatMenu::Update() {
	if (!Menu::GetLocalPlayer().m_InVehicle) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}

	uint32_t Model = Native::GetEntityModel(Menu::GetLocalPlayer().m_Vehicle);
	if (!(Native::IsThisModelABoat(Model) || Native::_IsThisModelAnEmergencyBoat(Model))) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}
}

/*Called once on submenu open*/
void VehicleHandlingBoatMenu::UpdateOnce() {
	ClearOptionsOffset(1);

	uint32_t Model = Native::GetEntityModel(Menu::GetLocalPlayer().m_Vehicle);

	/* Physical */
	if (m_Vars.m_Type == 0) {
		CachePhysical(Model);

		addOption(NumberOption<float>(SCROLL, getString("Box Front Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_BoxFrontMult, "%.1f", 0.1f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Box Rear Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_BoxRearMult, "%.1f", 0.1f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Box Side Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_BoxSideMult, "%.1f", 0.1f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Sample Top"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_SampleTop, "%.1f", 0.1f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Sample Bottom"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_SampleBottom, "%.1f", 0.1f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());
	}

	/* Aquaplane */
	if (m_Vars.m_Type == 1) {
		CacheAquaplane(Model);

		addOption(NumberOption<float>(SCROLL, getString("Aquaplane Force"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_AquaplaneForce, "%.2f", 0.01f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Aquaplane Push Water Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_AquaplanePushWaterMult, "%.1f", 0.1f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Aquaplane Push Water Cap"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_AquaplanePushWaterCap, "%.1f", 0.1f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Aquaplane Push Water Apply"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_AquaplanePushWaterApply, "%.3f", 0.3f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Keel Sphere Size"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_KeelSphereSize, "%.0f", 1.f); })
			.addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Propeller Radius"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_PropRadius, "%.1f", 0.1f); })
			.addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Impeller Offset"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_ImpellerOffset, "%.0f", 1.f); })
			.addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Impeller Force Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_ImpellerForceMult, "%.0f", 1.f); })
			.addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Prow Raise Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_ProwRaiseMult, "%.1f", 0.1f); })
			.addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax());
	}

	/* Rudder */
	if (m_Vars.m_Type == 2) {
		CacheRudder(Model);

		addOption(NumberOption<float>(SCROLL, getString("Rudder Force"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_RudderForce, "%.1f", 0.1f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Rudder Offset Submerge"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_RudderOffsetSubmerge, "%.1f", 0.1f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Rudder Offset Force"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_RudderOffsetForce, "%.1f", 0.1f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Rudder Offset Force Z Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_RudderOffsetForceZMult, "%.1f", 0.1f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());
	}

	/* Resistance */
	if (m_Vars.m_Type == 3) {
		CacheResistance(Model);

		addOption(NumberOption<float>(SCROLL, getString("Drag Coefficient"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_DragCoefficient, "%.0f", 1.f); })
			.addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Move Resistance - Horizontal (X)"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_VecMoveResistance.m_X, "%.0f", 1.f); })
			.addMin(-10000.f).addMax(10000.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Move Resistance - Vertical (Y)"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_VecMoveResistance.m_Y, "%.0f", 1.f); })
			.addMin(-10000.f).addMax(10000.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Move Resistance - Height (Z)"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_VecMoveResistance.m_Z, "%.0f", 1.f); })
			.addMin(-10000.f).addMax(10000.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Turn Resistance - Horizontal (X)"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_VecTurnResistance.m_X, "%.1f", 0.1f); })
			.addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Turn Resistance - Vertical (Y)"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_VecTurnResistance.m_Y, "%.1f", 0.1f); })
			.addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Turn Resistance - Height (Z)"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_VecTurnResistance.m_Z, "%.1f", 0.1f); })
			.addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax());
	}

	/* Miscellaneous */
	if (m_Vars.m_Type == 4) {
		CacheMisc(Model);

		addOption(NumberOption<float>(SCROLL, getString("Wave Audio Multiplier"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_WaveAudioMult, "%.1f", 0.1f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Look L/R Cam Height"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_Look_L_R_CamHeight, "%.0f", 1.f); })
			.addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Low Lod Angular Offset"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_LowLodAngOffset, "%.2f", 0.01f); })
			.addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Low Lod Draught Offset"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_LowLodDraughtOffset, "%.3f", 0.3f); })
			.addMin(-10.f).addMax(100.f).setScrollSpeed(10).showMax());

		addOption(NumberOption<float>(SCROLL, getString("Dinghy Sphere Buoy Const"))
			.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; })
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(GetData()->m_DinghySphereBuoyConst, "%.0f", 1.f); })
			.addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax());
	}
}

/*Called always*/
void VehicleHandlingBoatMenu::FeatureUpdate() {}

VehicleHandlingBoatMenu* _instance;
VehicleHandlingBoatMenu* VehicleHandlingBoatMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleHandlingBoatMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleHandlingBoatMenu::~VehicleHandlingBoatMenu() { delete _instance; }