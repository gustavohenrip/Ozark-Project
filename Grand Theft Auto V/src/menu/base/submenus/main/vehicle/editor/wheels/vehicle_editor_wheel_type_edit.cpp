#include "vehicle_editor_wheel_type_edit.hpp"
#include "menu/base/submenu_handler.hpp"
#include "vehicle_editor_wheel_type.hpp"
#include "../../vehicle_editor.hpp"

using namespace VehicleEditorWheelTypeEditMenuVars;

namespace VehicleEditorWheelTypeEditMenuVars {
	Vars_ m_Vars;
}

void VehicleEditorWheelTypeEditMenu::Init() {
	SetName("Wheel Type");
	SetParentSubmenu<VehicleEditorWheelTypeMenu>();

	addString("Stock Wheels");

	for (auto& Wheel : VehicleEditorMenuVars::m_Vars.m_WheelBike) addString(Native::_GetLabelText(Wheel.first));
	for (auto& Wheel : VehicleEditorMenuVars::m_Vars.m_WheelHighEnd) addString(Native::_GetLabelText(Wheel.first));
	for (auto& Wheel : VehicleEditorMenuVars::m_Vars.m_WheelLowrider) addString(Native::_GetLabelText(Wheel.first));
	for (auto& Wheel : VehicleEditorMenuVars::m_Vars.m_WheelMuscle) addString(Native::_GetLabelText(Wheel.first));
	for (auto& Wheel : VehicleEditorMenuVars::m_Vars.m_WheelOffroad) addString(Native::_GetLabelText(Wheel.first));
	for (auto& Wheel : VehicleEditorMenuVars::m_Vars.m_WheelSport) addString(Native::_GetLabelText(Wheel.first));
	for (auto& Wheel : VehicleEditorMenuVars::m_Vars.m_WheelSUV) addString(Native::_GetLabelText(Wheel.first));
	for (auto& Wheel : VehicleEditorMenuVars::m_Vars.m_WheelTuner) addString(Native::_GetLabelText(Wheel.first));
}

void VehicleEditorWheelTypeEditMenu::Update() {
	if (!Menu::GetLocalPlayer().m_InVehicle) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}

	VehicleEditorMenuVars::Scripts::DrawVehicleStats(Menu::GetLocalPlayer().m_Vehicle);
	if (Menu::GetBase()->m_ScrollOffset < Menu::GetSubmenuHandler()->GetTotalOptions() - Menu::GetBase()->m_MaxOptions && Menu::GetBase()->m_CurrentOption - Menu::GetBase()->m_ScrollOffset >= Menu::GetBase()->m_MaxOptions) Menu::GetBase()->m_ScrollOffset++;
}

