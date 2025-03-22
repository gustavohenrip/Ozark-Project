#include "misc_camera.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../misc.hpp"
#include "menu/base/util/camera.hpp"
#include "menu/base/util/timers.hpp"

using namespace miscCameraMenuVars;

namespace miscCameraMenuVars {
	vars_ vars;
}

void miscCameraMenu::init() {
	setName("Camera");
	setParentSubmenu<miscMenu>();

	addOption(toggleOption("Freecam")
		.addTranslation()
		.addToggle(vars.freecam)
		.addOnClick([] {
			native::clearFocus();
			if (!vars.freecam) {
				native::setFocusPosAndLevel(menu::getLocalPlayer().coords, { 0.f, 0.f, 0.f });
				if (native::doesCamExist(vars.freecamHandle)) {
					native::setCamActive(vars.freecamHandle, FALSE);
					native::renderScriptCams(FALSE, TRUE, 3000, TRUE, FALSE, TRUE);
					native::destroyCam(vars.freecamHandle, FALSE);
				}

				native::clearPedTasksImmediately(menu::getLocalPlayer().ped, FALSE, FALSE);
			} else {
				vars.lastFreecamCoords = menu::getLocalPlayer().coords;
				vars.lastFreecamRotation = { 0.f, 0.f, 0.f };
				vars.freecamHandle = 0;
			}
		}));

	addOption(toggleOption("Birds-eye-view")
		.addTranslation()
		.addToggle(vars.birdsEyeView)
		.addOnClick([] {
			if (!vars.birdsEyeView) {
				if (native::doesCamExist(vars.birdsEyeViewHandle)) {
					native::setCamActive(vars.birdsEyeViewHandle, FALSE);
					native::renderScriptCams(FALSE, TRUE, 3000, TRUE, FALSE, TRUE);
					native::destroyCam(vars.birdsEyeViewHandle, FALSE);
				}
			}
		}));

	addOption(numberOption<float>(TOGGLE, "Camera Zoom")
		.addTranslation()
		.addToggle(vars.cameraZoom)
		.addNumber(vars.cameraZoomVal, "%.0f", 1.f).addMin(0).addMax(1000.f).setScrollSpeed(10));
}

/*Called while in submenu*/
void miscCameraMenu::update() {}

/*Called once on submenu open*/
void miscCameraMenu::updateOnce() {}

/*Called always*/
void miscCameraMenu::featureUpdate() {
	if (vars.cameraZoom) {
		native::animateGameplayCamZoom(1.f, vars.cameraZoomVal);
	}

	if (vars.birdsEyeView) {
		if (!native::doesCamExist(vars.birdsEyeViewHandle)) {
			vars.birdsEyeViewHandle = native::createCam("DEFAULT_SCRIPTED_CAMERA", 0);
			native::attachCamToPedBone(vars.birdsEyeViewHandle, menu::getLocalPlayer().ped, 0x5226, 0.0f, 0.13f, 23.0f, 1.0f);
			native::setCamActive(vars.birdsEyeViewHandle, TRUE);
			native::renderScriptCams(TRUE, FALSE, 1000, TRUE, FALSE, FALSE);
		} else {
			auto Rotation = native::getGameplayCamRotation(2);
			native::setCamRotation(vars.birdsEyeViewHandle, -88.f, Rotation.y, Rotation.z, 2);
		}
	}

	if (vars.freecam) {
		if (!native::doesCamExist(vars.freecamHandle)) {
			if (vars.freecamHandle) {
				native::setCamActive(vars.freecamHandle, FALSE);
				native::renderScriptCams(FALSE, TRUE, 3000, TRUE, FALSE, TRUE);
				native::destroyCam(vars.freecamHandle, FALSE);
			}

			vars.freecamHandle = native::createCam("DEFAULT_SCRIPTED_CAMERA", 0);
			native::setCamActive(vars.freecamHandle, TRUE);
			native::renderScriptCams(TRUE, FALSE, 3000, TRUE, FALSE, TRUE);
			native::setCamCoord(vars.freecamHandle, vars.lastFreecamCoords.x, vars.lastFreecamCoords.y, vars.lastFreecamCoords.z);
			native::setCamRotation(vars.freecamHandle, vars.lastFreecamRotation.x, vars.lastFreecamRotation.y, vars.lastFreecamRotation.z, 2);
		} else {
			if (menu::getLocalPlayer().inVehicle) {
				native::setVehicleForwardSpeed(menu::getLocalPlayer().entity, 0.f);
			} else {
				native::taskStandStill(menu::getLocalPlayer().ped, 10);
			}

			auto coords = native::getCamCoord(vars.freecamHandle);
			auto rot = native::getGameplayCamRotation(0);
			native::setCamRotation(vars.freecamHandle, rot.x, rot.y, rot.z, 2);

			auto infront = menu::getCamera()->getCameraDirection();

			// W
			if (menu::util::getInput()->isPressed(true, 0x57)) {
				native::setCamCoord(vars.freecamHandle, coords.x + infront.x * 2.f, coords.y + infront.y * 2.f, coords.z + infront.z * 2.f);
			}

			// S
			if (menu::util::getInput()->isPressed(true, 0x53)) {
				native::setCamCoord(vars.freecamHandle, coords.x - infront.x * 2.f, coords.y - infront.y * 2.f, coords.z - infront.z * 2.f);
			}

			// A
			if (menu::util::getInput()->isPressed(true, 0x41)) {
				auto rotBackup = rot;
				rotBackup.x = 0.f;
				rotBackup.y = 0.f;
				rotBackup.z += 90.f;

				auto newCoords = coords + (menu::getCamera()->rotateToDirection(rotBackup) * 2.f);
				native::setCamCoord(vars.freecamHandle, newCoords.x, newCoords.y, newCoords.z);
			}

			// D
			if (menu::util::getInput()->isPressed(true, 0x44)) {
				auto rotBackup = rot;
				rotBackup.x = 0.f;
				rotBackup.y = 0.f;
				rotBackup.z -= 90.f;

				auto newCoords = coords + (menu::getCamera()->rotateToDirection(rotBackup) * 2.f);
				native::setCamCoord(vars.freecamHandle, newCoords.x, newCoords.y, newCoords.z);
			}

			vars.lastFreecamCoords = native::getCamCoord(vars.freecamHandle);
			vars.lastFreecamRotation = rot;

			static int camTimer = 0;
			menu::util::getTimers()->runTimedFunction(&camTimer, 1000, [&] {
				native::setFocusPosAndLevel({ vars.lastFreecamCoords.x, vars.lastFreecamCoords.y, vars.lastFreecamCoords.z }, { 0.f, 0.f, 0.f });
			});
		}
	}
}

miscCameraMenu* _instance;
miscCameraMenu* miscCameraMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new miscCameraMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

miscCameraMenu::~miscCameraMenu() { delete _instance; }