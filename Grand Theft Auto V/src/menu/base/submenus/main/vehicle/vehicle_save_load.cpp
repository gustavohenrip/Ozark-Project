#include "vehicle_save_load.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle.hpp"
#include "utils/va.hpp"
#include "utils/json.hpp"
#include "menu/base/util/notify.hpp"
#include "rage/engine.hpp"
#include "utils/log.hpp"

using namespace VehicleSaveLoadMenuVars;

namespace VehicleSaveLoadMenuVars {
	Vars_ m_Vars;

	void Load(const char* Name) {
		if (!Menu::GetLocalPlayer().m_InVehicle) {
			Menu::GetNotify()->NotifyStacked(VehicleSaveLoadMenu::GetInstance()->getString("You need to be in a vehicle!"));
			return;
		}

		try {
			std::ifstream Input(Utils::VA::VA("%s%s.json", Utils::GetConfig()->GetLSCPath().c_str(), Name));
			if (Input.good()) {
				nlohmann::json Object = nlohmann::json::parse(Input);

				Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);

				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ARMOR, Object["Armor"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_BRAKES, Object["Brakes"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTBUMPER, Object["FrontBumper"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_REARBUMPER, Object["RearBumper"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ENGINE, Object["Engine"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_EXHAUST, Object["Exhaust"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_HOOD, Object["Hood"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_HORNS, Object["Horn"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SIDESKIRT, Object["Skirts"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SUSPENSION, Object["Suspension"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TRANSMISSION, Object["Transmission"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SPOILER, Object["Spoiler"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_GRILLE, Object["Grille"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FENDER, Object["LeftFender"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_RIGHTFENDER, Object["RightFender"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ROOF, Object["Roof"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_CHASSIS, Object["Chassis"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, Object["FrontWheels"], Object["CustomTires"]);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_BACKWHEELS, Object["BackWheels"], Object["CustomTires"]);

				Native::ToggleVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TURBO, Object["Turbo"]);
				Native::ToggleVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_XENONLIGHTS, Object["Lights"]);
				Native::ToggleVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_UNK17, Object["Unk17"]);
				Native::ToggleVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_UNK19, Object["Unk19"]);
				Native::ToggleVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TIRESMOKE, Object["TireSmoke"]);
				Native::ToggleVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_UNK21, Object["Unk21"]);

				Native::SetVehicleWindowTint(Menu::GetLocalPlayer().m_Vehicle, Object["WindowTint"]);
				Native::SetVehicleXenonLightsColour(Menu::GetLocalPlayer().m_Vehicle, Object["XenonColor"]);
				Native::SetVehicleNumberPlateTextIndex(Menu::GetLocalPlayer().m_Vehicle, Object["PlateIndex"]);
				Native::SetVehicleNumberPlateText(Menu::GetLocalPlayer().m_Vehicle, Object["PlateText"].get<std::string>().c_str());
				Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, Object["WheelType"]);

				// Bennys
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_PLATEHOLDER, Object["BennysPlateholder"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_VANITY_PLATES, Object["BennysVanityPlate"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TRIM_DESIGN, Object["BennysTrimDesign"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ORNAMENTS, Object["BennysOrnaments"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_DASHBOARD, Object["BennysInteriorDesign"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_DIAL, Object["BennysDials"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_DOOR_SPEAKER, Object["BennysDoors"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SEATS, Object["BennysSeats"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_STEERINGWHEEL, Object["BennysSteeringWheel"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SHIFTER_LEAVERS, Object["BennysShifter"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_PLAQUES, Object["BennysPlaques"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TRUNK, Object["BennysTrunk"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SPEAKERS, Object["BennysAudioInstall"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_HYDRULICS, Object["BennysHydraulics"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ENGINE_BLOCK, Object["BennysEngineBlock"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_AIR_FILTER, Object["BennysAirFilters"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_STRUTS, Object["BennysStuntBrace"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ARCH_COVER, Object["BennysArchCovers"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_AERIALS, Object["BennysAerials"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TRIM, Object["BennysRearStripeColor"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TANK, Object["BennysTank"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_WINDOWS, Object["BennysWindowMod"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_LIVERY, Object["BennysLivery"], false);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SPECIAL_FRONT_RIMS, Object["BennysSpecialFrontRims"], Object["CustomTires"]);
				Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SPECIAL_REAR_RIMS, Object["BennysSpecialRearRims"], Object["CustomTires"]);

				Native::_SetVehicleNeonLightEnabled(Menu::GetLocalPlayer().m_Vehicle, 0, Object["Neon"][0]);
				Native::_SetVehicleNeonLightEnabled(Menu::GetLocalPlayer().m_Vehicle, 1, Object["Neon"][1]);
				Native::_SetVehicleNeonLightEnabled(Menu::GetLocalPlayer().m_Vehicle, 2, Object["Neon"][2]);
				Native::_SetVehicleNeonLightEnabled(Menu::GetLocalPlayer().m_Vehicle, 3, Object["Neon"][3]);

				Native::_SetVehicleNeonLightsColour(Menu::GetLocalPlayer().m_Vehicle, Object["NeonColor"][0], Object["NeonColor"][1], Object["NeonColor"][2]);
				Native::SetVehicleTyreSmokeColor(Menu::GetLocalPlayer().m_Vehicle, Object["TyreSmokeColor"][0], Object["TyreSmokeColor"][1], Object["TyreSmokeColor"][2]);

				Native::SetVehicleCustomPrimaryColour(Menu::GetLocalPlayer().m_Vehicle, Object["PrimaryColor"][0], Object["PrimaryColor"][1], Object["PrimaryColor"][2]);
				Native::SetVehicleCustomSecondaryColour(Menu::GetLocalPlayer().m_Vehicle, Object["SecondaryColor"][0], Object["SecondaryColor"][1], Object["SecondaryColor"][2]);

				Native::SetVehicleExtraColours(Menu::GetLocalPlayer().m_Vehicle, Object["PearlescentColor"], Object["WheelColor"]);

				*(float*)(Rage::Engine::GetEntityAddress(Menu::GetLocalPlayer().m_Vehicle) + 0x1360) = Object["Torque"];
				Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle->m_HandlingData->m_TractionCurveMax = Object["Traction"];
			}
		} catch (std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}
	}

	void Save(const char* Name) {
		if (!Menu::GetLocalPlayer().m_InVehicle) {
			Menu::GetNotify()->NotifyStacked(VehicleSaveLoadMenu::GetInstance()->getString("You need to be in a vehicle!"));
			return;
		}

		try {
			std::ofstream Output(Utils::VA::VA("%s%s.json", Utils::GetConfig()->GetLSCPath().c_str(), Name));
			nlohmann::json Object;

			Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);

			Object["Armor"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ARMOR);
			Object["Brakes"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_BRAKES);
			Object["FrontBumper"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTBUMPER);
			Object["RearBumper"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_REARBUMPER);
			Object["Engine"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ENGINE);
			Object["Exhaust"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_EXHAUST);
			Object["Hood"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_HOOD);
			Object["Horn"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_HORNS);
			Object["Skirts"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SIDESKIRT);
			Object["Suspension"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SUSPENSION);
			Object["Transmission"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TRANSMISSION);
			Object["Spoiler"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SPOILER);
			Object["Grille"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_GRILLE);
			Object["LeftFender"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FENDER);
			Object["RightFender"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_RIGHTFENDER);
			Object["Roof"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ROOF);
			Object["Chassis"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_CHASSIS);
			Object["FrontWheels"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS);
			Object["BackWheels"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_BACKWHEELS);

			Object["WindowTint"] = Native::GetVehicleWindowTint(Menu::GetLocalPlayer().m_Vehicle);
			Object["Turbo"] = Native::IsToggleModOn(Menu::GetLocalPlayer().m_Vehicle, MOD_TURBO);
			Object["Lights"] = Native::IsToggleModOn(Menu::GetLocalPlayer().m_Vehicle, MOD_XENONLIGHTS);
			Object["XenonColor"] = Native::GetVehicleXenonLightsColour(Menu::GetLocalPlayer().m_Vehicle);
			Object["PlateIndex"] = Native::GetVehicleNumberPlateTextIndex(Menu::GetLocalPlayer().m_Vehicle);
			Object["PlateText"] = Native::GetVehicleNumberPlateText(Menu::GetLocalPlayer().m_Vehicle);
			Object["WheelType"] = Native::GetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle);
			Object["CustomTires"] = Native::GetVehicleModVariation(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS);

			Object["Unk17"] = Native::IsToggleModOn(Menu::GetLocalPlayer().m_Vehicle, MOD_UNK17);
			Object["Unk19"] = Native::IsToggleModOn(Menu::GetLocalPlayer().m_Vehicle, MOD_UNK19);
			Object["TireSmoke"] = Native::IsToggleModOn(Menu::GetLocalPlayer().m_Vehicle, MOD_TIRESMOKE);
			Object["Unk21"] = Native::IsToggleModOn(Menu::GetLocalPlayer().m_Vehicle, MOD_UNK21);

			// Bennys
			Object["BennysPlateholder"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_PLATEHOLDER);
			Object["BennysVanityPlate"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_VANITY_PLATES);
			Object["BennysTrimDesign"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TRIM_DESIGN);
			Object["BennysOrnaments"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ORNAMENTS);
			Object["BennysInteriorDesign"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_DASHBOARD);
			Object["BennysDials"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_DIAL);
			Object["BennysDoors"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_DOOR_SPEAKER);
			Object["BennysSeats"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SEATS);
			Object["BennysSteeringWheel"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_STEERINGWHEEL);
			Object["BennysShifter"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SHIFTER_LEAVERS);
			Object["BennysPlaques"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_PLAQUES);
			Object["BennysTrunk"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TRUNK);
			Object["BennysAudioInstall"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SPEAKERS);
			Object["BennysHydraulics"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_HYDRULICS);
			Object["BennysEngineBlock"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ENGINE_BLOCK);
			Object["BennysAirFilters"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_AIR_FILTER);
			Object["BennysStuntBrace"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_STRUTS);
			Object["BennysArchCovers"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ARCH_COVER);
			Object["BennysAerials"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_AERIALS);
			Object["BennysRearStripeColor"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TRIM);
			Object["BennysTank"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TANK);
			Object["BennysWindowMod"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_WINDOWS);
			Object["BennysLivery"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_LIVERY);
			Object["BennysSpecialFrontRims"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SPECIAL_FRONT_RIMS);
			Object["BennysSpecialRearRims"] = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SPECIAL_REAR_RIMS);

			Object["Neon"] = {
				Native::_IsVehicleNeonLightEnabled(Menu::GetLocalPlayer().m_Vehicle, 0),
				Native::_IsVehicleNeonLightEnabled(Menu::GetLocalPlayer().m_Vehicle, 1),
				Native::_IsVehicleNeonLightEnabled(Menu::GetLocalPlayer().m_Vehicle, 2),
				Native::_IsVehicleNeonLightEnabled(Menu::GetLocalPlayer().m_Vehicle, 3)
			};

			Color NeonColor;
			Native::_GetVehicleNeonLightsColour(Menu::GetLocalPlayer().m_Vehicle, &NeonColor.R, &NeonColor.G, &NeonColor.B);
			Object["NeonColor"] = { NeonColor.R, NeonColor.G, NeonColor.B };

			Color TyreSmokeColor;
			Native::GetVehicleTyreSmokeColor(Menu::GetLocalPlayer().m_Vehicle, &TyreSmokeColor.R, &TyreSmokeColor.G, &TyreSmokeColor.B);
			Object["TyreSmokeColor"] = { TyreSmokeColor.R, TyreSmokeColor.G, TyreSmokeColor.B };

			Color VehicleColor;
			Native::GetVehicleColor(Menu::GetLocalPlayer().m_Vehicle, &VehicleColor.R, &VehicleColor.G, &VehicleColor.B);
			Object["VehicleColor"] = { VehicleColor.R, VehicleColor.G, VehicleColor.B };

			int PearlescentColor = 0;
			int WheelColor = 0;
			Native::GetVehicleExtraColours(Menu::GetLocalPlayer().m_Vehicle, &PearlescentColor, &WheelColor);
			Object["PearlescentColor"] = PearlescentColor;
			Object["WheelColor"] = WheelColor;

			Color PrimaryColor = VehicleColor;
			Color SecondaryColor = VehicleColor;
			
			if (Native::GetIsVehiclePrimaryColourCustom(Menu::GetLocalPlayer().m_Vehicle)) {
				Native::GetVehicleCustomPrimaryColour(Menu::GetLocalPlayer().m_Vehicle, &PrimaryColor.R, &PrimaryColor.G, &PrimaryColor.B);
			}

			if (Native::GetIsVehicleSecondaryColourCustom(Menu::GetLocalPlayer().m_Vehicle)) {
				Native::GetVehicleCustomSecondaryColour(Menu::GetLocalPlayer().m_Vehicle, &SecondaryColor.R, &SecondaryColor.G, &SecondaryColor.B);
			}

			Object["PrimaryColor"] = { PrimaryColor.R, PrimaryColor.G, PrimaryColor.B };
			Object["SecondaryColor"] = { SecondaryColor.R, SecondaryColor.G, SecondaryColor.B };

			Object["Torque"] = *(float*)(Rage::Engine::GetEntityAddress(Menu::GetLocalPlayer().m_Vehicle) + 0x1360);
			Object["Traction"] = Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle->m_HandlingData->m_TractionCurveMax;

			Output << Object.dump(4);
			Output.close();
			Menu::GetSubmenuHandler()->GetCurrentSubmenu()->UpdateOnce();
		} catch (std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}
	}
}

void VehicleSaveLoadMenu::Init() {
	SetName("Save and Load");
	SetParentSubmenu<VehicleMenu>();

	addString("You need to be in a vehicle!");
	addString("~m~None");

	addOption(ButtonOption("Save Vehicle Modifications")
		.addTranslation()
		.addKeyboard("Enter name of vehicle", 30, Save));

	addOption(BreakOption("Vehicles")
		.addTranslation());
}

void VehicleSaveLoadMenu::Update() {}

/*Called once on submenu open*/
void VehicleSaveLoadMenu::UpdateOnce() {
	ClearOptionsOffset(2);

	auto Vehicles = Utils::GetConfig()->GetFilesInDirectory(Utils::GetConfig()->GetLSCPath(), ".json");
	if (Vehicles.size()) {
		for (auto& Item : Vehicles) {
			addOption(ButtonOption(Item)
				.addOnClick([=] { Load(Item.c_str()); }));
		}
	} else {
		addOption(ButtonOption(getString("~m~None")));
	}
}

/*Called always*/
void VehicleSaveLoadMenu::FeatureUpdate() {}

VehicleSaveLoadMenu* _instance;
VehicleSaveLoadMenu* VehicleSaveLoadMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleSaveLoadMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleSaveLoadMenu::~VehicleSaveLoadMenu() { delete _instance; }