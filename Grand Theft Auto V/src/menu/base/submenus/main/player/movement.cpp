#include "movement.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../player.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/util/camera.hpp"

using namespace MovementMenuVars;

namespace MovementMenuVars {
	Vars_ m_Vars;

	double GetAnalogRange(int Stick) {
		return (((double)Native::GetControlValue(0, Stick)) - 127) / 127;
	}

	void NoClip() {
		if (Menu::Util::GetInput()->IsJustPressed(true, VK_F8)) {
			m_Vars.m_NoClipBind ^= true;
			if (!m_Vars.m_NoClipBind) {
				Native::SetEntityCollision(Menu::GetLocalPlayer().m_Entity, true, true);
				Native::FreezeEntityPosition(Menu::GetLocalPlayer().m_Entity, false);
				Native::EnableAllControlActions(0);
			}
		}

		if (m_Vars.m_NoClipBind) {
			Math::Vector3<float> pos;
			pos.m_X = GetAnalogRange(ControlFrontendAxisX) * m_Vars.m_NoClipSpeed;
			pos.m_Y = GetAnalogRange(ControlFrontendAxisY) * m_Vars.m_NoClipSpeed * -1.f;

			Math::Vector3<float> apos = Native::GetOffsetFromEntityInWorldCoords(Menu::GetLocalPlayer().m_Entity, pos.m_X, pos.m_Y, pos.m_Z);
			auto Input = Native::GetControlNormal(0, ControlAttack) - Native::GetControlNormal(0, ControlAim);

			if (Menu::Util::GetInput()->IsPressed(false, ControlFrontendRt) || Input > 0.f) {
				apos.m_Z += m_Vars.m_NoClipSpeed;
			}

			if (Menu::Util::GetInput()->IsPressed(false, ControlFrontendLt) || Input < 0.f) {
				apos.m_Z -= m_Vars.m_NoClipSpeed;
			}

			Native::SetEntityCoordsNoOffset(Menu::GetLocalPlayer().m_Entity, apos.m_X, apos.m_Y, apos.m_Z, false, false, true);

			Math::Vector3<float> rot = Native::GetGameplayCamRot(2);
			Native::SetEntityRotation(Menu::GetLocalPlayer().m_Entity, 0.f, rot.m_Y, rot.m_Z, 2, true);

			Native::FreezeEntityPosition(Menu::GetLocalPlayer().m_Entity, true);
			Native::SetEntityCollision(Menu::GetLocalPlayer().m_Entity, false, false);

			Native::DisableControlAction(0, ControlFrontendLt, true);
			Native::DisableControlAction(0, ControlFrontendRt, true);
			Native::DisableControlAction(0, ControlFrontendCancel, true);
		}
	}
}

void MovementMenu::Init() {
	SetName("Movement");
	SetParentSubmenu<PlayerMenu>();

	addOption(NumberOption<float>(TOGGLE, "No Clip")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_NoClip)
		.addNumber(m_Vars.m_NoClipSpeed, "%.1f", 0.1f).addMin(0.1f).addMax(10.f).showMax().canBeSaved(m_ParentNameStack)
		.addOnClick([this] {
			if (m_Vars.m_NoClip) {
				if (m_Vars.m_NoClipNotify) {
					m_Vars.m_NoClipNotify = false;

					if (Menu::GetSubmenuHandler()->GetCurrentSubmenu() == this) {
						Menu::GetNotify()->NotifyStacked(getString("~b~F8~w~ to enable/disable noclip"));
					}
				}
			} else {
				m_Vars.m_NoClipNotify = true;
				Native::SetEntityCollision(Menu::GetLocalPlayer().m_Entity, true, true);
				Native::FreezeEntityPosition(Menu::GetLocalPlayer().m_Entity, false);
				Native::EnableAllControlActions(0);
			}
		}).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(TOGGLE, "Run Multiplier")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_SuperRun)
		.addNumber(m_Vars.m_SuperRunSpeed, "%.2f", 0.01f).addMin(0.f).addMax(100.f).showMax().canLoop().setScrollSpeed(40)
		.addOnClick([] {
			if (Global::Vars::g_PedFactory && Global::Vars::g_PedFactory->m_LocalPed) {
				if (Global::Vars::g_PedFactory->m_LocalPed->m_PlayerInfo) {
					Global::Vars::g_PedFactory->m_LocalPed->m_PlayerInfo->SetRunSpeed(m_Vars.m_SuperRun ? m_Vars.m_SuperRunSpeed : 1.0f);
				}
			}
		}).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(TOGGLE, "Swim Multiplier")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_SuperSwim)
		.addNumber(m_Vars.m_SuperSwimSpeed, "%.2f", 0.01f).addMin(0.f).addMax(100.f).showMax().canLoop().setScrollSpeed(40)
		.addOnClick([] {
			if (Global::Vars::g_PedFactory && Global::Vars::g_PedFactory->m_LocalPed) {
				if (Global::Vars::g_PedFactory->m_LocalPed->m_PlayerInfo) {
					Global::Vars::g_PedFactory->m_LocalPed->m_PlayerInfo->m_SwimSpeed = 1.f;
				}
			}
		}).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(TOGGLE, "Super Run")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_SuperRunAlternative)
		.addNumber(m_Vars.m_SuperRunAlternativeSpeed, "%.2f", 0.01f).addMin(0.f).addMax(100.f).showMax().canLoop().setScrollSpeed(40).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Super Jump")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_SuperJump).canBeSaved(m_ParentNameStack));
}

void MovementMenu::Update() {}

/*Called once on submenu open*/
void MovementMenu::UpdateOnce() {}

/*Called always*/
void MovementMenu::FeatureUpdate() {
	if (m_Vars.m_NoClip) {
		NoClip();
	}

	if (m_Vars.m_SuperJump) {
		Native::SetSuperJumpThisFrame(Menu::GetLocalPlayer().m_ID);
	}

	if (m_Vars.m_SuperRunAlternative) {
		if (Native::IsDisabledControlPressed(0, ControlSprint)) {
			Native::ApplyForceToEntity(Menu::GetLocalPlayer().m_Ped, 1, 0.f, m_Vars.m_SuperRunAlternativeSpeed, 0.f, 0.f, 0.f, 0.f, 0, 1, 1, 1, 0, 1);
		}
	}
}

MovementMenu* _instance;
MovementMenu* MovementMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new MovementMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

MovementMenu::~MovementMenu() { delete _instance; }