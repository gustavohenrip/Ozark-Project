#include "vehicle_editor_wheel_color.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_editor_wheels.hpp"
#include "rage/engine.hpp"
#include "../../vehicle_editor.hpp"

using namespace VehicleEditorWheelColorMenuVars;

namespace VehicleEditorWheelColorMenuVars {
	Vars_ m_Vars;
}

void VehicleEditorWheelColorMenu::Init() {
	SetName("Wheel Color");
	SetParentSubmenu<VehicleEditorWheelsMenu>();
}

void VehicleEditorWheelColorMenu::Update() {
	if (!Menu::GetLocalPlayer().m_InVehicle) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}

	VehicleEditorMenuVars::Scripts::DrawVehicleStats(Menu::GetLocalPlayer().m_Vehicle);
	if (Menu::GetBase()->m_ScrollOffset < Menu::GetSubmenuHandler()->GetTotalOptions() - Menu::GetBase()->m_MaxOptions && Menu::GetBase()->m_CurrentOption - Menu::GetBase()->m_ScrollOffset >= Menu::GetBase()->m_MaxOptions) Menu::GetBase()->m_ScrollOffset++;
}

/*Called once on submenu open*/
void VehicleEditorWheelColorMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	VehicleEditorWheelsMenuVars::m_Vars.m_UpdateCache = true;
	m_Vars.m_Context.Reset();
	m_Vars.m_Context.m_Selected = 0;

	std::vector<std::shared_ptr<RadioOption>> TempOptions;

	int SelectedIndex = 99;

	uint64_t Paint = Rage::Engine::GetVehiclePaint(1);
	if (Paint) {
		uint64_t PaintInfo = *(uint64_t*)Paint;
		if (PaintInfo) {
			uint16_t PaintCount = *(uint16_t*)(Paint + 0x8);
			for (uint16_t i = 0; i < PaintCount; i++) {
				uint64_t PaintMeta = PaintInfo + (uint64_t(i) << 4);
				if (PaintMeta) {
					const char* PaintLabel = *(const char**)PaintMeta;
					if (PaintLabel) {
						int PaintIndex = (int)*(uint8_t*)(PaintMeta + 0x8);
						TempOptions.push_back(addOption(RadioOption(Native::_GetLabelText(PaintLabel))
							.addRadio(m_Vars.m_Context)
							.addOnClick([=] { VehicleEditorWheelsMenuVars::m_Vars.m_PaintIndex = PaintIndex; })
							.addOnHover([=] {
								int Perl = 0;
								int Cur = 0;
								Native::GetVehicleExtraColours(Menu::GetLocalPlayer().m_Vehicle, &Perl, &Cur);
								Native::SetVehicleExtraColours(Menu::GetLocalPlayer().m_Vehicle, Perl, PaintIndex);
							})));

						if (PaintIndex == VehicleEditorWheelsMenuVars::m_Vars.m_PaintIndex && SelectedIndex == 99) {
							SelectedIndex = i;
						}
					}
				}
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

/*Called always*/
void VehicleEditorWheelColorMenu::FeatureUpdate() {}

VehicleEditorWheelColorMenu* _instance;
VehicleEditorWheelColorMenu* VehicleEditorWheelColorMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleEditorWheelColorMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleEditorWheelColorMenu::~VehicleEditorWheelColorMenu() { delete _instance; }