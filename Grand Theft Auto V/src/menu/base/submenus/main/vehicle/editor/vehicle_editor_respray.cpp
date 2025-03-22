#include "vehicle_editor_respray.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_editor.hpp"
#include "respray/vehicle_editor_respray_type.hpp"

using namespace VehicleEditorResprayMenuVars;

namespace VehicleEditorResprayMenuVars {
	Vars_ m_Vars;
}

void VehicleEditorResprayMenu::Init() {
	SetName("Respray");
	SetParentSubmenu<VehicleEditorMenu>();

	addOption(SubmenuOption("Primary Color")
		.addTranslation()
		.addSubmenu<VehicleEditorResprayTypeMenu>()
		.addOnClick([] { VehicleEditorResprayTypeMenuVars::m_Vars.m_Type = 1; }));

	addOption(SubmenuOption("Secondary Color")
		.addTranslation()
		.addSubmenu<VehicleEditorResprayTypeMenu>()
		.addOnClick([] { VehicleEditorResprayTypeMenuVars::m_Vars.m_Type = 2; }));

	addOption(SubmenuOption("Livery")
		.addTranslation()
		.addSubmenu<VehicleEditorResprayTypeMenu>()
		.addOnClick([] { VehicleEditorResprayTypeMenuVars::m_Vars.m_Type = 3; })
		.addRequirement([] { return Native::GetVehicleLiveryCount(Menu::GetLocalPlayer().m_Vehicle) > 0 && Native::GetLiveryName(Menu::GetLocalPlayer().m_Vehicle, 0); }));
}

void VehicleEditorResprayMenu::Update() {
	if (!Menu::GetLocalPlayer().m_InVehicle) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}

	VehicleEditorMenuVars::Scripts::DrawVehicleStats(Menu::GetLocalPlayer().m_Vehicle);
}

/*Called once on submenu open*/
void VehicleEditorResprayMenu::UpdateOnce() {
	if (m_Vars.m_UpdateCache) {
		m_Vars.m_UpdateCache = false;

		Native::SetVehicleLivery(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_Livery);
	}

	m_Vars.m_Livery = Native::GetVehicleLivery(Menu::GetLocalPlayer().m_Vehicle);
}

/*Called always*/
void VehicleEditorResprayMenu::FeatureUpdate() {}

VehicleEditorResprayMenu* _instance;
VehicleEditorResprayMenu* VehicleEditorResprayMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleEditorResprayMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleEditorResprayMenu::~VehicleEditorResprayMenu() { delete _instance; }