#include "vehicle_editor_bennys_interior.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_editor.hpp"
#include "vehicle_editor_lsc.hpp"

using namespace VehicleEditorBennysInteriorMenuVars;

namespace VehicleEditorBennysInteriorMenuVars {
	Vars_ m_Vars;
}

void VehicleEditorBennysInteriorMenu::Init() {
	SetName("Benny's");
	SetParentSubmenu<VehicleEditorMenu>();

	addString("Audio Install");
	addString("Dash");
	addString("Dials");
	addString("Doors");
	addString("Ornaments");
	addString("Seats");
	addString("Shifter");
	addString("Steering Wheels");
}

void VehicleEditorBennysInteriorMenu::Update() {
	if (!Menu::GetLocalPlayer().m_InVehicle) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}

	VehicleEditorMenuVars::Scripts::DrawVehicleStats(Menu::GetLocalPlayer().m_Vehicle);
}

/*Called once on submenu open*/
void VehicleEditorBennysInteriorMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	if (m_Vars.m_UpdateCache) {
		m_Vars.m_UpdateCache = false;

		Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_DASHBOARD, VehicleEditorMenuVars::m_Vars.m_CachedStats.m_BennysInteriorDesign, false);
		Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_DIAL, VehicleEditorMenuVars::m_Vars.m_CachedStats.m_BennysDials, false);
		Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_DOOR_SPEAKER, VehicleEditorMenuVars::m_Vars.m_CachedStats.m_BennysDoors, false);
		Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SEATS, VehicleEditorMenuVars::m_Vars.m_CachedStats.m_BennysSeats, false);
		Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_STEERINGWHEEL, VehicleEditorMenuVars::m_Vars.m_CachedStats.m_BennysSteeringWheel, false);
		Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ORNAMENTS, VehicleEditorMenuVars::m_Vars.m_CachedStats.m_BennysOrnaments, false);
		Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SHIFTER_LEAVERS, VehicleEditorMenuVars::m_Vars.m_CachedStats.m_BennysShifter, false);
		Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SPEAKERS, VehicleEditorMenuVars::m_Vars.m_CachedStats.m_BennysAudioInstall, false);
	}

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_SPEAKERS)) {
		addOption(SubmenuOption(VehicleEditorMenuVars::GetVehicleModName(MOD_SPEAKERS, getString("Audio Install")))
			.addSubmenu<VehicleEditorLSCMenu>()
			.addOnClick([] {
			VehicleEditorMenuVars::CacheVehicleStats();
			VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_SPEAKERS;
			VehicleEditorLSCMenuVars::m_Vars.m_Value = &VehicleEditorMenuVars::m_Vars.m_CachedStats.m_BennysAudioInstall;
		}));
	}

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_DASHBOARD)) {
		addOption(SubmenuOption(VehicleEditorMenuVars::GetVehicleModName(MOD_DASHBOARD, getString("Dash")))
			.addSubmenu<VehicleEditorLSCMenu>()
			.addOnClick([] {
				VehicleEditorMenuVars::CacheVehicleStats();
				VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_DASHBOARD;
				VehicleEditorLSCMenuVars::m_Vars.m_Value = &VehicleEditorMenuVars::m_Vars.m_CachedStats.m_BennysInteriorDesign;
		}));
	}

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_DIAL)) {
		addOption(SubmenuOption(VehicleEditorMenuVars::GetVehicleModName(MOD_DIAL, getString("Dials")))
			.addSubmenu<VehicleEditorLSCMenu>()
			.addOnClick([] {
				VehicleEditorMenuVars::CacheVehicleStats();
				VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_DIAL;
				VehicleEditorLSCMenuVars::m_Vars.m_Value = &VehicleEditorMenuVars::m_Vars.m_CachedStats.m_BennysDials;
			}));
	}

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_DOOR_SPEAKER)) {
		addOption(SubmenuOption(VehicleEditorMenuVars::GetVehicleModName(MOD_DOOR_SPEAKER, getString("Doors")))
			.addSubmenu<VehicleEditorLSCMenu>()
			.addOnClick([] {
				VehicleEditorMenuVars::CacheVehicleStats();
				VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_DOOR_SPEAKER;
				VehicleEditorLSCMenuVars::m_Vars.m_Value = &VehicleEditorMenuVars::m_Vars.m_CachedStats.m_BennysDoors;
			}));
	}

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_ORNAMENTS)) {
		addOption(SubmenuOption(VehicleEditorMenuVars::GetVehicleModName(MOD_ORNAMENTS, getString("Ornaments")))
			.addSubmenu<VehicleEditorLSCMenu>()
			.addOnClick([] {
			VehicleEditorMenuVars::CacheVehicleStats();
			VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_ORNAMENTS;
			VehicleEditorLSCMenuVars::m_Vars.m_Value = &VehicleEditorMenuVars::m_Vars.m_CachedStats.m_BennysOrnaments;
		}));
	}

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_SEATS)) {
		addOption(SubmenuOption(VehicleEditorMenuVars::GetVehicleModName(MOD_SEATS, getString("Seats")))
			.addSubmenu<VehicleEditorLSCMenu>()
			.addOnClick([] {
				VehicleEditorMenuVars::CacheVehicleStats();
				VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_SEATS;
				VehicleEditorLSCMenuVars::m_Vars.m_Value = &VehicleEditorMenuVars::m_Vars.m_CachedStats.m_BennysSeats;
			}));
	}

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_SHIFTER_LEAVERS)) {
		addOption(SubmenuOption(VehicleEditorMenuVars::GetVehicleModName(MOD_SHIFTER_LEAVERS, getString("Shifter")))
			.addSubmenu<VehicleEditorLSCMenu>()
			.addOnClick([] {
			VehicleEditorMenuVars::CacheVehicleStats();
			VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_SHIFTER_LEAVERS;
			VehicleEditorLSCMenuVars::m_Vars.m_Value = &VehicleEditorMenuVars::m_Vars.m_CachedStats.m_BennysShifter;
		}));
	}

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_STEERINGWHEEL)) {
		addOption(SubmenuOption(VehicleEditorMenuVars::GetVehicleModName(MOD_STEERINGWHEEL, getString("Steering Wheels")))
			.addSubmenu<VehicleEditorLSCMenu>()
			.addOnClick([] {
				VehicleEditorMenuVars::CacheVehicleStats();
				VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_STEERINGWHEEL;
				VehicleEditorLSCMenuVars::m_Vars.m_Value = &VehicleEditorMenuVars::m_Vars.m_CachedStats.m_BennysSteeringWheel;
			}));
	}
}

/*Called always*/
void VehicleEditorBennysInteriorMenu::FeatureUpdate() {}

VehicleEditorBennysInteriorMenu* _instance;
VehicleEditorBennysInteriorMenu* VehicleEditorBennysInteriorMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleEditorBennysInteriorMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleEditorBennysInteriorMenu::~VehicleEditorBennysInteriorMenu() { delete _instance; }