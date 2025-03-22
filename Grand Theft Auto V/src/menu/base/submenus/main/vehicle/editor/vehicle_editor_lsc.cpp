#include "vehicle_editor_lsc.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_editor.hpp"
#include "utils/va.hpp"
#include "vehicle_editor_lights.hpp"
#include "lights/vehicle_editor_neon.hpp"
#include "vehicle_editor_horn.hpp"
#include "vehicle_editor_bumpers.hpp"
#include "vehicle_editor_fenders.hpp"
#include "vehicle_editor_bennys_interior.hpp"

using namespace VehicleEditorLSCMenuVars;

namespace VehicleEditorLSCMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> Armor[] = {
		{ TranslationString("Armor Upgrade 20%", true), 0, },
		{ TranslationString("Armor Upgrade 40%", true), 1, },
		{ TranslationString("Armor Upgrade 60%", true), 2, },
		{ TranslationString("Armor Upgrade 80%", true), 3, },
		{ TranslationString("Armor Upgrade 100%", true), 4 },
	};

	ScrollStruct<int> Suspension[] = {
		{ TranslationString("Lowered Suspension", true), 0, },
		{ TranslationString("Street Suspension", true), 1, },
		{ TranslationString("Sport Suspension", true), 2, },
		{ TranslationString("Competition Suspension", true), 3, },
		{ TranslationString("Race Suspension", true), 4, },
	};

	ScrollStruct<int> Transmission[] = {
		{ TranslationString("Street Transmission", true), 0, },
		{ TranslationString("Sports Transmission", true), 1, },
		{ TranslationString("Race Transmission", true), 2, },
	};

	ScrollStruct<int> Brakes[] = {
		{ TranslationString("Street Brakes", true), 0, },
		{ TranslationString("Sport Brakes", true), 1, },
		{ TranslationString("Race Brakes", true), 2, },
		{ TranslationString("Open Wheel Brakes", true), 3 },
	};

	ScrollStruct<std::vector<int>> Neons[] = {
		{ TranslationString("Front", true), { 2 }, },
		{ TranslationString("Back", true), { 3 }, },
		{ TranslationString("Sides", true), { 0, 1 }, },
		{ TranslationString("Front and Back", true), { 2, 3 }, },
		{ TranslationString("Front and Sides", true), { 2, 0, 1 }, },
		{ TranslationString("Back and Sides", true), { 3, 0, 1 }, },
		{ TranslationString("Front, Back and Sides", true), { 0, 1, 2, 3 } },
	};

	ScrollStruct<int> StandardHorns[] = {
		{ TranslationString("Stock Horn", true), -1, },
		{ TranslationString("Truck Horn", true), 0, },
		{ TranslationString("Cop Horn", true), 1, },
		{ TranslationString("Clown Horn", true), 2, },
	};

	ScrollStruct<int> MusicalHorns[] = {
		{ TranslationString("Star Spangled Banner 1", true), 28, },
		{ TranslationString("Star Spangled Banner 2", true), 29, },
		{ TranslationString("Star Spangled Banner 3", true), 30, },
		{ TranslationString("Star Spangled Banner 4", true), 31, },
		{ TranslationString("Jazz Horn 1", true), 24, },
		{ TranslationString("Jazz Horn 2", true), 25, },
		{ TranslationString("Jazz Horn 3", true), 26, },
		{ TranslationString("Scale - Do", true), 16, },
		{ TranslationString("Scale - Re", true), 17, },
		{ TranslationString("Scale - Mi", true), 18, },
		{ TranslationString("Scale - Fa", true), 19, },
		{ TranslationString("Scale - Sol", true), 20, },
		{ TranslationString("Scale - La", true), 21, },
		{ TranslationString("Scale - Ti", true), 22, },
		{ TranslationString("Scale - Do (high)", true), 23, },
		{ TranslationString("Classical Horn 1", true), 9, },
		{ TranslationString("Classical Horn 2", true), 10, },
		{ TranslationString("Classical Horn 3", true), 11, },
		{ TranslationString("Classical Horn 4", true), 12, },
		{ TranslationString("Classical Horn 5", true), 13, },
		{ TranslationString("Classical Horn 6", true), 14, },
		{ TranslationString("Classical Horn 7", true), 15, },
		{ TranslationString("Classical Horn 8", true), 32, },
		{ TranslationString("Musical Horn 1", true), 3, },
		{ TranslationString("Musical Horn 2", true), 4, },
		{ TranslationString("Musical Horn 3", true), 5, },
		{ TranslationString("Musical Horn 4", true), 6, },
		{ TranslationString("Musical Horn 5", true), 7, },
		{ TranslationString("Sad Trombone", true), 8 },
	};

	ScrollStruct<int> LoopHorns[] = {
		{ TranslationString("Jazz Horn Loop", true), 27, },
		{ TranslationString("San Andreas Loop", true), 42, },
		{ TranslationString("Liberty City Loop", true), 44, },
		{ TranslationString("Halloween Loop 1", true), 38, },
		{ TranslationString("Halloween Loop 2", true), 40, },
		{ TranslationString("Festive Loop 1", true), 46, },
		{ TranslationString("Festive Loop 2", true), 48, },
		{ TranslationString("Festive Loop 3", true), 50, },
		{ TranslationString("Air Horn 1", true), 52, },
		{ TranslationString("Air Horn 2", true), 54, },
		{ TranslationString("Air Horn 3", true), 56, },
		{ TranslationString("Classical Horn Loop 1", true), 33, },
		{ TranslationString("Classical Horn Loop 2", true), 34 },
	};

	ScrollStruct<int> XenonColors[] = {
		{ TranslationString("Default", true), -1 },
		{ TranslationString("White", true), 0 },
		{ TranslationString("Blue", true), 1 },
		{ TranslationString("Electric Blue", true), 2 },
		{ TranslationString("Mint Green", true), 3 },
		{ TranslationString("Lime Green", true), 4 },
		{ TranslationString("Yellow", true), 5 },
		{ TranslationString("Golden Shower", true), 6 },
		{ TranslationString("Orange", true), 7 },
		{ TranslationString("Red", true), 8 },
		{ TranslationString("Pony Pink", true), 9 },
		{ TranslationString("Hot PInk", true), 10 },
		{ TranslationString("Purple", true), 11 },
		{ TranslationString("Blacklight", true), 12 },
	};

	ScrollStruct<int> WindowTints[] = {
		{ TranslationString("Light Smoke", true), 4 },
		{ TranslationString("Dark Smoke", true), 3 },
		{ TranslationString("Limo", true), 2 },
		{ TranslationString("Pure Black", true), 5 },
		{ TranslationString("Green", true), 6 }
	};

	ScrollStruct<int> Plates[] = {
		{ TranslationString("Blue on White 1", true), 0 },
		{ TranslationString("Blue on White 2", true), 3 },
		{ TranslationString("Blue on White 3", true), 4 },
		{ TranslationString("Yellow on Blue", true), 2 },
		{ TranslationString("Yellow on Black", true), 1 },
		{ TranslationString("Yankton", true), 5 },
	};

	std::unordered_map<eVehicleModTypes, std::string> StockModNames = {
		{ MOD_EXHAUST, "Stock" },
		{ MOD_HOOD, "Stock" },
		{ MOD_SPOILER, "None" },
		{ MOD_GRILLE, "Stock" },
		{ MOD_FENDER, "Stock" },
		{ MOD_ROOF, "Stock" },
		{ MOD_CHASSIS, "None" },

		{ MOD_DASHBOARD, "None" },
		{ MOD_DIAL, "Stock" },
		{ MOD_DOOR_SPEAKER, "None" },
		{ MOD_SEATS, "None" },
		{ MOD_STEERINGWHEEL, "Stock" },
		{ MOD_LIVERY, "None" },
	};
}

