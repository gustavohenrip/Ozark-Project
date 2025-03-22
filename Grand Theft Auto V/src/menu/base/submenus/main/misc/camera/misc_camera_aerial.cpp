#include "misc_camera_aerial.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../misc_camera.hpp"

using namespace MiscCameraAerialMenuVars;

namespace MiscCameraAerialMenuVars {
	Vars_ m_Vars;
}

void MiscCameraAerialMenu::Init() {
	SetName("Aerial Camera");
	SetParentSubmenu<MiscCameraMenu>();

	addOption(ToggleOption("Toggle Camera")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_ToggleCamera)
		.addOnClick([] {
			if (!Native::DoesCamExist(m_Vars.m_AerialCam)) {
				m_Vars.m_AerialCam = Native::CreateCamera(26379945, true);
				Native::SetCamActive(m_Vars.m_AerialCam, true);
				Native::RenderScriptCams(true, true, 1000, true, false, 0);
			} else {
				Native::DestroyCam(m_Vars.m_AerialCam, true);
				Native::RenderScriptCams(false, true, 1000, true, false, 0);
			}
		}));

	addOption(NumberOption<int>(SCROLL, "Camera Distance")
		.addTranslation()
		.addNumber(m_Vars.m_Distance, "%i", 1).addMin(0).addMax(300).setScrollSpeed(10));
}

void MiscCameraAerialMenu::Update() {}

/*Called once on submenu open*/
void MiscCameraAerialMenu::UpdateOnce() {}

/*Called always*/
void MiscCameraAerialMenu::FeatureUpdate() {
	if (m_Vars.m_ToggleCamera) {
		if (Native::DoesCamExist(m_Vars.m_AerialCam)) {
			Math::Vector3<float> Coords = Native::GetOffsetFromEntityInWorldCoords(Menu::GetLocalPlayer().m_Entity, 0, (m_Vars.m_Distance / 2) * -1, m_Vars.m_Distance);
			Coords.m_Z = Menu::GetLocalPlayer().m_Coords.m_Z + m_Vars.m_Distance;
			Native::SetCamCoord(m_Vars.m_AerialCam, Coords.m_X, Coords.m_Y, Coords.m_Z);
			Native::PointCamAtEntity(m_Vars.m_AerialCam, Menu::GetLocalPlayer().m_Entity, 0, 0, 0, true);
		}
	}
}

MiscCameraAerialMenu* _instance;
MiscCameraAerialMenu* MiscCameraAerialMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new MiscCameraAerialMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

MiscCameraAerialMenu::~MiscCameraAerialMenu() { delete _instance; }