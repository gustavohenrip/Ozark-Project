#include "vehicle_editor_neon.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_editor_lights.hpp"
#include "../vehicle_editor_lsc.hpp"
#include "../../vehicle_editor.hpp"
#include "neon/vehicle_editor_neon_color.hpp"

using namespace VehicleEditorNeonMenuVars;

namespace VehicleEditorNeonMenuVars {
	Vars_ m_Vars;
}

void VehicleEditorNeonMenu::Init() {
	SetName("Neon Kits");
	SetParentSubmenu<VehicleEditorLightsMenu>();

	addOption(SubmenuOption("Neon Layout")
		.addTranslation()
		.addSubmenu<VehicleEditorLSCMenu>()
		.addOnClick([] { VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_NEON; }));

	addOption(SubmenuOption("Neon Color")
		.addTranslation()
		.addSubmenu<VehicleEditorNeonColorMenu>());
}

void VehicleEditorNeonMenu::Update() {
	if (!Menu::GetLocalPlayer().m_InVehicle) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}

	VehicleEditorMenuVars::Scripts::DrawVehicleStats(Menu::GetLocalPlayer().m_Vehicle);
}

/*Called once on submenu open*/
void VehicleEditorNeonMenu::UpdateOnce() {
	for (int i = 0; i < 4; i++) {
		Native::_SetVehicleNeonLightEnabled(Menu::GetLocalPlayer().m_Vehicle, i, VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Neon[i]);
	}

	Native::_SetVehicleNeonLightsColour(Menu::GetLocalPlayer().m_Vehicle, VehicleEditorMenuVars::m_Vars.m_CachedStats.m_NeonColor.R, VehicleEditorMenuVars::m_Vars.m_CachedStats.m_NeonColor.G, VehicleEditorMenuVars::m_Vars.m_CachedStats.m_NeonColor.B);
}

/*Called always*/
void VehicleEditorNeonMenu::FeatureUpdate() {}

VehicleEditorNeonMenu* _instance;
VehicleEditorNeonMenu* VehicleEditorNeonMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleEditorNeonMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleEditorNeonMenu::~VehicleEditorNeonMenu() { delete _instance; }