void VehicleEditorLSCMenu::Init() {
	SetName("Los Santos Customs");
	SetParentSubmenu<VehicleEditorMenu>();

	addString("None");
	addString("Stock EMS");
	addString("EMS Upgrade, Level 1");
	addString("EMS Upgrade, Level 2");
	addString("EMS Upgrade, Level 3");
	addString("EMS Upgrade, Level 4");
	addString("EMS Upgrade, Level 5");
	addString("Stock Suspension");
	addString("Stock Transmission");
	addString("Stock Brakes");
	addString("Turbo Tuning");
	addString("Stock Skirts");
	addString("Stock Lights");
	addString("Xenon Lights");
	addString("Xenon Color");
	addString("Stock Front Bumper");
	addString("Stock Rear Bumper");
	addString("Stock Left Fender");
	addString("Stock Right Fender");
	addString("Stock");
}

void VehicleEditorLSCMenu::Update() {
	if (!Menu::GetLocalPlayer().m_InVehicle) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}

	VehicleEditorMenuVars::Scripts::DrawVehicleStats(Menu::GetLocalPlayer().m_Vehicle);
	if (Menu::GetBase()->m_ScrollOffset < Menu::GetSubmenuHandler()->GetTotalOptions() - Menu::GetBase()->m_MaxOptions && Menu::GetBase()->m_CurrentOption - Menu::GetBase()->m_ScrollOffset >= Menu::GetBase()->m_MaxOptions) Menu::GetBase()->m_ScrollOffset++;
}

