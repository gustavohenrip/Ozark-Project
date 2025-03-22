#include "gravity_gun.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../weapon_mods.hpp"
#include "menu/base/util/raycast.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/util/camera.hpp"
#include "rage/entity/entity.hpp"
#include "menu/base/util/pools/pool_manager.hpp"

using namespace GravityGunMenuVars;

namespace GravityGunMenuVars {
	Vars_ m_Vars;

	void GravityGun() {
		static int Timer = 0;
		if (Menu::GetLocalPlayer().m_IsAiming) {
			auto Position = Native::GetOffsetFromEntityInWorldCoords(Menu::GetLocalPlayer().m_Ped, 0.4f, 0.0f, 0.8f);

			if (Native::DoesEntityExist(m_Vars.m_Entity)) {
				Native::SetInputExclusive(0, ControlFrontendUp);
				Native::SetInputExclusive(0, ControlFrontendDown);

				if (Native::IsControlPressed(0, ControlFrontendUp)) {
					m_Vars.m_Distance++;
				} else if (Native::IsControlPressed(0, ControlFrontendDown) && m_Vars.m_Distance > 3.f) {
					m_Vars.m_Distance--;
				}

				Menu::GetControlManager()->SimpleRequestControl(m_Vars.m_Entity);

				auto Velocity = ((Position + (Menu::Camera::GetCameraDirection() * m_Vars.m_Distance)) - Native::GetEntityCoords(m_Vars.m_Entity, false)) * m_Vars.m_Force;
				Native::SetEntityVelocity(m_Vars.m_Entity, Velocity.m_X, Velocity.m_Y, Velocity.m_Z);

				if (Menu::GetLocalPlayer().m_IsShooting) {
					Velocity = ((Position + (Menu::Camera::GetCameraDirection() * 5000.f)) - Native::GetEntityCoords(m_Vars.m_Entity, false)) * m_Vars.m_Force;
					Native::SetEntityVelocity(m_Vars.m_Entity, Velocity.m_X, Velocity.m_Y, Velocity.m_Z);
					m_Vars.m_Entity = 0;
					Timer = Native::GetGameTimer() + 750;
				}
			} else {
				m_Vars.m_Entity = 0;

				if (Native::GetGameTimer() > Timer) {
					Menu::Raycast Cast = Menu::RaycastInfront(9999.f);
					if (Cast.HitEntity()) {
						m_Vars.m_Entity = Cast.GetHitEntity();

						if (Native::IsEntityAPed(m_Vars.m_Entity)) {
							if (Native::IsPedInAnyVehicle(m_Vars.m_Entity, false)) {
								m_Vars.m_Entity = Native::GetVehiclePedIsIn(m_Vars.m_Entity, false);
							} else {
								if (Native::IsPedAPlayer(m_Vars.m_Entity)) {
									Timer = 0;
									m_Vars.m_Entity = 0;
									return;
								}

								// Native::SetPedRagdollForceFall(m_Vars.m_Entity);
								// Native::SetPedToRagdoll(m_Vars.m_Entity, -1, -1, 0, 1, 1, 0);
							}
						}

						Timer = 0;
					}
				}
			}

			return;
		}

		Timer = 0;
		m_Vars.m_Entity = 0;
	}
}

void GravityGunMenu::Init() {
	SetName("Gravity Gun");
	SetParentSubmenu<WeaponModsMenu>();

	addOption(ToggleOption("Toggle Gravity Gun")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Toggle).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Force")
		.addTranslation()
		.addNumber(m_Vars.m_Force, "%.1f", 0.1f).addMin(1.f).addMax(200.f).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Distance")
		.addTranslation()
		.addNumber(m_Vars.m_Distance, "%.0f", 1.f).addMin(1.f).addMax(200.f).canBeSaved(m_ParentNameStack)
		.addTooltip("Alternatively, you can use UP and DOWN"));
}

void GravityGunMenu::Update() {}

/*Called once on submenu open*/
void GravityGunMenu::UpdateOnce() {}

/*Called always*/
void GravityGunMenu::FeatureUpdate() {
	if (m_Vars.m_Toggle) {
		GravityGun();
	}
}

GravityGunMenu* _instance;
GravityGunMenu* GravityGunMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new GravityGunMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

GravityGunMenu::~GravityGunMenu() { delete _instance; }