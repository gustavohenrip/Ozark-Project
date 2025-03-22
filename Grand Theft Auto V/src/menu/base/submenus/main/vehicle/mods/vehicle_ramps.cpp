#include "vehicle_ramps.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_mods.hpp"
#include "menu/base/util/control_manager.hpp"

using namespace VehicleModsRampMenuVars;

namespace VehicleModsRampMenuVars {
	Vars_ m_Vars;

	Object CreateAttachedRamp(Entity attach_to, uint32_t model, int pos = 0, bool opacitated = true) {
		Menu::GetControlManager()->SimpleRequestModel(model);

		Math::Vector3<float> min, max;
		Native::GetModelDimensions(model, &min, &max);
		
		Math::Vector3<float> dim = { max.m_X - min.m_X, max.m_Y - min.m_Y, max.m_Z - min.m_Z };

		auto Coords = Native::GetEntityCoords(attach_to, true);
		Object ramp = Native::CreateObject(model, Coords.m_X, Coords.m_Y, Coords.m_Z, true, true, false);

		switch (pos) {
				// Front
			case 0: Native::AttachEntityToEntity(ramp, attach_to, 0, /**/ 0, dim.m_Y, -0.5, /**/ 0, 0, 180, true, true, true, false, 2, true); break;
				// Rear
			case 1: Native::AttachEntityToEntity(ramp, attach_to, 0, /**/ 0, (-1) * dim.m_Y, -0.5, /**/ 0, 0, 0, true, true, true, false, 2, true); break;
				// Left
			case 2: Native::AttachEntityToEntity(ramp, attach_to, 0, /**/ (-1) * dim.m_X, 0, -0.5, /**/ 0, 0, 270, true, true, true, false, 2, true); break;
				// Right
			case 3: Native::AttachEntityToEntity(ramp, attach_to, 0, /**/ dim.m_X, 0, -0.5, /**/ 0, 0, 90, true, true, true, false, 2, true); break;
		}

		if (opacitated) {
			Native::SetEntityAlpha(ramp, 100, true);
		}

		return ramp;
	}
	
	void Attach() {
		if (!Menu::GetLocalPlayer().m_InVehicle) return;

		char Buffer[50];
		sprintf(Buffer, "PROP_MP_RAMP_0%i", m_Vars.m_RampType);

		uint32_t Model = Native::GetHashKey(Buffer);

		if (m_Vars.m_FrontRamp) {
			CreateAttachedRamp(Menu::GetLocalPlayer().m_Vehicle, Model, 0, m_Vars.m_Transparency);
		}

		if (m_Vars.m_RearRamp) {
			CreateAttachedRamp(Menu::GetLocalPlayer().m_Vehicle, Model, 1, m_Vars.m_Transparency);
		}

		if (m_Vars.m_LeftRamp) {
			CreateAttachedRamp(Menu::GetLocalPlayer().m_Vehicle, Model, 2, m_Vars.m_Transparency);
		}

		if (m_Vars.m_RightRamp) {
			CreateAttachedRamp(Menu::GetLocalPlayer().m_Vehicle, Model, 3, m_Vars.m_Transparency);
		}
	}

	void Detach() {
		if (!Menu::GetLocalPlayer().m_InVehicle) return;
		auto Coords = Menu::GetLocalPlayer().m_Coords;

		uint32_t Ramps[] = { 0xB157C9E4, 0xF4F1511E, 0x93948E5E };
		for (int i = 0; i < 3; i++) {
			int Counter = 0;
		search:
			Object Ramp = Native::GetClosestObjectOfType(Coords.m_X, Coords.m_Y, Coords.m_Z, 20.f, Ramps[i], false, false, false);
			if (Ramp) {
				if (Native::DoesEntityExist(Ramp) && Native::IsEntityAttachedToEntity(Ramp, Menu::GetLocalPlayer().m_Vehicle)) {
					Menu::GetControlManager()->SimpleRequestControl(Ramp);

					Native::SetEntityAsMissionEntity(Ramp, true, true);
					Native::DeleteEntity(&Ramp);

					Counter++;
					if (Counter < 20) {
						goto search;
					}
				}
			}
		}
	}
}

void VehicleModsRampMenu::Init() {
	SetName("Ramps");
	SetParentSubmenu<VehicleModsMenu>();

	addOption(NumberOption<int>(SCROLL, "Ramp Type")
		.addTranslation().addHotkey()
		.addNumber(m_Vars.m_RampType, "%i", 1).addMin(1).addMax(3).showMax());

	addOption(ToggleOption("Ramp Transparency")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Transparency).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Front Ramp")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_FrontRamp).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Rear Ramp")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_RearRamp).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Left Ramp")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_LeftRamp).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Right Ramp")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_RightRamp).canBeSaved(m_ParentNameStack));

	addOption(ButtonOption("Apply Ramps")
		.addTranslation().addHotkey()
		.addOnClick(Attach));

	addOption(ButtonOption("Delete Attached Ramps")
		.addTranslation().addHotkey()
		.addOnClick(Detach));
}

void VehicleModsRampMenu::Update() {}

/*Called once on submenu open*/
void VehicleModsRampMenu::UpdateOnce() {}

/*Called always*/
void VehicleModsRampMenu::FeatureUpdate() {}

VehicleModsRampMenu* _instance;
VehicleModsRampMenu* VehicleModsRampMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleModsRampMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleModsRampMenu::~VehicleModsRampMenu() { delete _instance; }