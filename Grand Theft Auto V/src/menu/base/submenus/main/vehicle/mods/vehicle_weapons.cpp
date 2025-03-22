#include "vehicle_weapons.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_mods.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/timers.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/util/pools/pool_manager.hpp"

using namespace VehicleModsWeaponsMenuVars;

namespace VehicleModsWeaponsMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> LineType[] = {
		{ TranslationString("Point", true), 0 },
		{ TranslationString("Cross", true), 1 },
		{ TranslationString("Straight", true), 2 }
	};
}

void VehicleModsWeaponsMenu::Init() {
	SetName("Weapons");
	SetParentSubmenu<VehicleModsMenu>();

	addOption(ToggleOption("Toggle Weapons")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Toggle));

	addOption(NumberOption<float>(SCROLL, "Bullet Speed")
		.addTranslation()
		.addNumber(m_Vars.m_Speed, "%.0f", 1.f).addMax(1.f).addMax(10000.f).setScrollSpeed(5));

	addOption(ScrollOption<int>(TOGGLE, "Aiming Lines")
		.addTranslation()
		.addToggle(m_Vars.m_AimingLines)
		.addScroll(m_Vars.m_AimingLineType, 0, NUMOF(LineType), LineType).canBeSaved(m_ParentNameStack));

	addOption(BreakOption("Bullets")
		.addTranslation());

	for (auto& Bullet : Global::Arrays::g_VehicleWeapons) {
		addOption(RadioOption(Bullet.m_Name.Get())
			.addTranslation()
			.addRadio(m_Vars.m_Context));
	}
}

void VehicleModsWeaponsMenu::Update() {}

/*Called once on submenu open*/
void VehicleModsWeaponsMenu::UpdateOnce() {}

