#include "vehicle_multipliers.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_mods.hpp"
#include "rage/engine.hpp"

using namespace VehicleModsMultipliersMenuVars;

namespace VehicleModsMultipliersMenuVars {
	Vars_ m_Vars;

	void CacheTraction() {
		if (Menu::GetLocalPlayer().m_InVehicle) {
			auto Factory = Global::Vars::g_PedFactory;
			if (Factory) {
				auto Ped = Factory->m_LocalPed;
				if (Ped) {
					auto Vehicle = Ped->m_Vehicle;
					if (Vehicle) {
						auto Handling = Vehicle->m_HandlingData;
						if (Handling) {
							m_Vars.m_TractionCurveMax = Handling->m_TractionCurveMax;
							m_Vars.m_CachedTraction = true;
							return;
						}
					}
				}
			}
		}

		m_Vars.m_CachedTraction = false;
	}

	void SetTraction() {
		if (Menu::GetLocalPlayer().m_InVehicle) {
			auto Factory = Global::Vars::g_PedFactory;
			if (Factory) {
				auto Ped = Factory->m_LocalPed;
				if (Ped) {
					auto Vehicle = Ped->m_Vehicle;
					if (Vehicle) {
						auto Handling = Vehicle->m_HandlingData;
						if (Handling) {
							Handling->m_TractionCurveMax = m_Vars.m_TractionCurveMax;
						}
					}
				}
			}
		}
	}
}

void VehicleModsMultipliersMenu::Init() {
	SetName("Multipliers");
	SetParentSubmenu<VehicleModsMenu>();

	addOption(NumberOption<float>(SCROLL, "Power (RPM)")
		.addTranslation().addHotkey()
		.addNumber(m_Vars.m_RPM, "%.0f", 1.f).addMin(1.f).addMax(10000.f).showMax().setScrollSpeed(10)
		.addOnClick([] {
			if (Menu::GetLocalPlayer().m_InVehicle) {
				Native::_SetVehicleEnginePowerMultiplier(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_RPM);
			}
		}));

	addOption(NumberOption<float>(SCROLL, "Torque")
		.addTranslation().addHotkey()
		.addNumber(m_Vars.m_Torque, "%.0f", 1.f).addMin(1.f).addMax(10000.f).showMax().setScrollSpeed(10));

	addOption(NumberOption<float>(SCROLL, "Headlight")
		.addTranslation().addHotkey()
		.addNumber(m_Vars.m_Headlight, "%.0f", 1.f).addMin(1.f).addMax(150.f).showMax().setScrollSpeed(10));

	addOption(NumberOption<float>(SCROLL, "Traction")
		.addTranslation()
		.addNumber(m_Vars.m_TractionCurveMax, "%.1f", 0.1f).addMin(0.f).addMax(1000.f).showMax().setScrollSpeed(10)
		.addOnClick(SetTraction)
		.addRequirement([] { return m_Vars.m_CachedTraction; }));
}

void VehicleModsMultipliersMenu::Update() {}

/*Called once on submenu open*/
void VehicleModsMultipliersMenu::UpdateOnce() {
	CacheTraction();
}

/*Called always*/
void VehicleModsMultipliersMenu::FeatureUpdate() {
	if (Menu::GetLocalPlayer().m_InVehicle) {
		if (m_Vars.m_Torque != 1.f) {
			uint64_t Address = Rage::Engine::GetEntityAddress(Menu::GetLocalPlayer().m_Vehicle);
			if (Address) {
				*(float*)(Address + 0x1360) = m_Vars.m_Torque;
			}
		}

		if (m_Vars.m_Headlight != 1.f) {
			Native::SetVehicleLightMultiplier(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_Headlight);
		}
	}
}

VehicleModsMultipliersMenu* _instance;
VehicleModsMultipliersMenu* VehicleModsMultipliersMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleModsMultipliersMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleModsMultipliersMenu::~VehicleModsMultipliersMenu() { delete _instance; }