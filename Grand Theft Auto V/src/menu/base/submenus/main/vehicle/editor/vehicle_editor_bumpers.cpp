#include "vehicle_editor_bumpers.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_editor.hpp"
#include "vehicle_editor_lsc.hpp"

using namespace VehicleEditorBumpersMenuVars;

namespace VehicleEditorBumpersMenuVars {
	Vars_ m_Vars;
}

void VehicleEditorBumpersMenu::Init() {
	SetName("Bumpers");
	SetParentSubmenu<VehicleEditorMenu>();

	addString("Front Bumper");
	addString("Rear Bumper");
}

void VehicleEditorBumpersMenu::Update() {
	if (!Menu::GetLocalPlayer().m_InVehicle) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}

	VehicleEditorMenuVars::Scripts::DrawVehicleStats(Menu::GetLocalPlayer().m_Vehicle);
}

/*Called once on submenu open*/
void VehicleEditorBumpersMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
	Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTBUMPER, VehicleEditorMenuVars::m_Vars.m_CachedStats.m_FrontBumper, false);
	Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_REARBUMPER, VehicleEditorMenuVars::m_Vars.m_CachedStats.m_RearBumper, false);

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTBUMPER)) {
		addOption(SubmenuOption(getString("Front Bumper"))
			.addSubmenu<VehicleEditorLSCMenu>()
			.addOnClick([] {
				VehicleEditorMenuVars::CacheVehicleStats();
				VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_FRONTBUMPER;
				VehicleEditorLSCMenuVars::m_Vars.m_Value = &VehicleEditorMenuVars::m_Vars.m_CachedStats.m_FrontBumper;
			}));
	}

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_REARBUMPER)) {
		addOption(SubmenuOption(getString("Rear Bumper"))
			.addSubmenu<VehicleEditorLSCMenu>()
			.addOnClick([] {
				VehicleEditorMenuVars::CacheVehicleStats();
				VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_REARBUMPER;
				VehicleEditorLSCMenuVars::m_Vars.m_Value = &VehicleEditorMenuVars::m_Vars.m_CachedStats.m_RearBumper;
			}));
	}
}

/*Called always*/
void VehicleEditorBumpersMenu::FeatureUpdate() {}

VehicleEditorBumpersMenu* _instance;
VehicleEditorBumpersMenu* VehicleEditorBumpersMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleEditorBumpersMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleEditorBumpersMenu::~VehicleEditorBumpersMenu() { delete _instance; }