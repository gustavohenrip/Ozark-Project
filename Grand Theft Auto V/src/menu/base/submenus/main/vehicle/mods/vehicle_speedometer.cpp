#include "vehicle_speedometer.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_mods.hpp"
#include "menu/base/util/stacked_text_renderer.hpp"
#include "utils/va.hpp"

using namespace VehicleModsSpeedometerMenuVars;

namespace VehicleModsSpeedometerMenuVars {
	Vars_ m_Vars;
}

void VehicleModsSpeedometerMenu::Init() {
	SetName("Speedometer");
	SetParentSubmenu<VehicleModsMenu>();

	addOption(ToggleOption("Basic Speedometer")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_BasicSpeedometer)
		.addOnClick([] {
			if (m_Vars.m_BasicSpeedometer) {
				Menu::Util::GetStackedTextRenderer()->Alloc("SR_BASICSPEEDO", 19);
			} else {
				Menu::Util::GetStackedTextRenderer()->Dealloc("SR_BASICSPEEDO");
			}
		}).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Numberplate Speedometer")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_NumberplateSpeedometer)
		.addOnClick([] {
			if (m_Vars.m_NumberplateSpeedometer) {
				if (Menu::GetLocalPlayer().m_InVehicle) {
					m_Vars.m_PreviousNumberplateText = Native::GetVehicleNumberPlateText(Menu::GetLocalPlayer().m_Vehicle);
				}
			} else {
				Native::SetVehicleNumberPlateText(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_PreviousNumberplateText.c_str());
			}
		}).canBeSaved(m_ParentNameStack));

	addString("Speed");
}

void VehicleModsSpeedometerMenu::Update() {}

/*Called once on submenu open*/
void VehicleModsSpeedometerMenu::UpdateOnce() {}

/*Called always*/
void VehicleModsSpeedometerMenu::FeatureUpdate() {
	if (Menu::GetLocalPlayer().m_InVehicle) {
		if (m_Vars.m_BasicSpeedometer) {
			float Speed = Native::GetEntitySpeed(Menu::GetLocalPlayer().m_Vehicle) * Global::Vars::m_SelectedMetricSystem.first;
			Menu::Util::GetStackedTextRenderer()->Update("SR_BASICSPEEDO", getString("Speed"), Utils::VA::VA("%.0f %s", Speed, Global::Vars::m_SelectedMetricSystem.second.c_str()));
		}

		if (m_Vars.m_NumberplateSpeedometer) {
			float Speed = Native::GetEntitySpeed(Menu::GetLocalPlayer().m_Vehicle) * Global::Vars::m_SelectedMetricSystem.first;
			Native::SetVehicleNumberPlateText(Menu::GetLocalPlayer().m_Vehicle, Utils::VA::VA("%.0f %s", Speed, Global::Vars::m_SelectedMetricSystem.second.c_str()));
		}
	}
}

VehicleModsSpeedometerMenu* _instance;
VehicleModsSpeedometerMenu* VehicleModsSpeedometerMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleModsSpeedometerMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleModsSpeedometerMenu::~VehicleModsSpeedometerMenu() { delete _instance; }