/*Called once on submenu open*/
void VehicleEditorLSCMenu::UpdateOnce() {
	if (VehicleEditorMenuVars::m_Vars.m_LSC) {
		SetName("Los Santos Customs");
	} else SetName("Benny's");

	SetParentSubmenu<VehicleEditorMenu>();

	ClearOptionsOffset(0);

	VehicleEditorMenuVars::m_Vars.m_UpdateCache = true;
	VehicleEditorBennysInteriorMenuVars::m_Vars.m_UpdateCache = true;
	m_Vars.m_Context.Reset();
	m_Vars.m_Context.m_Selected = 0;

	if (m_Vars.m_ModType == MOD_ARMOR) {
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		TempOptions.push_back(addOption(RadioOption(getString("None"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([] {
				VehicleEditorMenuVars::UpdateCaches();
				VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Armor = -1;
			})
			.addOnHover([] {
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ARMOR, -1, false);
			})));

		for (int i = 0; i < Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_ARMOR); i++) {
			TempOptions.push_back(addOption(RadioOption(Armor[i].m_Name.Get())
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorMenuVars::UpdateCaches();
					VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Armor = Armor[i].m_Result;
				})
				.addOnHover([=] {
					Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
					Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ARMOR, Armor[i].m_Result, false);
				})));
		}

		if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Armor != -1) {
			if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Armor + 1 >= TempOptions.size()) return;
			TempOptions[VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Armor + 1]->UpdateSelected();
			Menu::GetBase()->m_CurrentOption = VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Armor + 1;
		}

		return;
	}

	if (m_Vars.m_ModType == MOD_ENGINE) {
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		TempOptions.push_back(addOption(RadioOption(getString("Stock EMS"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([] {
				VehicleEditorMenuVars::UpdateCaches();
				VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Engine = -1;
			})
			.addOnHover([] {
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ENGINE, -1, false);
			})));

		for (int i = 0; i < Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_ENGINE); i++) {
			TempOptions.push_back(addOption(RadioOption(getString(Utils::VA::VA("EMS Upgrade, Level %i", i + 1)))
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorMenuVars::UpdateCaches();
					VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Engine = i;
				})
				.addOnHover([=] {
					Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
					Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ENGINE, i, false);
				})));
		}

		if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Engine != -1) {
			if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Engine + 1 >= TempOptions.size()) return;
			TempOptions[VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Engine + 1]->UpdateSelected();
			Menu::GetBase()->m_CurrentOption = VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Engine + 1;
		}
		
		return;
	}

	if (m_Vars.m_ModType == MOD_SUSPENSION) {
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		TempOptions.push_back(addOption(RadioOption(getString("Stock Suspension"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([] {
				VehicleEditorMenuVars::UpdateCaches();
				VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Suspension = -1;
			})
			.addOnHover([] {
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SUSPENSION, -1, false);
			})));

		for (int i = 0; i < Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_SUSPENSION); i++) {
			TempOptions.push_back(addOption(RadioOption(Suspension[i].m_Name.Get())
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorMenuVars::UpdateCaches();
					VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Suspension = Suspension[i].m_Result;
				})
				.addOnHover([=] {
					Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
					Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SUSPENSION, Suspension[i].m_Result, false);
				})));
		}

		if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Suspension != -1) {
			if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Suspension + 1 >= TempOptions.size()) return;
			TempOptions[VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Suspension + 1]->UpdateSelected();
			Menu::GetBase()->m_CurrentOption = VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Suspension + 1;
		}

		return;
	}

	if (m_Vars.m_ModType == MOD_TRANSMISSION) {
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		TempOptions.push_back(addOption(RadioOption(getString("Stock Transmission"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([] {
				VehicleEditorMenuVars::UpdateCaches();
				VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Transmission = -1;
			})
			.addOnHover([] {
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TRANSMISSION, -1, false);
			})));

		for (int i = 0; i < NUMOF(Transmission); i++) {
			TempOptions.push_back(addOption(RadioOption(Transmission[i].m_Name.Get())
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorMenuVars::UpdateCaches();
					VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Transmission = Transmission[i].m_Result;
				})
				.addOnHover([=] {
					Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
					Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TRANSMISSION, Transmission[i].m_Result, false);
				})));
		}

		if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Transmission != -1) {
			if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Transmission + 1 >= TempOptions.size()) return;
			TempOptions[VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Transmission + 1]->UpdateSelected();
			Menu::GetBase()->m_CurrentOption = VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Transmission + 1;
		}

		return;
	}

	if (m_Vars.m_ModType == MOD_BRAKES) {
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		TempOptions.push_back(addOption(RadioOption(getString("Stock Brakes"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([] {
				VehicleEditorMenuVars::UpdateCaches();
				VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Brakes = -1;
			})
			.addOnHover([] {
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_BRAKES, -1, false);
			})));

		for (int i = 0; i < Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_BRAKES); i++) {
			TempOptions.push_back(addOption(RadioOption(Brakes[i].m_Name.Get())
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorMenuVars::UpdateCaches();
					VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Brakes = Brakes[i].m_Result;
				})
				.addOnHover([=] {
					Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
					Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_BRAKES, Brakes[i].m_Result, false);
				})));
		}

		if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Brakes != -1) {
			if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Brakes + 1 >= TempOptions.size()) return;
			TempOptions[VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Brakes + 1]->UpdateSelected();
			Menu::GetBase()->m_CurrentOption = VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Brakes + 1;
		}

		return;
	}

	if (m_Vars.m_ModType == MOD_TURBO) {
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		TempOptions.push_back(addOption(RadioOption(getString("None"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([] {
				VehicleEditorMenuVars::UpdateCaches();
				VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Turbo = 0;
			})
			.addOnHover([] {
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::ToggleVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TURBO, false);
			})));

		TempOptions.push_back(addOption(RadioOption(getString("Turbo Tuning"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([=] {
				VehicleEditorMenuVars::UpdateCaches();
				VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Turbo = 1;
			})
			.addOnHover([=] {
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::ToggleVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TURBO, true);
			})));

		if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Turbo) {
			TempOptions[1]->UpdateSelected();
			Menu::GetBase()->m_CurrentOption = 1;
		}

		return;
	}

	if (m_Vars.m_ModType == MOD_SIDESKIRT) {
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		TempOptions.push_back(addOption(RadioOption(getString("Stock Skirts"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([] {
				VehicleEditorMenuVars::UpdateCaches();
				VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Skirts = -1;
			})
			.addOnHover([] {
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SIDESKIRT, -1, false);
			})));

		for (int i = 0; i < Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_SIDESKIRT); i++) {
			auto Label = Native::_GetLabelText(Native::GetModTextLabel(Menu::GetLocalPlayer().m_Vehicle, MOD_SIDESKIRT, i));
			if (!strcmp(Label, "NULL")) Label = "Custom Skirts";

			TempOptions.push_back(addOption(RadioOption(Label)
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorMenuVars::UpdateCaches();
					VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Skirts = i;
				})
				.addOnHover([=] {
					Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
					Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SIDESKIRT, i, false);
				})));
		}

		if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Skirts != -1) {
			if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Skirts + 1 >= TempOptions.size()) return;
			TempOptions[VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Skirts + 1]->UpdateSelected();
			Menu::GetBase()->m_CurrentOption = VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Skirts + 1;
		}

		return;
	}

	if (m_Vars.m_ModType == MOD_XENONLIGHTS) {
		SetParentSubmenu<VehicleEditorLightsMenu>();
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		TempOptions.push_back(addOption(RadioOption(getString("Stock Lights"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([] {
				VehicleEditorMenuVars::UpdateCaches();
				VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Lights = 0;
			})
			.addOnHover([] {
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::ToggleVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_XENONLIGHTS, false);
			})));

		TempOptions.push_back(addOption(RadioOption(getString("Xenon Lights"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([=] {
				VehicleEditorMenuVars::UpdateCaches();
				VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Lights = 1;
			})
			.addOnHover([=] {
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::ToggleVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_XENONLIGHTS, true);
			})));

		addOption(ScrollOption<int>(SCROLL, getString("Xenon Color"))
			.addScroll(m_Vars.m_XenonColor, 0, NUMOF(XenonColors), XenonColors)
			.addOnClick([] {
				VehicleEditorMenuVars::UpdateCaches();
				VehicleEditorMenuVars::m_Vars.m_CachedStats.m_XenonColor = XenonColors[m_Vars.m_XenonColor].m_Result;
			})
			.addOnHover([] {
				Native::SetVehicleXenonLightsColour(Menu::GetLocalPlayer().m_Vehicle, XenonColors[m_Vars.m_XenonColor].m_Result);
			}).addRequirement([] { return VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Lights == 1; }));

		if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Lights) {
			TempOptions[1]->UpdateSelected();
			Menu::GetBase()->m_CurrentOption = 1;
		}

		for (int i = 0; i < NUMOF(XenonColors); i++) {
			if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_XenonColor == XenonColors[i].m_Result) {
				m_Vars.m_XenonColor = i;
				break;
			}
		}

		return;
	}

	if (m_Vars.m_ModType == MOD_NEON) {
		SetParentSubmenu<VehicleEditorNeonMenu>();
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		auto DisableNeons = [] {
			for (int i = 0; i < 4; i++) {
				Native::_SetVehicleNeonLightEnabled(Menu::GetLocalPlayer().m_Vehicle, i, false);
			}
		};

		auto SetNeons = [] (std::vector<int> Neons) {
			for (auto& Neon : Neons) {
				Native::_SetVehicleNeonLightEnabled(Menu::GetLocalPlayer().m_Vehicle, Neon, true);
			}
		};

		TempOptions.push_back(addOption(RadioOption(getString("None"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([=] {
				VehicleEditorMenuVars::UpdateCaches();
				// set neons enabled in vars
				for (int i = 0; i < 4; i++) {
					VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Neon[i] = false;
				}
			})
			.addOnHover([=] {
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				// set neons enabled native
				DisableNeons();
			})));

		for (int i = 0; i < NUMOF(Neons); i++) {
			TempOptions.push_back(addOption(RadioOption(Neons[i].m_Name.Get())
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorMenuVars::UpdateCaches();
					
					memset(&VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Neon, 0, sizeof(VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Neon));
					for (auto Neon : Neons[i].m_Result) {
						VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Neon[Neon] = true;
					}
				})
				.addOnHover([=] {
					Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
					DisableNeons();
					SetNeons(Neons[i].m_Result);
				})));
		}

		bool Left = VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Neon[0];
		bool Right = VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Neon[1];
		bool Front = VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Neon[2];
		bool Back = VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Neon[3];

		if (Front && (!Left && !Right && !Back)) {
			TempOptions[1]->UpdateSelected();
			Menu::GetBase()->m_CurrentOption = 1;
		} else if (Back && (!Left && !Right && !Front)) {
			TempOptions[2]->UpdateSelected();
			Menu::GetBase()->m_CurrentOption = 2;
		} else if ((Left && Right) && (!Front && !Back)) {
			TempOptions[3]->UpdateSelected();
			Menu::GetBase()->m_CurrentOption = 3;
		} else if (Front && Back && (!Left && !Right)) {
			TempOptions[4]->UpdateSelected();
			Menu::GetBase()->m_CurrentOption = 4;
		} else if (Front && Left & Right && (!Back)) {
			TempOptions[5]->UpdateSelected();
			Menu::GetBase()->m_CurrentOption = 5;
		} else if (Back && Left && Right && (!Front)) {
			TempOptions[6]->UpdateSelected();
			Menu::GetBase()->m_CurrentOption = 6;
		} else if (Front && Back && Left && Right) {
			TempOptions[7]->UpdateSelected();
			Menu::GetBase()->m_CurrentOption = 7;
		}

		return;
	}

	if (m_Vars.m_ModType == MOD_HORN_STANDARD) {
		SetParentSubmenu<VehicleEditorHornMenu>();
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		int SelectedIndex = 99;
		int CurrentMod = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_HORNS);

		for (int i = 0; i < NUMOF(StandardHorns); i++) {
			auto Horn = StandardHorns[i];
			TempOptions.push_back(addOption(RadioOption(Horn.m_Name.Get())
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorMenuVars::UpdateCaches();
					VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Horn = Horn.m_Result;
				})
				.addOnHover([=] {
					Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
					Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_HORNS, Horn.m_Result, false);
				})));

			if (Horn.m_Result == CurrentMod) {
				SelectedIndex = i;
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

	if (m_Vars.m_ModType == MOD_HORN_MUSICAL) {
		SetParentSubmenu<VehicleEditorHornMenu>();
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		int SelectedIndex = 99;
		int CurrentMod = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_HORNS);

		for (int i = 0; i < NUMOF(MusicalHorns); i++) {
			auto Horn = MusicalHorns[i];
			TempOptions.push_back(addOption(RadioOption(Horn.m_Name.Get())
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorMenuVars::UpdateCaches();
					VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Horn = Horn.m_Result;
				})
				.addOnHover([=] {
					Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
					Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_HORNS, Horn.m_Result, false);
				})));

			if (Horn.m_Result == CurrentMod) {
				SelectedIndex = i;
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

	if (m_Vars.m_ModType == MOD_HORN_LOOP) {
		SetParentSubmenu<VehicleEditorHornMenu>();
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		int SelectedIndex = 99;
		int CurrentMod = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_HORNS);

		for (int i = 0; i < NUMOF(LoopHorns); i++) {
			auto Horn = LoopHorns[i];
			TempOptions.push_back(addOption(RadioOption(Horn.m_Name.Get())
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorMenuVars::UpdateCaches();
					VehicleEditorMenuVars::m_Vars.m_CachedStats.m_Horn = Horn.m_Result;
				})
				.addOnHover([=] {
					Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
					Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_HORNS, Horn.m_Result, false);
				})));

			if (Horn.m_Result == CurrentMod) {
				SelectedIndex = i;
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

	if (m_Vars.m_ModType == MOD_FRONTBUMPER) {
		SetParentSubmenu<VehicleEditorBumpersMenu>();
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		TempOptions.push_back(addOption(RadioOption(getString("Stock Front Bumper"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([] {
				VehicleEditorMenuVars::UpdateCaches();
				VehicleEditorMenuVars::m_Vars.m_CachedStats.m_FrontBumper = -1;
			})
			.addOnHover([] {
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTBUMPER, -1, false);
			})));

		for (int i = 0; i < Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTBUMPER); i++) {
			TempOptions.push_back(addOption(RadioOption(Native::_GetLabelText(Native::GetModTextLabel(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTBUMPER, i)))
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorMenuVars::UpdateCaches();
					VehicleEditorMenuVars::m_Vars.m_CachedStats.m_FrontBumper = i;
				})
				.addOnHover([=] {
					Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
					Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTBUMPER, i, false);
				})));
		}

		if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_FrontBumper != -1) {
			if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_FrontBumper + 1 >= TempOptions.size()) return;
			TempOptions[VehicleEditorMenuVars::m_Vars.m_CachedStats.m_FrontBumper + 1]->UpdateSelected();
			Menu::GetBase()->m_CurrentOption = VehicleEditorMenuVars::m_Vars.m_CachedStats.m_FrontBumper + 1;
		}

		return;
	}

	if (m_Vars.m_ModType == MOD_REARBUMPER) {
		SetParentSubmenu<VehicleEditorBumpersMenu>();
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		TempOptions.push_back(addOption(RadioOption(getString("Stock Rear Bumper"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([] {
				VehicleEditorMenuVars::UpdateCaches();
				VehicleEditorMenuVars::m_Vars.m_CachedStats.m_RearBumper = -1;
			})
			.addOnHover([] {
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_REARBUMPER, -1, false);
			})));

		for (int i = 0; i < Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_REARBUMPER); i++) {
			TempOptions.push_back(addOption(RadioOption(Native::_GetLabelText(Native::GetModTextLabel(Menu::GetLocalPlayer().m_Vehicle, MOD_REARBUMPER, i)))
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorMenuVars::UpdateCaches();
					VehicleEditorMenuVars::m_Vars.m_CachedStats.m_RearBumper = i;
				})
				.addOnHover([=] {
					Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
					Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_REARBUMPER, i, false);
				})));
		}

		if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_RearBumper != -1) {
			if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_RearBumper + 1 >= TempOptions.size()) return;
			TempOptions[VehicleEditorMenuVars::m_Vars.m_CachedStats.m_RearBumper + 1]->UpdateSelected();
			Menu::GetBase()->m_CurrentOption = VehicleEditorMenuVars::m_Vars.m_CachedStats.m_RearBumper + 1;
		}

		return;
	}

	if (m_Vars.m_ModType == MOD_FENDER) {
		SetParentSubmenu<VehicleEditorFendersMenu>();
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		TempOptions.push_back(addOption(RadioOption(getString("Stock Left Fender"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([] {
				VehicleEditorMenuVars::UpdateCaches();
				VehicleEditorMenuVars::m_Vars.m_CachedStats.m_LeftFender = -1;
			})
			.addOnHover([] {
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FENDER, -1, false);
			})));

		for (int i = 0; i < Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_FENDER); i++) {
			TempOptions.push_back(addOption(RadioOption(Native::_GetLabelText(Native::GetModTextLabel(Menu::GetLocalPlayer().m_Vehicle, MOD_FENDER, i)))
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorMenuVars::UpdateCaches();
					VehicleEditorMenuVars::m_Vars.m_CachedStats.m_LeftFender = i;
				})
				.addOnHover([=] {
					Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
					Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FENDER, i, false);
				})));
		}

		if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_LeftFender != -1) {
			if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_LeftFender + 1 >= TempOptions.size()) return;
			TempOptions[VehicleEditorMenuVars::m_Vars.m_CachedStats.m_LeftFender + 1]->UpdateSelected();
			Menu::GetBase()->m_CurrentOption = VehicleEditorMenuVars::m_Vars.m_CachedStats.m_LeftFender + 1;
		}

		return;
	}

	if (m_Vars.m_ModType == MOD_RIGHTFENDER) {
		SetParentSubmenu<VehicleEditorFendersMenu>();
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		TempOptions.push_back(addOption(RadioOption(getString("Stock Right Fender"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([] {
				VehicleEditorMenuVars::UpdateCaches();
				VehicleEditorMenuVars::m_Vars.m_CachedStats.m_RightFender = -1;
			})
			.addOnHover([] {
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_RIGHTFENDER, -1, false);
			})));

		for (int i = 0; i < Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_RIGHTFENDER); i++) {
			TempOptions.push_back(addOption(RadioOption(Native::_GetLabelText(Native::GetModTextLabel(Menu::GetLocalPlayer().m_Vehicle, MOD_RIGHTFENDER, i)))
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorMenuVars::UpdateCaches();
					VehicleEditorMenuVars::m_Vars.m_CachedStats.m_RightFender = i;
				})
				.addOnHover([=] {
					Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
					Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_RIGHTFENDER, i, false);
				})));
		}

		if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_RightFender != -1) {
			if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_RightFender + 1 >= TempOptions.size()) return;
			TempOptions[VehicleEditorMenuVars::m_Vars.m_CachedStats.m_RightFender + 1]->UpdateSelected();
			Menu::GetBase()->m_CurrentOption = VehicleEditorMenuVars::m_Vars.m_CachedStats.m_RightFender + 1;
		}

		return;
	}

	if (m_Vars.m_ModType == MOD_WINDOWS_CUSTOM) {
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		TempOptions.push_back(addOption(RadioOption(getString("None"))
			.addRadio(m_Vars.m_Context)
			.addOnClick([] {
				VehicleEditorMenuVars::UpdateCaches();
				VehicleEditorMenuVars::m_Vars.m_CachedStats.m_WindowTint = 0;
			})
			.addOnHover([] {
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::SetVehicleWindowTint(Menu::GetLocalPlayer().m_Vehicle, 0);
			})));

		for (int i = 0; i < NUMOF(WindowTints); i++) {
			TempOptions.push_back(addOption(RadioOption(WindowTints[i].m_Name.Get())
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorMenuVars::UpdateCaches();
					VehicleEditorMenuVars::m_Vars.m_CachedStats.m_WindowTint = WindowTints[i].m_Result;
				})
				.addOnHover([=] {
					Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
					Native::SetVehicleWindowTint(Menu::GetLocalPlayer().m_Vehicle, WindowTints[i].m_Result);
				})));
		}

		if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_WindowTint > 0) {
			if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_WindowTint >= TempOptions.size()) return;
			TempOptions[VehicleEditorMenuVars::m_Vars.m_CachedStats.m_WindowTint]->UpdateSelected(); // crash here
			Menu::GetBase()->m_CurrentOption = VehicleEditorMenuVars::m_Vars.m_CachedStats.m_WindowTint;
		}

		return;
	}

	if (m_Vars.m_ModType == MOD_PLATES) {
		std::vector<std::shared_ptr<RadioOption>> TempOptions;

		for (int i = 0; i < NUMOF(Plates); i++) {
			TempOptions.push_back(addOption(RadioOption(Plates[i].m_Name.Get())
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorMenuVars::UpdateCaches();
					VehicleEditorMenuVars::m_Vars.m_CachedStats.m_PlateIndex = Plates[i].m_Result;
				})
				.addOnHover([=] {
					Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
					Native::SetVehicleNumberPlateTextIndex(Menu::GetLocalPlayer().m_Vehicle, Plates[i].m_Result);
				})));
		}

		if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_PlateIndex != 0) {
			if (VehicleEditorMenuVars::m_Vars.m_CachedStats.m_PlateIndex >= TempOptions.size()) return;
			TempOptions[VehicleEditorMenuVars::m_Vars.m_CachedStats.m_PlateIndex]->UpdateSelected();
			Menu::GetBase()->m_CurrentOption = VehicleEditorMenuVars::m_Vars.m_CachedStats.m_PlateIndex;
		}
		return;
	}

	if (m_Vars.m_ModType == MOD_DASHBOARD
		|| m_Vars.m_ModType == MOD_DIAL
		|| m_Vars.m_ModType == MOD_DOOR_SPEAKER
		|| m_Vars.m_ModType == MOD_SEATS
		|| m_Vars.m_ModType == MOD_STEERINGWHEEL
		|| m_Vars.m_ModType == MOD_ORNAMENTS
		|| m_Vars.m_ModType == MOD_SHIFTER_LEAVERS
		|| m_Vars.m_ModType == MOD_SPEAKERS) {
		SetParentSubmenu<VehicleEditorBennysInteriorMenu>();
	}

	std::vector<std::shared_ptr<RadioOption>> TempOptions;

	std::string StockName = getString("None");
	if (StockModNames.find(m_Vars.m_ModType) != StockModNames.end()) {
		StockName = getString(StockModNames[m_Vars.m_ModType]);
	}

	TempOptions.push_back(addOption(RadioOption(StockName)
		.addRadio(m_Vars.m_Context)
		.addOnClick([=] {
			VehicleEditorMenuVars::UpdateCaches();
			*m_Vars.m_Value = -1;
		})
		.addOnHover([] {
			Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_ModType, -1, false);
		})));

	for (int i = 0; i < Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_ModType); i++) {
		const char* ModLabel = Native::GetModTextLabel(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_ModType, i);
		if (IsValidPtr(ModLabel)) {
			TempOptions.push_back(addOption(RadioOption(Native::_GetLabelText(ModLabel))
				.addRadio(m_Vars.m_Context)
				.addOnClick([=] {
					VehicleEditorMenuVars::UpdateCaches();
					*m_Vars.m_Value = i;
				})
				.addOnHover([=] {
				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_ModType, i, false);
			})));
		}
	}

	if (m_Vars.m_Value) {
		if (*m_Vars.m_Value != -1) {
			if (*m_Vars.m_Value + 1 >= TempOptions.size()) return;

			if (TempOptions[*m_Vars.m_Value + 1]) {
				TempOptions[*m_Vars.m_Value + 1]->UpdateSelected();
			}

			Menu::GetBase()->m_CurrentOption = *m_Vars.m_Value + 1;
		}
	}
}

/*Called always*/
void VehicleEditorLSCMenu::FeatureUpdate() {}

VehicleEditorLSCMenu* _instance;
VehicleEditorLSCMenu* VehicleEditorLSCMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleEditorLSCMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleEditorLSCMenu::~VehicleEditorLSCMenu() { delete _instance; }