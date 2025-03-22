#include "vehicle_editor_wheels.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_editor.hpp"
#include "wheels/vehicle_editor_wheel_type.hpp"
#include "wheels/vehicle_editor_wheel_tires.hpp"
#include "wheels/vehicle_editor_wheel_color.hpp"

using namespace VehicleEditorWheelsMenuVars;

namespace VehicleEditorWheelsMenuVars {
	Vars_ m_Vars;
}

void VehicleEditorWheelsMenu::Init() {
	SetName("Wheels");
	SetParentSubmenu<VehicleEditorMenu>();

	addOption(SubmenuOption("Wheel Type")
		.addTranslation()
		.addSubmenu<VehicleEditorWheelTypeMenu>());

	addOption(SubmenuOption("Wheel Color")
		.addTranslation()
		.addSubmenu<VehicleEditorWheelColorMenu>());

	addOption(SubmenuOption("Tires")
		.addTranslation()
		.addSubmenu<VehicleEditorWheelTiresMenu>());
}

void VehicleEditorWheelsMenu::Update() {
	if (!Menu::GetLocalPlayer().m_InVehicle) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}

	VehicleEditorMenuVars::Scripts::DrawVehicleStats(Menu::GetLocalPlayer().m_Vehicle);
}

/*Called once on submenu open*/
void VehicleEditorWheelsMenu::UpdateOnce() {
	if (m_Vars.m_UpdateCache) {
		m_Vars.m_UpdateCache = false;

		int Perl = 0;
		int Cur = 0;
		Native::GetVehicleExtraColours(Menu::GetLocalPlayer().m_Vehicle, &Perl, &Cur);
		Native::SetVehicleExtraColours(Menu::GetLocalPlayer().m_Vehicle, Perl, m_Vars.m_PaintIndex);
	}

	int Perl = 0;
	Native::GetVehicleExtraColours(Menu::GetLocalPlayer().m_Vehicle, &Perl, &m_Vars.m_PaintIndex);
}

/*Called always*/
void VehicleEditorWheelsMenu::FeatureUpdate() {}

VehicleEditorWheelsMenu* _instance;
VehicleEditorWheelsMenu* VehicleEditorWheelsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleEditorWheelsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleEditorWheelsMenu::~VehicleEditorWheelsMenu() { delete _instance; }