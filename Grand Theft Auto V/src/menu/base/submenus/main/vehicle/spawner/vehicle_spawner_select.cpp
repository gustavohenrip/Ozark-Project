#include "vehicle_spawner_select.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_spawner.hpp"
#include "rage/engine.hpp"
#include "utils/log.hpp"
#include "utils/fiber_pool.hpp"
#include "utils/fibers.hpp"
#include "menu/base/util/panels.hpp"

using namespace VehicleSpawnerSelectMenuVars;

namespace VehicleSpawnerSelectMenuVars {
	Vars_ m_Vars;

	std::string TextureList[] = {
		"candc_apartments",
		"candc_assault",
		"candc_battle",
		"candc_casinoheist",
		"candc_chopper",
		"candc_default",
		"candc_executive1",
		"candc_gunrunning",
		"candc_hacker",
		"candc_importexport",
		"candc_smuggler",
		"candc_truck",
		"candc_xmas2017",
		"casino_suites",
		"dock_default",
		"dock_dlc_banner",
		"dock_dlc_color",
		"dock_dlc_executive1",
		"dock_dlc_fittings",
		"dock_dlc_flag",
		"dock_dlc_lights",
		"dock_dlc_model",
		"dock_dlc_slides",
		"dock_yacht_backgrounds",
		"elt_default",
		"elt_dlc_apartments",
		"elt_dlc_assault",
		"elt_dlc_battle",
		"elt_dlc_business",
		"elt_dlc_executive1",
		"elt_dlc_luxe",
		"elt_dlc_pilot",
		"elt_dlc_smuggler",
		"lgm_default",
		"lgm_dlc_apartments",
		"lgm_dlc_arena",
		"lgm_dlc_assault",
		"lgm_dlc_battle",
		"lgm_dlc_biker",
		"lgm_dlc_business",
		"lgm_dlc_business2",
		"lgm_dlc_casinoheist",
		"lgm_dlc_executive1",
		"lgm_dlc_gunrunning",
		"lgm_dlc_heist",
		"lgm_dlc_importexport",
		"lgm_dlc_lts_creator",
		"lgm_dlc_luxe",
		"lgm_dlc_pilot",
		"lgm_dlc_smuggler",
		"lgm_dlc_specialraces",
		"lgm_dlc_stunt",
		"lgm_dlc_summer2020",
		"lgm_dlc_valentines",
		"lgm_dlc_valentines2",
		"lgm_dlc_vinewood",
		"lgm_dlc_xmas2017",
		"lsc_default",
		"lsc_dlc_import_export",
		"lsc_dlc_summer2020",
		"lsc_jan2016",
		"lsc_lowrider2",
		"mba_vehicles",
		"pandm_default",
		"sssa_default",
		"sssa_dlc_arena",
		"sssa_dlc_assault",
		"sssa_dlc_battle",
		"sssa_dlc_biker",
		"sssa_dlc_business",
		"sssa_dlc_business2",
		"sssa_dlc_casinoheist",
		"sssa_dlc_christmas_2",
		"sssa_dlc_christmas_3",
		"sssa_dlc_executive_1",
		"sssa_dlc_halloween",
		"sssa_dlc_heist",
		"sssa_dlc_hipster",
		"sssa_dlc_independence",
		"sssa_dlc_lts_creator",
		"sssa_dlc_mp_to_sp",
		"sssa_dlc_smuggler",
		"sssa_dlc_stunt",
		"sssa_dlc_summer2020",
		"sssa_dlc_valentines",
		"sssa_dlc_vinewood",
		"sssa_dlc_xmas2017"
	};

	void PopulateTextures() {
		LOG_INFO("Populating vehicle textures");

		Utils::GetFiberPool()->Push([=] {
			for (auto TextureName : TextureList) {
				bool Loaded = true;

				while (!Native::HasStreamedTextureDictLoaded(TextureName.c_str())) {
					Loaded = false;
					Native::RequestStreamedTextureDict(TextureName.c_str(), true);
					Utils::WaitFiber(100);
				}

				Utils::WaitFiber(250);

				Rage::Types::TxdStore* Txd = Global::Vars::g_TextureStore;
				if (IsValidPtr(Txd) && IsValidPtr(Txd->m_Pool) && IsValidPtr(Txd->m_Mask)) {
					Rage::Types::DictionaryPool* Pool = Txd->m_Pool;
					for (uint32_t i = 0; i < Txd->m_Count; i++) {
						if (~(Txd->m_Mask[i] >> 7) & 1) {
							Rage::Types::DictionaryPool Element = Pool[i];
							if (IsValidPtr(Element.m_Pool)) {
								if (Element.m_DictionaryHash == Native::GetHashKey(TextureName.c_str())) {
									for (short j = 0; j < Element.m_Pool->m_Count; j++) {
										Rage::Types::DictionaryItem* Item = Element.m_Pool->m_Items[j];
										if (IsValidPtr(Item)) {
											if (IsValidPtr(Item->m_Name)) {
												// don't process the bullshit bigman bossman im fucking gay
												if (!strstr(Item->m_Name, "_")) {
													m_Vars.m_Textures[Native::GetHashKey(Item->m_Name)] = TextureName;
												}
											}
										}
									}
								}
							}
						}
					}
				}

				if (!Loaded) {
					Native::SetStreamedTextureDictAsNoLongerNeeded(TextureName.c_str());
				}
			}

			LOG_SUCCESS("Processed %i vehicle texture entries", m_Vars.m_Textures.size());
		});
	}

