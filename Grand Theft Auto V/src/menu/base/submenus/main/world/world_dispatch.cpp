#include "world_dispatch.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../world.hpp"

using namespace WorldDispatchMenuVars;

namespace WorldDispatchMenuVars {
	Vars_ m_Vars;

	bool IsToggled(eDispatchType Service) {
		if (Global::Vars::g_DispatchServiceTable) {
			auto Table = *(uint64_t*)(Global::Vars::g_DispatchServiceTable + (Service * 8));
			if (Table) {
				return *(bool*)(Table + 0x20);
			}
		}

		return false;
	}

	void SetToggle(eDispatchType Service, bool Toggle) {
		if (Global::Vars::g_DispatchServiceTable) {
			auto Table = *(uint64_t*)(Global::Vars::g_DispatchServiceTable + (Service * 8));
			if (Table) {
				*(bool*)(Table + 0x20) = !Toggle;
			}
		}
	}
}

void WorldDispatchMenu::Init() {
	SetName("Dispatch");
	SetParentSubmenu<WorldMenu>();

	addOption(ToggleOption("Disable Police Automobile")
		.addTranslation()
		.addToggle(m_Vars.m_DisablePoliceAutomobile)
		.addOnClick([] { SetToggle(DT_PoliceAutomobile, m_Vars.m_DisablePoliceAutomobile); }));

	addOption(ToggleOption("Disable Police Helicopter")
		.addTranslation()
		.addToggle(m_Vars.m_DisablePoliceHelicopter)
		.addOnClick([] { SetToggle(DT_PoliceHelicopter, m_Vars.m_DisablePoliceHelicopter); }));

	addOption(ToggleOption("Disable Police Riders")
		.addTranslation()
		.addToggle(m_Vars.m_DisablePoliceRiders)
		.addOnClick([] { SetToggle(DT_PoliceRiders, m_Vars.m_DisablePoliceRiders); }));

	addOption(ToggleOption("Disable Police Vehicle Request")
		.addTranslation()
		.addToggle(m_Vars.m_DisablePoliceVehicleRequest)
		.addOnClick([] { SetToggle(DT_PoliceVehicleRequest, m_Vars.m_DisablePoliceVehicleRequest); }));

	addOption(ToggleOption("Disable Police Road Block")
		.addTranslation()
		.addToggle(m_Vars.m_DisablePoliceRoadBlock)
		.addOnClick([] { SetToggle(DT_PoliceRoadBlock, m_Vars.m_DisablePoliceRoadBlock); }));

	addOption(ToggleOption("Disable Police Boat")
		.addTranslation()
		.addToggle(m_Vars.m_DisablePoliceBoat)
		.addOnClick([] { SetToggle(DT_PoliceBoat, m_Vars.m_DisablePoliceBoat); }));

	addOption(ToggleOption("Disable Swat Automobile")
		.addTranslation()
		.addToggle(m_Vars.m_DisableSwatAutomobile)
		.addOnClick([] { SetToggle(DT_SwatAutomobile, m_Vars.m_DisableSwatAutomobile); }));

	addOption(ToggleOption("Disable Swat Helicopter")
		.addTranslation()
		.addToggle(m_Vars.m_DisableSwatHelicopter)
		.addOnClick([] { SetToggle(DT_SwatHelicopter, m_Vars.m_DisableSwatHelicopter); }));

	addOption(ToggleOption("Disable Fire Department")
		.addTranslation()
		.addToggle(m_Vars.m_DisableFireDepartment)
		.addOnClick([] { SetToggle(DT_FireDepartment, m_Vars.m_DisableFireDepartment); }));

	addOption(ToggleOption("Disable Ambulance Department")
		.addTranslation()
		.addToggle(m_Vars.m_DisableAmbulanceDepartment)
		.addOnClick([] { SetToggle(DT_AmbulanceDepartment, m_Vars.m_DisableAmbulanceDepartment); }));

	addOption(ToggleOption("Disable Gangs")
		.addTranslation()
		.addToggle(m_Vars.m_DisableGangs)
		.addOnClick([] { SetToggle(DT_Gangs, m_Vars.m_DisableGangs); }));

	addOption(ToggleOption("Disable Army Vehicle")
		.addTranslation()
		.addToggle(m_Vars.m_DisableArmyVehicle)
		.addOnClick([] { SetToggle(DT_ArmyVehicle, m_Vars.m_DisableArmyVehicle); }));

	addOption(ToggleOption("Disable Bikers")
		.addTranslation()
		.addToggle(m_Vars.m_DisableBikers)
		.addOnClick([] { SetToggle(DT_BikerBackup, m_Vars.m_DisableBikers); }));
}

void WorldDispatchMenu::Update() {}

/*Called once on submenu open*/
void WorldDispatchMenu::UpdateOnce() {
	m_Vars.m_DisablePoliceAutomobile = !IsToggled(DT_PoliceAutomobile);
	m_Vars.m_DisablePoliceHelicopter = !IsToggled(DT_PoliceHelicopter);
	m_Vars.m_DisablePoliceRiders = !IsToggled(DT_PoliceRiders);
	m_Vars.m_DisablePoliceVehicleRequest = !IsToggled(DT_PoliceVehicleRequest);
	m_Vars.m_DisablePoliceRoadBlock = !IsToggled(DT_PoliceRoadBlock);
	m_Vars.m_DisablePoliceBoat = !IsToggled(DT_PoliceBoat);
	m_Vars.m_DisableSwatAutomobile = !IsToggled(DT_SwatAutomobile);
	m_Vars.m_DisableSwatHelicopter = !IsToggled(DT_SwatHelicopter);
	m_Vars.m_DisableFireDepartment = !IsToggled(DT_FireDepartment);
	m_Vars.m_DisableAmbulanceDepartment = !IsToggled(DT_AmbulanceDepartment);
	m_Vars.m_DisableGangs = !IsToggled(DT_Gangs);
	m_Vars.m_DisableArmyVehicle = !IsToggled(DT_ArmyVehicle);
	m_Vars.m_DisableBikers = !IsToggled(DT_BikerBackup);
}

/*Called always*/
void WorldDispatchMenu::FeatureUpdate() {}

WorldDispatchMenu* _instance;
WorldDispatchMenu* WorldDispatchMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WorldDispatchMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WorldDispatchMenu::~WorldDispatchMenu() { delete _instance; }