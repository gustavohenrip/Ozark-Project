#include "vehicle_editor_respray_edit.hpp"
#include "menu/base/submenu_handler.hpp"
#include "vehicle_editor_respray_type.hpp"
#include "../../vehicle_editor.hpp"
#include "rage/engine.hpp"

using namespace VehicleEditorResprayEditMenuVars;

namespace VehicleEditorResprayEditMenuVars {
	Vars_ m_Vars;
}

void VehicleEditorResprayEditMenu::Init() {
	SetName("Respray");
	SetParentSubmenu<VehicleEditorResprayTypeMenu>();
}

void VehicleEditorResprayEditMenu::Update() {
	if (!Menu::GetLocalPlayer().m_InVehicle) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}

	VehicleEditorMenuVars::Scripts::DrawVehicleStats(Menu::GetLocalPlayer().m_Vehicle);
	if (Menu::GetBase()->m_ScrollOffset < Menu::GetSubmenuHandler()->GetTotalOptions() - Menu::GetBase()->m_MaxOptions && Menu::GetBase()->m_CurrentOption - Menu::GetBase()->m_ScrollOffset >= Menu::GetBase()->m_MaxOptions) Menu::GetBase()->m_ScrollOffset++;
}

/*Called once on submenu open*/
void VehicleEditorResprayEditMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	VehicleEditorResprayTypeMenuVars::m_Vars.m_UpdateCache = true;
	m_Vars.m_Context.Reset();
	m_Vars.m_Context.m_Selected = 0;

	std::vector<std::shared_ptr<RadioOption>> TempOptions;
	int SelectedIndex = 99;

	uint64_t Paint = Rage::Engine::GetVehiclePaint(m_Vars.m_Type);
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
							.addOnClick([=] {
								if (m_Vars.m_Type == 2) {
									VehicleEditorResprayTypeMenuVars::m_Vars.m_PaintIndex[VehicleEditorResprayTypeMenuVars::PEARLESCENT] = PaintIndex;
								} else {
									if (VehicleEditorResprayTypeMenuVars::m_Vars.m_Type == 1) {
										VehicleEditorResprayTypeMenuVars::m_Vars.m_PaintIndex[VehicleEditorResprayTypeMenuVars::PRIMARY] = PaintIndex;
									} else {
										VehicleEditorResprayTypeMenuVars::m_Vars.m_PaintIndex[VehicleEditorResprayTypeMenuVars::SECONDARY] = PaintIndex;
									}
								}
							})
							.addOnHover([=] {
								if (m_Vars.m_Type == 2) {
									// PEARLESCENT
									int WheelColor = 0;
									int PerlColor = 0;
									Native::GetVehicleExtraColours(Menu::GetLocalPlayer().m_Vehicle, &PerlColor, &WheelColor);
									Native::SetVehicleExtraColours(Menu::GetLocalPlayer().m_Vehicle, PaintIndex, WheelColor);
								} else {
									if (VehicleEditorResprayTypeMenuVars::m_Vars.m_Type == 1) {
										// PRIMARY
										int Primary = 0;
										int Secondary = 0;
										Native::GetVehicleColours(Menu::GetLocalPlayer().m_Vehicle, &Primary, &Secondary);
										Native::SetVehicleColours(Menu::GetLocalPlayer().m_Vehicle, PaintIndex, Secondary);
									} else {
										// SECONDARY
										int Primary = 0;
										int Secondary = 0;
										Native::GetVehicleColours(Menu::GetLocalPlayer().m_Vehicle, &Primary, &Secondary);
										Native::SetVehicleColours(Menu::GetLocalPlayer().m_Vehicle, Primary, PaintIndex);
									}
								}
							})));

						if (SelectedIndex == 99) {
							if (m_Vars.m_Type == 2) {
								// PEARLESCENT
								if (PaintIndex == VehicleEditorResprayTypeMenuVars::m_Vars.m_PaintIndex[VehicleEditorResprayTypeMenuVars::PEARLESCENT]) {
									SelectedIndex = i;
								}
							} else {
								if (VehicleEditorResprayTypeMenuVars::m_Vars.m_Type == 1) {
									if (PaintIndex == VehicleEditorResprayTypeMenuVars::m_Vars.m_PaintIndex[VehicleEditorResprayTypeMenuVars::PRIMARY]) {
										SelectedIndex = i;
									}
								} else {
									if (PaintIndex == VehicleEditorResprayTypeMenuVars::m_Vars.m_PaintIndex[VehicleEditorResprayTypeMenuVars::SECONDARY]) {
										SelectedIndex = i;
									}
								}
							}
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
void VehicleEditorResprayEditMenu::FeatureUpdate() {}

VehicleEditorResprayEditMenu* _instance;
VehicleEditorResprayEditMenu* VehicleEditorResprayEditMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleEditorResprayEditMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleEditorResprayEditMenu::~VehicleEditorResprayEditMenu() { delete _instance; }