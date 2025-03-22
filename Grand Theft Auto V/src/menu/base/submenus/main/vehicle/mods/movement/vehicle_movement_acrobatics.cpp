#include "vehicle_movement_acrobatics.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_movement.hpp"
#include "global/arrays.hpp"

using namespace VehicleModsMovementAcrobaticsMenuVars;

namespace VehicleModsMovementAcrobaticsMenuVars {
	Vars_ m_Vars;
}

void VehicleModsMovementAcrobaticsMenu::Init() {
	SetName("Acrobatics");
	SetParentSubmenu<VehicleModsMovementMenu>();

	for (auto& Acrobat : Global::Arrays::g_Acrobatics) {
		addOption(ButtonOption(Acrobat.m_Name.Get())
			.addTranslation().addHotkey()
			.addOnClick([=] { VehicleModsMovementMenuVars::RunAcrobatic(Menu::GetLocalPlayer().m_Vehicle, Acrobat.m_Result); }));
	}
}

void VehicleModsMovementAcrobaticsMenu::Update() {}

/*Called once on submenu open*/
void VehicleModsMovementAcrobaticsMenu::UpdateOnce() {}

/*Called always*/
void VehicleModsMovementAcrobaticsMenu::FeatureUpdate() {}

VehicleModsMovementAcrobaticsMenu* _instance;
VehicleModsMovementAcrobaticsMenu* VehicleModsMovementAcrobaticsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleModsMovementAcrobaticsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleModsMovementAcrobaticsMenu::~VehicleModsMovementAcrobaticsMenu() { delete _instance; }