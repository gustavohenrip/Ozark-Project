#include "vehicle_modifiers.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_mods.hpp"
#include "rage/engine.hpp"
#include "vehicle_handling.hpp"

using namespace VehicleModsModifiersMenuVars;

namespace VehicleModsModifiersMenuVars {
	Vars_ m_Vars;

	void SetSuspension() {
		if (Menu::GetLocalPlayer().m_InVehicle) {
			auto Address = Rage::Engine::GetEntityAddress(Menu::GetLocalPlayer().m_Vehicle);
			if (Address) {
				if (*(uint32_t*)(Address + 0xBE8) <= 10) {
					*(float*)(Address + 0x19DC) = -m_Vars.m_Suspension;
				}
			}
		}
	}

	void SetTireScale() {
		if (Global::Vars::g_PedFactory) {
			if (Global::Vars::g_PedFactory->m_LocalPed) {
				if (Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle) {
					auto C = Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle;
					if (C->m_VehicleDrawHandler) {
						if (C->m_VehicleDrawHandler->m_VehicleStreamRender) {
							C->m_VehicleDrawHandler->m_VehicleStreamRender->m_TireSize = m_Vars.m_TireSize;
						}
					}
				}
			}
		}
	}

	void SetTireWidth() {
		if (Global::Vars::g_PedFactory) {
			if (Global::Vars::g_PedFactory->m_LocalPed) {
				if (Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle) {
					auto C = Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle;
					if (C->m_VehicleDrawHandler) {
						if (C->m_VehicleDrawHandler->m_VehicleStreamRender) {
							C->m_VehicleDrawHandler->m_VehicleStreamRender->m_TireWidth = m_Vars.m_TireWidth;
						}
					}
				}
			}
		}
	}
}

void VehicleModsModifiersMenu::Init() {
	SetName("Modifiers");
	SetParentSubmenu<VehicleModsMenu>();

	addString("Wheel Scale");
	addString("Wheel Width");
	addString("Vehicle Handling");

	addOption(SubmenuOption(getString("Vehicle Handling"))
		.addSubmenu<VehicleModsHandlingMenu>()
		.addRequirement([] { return Menu::GetLocalPlayer().m_InVehicle; }));

	addOption(NumberOption<float>(SCROLL, "Advanced Suspension")
		.addTranslation()
		.addNumber(m_Vars.m_Suspension, "%.2f", 0.01f).addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax()
		.addOnClick(SetSuspension)
		.addRequirement([] { return m_Vars.m_HasSuspension; }));

	addOption(NumberOption<float>(SCROLL, getString("Wheel Scale"))
		.addNumber(m_Vars.m_TireSize, "%.2f", 0.01f).addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax()
		.addOnClick(SetTireScale)
		.addRequirement([] { return m_Vars.m_HasCustomWheels; }));

	addOption(NumberOption<float>(SCROLL, getString("Wheel Width"))
		.addNumber(m_Vars.m_TireWidth, "%.2f", 0.01f).addMin(-100.f).addMax(100.f).setScrollSpeed(10).showMax()
		.addOnClick(SetTireWidth)
		.addRequirement([] { return m_Vars.m_HasCustomWheels; }));

	// Greyed out
	addOption(ButtonOption("~c~" + getString("Vehicle Handling"))
		.addRequirement([] { return !Menu::GetLocalPlayer().m_InVehicle; }));

	addOption(ButtonOption("~c~" + getString("Wheel Scale"))
		.addRequirement([] { return !m_Vars.m_HasCustomWheels; })
		.addTooltip("You need custom tires to edit wheels"));

	addOption(ButtonOption("~c~" + getString("Wheel Width"))
		.addRequirement([] { return !m_Vars.m_HasCustomWheels; })
		.addTooltip("You need custom tires to edit wheels"));
}

void VehicleModsModifiersMenu::Update() {
	m_Vars.m_HasSuspension = false;
	m_Vars.m_HasCustomWheels = false;

	if (Menu::GetLocalPlayer().m_InVehicle) {
		auto Address = Rage::Engine::GetEntityAddress(Menu::GetLocalPlayer().m_Vehicle);
		if (Address) {
			if (*(uint32_t*)(Address + 0xBE8) <= 10) {
				m_Vars.m_HasSuspension = true;
				m_Vars.m_Suspension = -*(float*)(Address + 0x19DC);
			}

			if (Global::Vars::g_PedFactory) {
				if (Global::Vars::g_PedFactory->m_LocalPed) {
					if (Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle) {
						auto C = Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle;
						if (C->m_VehicleDrawHandler) {
							if (C->m_VehicleDrawHandler->m_VehicleStreamRender) {
								m_Vars.m_HasCustomWheels = true;
								m_Vars.m_TireSize = C->m_VehicleDrawHandler->m_VehicleStreamRender->m_TireSize;
								m_Vars.m_TireWidth = C->m_VehicleDrawHandler->m_VehicleStreamRender->m_TireWidth;
							}
						}
					}
				}
			}
		}
	}
}

/*Called once on submenu open*/
void VehicleModsModifiersMenu::UpdateOnce() {}

/*Called always*/
void VehicleModsModifiersMenu::FeatureUpdate() {}

VehicleModsModifiersMenu* _instance;
VehicleModsModifiersMenu* VehicleModsModifiersMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleModsModifiersMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleModsModifiersMenu::~VehicleModsModifiersMenu() { delete _instance; }