#include "vehicle_autopilot.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_mods.hpp"
#include "menu/base/util/notify.hpp"

using namespace VehicleModsAutopilotMenuVars;

namespace VehicleModsAutopilotMenuVars {
	Vars_ m_Vars;

	ScrollStruct<Math::Vector3_<float>> Destinations[] = {
		{ TranslationString("Wander", true), { 0.f, 0.f, 0.f } },
		{ TranslationString("Waypoint", true), { 0.f, 0.f, 0.f } },
		{ TranslationString("Eclipse Towers", true), { -775.0500f, 312.32f, 85.7f } },
		{ TranslationString("Los Santos Airport", true), { -1102.2910f, -2894.5160f, 13.9467f } },
		{ TranslationString("Fort Zancudo", true), { -2012.8470f, 2956.5270f, 32.8101f } },
		{ TranslationString("Mount Chiliad", true), { 430.2037f, 5614.7340f, 766.1684f } },
		{ TranslationString("Diamond Casino", true), { 919.94f, 50.7f, 80.90f } },
	};

	ScrollStruct<int> Styles[] = {
		{ TranslationString("Normal", true), 786603 },
		{ TranslationString("Ignore Stops", true), 2883621 },
		{ TranslationString("Rushed", true), 1074528293 }
	};

	void Disable() {
		Native::ClearPedTasksImmediately(Menu::GetLocalPlayer().m_Ped);
		Native::SetPedIntoVehicle(Menu::GetLocalPlayer().m_Ped, Menu::GetLocalPlayer().m_Vehicle, -1);
	}

	void Enable() {
		Disable();

		switch (m_Vars.m_Destinations) {
			case 0:
				Native::TaskVehicleDriveWander(Menu::GetLocalPlayer().m_Ped, Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_Speed, Styles[m_Vars.m_Styles].m_Result);
				break;

			case 1:
				if (Native::DoesBlipExist(Native::GetFirstBlipInfoId(8))) {
					auto Coords = Native::GetBlipCoords(Native::GetFirstBlipInfoId(8));
					Native::TaskVehicleDriveToCoordLongrange(Menu::GetLocalPlayer().m_Ped, Menu::GetLocalPlayer().m_Vehicle, Coords.m_X, Coords.m_Y, Coords.m_Z, m_Vars.m_Speed, Styles[m_Vars.m_Styles].m_Result, 15.f);
				} else {
					Menu::GetNotify()->NotifyStacked(VehicleModsAutopilotMenu::GetInstance()->getString("No waypoint found"));
				}

				break;

			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				auto Coords = Destinations[m_Vars.m_Destinations].m_Result;
				Native::TaskVehicleDriveToCoordLongrange(Menu::GetLocalPlayer().m_Ped, Menu::GetLocalPlayer().m_Vehicle, Coords.m_X, Coords.m_Y, Coords.m_Z, m_Vars.m_Speed, Styles[m_Vars.m_Styles].m_Result, 15.f);
				break;
		}
	}
}

void VehicleModsAutopilotMenu::Init() {
	SetName("Autopilot");
	SetParentSubmenu<VehicleModsMenu>();

	addString("No waypoint found");

	addOption(ScrollOption<Math::Vector3_<float>>(SCROLL, "Destination")
		.addTranslation()
		.addScroll(m_Vars.m_Destinations, 0, NUMOF(Destinations), Destinations));

	addOption(ScrollOption<int>(SCROLL, "Style")
		.addTranslation()
		.addScroll(m_Vars.m_Styles, 0, NUMOF(Styles), Styles));
	
	addOption(NumberOption<float>(SCROLL, "Speed")
		.addTranslation()
		.addNumber(m_Vars.m_Speed, "%.0f", 1.f).addMin(0).addMax(200.f).showMax());

	addOption(ButtonOption("Enable Autopilot")
		.addTranslation().addHotkey()
		.addOnClick(Enable));

	addOption(ButtonOption("Disable Autopilot")
		.addTranslation().addHotkey()
		.addOnClick(Disable));
}

void VehicleModsAutopilotMenu::Update() {}

/*Called once on submenu open*/
void VehicleModsAutopilotMenu::UpdateOnce() {}

/*Called always*/
void VehicleModsAutopilotMenu::FeatureUpdate() {}

VehicleModsAutopilotMenu* _instance;
VehicleModsAutopilotMenu* VehicleModsAutopilotMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleModsAutopilotMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleModsAutopilotMenu::~VehicleModsAutopilotMenu() { delete _instance; }