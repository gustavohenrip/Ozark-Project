#include "teleport_directions.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../teleport.hpp"

using namespace TeleportDirectionsMenuVars;

namespace TeleportDirectionsMenuVars {
	Vars_ m_Vars;

	void TeleportDirection(int Direction) {
		Math::Vector3<float> Coords = { 0.f, 0.f, 0.f };

		switch (Direction) {
			case 0:
				Coords.m_Y = m_Vars.m_Directions[0] + 1.f;
				break;
			case 1:
				Coords.m_Y = (m_Vars.m_Directions[1] * -1.f) + 1.f;
				break;
			case 2:
				Coords.m_Z = m_Vars.m_Directions[2] + 1.f;
				break;
			case 3:
				Coords.m_Z = (m_Vars.m_Directions[3] * -1.f) + 1.f;
				break;
			case 4:
				Coords.m_X = (m_Vars.m_Directions[4] * -1.f) + 1.f;
				break;
			case 5:
				Coords.m_X = m_Vars.m_Directions[5] + 1.f;
				break;
		}

		Math::Vector3<float> Velocity;
		if (Menu::GetLocalPlayer().m_InVehicle) {
			Velocity = Native::GetEntityVelocity(Menu::GetLocalPlayer().m_Vehicle);
		} else {
			Coords.m_Z -= 1.f;
		}

		auto C = Native::GetOffsetFromEntityInWorldCoords(Menu::GetLocalPlayer().m_Entity, Coords.m_X, Coords.m_Y, Coords.m_Z);
		Native::SetEntityCoordsNoOffset(Menu::GetLocalPlayer().m_Entity, C.m_X, C.m_Y, C.m_Z, false, false, false);

		if (Menu::GetLocalPlayer().m_InVehicle && Native::IsVehicleOnAllWheels(Menu::GetLocalPlayer().m_Vehicle)) {
			Native::SetEntityVelocity(Menu::GetLocalPlayer().m_Vehicle, Velocity.m_X, Velocity.m_Y, Velocity.m_Z);
			Native::SetVehicleEngineOn(Menu::GetLocalPlayer().m_Vehicle, true, true, true);
		}
	}
}

void TeleportDirectionsMenu::Init() {
	SetName("Directions");
	SetParentSubmenu<TeleportMenu>();

	addOption(NumberOption<float>(SCROLLSELECT, "Forwards")
		.addTranslation().addHotkey()
		.addNumber(m_Vars.m_Directions[0], "%.0f", 1.f).addMin(1.f).addMax(100.f)
		.addOnClick([] { TeleportDirection(0); }));

	addOption(NumberOption<float>(SCROLLSELECT, "Backwards")
		.addTranslation().addHotkey()
		.addNumber(m_Vars.m_Directions[1], "%.0f", 1.f).addMin(1.f).addMax(100.f)
		.addOnClick([] { TeleportDirection(1); }));

	addOption(NumberOption<float>(SCROLLSELECT, "Above")
		.addTranslation().addHotkey()
		.addNumber(m_Vars.m_Directions[2], "%.0f", 1.f).addMin(1.f).addMax(100.f)
		.addOnClick([] { TeleportDirection(2); }));

	addOption(NumberOption<float>(SCROLLSELECT, "Below")
		.addTranslation().addHotkey()
		.addNumber(m_Vars.m_Directions[3], "%.0f", 1.f).addMin(1.f).addMax(100.f)
		.addOnClick([] { TeleportDirection(3); }));

	addOption(NumberOption<float>(SCROLLSELECT, "Left")
		.addTranslation().addHotkey()
		.addNumber(m_Vars.m_Directions[4], "%.0f", 1.f).addMin(1.f).addMax(100.f)
		.addOnClick([] { TeleportDirection(4); }));

	addOption(NumberOption<float>(SCROLLSELECT, "Right")
		.addTranslation().addHotkey()
		.addNumber(m_Vars.m_Directions[5], "%.0f", 1.f).addMin(1.f).addMax(100.f)
		.addOnClick([] { TeleportDirection(5); }));
}

void TeleportDirectionsMenu::Update() {}

/*Called once on submenu open*/
void TeleportDirectionsMenu::UpdateOnce() {}

/*Called always*/
void TeleportDirectionsMenu::FeatureUpdate() {}

TeleportDirectionsMenu* _instance;
TeleportDirectionsMenu* TeleportDirectionsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new TeleportDirectionsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

TeleportDirectionsMenu::~TeleportDirectionsMenu() { delete _instance; }