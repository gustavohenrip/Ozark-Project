#include "vehicle_editor_fenders.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_editor.hpp"
#include "vehicle_editor_lsc.hpp"

using namespace VehicleEditorFendersMenuVars;

namespace VehicleEditorFendersMenuVars {
	Vars_ m_Vars;
}

void VehicleEditorFendersMenu::Init() {
	SetName("Fenders");
	SetParentSubmenu<VehicleEditorMenu>();

	addString("Left Fender");
	addString("Right Bumper");
}

void VehicleEditorFendersMenu::Update() {
	if (!Menu::GetLocalPlayer().m_InVehicle) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}

	VehicleEditorMenuVars::Scripts::DrawVehicleStats(Menu::GetLocalPlayer().m_Vehicle);
}

/*Called once on submenu open*/
void VehicleEditorFendersMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
	Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FENDER, VehicleEditorMenuVars::m_Vars.m_CachedStats.m_LeftFender, false);
	Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_RIGHTFENDER, VehicleEditorMenuVars::m_Vars.m_CachedStats.m_RightFender, false);

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_FENDER)) {
		addOption(SubmenuOption(getString("Left Fender"))
			.addSubmenu<VehicleEditorLSCMenu>()
			.addOnClick([] {
				VehicleEditorMenuVars::CacheVehicleStats();
				VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_FENDER;
				VehicleEditorLSCMenuVars::m_Vars.m_Value = &VehicleEditorMenuVars::m_Vars.m_CachedStats.m_LeftFender;
			}));
	}

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_RIGHTFENDER)) {
		addOption(SubmenuOption(getString("Right Bumper"))
			.addSubmenu<VehicleEditorLSCMenu>()
			.addOnClick([] {
				VehicleEditorMenuVars::CacheVehicleStats();
				VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_RIGHTFENDER;
				VehicleEditorLSCMenuVars::m_Vars.m_Value = &VehicleEditorMenuVars::m_Vars.m_CachedStats.m_RightFender;
			}));
	}
}

/*Called always*/
void VehicleEditorFendersMenu::FeatureUpdate() {}

VehicleEditorFendersMenu* _instance;
VehicleEditorFendersMenu* VehicleEditorFendersMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleEditorFendersMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleEditorFendersMenu::~VehicleEditorFendersMenu() { delete _instance; }