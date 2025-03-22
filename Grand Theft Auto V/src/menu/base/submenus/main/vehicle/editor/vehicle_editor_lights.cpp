#include "vehicle_editor_lights.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_editor.hpp"
#include "lights/vehicle_editor_neon.hpp"
#include "vehicle_editor_lsc.hpp"

using namespace VehicleEditorLightsMenuVars;

namespace VehicleEditorLightsMenuVars {
	Vars_ m_Vars;
}

void VehicleEditorLightsMenu::Init() {
	SetName("Lights");
	SetParentSubmenu<VehicleEditorMenu>();

	addOption(SubmenuOption("Headlights")
		.addTranslation()
		.addSubmenu<VehicleEditorLSCMenu>()
		.addOnClick([] {
			VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_XENONLIGHTS;
			VehicleEditorLSCMenuVars::m_Vars.m_Value = &VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Lights;
		}));

	addOption(SubmenuOption("Neon Kits")
		.addTranslation()
		.addSubmenu<VehicleEditorNeonMenu>()
		.addRequirement([] { return Native::IsThisModelACar(Native::GetEntityModel(Menu::GetLocalPlayer().m_Vehicle)); }));
}

void VehicleEditorLightsMenu::Update() {
	if (!Menu::GetLocalPlayer().m_InVehicle) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}

	VehicleEditorMenuVars::Scripts::DrawVehicleStats(Menu::GetLocalPlayer().m_Vehicle);
}

/*Called once on submenu open*/
void VehicleEditorLightsMenu::UpdateOnce() {
	Native::ToggleVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_XENONLIGHTS, (bool)VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Lights);
}

/*Called always*/
void VehicleEditorLightsMenu::FeatureUpdate() {}

VehicleEditorLightsMenu* _instance;
VehicleEditorLightsMenu* VehicleEditorLightsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleEditorLightsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleEditorLightsMenu::~VehicleEditorLightsMenu() { delete _instance; }