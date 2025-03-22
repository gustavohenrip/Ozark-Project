#include "vehicle_editor_wheel_tires.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_editor_wheels.hpp"
#include "../../vehicle_editor.hpp"
#include "vehicle_editor_wheel_tires_edit.hpp"

using namespace VehicleEditorWheelTiresMenuVars;

namespace VehicleEditorWheelTiresMenuVars {
	Vars_ m_Vars;
}

void VehicleEditorWheelTiresMenu::Init() {
	SetName("Tires");
	SetParentSubmenu<VehicleEditorWheelsMenu>();

	addOption(SubmenuOption("Tire Design")
		.addTranslation()
		.addSubmenu<VehicleEditorWheelTiresEditMenu>()
		.addOnClick([] { VehicleEditorWheelTiresEditMenuVars::m_Vars.m_Type = 1; }));

	addOption(SubmenuOption("Tire Enhancements")
		.addTranslation()
		.addSubmenu<VehicleEditorWheelTiresEditMenu>()
		.addOnClick([] { VehicleEditorWheelTiresEditMenuVars::m_Vars.m_Type = 2; }));

	addOption(SubmenuOption("Tire Smoke")
		.addTranslation()
		.addSubmenu<VehicleEditorWheelTiresEditMenu>()
		.addOnClick([] { VehicleEditorWheelTiresEditMenuVars::m_Vars.m_Type = 3; }));
}

void VehicleEditorWheelTiresMenu::Update() {
	if (!Menu::GetLocalPlayer().m_InVehicle) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}

	VehicleEditorMenuVars::Scripts::DrawVehicleStats(Menu::GetLocalPlayer().m_Vehicle);
}

/*Called once on submenu open*/
void VehicleEditorWheelTiresMenu::UpdateOnce() {
	if (m_Vars.m_UpdateCache) {
		m_Vars.m_UpdateCache = false;

		Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
		Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS), m_Vars.m_CustomTires);
		Native::ToggleVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TIRESMOKE, m_Vars.m_TireSmoke);
		Native::SetVehicleTyreSmokeColor(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_TireSmokeColor.R, m_Vars.m_TireSmokeColor.G, m_Vars.m_TireSmokeColor.B);
	}

	m_Vars.m_CustomTires = Native::GetVehicleModVariation(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS);
	m_Vars.m_TireSmoke = Native::IsToggleModOn(Menu::GetLocalPlayer().m_Vehicle, MOD_TIRESMOKE);
	Native::GetVehicleTyreSmokeColor(Menu::GetLocalPlayer().m_Vehicle, &m_Vars.m_TireSmokeColor.R, &m_Vars.m_TireSmokeColor.G, &m_Vars.m_TireSmokeColor.B);
}

/*Called always*/
void VehicleEditorWheelTiresMenu::FeatureUpdate() {}

VehicleEditorWheelTiresMenu* _instance;
VehicleEditorWheelTiresMenu* VehicleEditorWheelTiresMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleEditorWheelTiresMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleEditorWheelTiresMenu::~VehicleEditorWheelTiresMenu() { delete _instance; }