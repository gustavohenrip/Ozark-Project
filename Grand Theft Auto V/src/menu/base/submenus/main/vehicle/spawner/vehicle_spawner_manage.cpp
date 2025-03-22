#include "vehicle_spawner_manage.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_spawner.hpp"
#include "utils/va.hpp"
#include "vehicle_spawner_edit.hpp"

using namespace VehicleSpawnerManageMenuVars;

namespace VehicleSpawnerManageMenuVars {
	Vars_ m_Vars;
}

void VehicleSpawnerManageMenu::Init() {
	SetName("Manage");
	SetParentSubmenu<VehicleSpawnerMenu>();

	addOption(SubmenuOption("All Vehicles")
		.addTranslation()
		.addOnClick([] { VehicleSpawnerEditMenuVars::m_Vars.m_EditingAll = true; })
		.addSubmenu<VehicleSpawnerEditMenu>());

	addOption(ButtonOption("Clear Manager")
		.addTranslation()
		.addOnClick([this] { m_Vars.m_SpawnedVehicles.clear(); UpdateOnce(); })
		.addTooltip("Does NOT delete the vehicles from the world, only from the manager"));

	addOption(BreakOption("Vehicles")
		.addTranslation());

	addString("~m~None");
}

void VehicleSpawnerManageMenu::Update() {}

/*Called once on submenu open*/
void VehicleSpawnerManageMenu::UpdateOnce() {
	ClearOptionsOffset(3);

	for (std::size_t i = 0; i < m_Vars.m_SpawnedVehicles.size(); i++) {
		if (!Native::DoesEntityExist(m_Vars.m_SpawnedVehicles[i].m_Vehicle)) {
			m_Vars.m_SpawnedVehicles.erase(m_Vars.m_SpawnedVehicles.begin() + i);
		}
	}
	
	if (!m_Vars.m_SpawnedVehicles.empty()) {
		for (std::size_t i = 0; i < m_Vars.m_SpawnedVehicles.size(); i++) {
			addOption(SubmenuOption(Utils::VA::VA("[%i] %s", i + 1, m_Vars.m_SpawnedVehicles[i].m_Name.c_str()))
				.addOnClick([=] {
					VehicleSpawnerEditMenuVars::m_Vars.m_EditingAll = false;
					VehicleSpawnerEditMenuVars::m_Vars.m_SelectedVehicle = i;
				})
				.addSubmenu<VehicleSpawnerEditMenu>());
		}
	} else {
		addOption(ButtonOption(getString("~m~None")));
	}
}

/*Called always*/
void VehicleSpawnerManageMenu::FeatureUpdate() {}

VehicleSpawnerManageMenu* _instance;
VehicleSpawnerManageMenu* VehicleSpawnerManageMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleSpawnerManageMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleSpawnerManageMenu::~VehicleSpawnerManageMenu() { delete _instance; }