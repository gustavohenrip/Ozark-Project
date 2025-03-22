#include "vehicle_editor_horn.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_editor.hpp"
#include "vehicle_editor_lsc.hpp"

using namespace VehicleEditorHornMenuVars;

namespace VehicleEditorHornMenuVars {
	Vars_ m_Vars;
}

void VehicleEditorHornMenu::Init() {
	SetName("Horn");
	SetParentSubmenu<VehicleEditorMenu>();

	addOption(SubmenuOption("Standard")
		.addTranslation()
		.addSubmenu<VehicleEditorLSCMenu>()
		.addOnClick([] {
			VehicleEditorMenuVars::CacheVehicleStats();
			VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_HORN_STANDARD;
		}));

	addOption(SubmenuOption("Musical")
		.addTranslation()
		.addSubmenu<VehicleEditorLSCMenu>()
		.addOnClick([] {
			VehicleEditorMenuVars::CacheVehicleStats();
			VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_HORN_MUSICAL;
		}));

	addOption(SubmenuOption("Loop")
		.addTranslation()
		.addSubmenu<VehicleEditorLSCMenu>()
		.addOnClick([] {
			VehicleEditorMenuVars::CacheVehicleStats();
			VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_HORN_LOOP;
		}));
}

void VehicleEditorHornMenu::Update() {
	if (!Menu::GetLocalPlayer().m_InVehicle) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}

	VehicleEditorMenuVars::Scripts::DrawVehicleStats(Menu::GetLocalPlayer().m_Vehicle);
}

/*Called once on submenu open*/
void VehicleEditorHornMenu::UpdateOnce() {
	Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_HORNS, VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Horn, false);
}

/*Called always*/
void VehicleEditorHornMenu::FeatureUpdate() {}

VehicleEditorHornMenu* _instance;
VehicleEditorHornMenu* VehicleEditorHornMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleEditorHornMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleEditorHornMenu::~VehicleEditorHornMenu() { delete _instance; }