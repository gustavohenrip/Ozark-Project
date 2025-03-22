#include "vehicle_particle_fx.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_mods.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/control_manager.hpp"
#include "utils/va.hpp"
#include "menu/base/util/timers.hpp"

using namespace VehicleModsParticleFXMenuVars;

namespace VehicleModsParticleFXMenuVars {
	Vars_ m_Vars;
}

void VehicleModsParticleFXMenu::Init() {
	SetName("Particle FX");
	SetParentSubmenu<VehicleModsMenu>();

	addOption(ToggleOption("Toggle Particle FX")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Toggle));

	addOption(ScrollOption<Global::Arrays::Particle>(SCROLL, "Particle")
		.addTranslation()
		.addScroll(m_Vars.m_Particle, 0, NUMOF(Global::Arrays::g_Particles), Global::Arrays::g_Particles));

	addOption(NumberOption<float>(SCROLL, "Scale")
		.addTranslation()
		.addNumber(m_Vars.m_Scale, "%.2f", 0.01f).addMin(0.01f).addMax(10.f).setScrollSpeed(10));

	addOption(BreakOption("Bones")
		.addTranslation());
}

void VehicleModsParticleFXMenu::Update() {}

/*Called once on submenu open*/
void VehicleModsParticleFXMenu::UpdateOnce() {
	ClearOptionsOffset(3);

	for (int i = 0; i < NUMOF(Global::Arrays::g_VehicleBones); i++) {
		auto Bone = Global::Arrays::g_VehicleBones[i];

		if (Native::GetEntityBoneIndexByName(Menu::GetLocalPlayer().m_Vehicle, Bone.m_Result) > 0) {
			addOption(ToggleOption(Bone.m_Name.Get())
				.addToggle(m_Vars.m_Bones[i]));
		} else {
			m_Vars.m_Bones[i] = false;
		}
	}
}

/*Called always*/
void VehicleModsParticleFXMenu::FeatureUpdate() {
	static int Timer[11];
	static int Timer2[16];

	if (Menu::GetLocalPlayer().m_InVehicle) {
		if (m_Vars.m_Toggle) {
			for (int i = 0; i < NUMOF(Global::Arrays::g_VehicleBones); i++) {
				if (m_Vars.m_Bones[i]) {
					if (i == NUMOF(Global::Arrays::g_VehicleBones) - 1) {
						for (int j = 0; j < 16; j++) {
							auto Index = Native::GetEntityBoneIndexByName(Menu::GetLocalPlayer().m_Vehicle, j == 0 ? "exhaust" : Utils::VA::VA("exhaust_%i", j));

							if (Index) {
								auto Coords = Native::GetWorldPositionOfEntityBone(Menu::GetLocalPlayer().m_Vehicle, Index);
								if (!Coords.IsZero()) {
									auto Particle = Global::Arrays::g_Particles[m_Vars.m_Particle];
									
									Menu::Timers::RunTimedFunction(&Timer2[j], 100, [=] {
										Native::RequestNamedPtfxAsset(Particle.m_Result.m_Dict);
										Native::_UseParticleFxAssetNextCall(Particle.m_Result.m_Dict);
										Native::_StartParticleFxNonLoopedAtCoord2(Particle.m_Result.m_Texture, Coords.m_X, Coords.m_Y, Coords.m_Z, 0.f, 0.f, 0.f, m_Vars.m_Scale, false, false, false);
									});
								}
							}
						}
					} else {
						auto Bone = Global::Arrays::g_VehicleBones[i];
						auto Index = Native::GetEntityBoneIndexByName(Menu::GetLocalPlayer().m_Vehicle, Bone.m_Result);

						if (Index) {
							auto Coords = Native::GetWorldPositionOfEntityBone(Menu::GetLocalPlayer().m_Vehicle, Index);
							if (!Coords.IsZero()) {
								auto Particle = Global::Arrays::g_Particles[m_Vars.m_Particle];
								
								Menu::Timers::RunTimedFunction(&Timer[i], 100, [=] {
									Native::RequestNamedPtfxAsset(Particle.m_Result.m_Dict);
									Native::_UseParticleFxAssetNextCall(Particle.m_Result.m_Dict);
									Native::_StartParticleFxNonLoopedAtCoord2(Particle.m_Result.m_Texture, Coords.m_X, Coords.m_Y, Coords.m_Z, 0.f, 0.f, 0.f, m_Vars.m_Scale, false, false, false);
								});
							}
						} else {
							m_Vars.m_Bones[i] = false;
						}
					}
				}
			}
		}
	}
}

VehicleModsParticleFXMenu* _instance;
VehicleModsParticleFXMenu* VehicleModsParticleFXMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleModsParticleFXMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleModsParticleFXMenu::~VehicleModsParticleFXMenu() { delete _instance; }