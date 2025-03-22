#include "vehicle_editor_wheel_type.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_editor_wheels.hpp"
#include "../../vehicle_editor.hpp"
#include "vehicle_editor_wheel_type_edit.hpp"

using namespace VehicleEditorWheelTypeMenuVars;

namespace VehicleEditorWheelTypeMenuVars {
	Vars_ m_Vars;
}

void VehicleEditorWheelTypeMenu::Init() {
	SetName("Wheel Type");
	SetParentSubmenu<VehicleEditorWheelsMenu>();

	addString("Front Wheel");
	addString("Rear Wheel");
	addString("High End");
	addString("Lowrider");
	addString("Muscle");
	addString("Offroad");
	addString("Sport");
	addString("SUV");
	addString("Tuner");
}

void VehicleEditorWheelTypeMenu::Update() {
	if (!Menu::GetLocalPlayer().m_InVehicle) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}

	VehicleEditorMenuVars::Scripts::DrawVehicleStats(Menu::GetLocalPlayer().m_Vehicle);
	if (Menu::GetBase()->m_ScrollOffset < Menu::GetSubmenuHandler()->GetTotalOptions() - Menu::GetBase()->m_MaxOptions && Menu::GetBase()->m_CurrentOption - Menu::GetBase()->m_ScrollOffset == Menu::GetBase()->m_MaxOptions) Menu::GetBase()->m_ScrollOffset++;
}

/*Called once on submenu open*/
void VehicleEditorWheelTypeMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	if (m_Vars.m_UpdateCache) {
		m_Vars.m_UpdateCache = false;
		
		if (Menu::GetLocalPlayer().m_InVehicle) {
			Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_CachedWheelType);
			Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_ModType, m_Vars.m_CachedWheelIndex, Native::GetVehicleModVariation(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_ModType));
		}
	}

	if (Native::IsThisModelABike(Native::GetEntityModel(Menu::GetLocalPlayer().m_Vehicle))) {
		addOption(SubmenuOption(getString("Front Wheel"))
			.addSubmenu<VehicleEditorWheelTypeEditMenu>()
			.addOnClick([] {
				VehicleEditorWheelTypeEditMenuVars::m_Vars.m_WheelType = 1;
				m_Vars.m_CachedWheelIndex = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS);
				m_Vars.m_CachedWheelType = Native::GetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle);
				m_Vars.m_ModType = MOD_FRONTWHEELS;
			}));

		addOption(SubmenuOption(getString("Rear Wheel"))
			.addSubmenu<VehicleEditorWheelTypeEditMenu>()
			.addOnClick([] {
				VehicleEditorWheelTypeEditMenuVars::m_Vars.m_WheelType = 2;
				m_Vars.m_CachedWheelIndex = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_BACKWHEELS);
				m_Vars.m_CachedWheelType = Native::GetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle);
				m_Vars.m_ModType = MOD_BACKWHEELS;
			}));
	} else {
		addOption(SubmenuOption(getString("High End"))
			.addSubmenu<VehicleEditorWheelTypeEditMenu>()
			.addOnClick([] {
				VehicleEditorWheelTypeEditMenuVars::m_Vars.m_WheelType = 3;
				m_Vars.m_CachedWheelIndex = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS);
				m_Vars.m_CachedWheelType = Native::GetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle);
				m_Vars.m_ModType = MOD_FRONTWHEELS;
			}));

		addOption(SubmenuOption(getString("Lowrider"))
			.addSubmenu<VehicleEditorWheelTypeEditMenu>()
			.addOnClick([] {
				VehicleEditorWheelTypeEditMenuVars::m_Vars.m_WheelType = 4;
				m_Vars.m_CachedWheelIndex = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS);
				m_Vars.m_CachedWheelType = Native::GetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle);
				m_Vars.m_ModType = MOD_FRONTWHEELS;
			}));

		addOption(SubmenuOption(getString("Muscle"))
			.addSubmenu<VehicleEditorWheelTypeEditMenu>()
			.addOnClick([] {
				VehicleEditorWheelTypeEditMenuVars::m_Vars.m_WheelType = 5;
				m_Vars.m_CachedWheelIndex = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS);
				m_Vars.m_CachedWheelType = Native::GetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle);
				m_Vars.m_ModType = MOD_FRONTWHEELS;
			}));

		addOption(SubmenuOption(getString("Offroad"))
			.addSubmenu<VehicleEditorWheelTypeEditMenu>()
			.addOnClick([] {
				VehicleEditorWheelTypeEditMenuVars::m_Vars.m_WheelType = 6;
				m_Vars.m_CachedWheelIndex = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS);
				m_Vars.m_CachedWheelType = Native::GetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle);
				m_Vars.m_ModType = MOD_FRONTWHEELS;
			}));

		addOption(SubmenuOption(getString("Sport"))
			.addSubmenu<VehicleEditorWheelTypeEditMenu>()
			.addOnClick([] {
				VehicleEditorWheelTypeEditMenuVars::m_Vars.m_WheelType = 7;
				m_Vars.m_CachedWheelIndex = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS);
				m_Vars.m_CachedWheelType = Native::GetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle);
				m_Vars.m_ModType = MOD_FRONTWHEELS;
			}));

		addOption(SubmenuOption(getString("SUV"))
			.addSubmenu<VehicleEditorWheelTypeEditMenu>()
			.addOnClick([] {
				VehicleEditorWheelTypeEditMenuVars::m_Vars.m_WheelType = 8;
				m_Vars.m_CachedWheelIndex = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS);
				m_Vars.m_CachedWheelType = Native::GetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle);
				m_Vars.m_ModType = MOD_FRONTWHEELS;
			}));

		addOption(SubmenuOption(getString("Tuner"))
			.addSubmenu<VehicleEditorWheelTypeEditMenu>()
			.addOnClick([] {
				VehicleEditorWheelTypeEditMenuVars::m_Vars.m_WheelType = 9;
				m_Vars.m_CachedWheelIndex = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS);
				m_Vars.m_CachedWheelType = Native::GetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle);
				m_Vars.m_ModType = MOD_FRONTWHEELS;
			}));
	}
}

/*Called always*/
void VehicleEditorWheelTypeMenu::FeatureUpdate() {}

VehicleEditorWheelTypeMenu* _instance;
VehicleEditorWheelTypeMenu* VehicleEditorWheelTypeMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleEditorWheelTypeMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleEditorWheelTypeMenu::~VehicleEditorWheelTypeMenu() { delete _instance; }