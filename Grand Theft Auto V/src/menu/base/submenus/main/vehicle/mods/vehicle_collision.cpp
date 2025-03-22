#include "vehicle_collision.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_mods.hpp"
#include "menu/base/util/pools/pool_manager.hpp"

using namespace VehicleModsCollisionMenuVars;

namespace VehicleModsCollisionMenuVars {
	Vars_ m_Vars;
}

void VehicleModsCollisionMenu::Init() {
	SetName("Collision");
	SetParentSubmenu<VehicleModsMenu>();

	addOption(ToggleOption("Disable Full Collision")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DisableFullCollision)
		.addOnClick([] { if (!m_Vars.m_DisableFullCollision) if(Menu::GetLocalPlayer().m_InVehicle) Native::SetEntityCollision(Menu::GetLocalPlayer().m_Vehicle, true, true);  })
		.canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Disable Vehicle Collision")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DisableVehicleCollision).canBeSaved(m_ParentNameStack));
}

void VehicleModsCollisionMenu::Update() {}

/*Called once on submenu open*/
void VehicleModsCollisionMenu::UpdateOnce() {}

/*Called always*/
void VehicleModsCollisionMenu::FeatureUpdate() {
	if (Menu::GetLocalPlayer().m_InVehicle) {
		if (m_Vars.m_DisableFullCollision) {
			Native::SetEntityCollision(Menu::GetLocalPlayer().m_Vehicle, !m_Vars.m_DisableFullCollision, true);
		}

		if (m_Vars.m_DisableVehicleCollision) {
			Menu::Pools::GetVehiclePool()->Run([] (Vehicle Veh) {
				Native::SetEntityNoCollisionEntity(Veh, Menu::GetLocalPlayer().m_Vehicle, true);
			});
		}
	}
}

VehicleModsCollisionMenu* _instance;
VehicleModsCollisionMenu* VehicleModsCollisionMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleModsCollisionMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleModsCollisionMenu::~VehicleModsCollisionMenu() { delete _instance; }