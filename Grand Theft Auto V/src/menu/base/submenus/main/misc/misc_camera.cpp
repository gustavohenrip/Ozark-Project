#include "misc_camera.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../misc.hpp"
#include "menu/base/util/camera.hpp"
#include "menu/base/util/timers.hpp"
#include "camera/misc_camera_aerial.hpp"
#include "menu/base/util/instructionals.hpp"

using namespace MiscCameraMenuVars;

namespace MiscCameraMenuVars {
	Vars_ m_Vars;
}

void MiscCameraMenu::Init() {
	SetName("Camera");
	SetParentSubmenu<MiscMenu>();

	addOption(SubmenuOption("Aerial Camera")
		.addTranslation().addHotkey()
		.addSubmenu<MiscCameraAerialMenu>());

	addOption(ToggleOption("Freecam")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Freecam)
		.addOnClick([] {
			if (!m_Vars.m_Freecam) {
				if (Native::DoesCamExist(m_Vars.m_FreecamHandle)) {
					Native::SetCamActive(m_Vars.m_FreecamHandle, false);
					Native::RenderScriptCams(false, true, 1000, true, false, 1);
					Native::DestroyCam(m_Vars.m_FreecamHandle, false);
					Native::SetFocusEntity(Menu::GetLocalPlayer().m_Ped);
				}
			}
		}));

	addOption(NumberOption<float>(TOGGLE, "Camera Zoom")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_CameraZoom)
		.addNumber(m_Vars.m_CameraZoomVal, "%.0f", 1.f).addMin(0).addMax(1000.f).setScrollSpeed(10));
}

void MiscCameraMenu::Update() {}

/*Called once on submenu open*/
void MiscCameraMenu::UpdateOnce() {}

/*Called always*/
void MiscCameraMenu::FeatureUpdate() {
	if (m_Vars.m_CameraZoom) {
		Native::_AnimateGameplayCamZoom(1.f, m_Vars.m_CameraZoomVal);
	}

	if (m_Vars.m_Freecam) {
		if (!Native::DoesCamExist(m_Vars.m_FreecamHandle)) {
			Math::Vector3<float> Coords = Menu::GetLocalPlayer().m_Coords;
			m_Vars.m_FreecamHandle = Native::CreateCam("DEFAULT_SCRIPTED_CAMERA", 0);
			Native::SetCamActive(m_Vars.m_FreecamHandle, true);
			Native::RenderScriptCams(true, true, 1000, true, false, 1);
			Native::SetCamCoord(m_Vars.m_FreecamHandle, Coords.m_X, Coords.m_Y, Coords.m_Z + 7.0f);
		} else {
			Math::Vector3<float> CamCoords = Native::GetCamCoord(m_Vars.m_FreecamHandle);
			Math::Vector3<float> GameCamRot = Native::GetGameplayCamRot(0);
			Native::SetCamRot(m_Vars.m_FreecamHandle, GameCamRot.m_X, GameCamRot.m_Y, GameCamRot.m_Z, 2);

			if (Menu::GetLocalPlayer().m_InVehicle) {
				Native::SetVehicleForwardSpeed(Menu::GetLocalPlayer().m_Vehicle, 0.0f);
			} else {
				Native::TaskStandStill(Menu::GetLocalPlayer().m_Ped, 10);
			}

			Math::Vector3<float> Infront = Menu::Camera::GetCameraDirection();

			if (Menu::Util::GetInput()->IsPressed(true, 'W')) {
				Native::SetCamCoord(m_Vars.m_FreecamHandle, CamCoords.m_X + Infront.m_X * 2.f, CamCoords.m_Y + Infront.m_Y * 2.f, CamCoords.m_Z + Infront.m_Z * 2.f);
			} else if (Menu::Util::GetInput()->IsPressed(true, 'S')) {
				Native::SetCamCoord(m_Vars.m_FreecamHandle, CamCoords.m_X - Infront.m_X * 2.f, CamCoords.m_Y - Infront.m_Y * 2.f, CamCoords.m_Z - Infront.m_Z * 2.f);
			}

			if (Menu::Util::GetInput()->IsPressed(true, 0x41)) {
				Math::Vector3<float> GameplayCamRotNew = GameCamRot;

				GameplayCamRotNew.m_X = 0.f;
				GameplayCamRotNew.m_Y = 0.f;
				GameplayCamRotNew.m_Z += 90.f;

				Math::Vector3<float> NewCoords = CamCoords + (Menu::Camera::GetCameraDirection(GameplayCamRotNew) * 2.f);
				Native::SetCamCoord(m_Vars.m_FreecamHandle, NewCoords.m_X, NewCoords.m_Y, NewCoords.m_Z);
			}

			if (Menu::Util::GetInput()->IsPressed(true, 0x44)) {
				Math::Vector3<float> GameplayCamRotNew = GameCamRot;

				GameplayCamRotNew.m_X = 0.f;
				GameplayCamRotNew.m_Y = 0.f;
				GameplayCamRotNew.m_Z -= 90.f;

				Math::Vector3<float> NewCoords = CamCoords + (Menu::Camera::GetCameraDirection(GameplayCamRotNew) * 2.f);
				Native::SetCamCoord(m_Vars.m_FreecamHandle, NewCoords.m_X, NewCoords.m_Y, NewCoords.m_Z);
			}

			CamCoords = Native::GetCamCoord(m_Vars.m_FreecamHandle);

			static int TIMER = 0;
			Menu::Timers::RunTimedFunction(&TIMER, 1000, [&] {
				Native::_SetFocusArea(CamCoords.m_X, CamCoords.m_Y, CamCoords.m_Z, 0, 0, 0);
			});
		}
	}
}

MiscCameraMenu* _instance;
MiscCameraMenu* MiscCameraMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new MiscCameraMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

MiscCameraMenu::~MiscCameraMenu() { delete _instance; }