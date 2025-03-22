#include "vehicle_boost.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_mods.hpp"
#include "rage/engine.hpp"

using namespace VehicleModsBoostMenuVars;

namespace VehicleModsBoostMenuVars {
	Vars_ m_Vars;

	void Boost(Vehicle Veh, bool Forwards) {
		if (m_Vars.m_BoostFX) {
			Native::SetVehicleBoostActive(Veh, true);
			Native::_StartScreenEffect("RaceTurbo", -1, false);
		}

		if (m_Vars.m_BoostSound) {
			Native::RequestScriptAudioBank("DLC_IMPORTEXPORT/STEAL_STUNT", false, -1);
			Native::PlaySoundFromEntity(-1, "Speed_Boost", Veh, "DLC_IE_Stunt_Man_Sounds", false, 0);
		}

		Native::SetVehicleForwardSpeed(Veh, Forwards ? m_Vars.m_BoostForwards + Native::GetEntitySpeed(Veh) : -m_Vars.m_BoostBackwards);

		if (m_Vars.m_BoostFX) {
			Native::_StopScreenEffect("RaceTurbo");
			Native::SetVehicleBoostActive(Veh, false);
		}
	}
}

void VehicleModsBoostMenu::Init() {
	SetName("Boost");
	SetParentSubmenu<VehicleModsMenu>();

	addOption(ToggleOption("Boost FX")
		.addTranslation()
		.addToggle(m_Vars.m_BoostFX).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Boost Sound")
		.addTranslation()
		.addToggle(m_Vars.m_BoostSound).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Nitrous Oxide (horn)")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_NitrousOxide).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<int>(SCROLLSELECT, "Boost Forwards")
		.addTranslation().addHotkey()
		.addNumber(m_Vars.m_BoostForwards, "%i", 1).addMin(0).addMax(5000).setScrollSpeed(10).showMax()
		.addOnClick([] { Boost(Menu::GetLocalPlayer().m_Vehicle, true); }));

	addOption(NumberOption<int>(SCROLLSELECT, "Boost Backwards")
		.addTranslation().addHotkey()
		.addNumber(m_Vars.m_BoostBackwards, "%i", 1).addMin(0).addMax(5000).setScrollSpeed(10).showMax()
		.addOnClick([] { Boost(Menu::GetLocalPlayer().m_Vehicle, false); }));

	addOption(ToggleOption("Unlimited Special Ability")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_UnlimitedSpecialAbility).canBeSaved(m_ParentNameStack));
}

void VehicleModsBoostMenu::Update() {}

/*Called once on submenu open*/
void VehicleModsBoostMenu::UpdateOnce() {}

/*Called always*/
void VehicleModsBoostMenu::FeatureUpdate() {
	if (Menu::GetLocalPlayer().m_InVehicle) {
		if (m_Vars.m_UnlimitedSpecialAbility) {
			if (Native::_HasVehicleRocketBoost(Menu::GetLocalPlayer().m_Vehicle)) {
				static std::pair<uint32_t, bool> Yes = { 0, false };

				if (Native::_IsVehicleRocketBoostActive(Menu::GetLocalPlayer().m_Vehicle)) {
					if (!Yes.second) {
						Yes.second = true;
						Yes.first = GetTickCount() + 3500;
					}

					Native::_SetVehicleRocketBoostRefillTime(Menu::GetLocalPlayer().m_Vehicle, 0.0f);
					Native::_SetVehicleRocketBoostPercentage(Menu::GetLocalPlayer().m_Vehicle, 100.f);

					if (Yes.second) {
						if (GetTickCount() > Yes.first) {
							Yes.second = false;
							Native::_SetVehicleRocketBoostActive(Menu::GetLocalPlayer().m_Vehicle, false);
						}
					}
				}
			}
		}

		static bool Reset = false;
		if (m_Vars.m_NitrousOxide) {
			if (Menu::GetLocalPlayer().m_InVehicle) {
				auto Veh = Menu::GetLocalPlayer().m_Vehicle;
				if (Native::IsHornActive(Veh)) {
					if (m_Vars.m_BoostFX) {
						Native::SetVehicleBoostActive(Veh, true);
						Native::_StartScreenEffect("RaceTurbo", -1, false);
					}

					if (m_Vars.m_BoostSound) {
						Native::_StartScreenEffect("Raceturbo", 0, 0);
					}

					Native::_SetVehicleEnginePowerMultiplier(Menu::GetLocalPlayer().m_Vehicle, 10.f);

					uint64_t Address = Rage::Engine::GetEntityAddress(Veh);
					if (Address) {
						*(float*)(Address + 0x1360) = 30.f;
					}

					if (m_Vars.m_BoostFX) {
						Native::_StopScreenEffect("RaceTurbo");
						Native::SetVehicleBoostActive(Veh, false);
					}

					Reset = true;
				} else {
					if (Reset) {
						uint64_t Address = Rage::Engine::GetEntityAddress(Veh);
						if (Address) {
							*(float*)(Address + 0x1360) = 1.f;
						}

						Reset = false;
					}
				}
			}
		}
	}
}

VehicleModsBoostMenu* _instance;
VehicleModsBoostMenu* VehicleModsBoostMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleModsBoostMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleModsBoostMenu::~VehicleModsBoostMenu() { delete _instance; }