	std::pair<std::pair<std::string, std::string>, Rage::Types::VehicleModelInfo*> GetVehicleModelTexture(uint32_t Model) {
		Rage::Types::VehicleModelInfo* Info = (Rage::Types::VehicleModelInfo*)Rage::Engine::GetModelInfo(Model);
		if (Info) {
			auto Vit = std::find_if(m_Vars.m_Textures.begin(), m_Vars.m_Textures.end(), [=] (std::pair<uint32_t, std::string> Element) {
				return Element.first == Model;
			});

			if (Vit != m_Vars.m_Textures.end()) {
				return { std::make_pair(Vit->second, Info->m_ModelName), Info };
			}

			// try again for 2
			std::string Name = std::string(Info->m_ModelName);
			Name += "2";

			Vit = std::find_if(m_Vars.m_Textures.begin(), m_Vars.m_Textures.end(), [=] (std::pair<uint32_t, std::string> Element) {
				return Element.first == Native::GetHashKey(Name.c_str());
			});

			if (Vit != m_Vars.m_Textures.end()) {
				return { std::make_pair(Vit->second, Name), Info };
			}
		}

		return { std::make_pair("", ""), nullptr };
	}

	void Preview(uint32_t Model) {
		Menu::Panels::PanelCustomVehiclePreview Data;

		auto Texture = GetVehicleModelTexture(Model);
		if (!Texture.first.first.empty()) {
			strcpy(Data.m_Dict, Texture.first.first.c_str());
			strcpy(Data.m_Name, Texture.first.second.c_str());
		}

		Data.m_Model = Model;
		Data.m_MaxSpeed = Native::_GetVehicleModelMaxSpeed(Model) * Global::Vars::m_SelectedMetricSystem.first;
		Data.m_Seats = Native::GetVehicleModelNumberOfSeats(Model);
		Data.m_MaxBraking = Native::GetVehicleModelMaxBraking(Model);
		Data.m_MaxTraction = Native::GetVehicleModelMaxTraction(Model);

		if (Texture.second) {
			strcpy(Data.m_Manufacturer, Texture.second->m_ManufacturerName);
		} else {
			Menu::Panels::GetPanelManager()->TogglePanelRender("PANEL_VEHICLE_PREVIEW", false);
			return;
		}

		Menu::Panels::GetPanelManager()->SetStructure(Menu::Panels::GetPanelManager()->GetChild(Menu::Panels::GetPanelManager()->GetParent("PANEL_VEHICLE_PREVIEW"), "PANEL_VEHICLE_PREVIEW_INFO"), &Data, sizeof(Data));
		Menu::Panels::GetPanelManager()->TogglePanelRender("PANEL_VEHICLE_PREVIEW", true);
	}
}

void VehicleSpawnerSelectMenu::Init() {
	SetName("Vehicle Spawner");
	SetParentSubmenu<VehicleSpawnerMenu>();

	PopulateTextures();
}

void VehicleSpawnerSelectMenu::Update() {}

/*Called once on submenu open*/
void VehicleSpawnerSelectMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	struct FoundVehicles {
		std::string m_Name;
		int m_Count;
		uint32_t m_Hash;
	};

	std::vector<FoundVehicles> Vehicles;

	for (auto& Hash : VehicleSpawnerMenuVars::m_Vars.m_VehicleHashes[m_Vars.m_Selected]) {
		auto DisplayName = Native::GetDisplayNameFromVehicleModel(Hash);
		if (DisplayName) {
			auto Label = Native::_GetLabelText(DisplayName);
			if (Label) {
				Vehicles.push_back({ Label, 0, Hash });
			}
		}
	}

	std::vector<std::pair<std::string, int>> FoundNames;
	for (auto& Vehicle : Vehicles) {
		auto& Vit = std::find_if(FoundNames.begin(), FoundNames.end(), [=] (std::pair<std::string, int >& Element) {
			return Element.first == Vehicle.m_Name;
		});

		if (Vit == FoundNames.end()) {
			FoundNames.push_back({ Vehicle.m_Name, 0 });
		} else {
			Vit->second++;
			Vehicle.m_Name += " " + std::to_string(Vit->second + 1);
		}
	}

	for (auto& Vehicle : Vehicles) {
		addOption(ButtonOption(Vehicle.m_Name)
			.addOnHover([=] { Preview(Vehicle.m_Hash); })
			.addOnClick([=] { VehicleSpawnerMenuVars::SpawnVehicle(Vehicle.m_Hash); }));
	}
}

/*Called always*/
void VehicleSpawnerSelectMenu::FeatureUpdate() {}

VehicleSpawnerSelectMenu* _instance;
VehicleSpawnerSelectMenu* VehicleSpawnerSelectMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleSpawnerSelectMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleSpawnerSelectMenu::~VehicleSpawnerSelectMenu() { delete _instance; }