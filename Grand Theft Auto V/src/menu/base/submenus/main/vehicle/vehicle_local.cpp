#include "vehicle_local.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "global/arrays.hpp"
#include "spawner/vehicle_spawner_edit.hpp"

using namespace VehicleLocalMenuVars;

namespace VehicleLocalMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> Driver[] = {
		{ TranslationString("Kick", true), 0 },
		{ TranslationString("Kill", true), 1 }
	};

	ScrollStruct<float> Boost[] = {
		{ TranslationString("Forwards", true), 100.f },
		{ TranslationString("Backwards", true), -100.f }
	};
}

void VehicleLocalMenu::Init() {
	SetName("Local Vehicles");
	SetParentSubmenu<VehicleMenu>();

	addOption(SubmenuOption("ESP")
		.addTranslation()
		.addSubmenu<ESPMenu>()
		.addOnClick([] {
			ESPMenu::GetInstance()->SetParentSubmenu<VehicleLocalMenu>();
			ESPMenuVars::m_Vars.m_ESP = &m_Vars.m_ESP;
			ESPMenuVars::m_Vars.m_Player = false;
		}));

	addOption(ScrollOption<int>(SCROLLSELECT, "Driver")
		.addTranslation()
		.addOnClick([] { VehicleSpawnerEditMenuVars::Run(0, false, true, &m_Vars.m_Driver); })
		.addScroll(m_Vars.m_Driver, 0, NUMOF(Driver), Driver));

	addOption(ScrollOption<float>(SCROLLSELECT, "Boost")
		.addTranslation()
		.addOnClick([] { VehicleSpawnerEditMenuVars::Run(1, false, true, &m_Vars.m_Gravity); })
		.addScroll(m_Vars.m_Boost, 0, NUMOF(Boost), Boost));

	addOption(ScrollOption<std::pair<Math::Vector3_<float>, Math::Vector3_<float>>>(SCROLLSELECT, "Acrobatics")
		.addTranslation()
		.addOnClick([] { VehicleSpawnerEditMenuVars::Run(2, false, true, &m_Vars.m_Acrobatics); })
		.addScroll(m_Vars.m_Acrobatics, 0, NUMOF(Global::Arrays::g_Acrobatics), Global::Arrays::g_Acrobatics));

	addOption(ScrollOption<float>(SCROLL, "Gravity")
		.addTranslation()
		.addOnClick([] { VehicleSpawnerEditMenuVars::Run(3, false, true, &m_Vars.m_Gravity); })
		.addScroll(m_Vars.m_Gravity, 0, NUMOF(Global::Arrays::g_Gravity), Global::Arrays::g_Gravity));

	addOption(ButtonOption("Teleport to Me")
		.addTranslation()
		.addOnClick([] { VehicleSpawnerEditMenuVars::Run(4, false, true, &m_Vars.m_Teleport); }));

	addOption(ButtonOption("Fix")
		.addTranslation()
		.addOnClick([] { VehicleSpawnerEditMenuVars::Run(5, false, true, nullptr); }));

	addOption(ButtonOption("Empty Vehicle Seats")
		.addTranslation()
		.addOnClick([] { VehicleSpawnerEditMenuVars::Run(6, false, true, nullptr); }));

	addOption(ButtonOption("Random Paint")
		.addTranslation()
		.addOnClick([] { VehicleSpawnerEditMenuVars::Run(7, false, true, nullptr); }));

	addOption(ButtonOption("Honk Horn")
		.addTranslation()
		.addOnClick([] { VehicleSpawnerEditMenuVars::Run(8, false, true, nullptr); }));

	addOption(ButtonOption("Drive to Me")
		.addTranslation()
		.addOnClick([] { VehicleSpawnerEditMenuVars::Run(9, false, true, nullptr); }));

	addOption(ButtonOption("Explode")
		.addTranslation()
		.addOnClick([] { VehicleSpawnerEditMenuVars::Run(10, false, true, nullptr); }));

	addOption(ButtonOption("Delete")
		.addTranslation()
		.addOnClick([] {
			VehicleSpawnerEditMenuVars::Run(11, false, true, nullptr);
		}));
}

void VehicleLocalMenu::Update() {}

/*Called once on submenu open*/
void VehicleLocalMenu::UpdateOnce() {}

/*Called always*/
void VehicleLocalMenu::FeatureUpdate() {
	if (m_Vars.m_ESP.m_NameLineOfSightRainbowToggle
		|| m_Vars.m_ESP.m_NameRainbowToggle
		|| m_Vars.m_ESP.m_Name
		|| m_Vars.m_ESP.m_SnaplineLineOfSightRainbowToggle
		|| m_Vars.m_ESP.m_SnaplineRainbowToggle
		|| m_Vars.m_ESP.m_Snapline
		|| m_Vars.m_ESP.m_3DLineOfSightRainbowToggle
		|| m_Vars.m_ESP.m_3DRainbowToggle
		|| m_Vars.m_ESP.m_3D) {
		Menu::Pools::GetVehiclePool()->Run([](Vehicle Veh) {
			// Name ESP
			if (m_Vars.m_ESP.m_NameLineOfSightRainbowToggle) {
				m_Vars.m_ESP.m_NameLineOfSightRainbow.Run();
			}

			if (m_Vars.m_ESP.m_NameRainbowToggle) {
				m_Vars.m_ESP.m_NameRainbow.Run();
			}

			if (m_Vars.m_ESP.m_Name) {
				ESPMenuVars::NameESP(m_Vars.m_ESP, Veh, Native::_GetLabelText(Native::GetDisplayNameFromVehicleModel(Native::GetEntityModel(Veh))));
			}

			// Snapline ESP
			if (m_Vars.m_ESP.m_SnaplineLineOfSightRainbowToggle) {
				m_Vars.m_ESP.m_SnaplineLineOfSightRainbow.Run();
			}

			if (m_Vars.m_ESP.m_SnaplineRainbowToggle) {
				m_Vars.m_ESP.m_SnaplineRainbow.Run();
			}

			if (m_Vars.m_ESP.m_Snapline) {
				ESPMenuVars::SnaplineESP(m_Vars.m_ESP, Veh);
			}

			// 3D ESP
			if (m_Vars.m_ESP.m_3DLineOfSightRainbowToggle) {
				m_Vars.m_ESP.m_3DLineOfSightRainbow.Run();
			}

			if (m_Vars.m_ESP.m_3DRainbowToggle) {
				m_Vars.m_ESP.m_3DRainbow.Run();
			}

			if (m_Vars.m_ESP.m_3D) {
				ESPMenuVars::_3DESP(m_Vars.m_ESP, Veh);
			}
		});
	}
}

VehicleLocalMenu* _instance;
VehicleLocalMenu* VehicleLocalMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleLocalMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleLocalMenu::~VehicleLocalMenu() { delete _instance; }