#include "network_spoofing_vehicle_rotation.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_spoofing.hpp"
#include "menu/base/submenus/main/vehicle/vehicle_mods.hpp"
#include "rage/engine.hpp"

using namespace NetworkSpoofingVehicleRotationMenuVars;

namespace NetworkSpoofingVehicleRotationMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> Axis[] = {
		{ TranslationString("Pitch (X)", true), 0 },
		{ TranslationString("Roll (Y)", true), 1 },
		{ TranslationString("Yaw (Z)", true), 2 }
	};
}

void NetworkSpoofingVehicleRotationMenu::Init() {
	SetName("Vehicle Rotation");
	SetParentSubmenu<NetworkSpoofingMenu>();

	addOption(ToggleOption("Toggle Preview Ghost")
		.addTranslation()
		.addToggle(m_Vars.m_Ghost).canBeSaved(m_ParentNameStack)
		.addTooltip("Spawns a ghost to show you what your chances look like to other players"));

	addOption(NumberOption<float>(TOGGLE, "Spinbot")
		.addTranslation()
		.addToggle(m_Vars.m_Spinbot)
		.addNumber(m_Vars.m_SpinbotSpeed, "%.0f", 1.f).addMin(0.f).addMax(100.f).setScrollSpeed(10).canBeSaved(m_ParentNameStack));

	addOption(ScrollOption<int>(SCROLL, "Spinbot Axis")
		.addTranslation()
		.addScroll(m_Vars.m_SpinbotAxis, 0, NUMOF(Axis), Axis).canBeSaved(m_ParentNameStack));

	addOption(ScrollOption<int>(TOGGLE, "Jitter")
		.addTranslation()
		.addToggle(m_Vars.m_Jitter)
		.addScroll(m_Vars.m_JitterAxis, 0, NUMOF(Axis), Axis).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Static")
		.addTranslation()
		.addToggle(m_Vars.m_Static).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Relative")
		.addTranslation()
		.addToggle(m_Vars.m_Relative).canBeSaved(m_ParentNameStack));

	addOption(BreakOption("Static")
		.addTranslation()
		.addRequirement([] { return m_Vars.m_Static; }));

	addOption(NumberOption<float>(TOGGLE, "Pitch (X)")
		.addTranslation()
		.addToggle(m_Vars.m_StaticToggle.m_X)
		.addNumber(m_Vars.m_StaticValue.m_X, "%.0f", 1.f).addMin(0.f).addMax(360.f).setScrollSpeed(10).canBeSaved(m_ParentNameStack)
		.addRequirement([] { return m_Vars.m_Static; }));

	addOption(NumberOption<float>(TOGGLE, "Roll (Y)")
		.addTranslation()
		.addToggle(m_Vars.m_StaticToggle.m_Y)
		.addNumber(m_Vars.m_StaticValue.m_Y, "%.0f", 1.f).addMin(0.f).addMax(360.f).setScrollSpeed(10).canBeSaved(m_ParentNameStack)
		.addRequirement([] { return m_Vars.m_Static; }));

	addOption(NumberOption<float>(TOGGLE, "Yaw (Z)")
		.addTranslation()
		.addToggle(m_Vars.m_StaticToggle.m_Z)
		.addNumber(m_Vars.m_StaticValue.m_Z, "%.0f", 1.f).addMin(0.f).addMax(360.f).setScrollSpeed(10).canBeSaved(m_ParentNameStack)
		.addRequirement([] { return m_Vars.m_Static; }));
}

void NetworkSpoofingVehicleRotationMenu::Update() {}

/*Called once on submenu open*/
void NetworkSpoofingVehicleRotationMenu::UpdateOnce() {
	if (Menu::GetLocalPlayer().m_InVehicle) {
		if (m_Vars.m_Ghost) {
			if (!Native::DoesEntityExist(m_Vars.m_GhostHandle)) {
				m_Vars.m_GhostHandle = VehicleModsMenuVars::CloneVehicle(Menu::GetLocalPlayer().m_Vehicle);

				if (Native::DoesEntityExist(m_Vars.m_GhostHandle)) {
					Native::_NetworkSetEntityVisibleToNetwork(m_Vars.m_GhostHandle, false);
					Native::SetEntityAlpha(m_Vars.m_GhostHandle, 100, false);
					Native::SetEntityCollision(m_Vars.m_GhostHandle, false, false);
					Native::AttachEntityToEntity(m_Vars.m_GhostHandle, Menu::GetLocalPlayer().m_Vehicle, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0, 0, 1, 1, 1);
				}
			}
		} else {
			if (Native::DoesEntityExist(m_Vars.m_GhostHandle)) {
				Native::DetachEntity(m_Vars.m_GhostHandle, true, false);
				Native::DeleteEntity(&m_Vars.m_GhostHandle);
				m_Vars.m_GhostHandle = 0;
			}
		}
	}
}

