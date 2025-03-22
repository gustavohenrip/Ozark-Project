#include "vehicle_editor_respray_type.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_editor_respray.hpp"
#include "../../vehicle_editor.hpp"
#include "vehicle_editor_respray_edit.hpp"

using namespace VehicleEditorResprayTypeMenuVars;

namespace VehicleEditorResprayTypeMenuVars {
	Vars_ m_Vars;
}

void VehicleEditorResprayTypeMenu::Init() {
	SetName("Respray");
	SetParentSubmenu<VehicleEditorResprayMenu>();

	addString("Chrome");
	addString("Classic");
	addString("Matte");
	addString("Metallic");
	addString("Metals");
	addString("Pearlescent");
}

void VehicleEditorResprayTypeMenu::Update() {
	if (!Menu::GetLocalPlayer().m_InVehicle) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}

	VehicleEditorMenuVars::Scripts::DrawVehicleStats(Menu::GetLocalPlayer().m_Vehicle);
}

/*Called once on submenu open*/
void VehicleEditorResprayTypeMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	VehicleEditorResprayMenuVars::m_Vars.m_UpdateCache = true;
	m_Vars.m_Context.Reset();
	m_Vars.m_Context.m_Selected = 0;

	if (m_Vars.m_UpdateCache) {
		m_Vars.m_UpdateCache = false;

		Native::SetVehicleColours(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_PaintIndex[PRIMARY], m_Vars.m_PaintIndex[SECONDARY]);

		int WheelColor = 0;
		int Pearl = 0;
		Native::GetVehicleExtraColours(Menu::GetLocalPlayer().m_Vehicle, &Pearl, &WheelColor);
		Native::SetVehicleExtraColours(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_PaintIndex[PEARLESCENT], WheelColor);
	}

	int WheelColor = 0;
	Native::GetVehicleColours(Menu::GetLocalPlayer().m_Vehicle, &m_Vars.m_PaintIndex[PRIMARY], &m_Vars.m_PaintIndex[SECONDARY]);
	Native::GetVehicleExtraColours(Menu::GetLocalPlayer().m_Vehicle, &m_Vars.m_PaintIndex[PEARLESCENT], &WheelColor);

	if (m_Vars.m_Type == 1 || m_Vars.m_Type == 2) {
		addOption(SubmenuOption(getString("Chrome"))
			.addSubmenu<VehicleEditorResprayEditMenu>()
			.addOnClick([] { VehicleEditorResprayEditMenuVars::m_Vars.m_Type = 5; }));

		addOption(SubmenuOption(getString("Classic"))
			.addSubmenu<VehicleEditorResprayEditMenu>()
			.addOnClick([] { VehicleEditorResprayEditMenuVars::m_Vars.m_Type = 1; }));

		addOption(SubmenuOption(getString("Matte"))
			.addSubmenu<VehicleEditorResprayEditMenu>()
			.addOnClick([] { VehicleEditorResprayEditMenuVars::m_Vars.m_Type = 3; }));

		addOption(SubmenuOption(getString("Metallic"))
			.addSubmenu<VehicleEditorResprayEditMenu>()
			.addOnClick([] { VehicleEditorResprayEditMenuVars::m_Vars.m_Type = 0; }));

		addOption(SubmenuOption(getString("Metals"))
			.addSubmenu<VehicleEditorResprayEditMenu>()
			.addOnClick([] { VehicleEditorResprayEditMenuVars::m_Vars.m_Type = 4; }));

		// Primary only
		if (m_Vars.m_Type == 1) {
			addOption(SubmenuOption(getString("Pearlescent"))
				.addSubmenu<VehicleEditorResprayEditMenu>()
				.addOnClick([] { VehicleEditorResprayEditMenuVars::m_Vars.m_Type = 2; }));
		}
	} else {
		// Livery
		std::vector<std::shared_ptr<RadioOption>> TempOptions;
		int SelectedIndex = 99;

		for (int i = 0; i < Native::GetVehicleLiveryCount(Menu::GetLocalPlayer().m_Vehicle); i++) {
			auto LiveryName = Native::GetLiveryName(Menu::GetLocalPlayer().m_Vehicle, i);
			if (LiveryName) {
				TempOptions.push_back(addOption(RadioOption(Native::_GetLabelText(LiveryName))
					.addRadio(m_Vars.m_Context)
					.addOnClick([=] { VehicleEditorResprayMenuVars::m_Vars.m_Livery = i; })
					.addOnHover([=] {
						Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
						Native::SetVehicleLivery(Menu::GetLocalPlayer().m_Vehicle, i);
					})));

				if (VehicleEditorResprayMenuVars::m_Vars.m_Livery == i) {
					SelectedIndex = i;
				}
			}
		}

		if (SelectedIndex != 0) {
			if (SelectedIndex != 99) {
				TempOptions[SelectedIndex]->UpdateSelected();
				Menu::GetBase()->m_CurrentOption = SelectedIndex;
			} else {
				m_Vars.m_Context.m_Toggles[m_Vars.m_Context.m_Selected] = false;
				m_Vars.m_Context.m_Selected = 99;
				m_Vars.m_Context.m_Toggles[99] = true;
			}
		}
	}
}

/*Called always*/
void VehicleEditorResprayTypeMenu::FeatureUpdate() {}

VehicleEditorResprayTypeMenu* _instance;
VehicleEditorResprayTypeMenu* VehicleEditorResprayTypeMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleEditorResprayTypeMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleEditorResprayTypeMenu::~VehicleEditorResprayTypeMenu() { delete _instance; }