/*Called once on submenu open*/
void VehicleEditorWheelTypeEditMenu::UpdateOnce() {
	ClearOptionsOffset(0);
	VehicleEditorWheelTypeMenuVars::m_Vars.m_UpdateCache = true;
	m_Vars.m_Context.Reset();
	m_Vars.m_Context.m_Selected = 0;

	/* Bike Wheels */
	if (m_Vars.m_WheelType == 1 || m_Vars.m_WheelType == 2) {
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		TempOptions.push_back(addOption(RadioOption(getString("Stock Wheels"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([] {
				VehicleEditorMenuVars::UpdateCaches();
				VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelIndex = -1;
				VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelType = 6;
			})
			.addOnHover([] {
				Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, 6);
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_WheelType == 1 ? MOD_FRONTWHEELS : MOD_BACKWHEELS, -1, Native::GetVehicleModVariation(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_WheelType == 1 ? MOD_FRONTWHEELS : MOD_BACKWHEELS));
			})));

		int SelectedIndex = 99;
		int CurrentMod = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_WheelType == 1 ? MOD_FRONTWHEELS : MOD_BACKWHEELS);

		for (auto& Wheel : VehicleEditorMenuVars::m_Vars.m_WheelBike) {
			TempOptions.push_back(addOption(RadioOption(getString(Native::_GetLabelText(Wheel.first)))
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorMenuVars::UpdateCaches();
					VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelIndex = Wheel.second;
					VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelType = 6;
				})
				.addOnHover([=] {
					Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, 6);
					Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
					Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_WheelType == 1 ? MOD_FRONTWHEELS : MOD_BACKWHEELS, Wheel.second, Native::GetVehicleModVariation(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_WheelType == 1 ? MOD_FRONTWHEELS : MOD_BACKWHEELS));
				})));

			if (Wheel.second == CurrentMod) {
				SelectedIndex = Wheel.second;
			}
		}

		if (CurrentMod != -1 && VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelType == 6) {
			if (SelectedIndex != 99) {
				TempOptions[SelectedIndex + 1]->UpdateSelected();
				Menu::GetBase()->m_CurrentOption = SelectedIndex + 1;
			} else {
				m_Vars.m_Context.m_Toggles[m_Vars.m_Context.m_Selected] = false;
				m_Vars.m_Context.m_Selected = 99;
				m_Vars.m_Context.m_Toggles[99] = true;
			}
		}

		return;
	}

	/* Highend */
	if (m_Vars.m_WheelType == 3) {
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		TempOptions.push_back(addOption(RadioOption(getString("Stock Wheels"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([] {
				VehicleEditorMenuVars::UpdateCaches();
				VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelIndex = -1;
				VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelType = 7;
			})
			.addOnHover([] {
				Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, 7);
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, -1, Native::GetVehicleModVariation(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS));
			})));

		int SelectedIndex = 99;
		int CurrentMod = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS);

		for (auto& Wheel : VehicleEditorMenuVars::m_Vars.m_WheelHighEnd) {
			TempOptions.push_back(addOption(RadioOption(getString(Native::_GetLabelText(Wheel.first)))
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorMenuVars::UpdateCaches();
					VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelIndex = Wheel.second;
					VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelType = 7;
				})
				.addOnHover([=] {
					Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, 7);
					Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
					Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, Wheel.second, Native::GetVehicleModVariation(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS));
				})));

			if (Wheel.second == CurrentMod) {
				SelectedIndex = Wheel.second;
			}
		}

		if (CurrentMod != -1 && VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelType == 7) {
			if (SelectedIndex != 99) {
				TempOptions[SelectedIndex + 1]->UpdateSelected();
				Menu::GetBase()->m_CurrentOption = SelectedIndex + 1;
			} else {
				m_Vars.m_Context.m_Toggles[m_Vars.m_Context.m_Selected] = false;
				m_Vars.m_Context.m_Selected = 99;
				m_Vars.m_Context.m_Toggles[99] = true;
			}
		}

		return;
	}

	/* Lowrider */
	if (m_Vars.m_WheelType == 4) {
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		TempOptions.push_back(addOption(RadioOption(getString("Stock Wheels"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([] {
				VehicleEditorMenuVars::UpdateCaches();
				VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelIndex = -1;
				VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelType = 2;
			})
			.addOnHover([] {
				Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, 2);
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, -1, Native::GetVehicleModVariation(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS));
			})));

		int SelectedIndex = 99;
		int CurrentMod = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS);

		for (auto& Wheel : VehicleEditorMenuVars::m_Vars.m_WheelLowrider) {
			TempOptions.push_back(addOption(RadioOption(getString(Native::_GetLabelText(Wheel.first)))
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorMenuVars::UpdateCaches();
					VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelIndex = Wheel.second;
					VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelType = 2;
				})
				.addOnHover([=] {
					Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, 2);
					Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
					Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, Wheel.second, Native::GetVehicleModVariation(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS));
				})));

			if (Wheel.second == CurrentMod) {
				SelectedIndex = Wheel.second;
			}
		}

		if (CurrentMod != -1 && VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelType == 2) {
			if (SelectedIndex != 99) {
				TempOptions[SelectedIndex + 1]->UpdateSelected();
				Menu::GetBase()->m_CurrentOption = SelectedIndex + 1;
			} else {
				m_Vars.m_Context.m_Toggles[m_Vars.m_Context.m_Selected] = false;
				m_Vars.m_Context.m_Selected = 99;
				m_Vars.m_Context.m_Toggles[99] = true;
			}
		}

		return;
	}

	/* Muscle */
	if (m_Vars.m_WheelType == 5) {
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		TempOptions.push_back(addOption(RadioOption(getString("Stock Wheels"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([] {
				VehicleEditorMenuVars::UpdateCaches();
				VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelIndex = -1;
				VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelType = 1;
			})
			.addOnHover([] {
				Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, 1);
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, -1, Native::GetVehicleModVariation(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS));
			})));

		int SelectedIndex = 99;
		int CurrentMod = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS);

		for (auto& Wheel : VehicleEditorMenuVars::m_Vars.m_WheelMuscle) {
			TempOptions.push_back(addOption(RadioOption(getString(Native::_GetLabelText(Wheel.first)))
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorMenuVars::UpdateCaches();
					VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelIndex = Wheel.second;
					VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelType = 1;
				})
				.addOnHover([=] {
					Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, 1);
					Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
					Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, Wheel.second, Native::GetVehicleModVariation(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS));
				})));

			if (Wheel.second == CurrentMod) {
				SelectedIndex = Wheel.second;
			}
		}

		if (CurrentMod != -1 && VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelType == 1) {
			if (SelectedIndex != 99) {
				TempOptions[SelectedIndex + 1]->UpdateSelected();
				Menu::GetBase()->m_CurrentOption = SelectedIndex + 1;
			} else {
				m_Vars.m_Context.m_Toggles[m_Vars.m_Context.m_Selected] = false;
				m_Vars.m_Context.m_Selected = 99;
				m_Vars.m_Context.m_Toggles[99] = true;
			}
		}

		return;
	}

	/* Offroad */
	if (m_Vars.m_WheelType == 6) {
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		TempOptions.push_back(addOption(RadioOption(getString("Stock Wheels"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([] {
				VehicleEditorMenuVars::UpdateCaches();
				VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelIndex = -1;
				VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelType = 4;
			})
			.addOnHover([] {
				Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, 4);
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, -1, Native::GetVehicleModVariation(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS));
			})));

		int SelectedIndex = 99;
		int CurrentMod = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS);

		for (auto& Wheel : VehicleEditorMenuVars::m_Vars.m_WheelOffroad) {
			TempOptions.push_back(addOption(RadioOption(getString(Native::_GetLabelText(Wheel.first)))
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorMenuVars::UpdateCaches();
					VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelIndex = Wheel.second;
					VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelType = 4;
				})
				.addOnHover([=] {
					Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, 4);
					Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
					Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, Wheel.second, Native::GetVehicleModVariation(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS));
				})));

			if (Wheel.second == CurrentMod) {
				SelectedIndex = Wheel.second;
			}
		}

		if (CurrentMod != -1 && VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelType == 4) {
			if (SelectedIndex != 99) {
				TempOptions[SelectedIndex + 1]->UpdateSelected();
				Menu::GetBase()->m_CurrentOption = SelectedIndex + 1;
			} else {
				m_Vars.m_Context.m_Toggles[m_Vars.m_Context.m_Selected] = false;
				m_Vars.m_Context.m_Selected = 99;
				m_Vars.m_Context.m_Toggles[99] = true;
			}
		}

		return;
	}

	/* Sport */
	if (m_Vars.m_WheelType == 7) {
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		TempOptions.push_back(addOption(RadioOption(getString("Stock Wheels"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([] {
				VehicleEditorMenuVars::UpdateCaches();
				VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelIndex = -1;
				VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelType = 0;
			})
			.addOnHover([] {
				Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, -1, Native::GetVehicleModVariation(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS));
			})));

		int SelectedIndex = 99;
		int CurrentMod = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS);

		for (auto& Wheel : VehicleEditorMenuVars::m_Vars.m_WheelSport) {
			TempOptions.push_back(addOption(RadioOption(getString(Native::_GetLabelText(Wheel.first)))
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorMenuVars::UpdateCaches();
					VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelIndex = Wheel.second;
					VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelType = 0;
				})
				.addOnHover([=] {
					Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, 0);
					Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
					Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, Wheel.second, Native::GetVehicleModVariation(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS));
				})));

			if (Wheel.second == CurrentMod) {
				SelectedIndex = Wheel.second;
			}
		}

		if (CurrentMod != -1 && VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelType == 0) {
			if (SelectedIndex != 99) {
				TempOptions[SelectedIndex + 1]->UpdateSelected();
				Menu::GetBase()->m_CurrentOption = SelectedIndex + 1;
			} else {
				m_Vars.m_Context.m_Toggles[m_Vars.m_Context.m_Selected] = false;
				m_Vars.m_Context.m_Selected = 99;
				m_Vars.m_Context.m_Toggles[99] = true;
			}
		}

		return;
	}

	/* SUV */
	if (m_Vars.m_WheelType == 8) {
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		TempOptions.push_back(addOption(RadioOption(getString("Stock Wheels"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([] {
				VehicleEditorMenuVars::UpdateCaches();
				VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelIndex = -1;
				VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelType = 3;
			})
			.addOnHover([] {
				Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, 3);
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, -1, Native::GetVehicleModVariation(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS));
			})));

		int SelectedIndex = 99;
		int CurrentMod = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS);

		for (auto& Wheel : VehicleEditorMenuVars::m_Vars.m_WheelSUV) {
			TempOptions.push_back(addOption(RadioOption(getString(Native::_GetLabelText(Wheel.first)))
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorMenuVars::UpdateCaches();
					VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelIndex = Wheel.second;
					VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelType = 3;
				})
				.addOnHover([=] {
					Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, 3);
					Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
					Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, Wheel.second, Native::GetVehicleModVariation(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS));
				})));

			if (Wheel.second == CurrentMod) {
				SelectedIndex = Wheel.second;
			}
		}

		if (CurrentMod != -1 && VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelType == 3) {
			if (SelectedIndex != 99) {
				TempOptions[SelectedIndex + 1]->UpdateSelected();
				Menu::GetBase()->m_CurrentOption = SelectedIndex + 1;
			} else {
				m_Vars.m_Context.m_Toggles[m_Vars.m_Context.m_Selected] = false;
				m_Vars.m_Context.m_Selected = 99;
				m_Vars.m_Context.m_Toggles[99] = true;
			}
		}

		return;
	}

	/* Tuner */
	if (m_Vars.m_WheelType == 9) {
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		TempOptions.push_back(addOption(RadioOption(getString("Stock Wheels"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([] {
				VehicleEditorMenuVars::UpdateCaches();
				VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelIndex = -1;
				VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelType = 5;
			})
			.addOnHover([] {
				Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, 5);
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, -1, Native::GetVehicleModVariation(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS));
			})));

		int SelectedIndex = 99;
		int CurrentMod = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS);

		for (auto& Wheel : VehicleEditorMenuVars::m_Vars.m_WheelTuner) {
			TempOptions.push_back(addOption(RadioOption(getString(Native::_GetLabelText(Wheel.first)))
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorMenuVars::UpdateCaches();
					VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelIndex = Wheel.second;
					VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelType = 5;
				})
				.addOnHover([=] {
					Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, 5);
					Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
					Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, Wheel.second, Native::GetVehicleModVariation(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS));
				})));

			if (Wheel.second == CurrentMod) {
				SelectedIndex = Wheel.second;
			}
		}

		if (CurrentMod != -1 && VehicleEditorWheelTypeMenuVars::m_Vars.m_CachedWheelType == 5) {
			if (SelectedIndex != 99) {
				TempOptions[SelectedIndex + 1]->UpdateSelected();
				Menu::GetBase()->m_CurrentOption = SelectedIndex + 1;
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
void VehicleEditorWheelTypeEditMenu::FeatureUpdate() {}

VehicleEditorWheelTypeEditMenu* _instance;
VehicleEditorWheelTypeEditMenu* VehicleEditorWheelTypeEditMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleEditorWheelTypeEditMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleEditorWheelTypeEditMenu::~VehicleEditorWheelTypeEditMenu() { delete _instance; }