/*Called always*/
void NetworkSpoofingVehicleRotationMenu::FeatureUpdate() {
	if (Menu::GetLocalPlayer().m_InVehicle) {
		m_Vars.m_HookValue.m_X = Menu::GetLocalPlayer().m_Rotation.m_X;
		m_Vars.m_HookValue.m_Y = Menu::GetLocalPlayer().m_Rotation.m_Y;
		m_Vars.m_HookValue.m_Z = Menu::GetLocalPlayer().m_Rotation.m_Z;

		if (m_Vars.m_Static) {
			if (m_Vars.m_StaticToggle.m_X) m_Vars.m_HookValue.m_X = m_Vars.m_StaticValue.m_X + (m_Vars.m_Relative ? Menu::GetLocalPlayer().m_Rotation.m_X : 0.f);
			if (m_Vars.m_StaticToggle.m_Y) m_Vars.m_HookValue.m_Y = m_Vars.m_StaticValue.m_Y + (m_Vars.m_Relative ? Menu::GetLocalPlayer().m_Rotation.m_Y : 0.f);
			if (m_Vars.m_StaticToggle.m_Z) m_Vars.m_HookValue.m_Z = m_Vars.m_StaticValue.m_Z + (m_Vars.m_Relative ? Menu::GetLocalPlayer().m_Rotation.m_Z : 0.f);
		}

		if (m_Vars.m_Spinbot) {
			static float Value = 0.f;
			Value += m_Vars.m_SpinbotSpeed;
			if (Value > 360.f) Value = (Value - 360.f);

			switch (m_Vars.m_SpinbotAxis) {
				case 0:
					m_Vars.m_HookValue.m_X = Value;
					break;

				case 1:
					m_Vars.m_HookValue.m_Y = Value;
					break;

				case 2:
					m_Vars.m_HookValue.m_Z = Value;
					break;
			}
		}

		if (m_Vars.m_Jitter) {
			static float Value = 0.f;
			Value += Native::GetRandomFloatInRange(5.f, 100.f);
			if (Value > 360.f) Value = (Value - 360.f);

			switch (m_Vars.m_JitterAxis) {
				case 0:
					m_Vars.m_HookValue.m_X = Value;
					break;

				case 1:
					m_Vars.m_HookValue.m_Y = Value;
					break;

				case 2:
					m_Vars.m_HookValue.m_Z = Value;
					break;
			}
		}

		if (m_Vars.m_GhostHandle && Native::DoesEntityExist(m_Vars.m_GhostHandle)) {
			/*Rage::Types::Vehicle* GhostAddress = (Rage::Types::Vehicle*)Rage::Engine::GetEntityAddress(m_Vars.m_GhostHandle);
			if (GhostAddress) {
				memcpy(GhostAddress->GetMatrix(), &m_Vars.m_GhostMatrix, sizeof(m_Vars.m_GhostMatrix));
			}*/

			Native::AttachEntityToEntity(m_Vars.m_GhostHandle, Menu::GetLocalPlayer().m_Vehicle, 0, 0.0f, 0.0f, 0.0f, m_Vars.m_HookValue.m_X - Menu::GetLocalPlayer().m_Rotation.m_X, m_Vars.m_HookValue.m_Y - Menu::GetLocalPlayer().m_Rotation.m_Y, m_Vars.m_HookValue.m_Z - Menu::GetLocalPlayer().m_Rotation.m_Z, 0, 0, 0, 1, 1, 1);
		}

		m_Vars.m_HookValue.m_X *= 0.0174532924f;
		m_Vars.m_HookValue.m_Y *= 0.0174532924f;
		m_Vars.m_HookValue.m_Z *= 0.0174532924f;
	}
}

NetworkSpoofingVehicleRotationMenu* _instance;
NetworkSpoofingVehicleRotationMenu* NetworkSpoofingVehicleRotationMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkSpoofingVehicleRotationMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkSpoofingVehicleRotationMenu::~NetworkSpoofingVehicleRotationMenu() { delete _instance; }