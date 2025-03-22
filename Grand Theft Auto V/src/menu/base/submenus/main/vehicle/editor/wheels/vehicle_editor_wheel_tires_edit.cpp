#include "vehicle_editor_wheel_tires_edit.hpp"
#include "menu/base/submenu_handler.hpp"
#include "vehicle_editor_wheel_tires.hpp"
#include "../../vehicle_editor.hpp"

using namespace VehicleEditorWheelTiresEditMenuVars;

namespace VehicleEditorWheelTiresEditMenuVars {
	Vars_ m_Vars;

	ScrollStruct<Color> TireSmoke[] = {
		{ TranslationString("White Tire Smoke", true), { 255, 255, 255 } },
		{ TranslationString("Black Tire Smoke", true), { 20, 20, 20 } },
		{ TranslationString("Blue Tire Smoke", true), { 0, 174, 239 } },
		{ TranslationString("Yellow Tire Smoke", true), { 252, 238, 0 } },
		{ TranslationString("Purple Tire Smoke", true), { -1, -1, -1 } },
		{ TranslationString("Orange Tire Smoke", true), { 255, 127, 0 } },
		{ TranslationString("Green Tire Smoke", true), { -1, -1, -1} },
		{ TranslationString("Red Tire Smoke", true), { 226, 6, 6 } },
		{ TranslationString("Pink Tire Smoke", true), { -1, -1, -1 } },
		{ TranslationString("Brown Tire Smoke", true), { -1, -1, -1 } },
		{ TranslationString("Patriot Tire Smoke", true), { 0, 0, 0 } },
	};
}

void VehicleEditorWheelTiresEditMenu::Init() {
	SetName("Tires");
	SetParentSubmenu<VehicleEditorWheelTiresMenu>();

	addString("Stock Tires");
	addString("Custom Tires");
	addString("Bulletproof Tires");
}

void VehicleEditorWheelTiresEditMenu::Update() {
	if (!Menu::GetLocalPlayer().m_InVehicle) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}

	VehicleEditorMenuVars::Scripts::DrawVehicleStats(Menu::GetLocalPlayer().m_Vehicle);
}

/*Called once on submenu open*/
void VehicleEditorWheelTiresEditMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	VehicleEditorWheelTiresMenuVars::m_Vars.m_UpdateCache = true;
	m_Vars.m_Context.Reset();
	m_Vars.m_Context.m_Selected = 0;

	// Populate hud element colors
	if (TireSmoke[4].m_Result.R == -1) {
		Native::GetHudColour(21, &TireSmoke[4].m_Result.R, &TireSmoke[4].m_Result.G, &TireSmoke[4].m_Result.B, &TireSmoke[4].m_Result.A);
		Native::GetHudColour(18, &TireSmoke[6].m_Result.R, &TireSmoke[6].m_Result.G, &TireSmoke[6].m_Result.B, &TireSmoke[6].m_Result.A);
		Native::GetHudColour(24, &TireSmoke[8].m_Result.R, &TireSmoke[8].m_Result.G, &TireSmoke[8].m_Result.B, &TireSmoke[8].m_Result.A);
		Native::GetHudColour(107, &TireSmoke[9].m_Result.R, &TireSmoke[9].m_Result.G, &TireSmoke[9].m_Result.B, &TireSmoke[9].m_Result.A);
	}

	// Tire Design
	if (m_Vars.m_Type == 1) {
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		TempOptions.push_back(addOption(RadioOption(getString("Stock Tires"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([] { VehicleEditorWheelTiresMenuVars::m_Vars.m_CustomTires = false; })
			.addOnHover([] {
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS), false);
			})));

		TempOptions.push_back(addOption(RadioOption(getString("Custom Tires"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([] { VehicleEditorWheelTiresMenuVars::m_Vars.m_CustomTires = true; })
			.addOnHover([] {
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS), true);
			})));

		if (Native::GetVehicleModVariation(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS)) {
			TempOptions[1]->UpdateSelected();
			Menu::GetBase()->m_CurrentOption = 1;
		}

		return;
	}

	// Tire Enhancements
	if (m_Vars.m_Type == 2) {
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		TempOptions.push_back(addOption(RadioOption(getString("Bulletproof Tires"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([] {
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::SetVehicleTyresCanBurst(Menu::GetLocalPlayer().m_Vehicle, false);
			})));

		if (Native::GetVehicleTyresCanBurst(Menu::GetLocalPlayer().m_Vehicle)) {
			m_Vars.m_Context.m_Toggles[m_Vars.m_Context.m_Selected] = false;
			m_Vars.m_Context.m_Selected = 99;
			m_Vars.m_Context.m_Toggles[99] = true;
		}

		return;
	}

	// Tire Smoke
	if (m_Vars.m_Type == 3) {
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		int SelectedIndex = 99;
		bool Toggled = Native::IsToggleModOn(Menu::GetLocalPlayer().m_Vehicle, MOD_TIRESMOKE);
		Color CurrentColor;
		Native::GetVehicleTyreSmokeColor(Menu::GetLocalPlayer().m_Vehicle , &CurrentColor.R, &CurrentColor.G, &CurrentColor.B);

		for (int i = 0; i < NUMOF(TireSmoke); i++) {
			auto Smoke = TireSmoke[i];
			TempOptions.push_back(addOption(RadioOption(Smoke.m_Name.Get())
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorWheelTiresMenuVars::m_Vars.m_TireSmoke = true;
					VehicleEditorWheelTiresMenuVars::m_Vars.m_TireSmokeColor = Smoke.m_Result;
				})
				.addOnHover([=] {
					Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
					Native::ToggleVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TIRESMOKE, true);
					Native::SetVehicleTyreSmokeColor(Menu::GetLocalPlayer().m_Vehicle, Smoke.m_Result.R, Smoke.m_Result.G, Smoke.m_Result.B);
				})));

			if (Toggled) {
				if (Smoke.m_Result.R == CurrentColor.R
					&& Smoke.m_Result.G == CurrentColor.G
					&& Smoke.m_Result.B == CurrentColor.B) {
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

		return;
	}
}

/*Called always*/
void VehicleEditorWheelTiresEditMenu::FeatureUpdate() {}

VehicleEditorWheelTiresEditMenu* _instance;
VehicleEditorWheelTiresEditMenu* VehicleEditorWheelTiresEditMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleEditorWheelTiresEditMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleEditorWheelTiresEditMenu::~VehicleEditorWheelTiresEditMenu() { delete _instance; }