/*Called always*/
void VehicleModsWeaponsMenu::FeatureUpdate() {
	if (Menu::GetLocalPlayer().m_InVehicle) {
		if (m_Vars.m_Toggle) {
			static int Timer = 0;

			Math::Vector3<float> ModelSizeMin;
			Math::Vector3<float> ModelSizeMax;
			Native::GetModelDimensions(Native::GetEntityModel(Menu::GetLocalPlayer().m_Vehicle), &ModelSizeMin, &ModelSizeMax);

			Math::Vector3<float> OriginLeft;
			Math::Vector3<float> OriginRight;
			Math::Vector3<float> TargetLeft;
			Math::Vector3<float> TargetRight;

			// Min: -1.096 -2.676 -0.818, Max: 1.096 2.577 1.103

			auto Calculation = ModelSizeMax - ModelSizeMin;

			switch (m_Vars.m_AimingLineType) {
				case 0:
					OriginLeft = Native::GetOffsetFromEntityInWorldCoords(Menu::GetLocalPlayer().m_Vehicle, Calculation.m_X / 4.f, ModelSizeMax.m_Y + 1.f, 0.5f);
					OriginRight = Native::GetOffsetFromEntityInWorldCoords(Menu::GetLocalPlayer().m_Vehicle, -(Calculation.m_X / 4.f), ModelSizeMax.m_Y + 1.f, 0.5f);

					TargetLeft = Native::GetOffsetFromEntityInWorldCoords(Menu::GetLocalPlayer().m_Vehicle, (Calculation.m_X / 4.f) - 0.9f, ModelSizeMax.m_Y + 300.f, 0.5f);
					TargetRight = Native::GetOffsetFromEntityInWorldCoords(Menu::GetLocalPlayer().m_Vehicle, -(Calculation.m_X / 4.f) + 0.9f, ModelSizeMax.m_Y + 300.f, 0.5f);
					break;
				case 1:
					OriginLeft = Native::GetOffsetFromEntityInWorldCoords(Menu::GetLocalPlayer().m_Vehicle, Calculation.m_X / 4.f, ModelSizeMax.m_Y + 1.f, 0.5f);
					OriginRight = Native::GetOffsetFromEntityInWorldCoords(Menu::GetLocalPlayer().m_Vehicle, -(Calculation.m_X / 4.f), ModelSizeMax.m_Y + 1.f, 0.5f);

					TargetLeft = Native::GetOffsetFromEntityInWorldCoords(Menu::GetLocalPlayer().m_Vehicle, (Calculation.m_X / 4.f) - 5.f, ModelSizeMax.m_Y + 300.f, 0.5f);
					TargetRight = Native::GetOffsetFromEntityInWorldCoords(Menu::GetLocalPlayer().m_Vehicle, -(Calculation.m_X / 4.f) + 5.f, ModelSizeMax.m_Y + 300.f, 0.5f);
					break;
				case 2:
					OriginLeft = Native::GetOffsetFromEntityInWorldCoords(Menu::GetLocalPlayer().m_Vehicle, Calculation.m_X / 4.f, ModelSizeMax.m_Y + 1.f, 0.5f);
					OriginRight = Native::GetOffsetFromEntityInWorldCoords(Menu::GetLocalPlayer().m_Vehicle, -(Calculation.m_X / 4.f), ModelSizeMax.m_Y + 1.f, 0.5f);

					TargetLeft = Native::GetOffsetFromEntityInWorldCoords(Menu::GetLocalPlayer().m_Vehicle, (Calculation.m_X / 4.f) + 8.f, ModelSizeMax.m_Y + 300.f, 0.5f);
					TargetRight = Native::GetOffsetFromEntityInWorldCoords(Menu::GetLocalPlayer().m_Vehicle, -(Calculation.m_X / 4.f) - 8.f, ModelSizeMax.m_Y + 300.f, 0.5f);
					break;
			}

			Menu::Timers::RunTimedFunction(&Timer, 100, [=] {
				if (Native::IsHornActive(Menu::GetLocalPlayer().m_Vehicle)) {
					Menu::GetControlManager()->SimpleRequestWeaponAsset(Global::Arrays::g_VehicleWeapons[m_Vars.m_Context.m_Selected].m_Result);
					Native::ShootSingleBulletBetweenCoordsIgnoreEntity(OriginRight.m_X, OriginRight.m_Y, OriginRight.m_Z, TargetRight.m_X, TargetRight.m_Y, TargetRight.m_Z, 250, 0, Global::Arrays::g_VehicleWeapons[m_Vars.m_Context.m_Selected].m_Result, 0, true, false, Native::GetEntitySpeed(Menu::GetLocalPlayer().m_Vehicle) + m_Vars.m_Speed, Menu::GetLocalPlayer().m_Entity, 0);
					Native::ShootSingleBulletBetweenCoordsIgnoreEntity(OriginLeft.m_X, OriginLeft.m_Y, OriginLeft.m_Z, TargetLeft.m_X, TargetLeft.m_Y, TargetLeft.m_Z, 250, 0, Global::Arrays::g_VehicleWeapons[m_Vars.m_Context.m_Selected].m_Result, 0, true, false, Native::GetEntitySpeed(Menu::GetLocalPlayer().m_Vehicle) + m_Vars.m_Speed, Menu::GetLocalPlayer().m_Entity, 0);
				}
			});

			if (m_Vars.m_AimingLines) {
				Native::DrawLine(OriginRight.m_X, OriginRight.m_Y, OriginRight.m_Z, TargetRight.m_X, TargetRight.m_Y, TargetRight.m_Z, 255, 0, 0, 255);
				Native::DrawLine(OriginLeft.m_X, OriginLeft.m_Y, OriginLeft.m_Z, TargetLeft.m_X, TargetLeft.m_Y, TargetLeft.m_Z, 255, 0, 0, 255);
			}
		}
	}
}

VehicleModsWeaponsMenu* _instance;
VehicleModsWeaponsMenu* VehicleModsWeaponsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleModsWeaponsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleModsWeaponsMenu::~VehicleModsWeaponsMenu() { delete _instance; }