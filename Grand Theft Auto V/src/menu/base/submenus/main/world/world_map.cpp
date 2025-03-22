#include "world_map.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../teleport.hpp"
#include "menu/base/submenus/main/teleport.hpp"

using namespace WorldMapMenuVars;

namespace WorldMapMenuVars {
	Vars_ m_Vars;

	void RequestIPLSet(const char** ipls, int count) {
		for (int i = 0; i < count; i++) {
			if (!Native::IsIplActive(ipls[i])) {
				Native::RequestIpl(ipls[i]);
			}
		}
	}

	void RemoveIPLSet(const char** ipls, int count) {
		for (int i = 0; i < count; i++) {
			if (Native::IsIplActive(ipls[i])) {
				Native::RemoveIpl(ipls[i]);
			}
		}
	}
}

void WorldMapMenu::Init() {
	SetName("IPLs");
	SetParentSubmenu<TeleportMenu>();

	// Buildings
	
	addOption(ButtonOption("North Yankton")
		.addTranslation().addHotkey()
		.addOnClick([] {
			const char* Maps[] = { "plg_01", "prologue01", "prologue01_lod", "prologue01c", "prologue01c_lod", "prologue01d", "prologue01d_lod", "prologue01e", "prologue01e_lod", "prologue01f", "prologue01f_lod", "prologue01g", "prologue01h", "prologue01h_lod", "prologue01i", "prologue01i_lod", "prologue01j", "prologue01j_lod", "prologue01k", "prologue01k_lod", "prologue01z", "prologue01z_lod", "plg_02", "prologue02", "prologue02_lod", "plg_03", "prologue03", "prologue03_lod", "prologue03b", "prologue03b_lod", "prologue03_grv_dug", "prologue03_grv_dug_lod", "prologue_grv_torch", "plg_04", "prologue04", "prologue04_lod", "prologue04b", "prologue04b_lod", "prologue04_cover", "des_protree_end", "des_protree_start", "des_protree_start_lod", "plg_05", "prologue05", "prologue05_lod", "prologue05b", "prologue05b_lod", "plg_06", "prologue06", "prologue06_lod", "prologue06b", "prologue06b_lod", "prologue06_int", "prologue06_int_lod", "prologue06_pannel", "prologue06_pannel_lod", "prologue_m2_door", "prologue_m2_door_lod", "plg_occl_00", "prologue_occl", "plg_rd", "prologuerd", "prologuerdb", "prologuerd_lod" };
			RequestIPLSet(Maps, 64);
			TeleportMenuVars::Teleport({ 3360.19f, -4849.67f, 111.8f });
		}));

	addOption(ButtonOption("Carrier")
		.addTranslation().addHotkey()
		.addOnClick([] {
			const char* Maps[] = { "hei_carrier", "hei_carrier_DistantLights", "hei_Carrier_int1", "hei_Carrier_int2", "hei_Carrier_int3", "hei_Carrier_int4", "hei_Carrier_int5", "hei_Carrier_int6", "hei_carrier_LODLights" };
			RequestIPLSet(Maps, 9);
			TeleportMenuVars::Teleport({ 3016.46f, -4534.09f, 14.84f });
		}));

	addOption(ButtonOption("Heist Yacht")
		.addTranslation().addHotkey()
		.addOnClick([] {
			Native::RemoveIpl("smboat");
			const char* Maps[] = { "hei_yacht_heist", "hei_yacht_heist_Bar", "hei_yacht_heist_Bedrm", "hei_yacht_heist_Bridge", "hei_yacht_heist_DistantLights", "hei_yacht_heist_enginrm", "hei_yacht_heist_LODLights", "hei_yacht_heist_Lounge" };
			RequestIPLSet(Maps, 8);
			TeleportMenuVars::Teleport({ -2045.8f, -1031.2f, 11.9f });
		}));

	addOption(ButtonOption("Yacht")
		.addTranslation().addHotkey()
		.addOnClick([] {
			Native::RequestIpl("smboat");
			const char* Maps[] = { "hei_yacht_heist", "hei_yacht_heist_Bar", "hei_yacht_heist_Bedrm", "hei_yacht_heist_Bridge", "hei_yacht_heist_DistantLights", "hei_yacht_heist_enginrm", "hei_yacht_heist_LODLights", "hei_yacht_heist_Lounge" };
			RemoveIPLSet(Maps, 8);
			TeleportMenuVars::Teleport({ -2045.8f, -1031.2f, 11.9f });
		}));

	addOption(ButtonOption("Desert UFO")
		.addTranslation().addHotkey()
		.addOnClick([] {
			Native::RequestIpl("ufo");
			TeleportMenuVars::Teleport({ 2490.47729f, 3774.84351f, 2414.035f });
		}));

	addOption(ButtonOption("Fort Zancudo UFO")
		.addTranslation().addHotkey()
		.addOnClick([] {
			Native::RequestIpl("ufo");
			TeleportMenuVars::Teleport({ -2051.99463f, 3237.05835f, 1456.97021f });
		}));

	addOption(ButtonOption("Cluckin Bell")
		.addTranslation().addHotkey()
		.addOnClick([] {
			Native::RemoveIpl("CS1_02_cf_offmission");
			const char* Maps[] = { "CS1_02_cf_onmission1", "CS1_02_cf_onmission2", "CS1_02_cf_onmission3", "CS1_02_cf_onmission4" };
			RequestIPLSet(Maps, 4);
			TeleportMenuVars::Teleport({ -72.68752f, 6253.72656f, 31.08991f });
		}));

	addOption(ButtonOption("Jewelery Store")
		.addTranslation().addHotkey()
		.addOnClick([] {
			Native::RemoveIpl("jewel2fake");
			Native::RemoveIpl("bh1_16_refurb");
			Native::RequestIpl("post_hiest_unload");
			TeleportMenuVars::Teleport({ -630.4f, -236.7f, 40.0f });
		}));

	addOption(ButtonOption("Morgue")
		.addTranslation().addHotkey()
		.addOnClick([] {
			Native::RequestIpl("Coroner_Int_on");
			TeleportMenuVars::Teleport({ 244.9f, -1374.7f, 39.5f });
		}));

	addOption(ButtonOption("FIB Lobby")
		.addTranslation().addHotkey()
		.addOnClick([] {
			Native::RequestIpl("FIBlobby");
			TeleportMenuVars::Teleport({ 110.4f, -744.2f, 45.7f });
		}));

	addOption(ButtonOption("LifeInvader Lobby")
		.addTranslation().addHotkey()
		.addOnClick([] {
			Native::RequestIpl("facelobby");
			TeleportMenuVars::Teleport({ -1047.9f, -233.0f, 39.0f });
		}));

	addOption(ButtonOption("Destroyed Hospital")
		.addTranslation().addHotkey()
		.addOnClick([] {
			Native::RemoveIpl("RC12B_Default");
			Native::RemoveIpl("RC12B_Fixed");
			Native::RequestIpl("RC12B_Destroyed");
			Native::RequestIpl("RC12B_HospitalInterior");
			TeleportMenuVars::Teleport({ 356.8f, -590.1f, 43.3f });
		}));

	addOption(ButtonOption("Cargo Ship")
		.addTranslation().addHotkey()
		.addOnClick([] {
			Native::RequestIpl("cargoship");
			TeleportMenuVars::Teleport({ -90.0f, -2365.8f, 14.3f });
		}));

	addOption(ButtonOption("Train Crash")
		.addTranslation().addHotkey()
		.addOnClick([] {
			Native::RequestIpl("canyonriver01_traincrash");
			Native::RequestIpl("railing_end");
			Native::RemoveIpl("railing_start");
			Native::RemoveIpl("canyonriver01");
			TeleportMenuVars::Teleport({ -532.1309f, 4526.187f, 88.7955f });
		}));

	addOption(ButtonOption("Broken Stilt House")
		.addTranslation().addHotkey()
		.addOnClick([] {
			Native::RequestIpl("DES_StiltHouse_imapend");
			Native::RemoveIpl("DES_StiltHouse_imapstart");
			Native::RemoveIpl("des_stilthouse_rebuild");
			TeleportMenuVars::Teleport({ -1020.5f, 663.41f, 154.75f });
		}));

	addOption(ButtonOption("Plane Crash")
		.addTranslation().addHotkey()
		.addOnClick([] {
			Native::RequestIpl("Plane_crash_trench");
			TeleportMenuVars::Teleport({ 2814.7f, 4758.5f, 50.0f });
		}));
	}

void WorldMapMenu::Update() {}

/*Called once on submenu open*/
void WorldMapMenu::UpdateOnce() {}

/*Called always*/
void WorldMapMenu::FeatureUpdate() {}

WorldMapMenu* _instance;
WorldMapMenu* WorldMapMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WorldMapMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WorldMapMenu::~WorldMapMenu() { delete _instance; }