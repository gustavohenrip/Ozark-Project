#include "misc.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../main.hpp"
#include "menu/base/util/stacked_text_render.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/notify.hpp"
#include "misc/misc_display.hpp"
#include "misc/misc_heads_up_display.hpp"
#include "misc/misc_camera.hpp"
#include "menu/base/renderer.hpp"

using namespace miscMenuVars;

namespace miscMenuVars {
	vars_ vars;

	scrollStruct<int> fps[] = {
		"FPS", 0,
		"Average", 1,
		"Both", 2
	};

	scrollStruct<float> metric[] = {
		"MPH", 2.236936f,
		"KPH", 3.6f,
		"Feet", 1.f
	};

	void noclip() {
		Entity entity = native::isPedOnMount(menu::getLocalPlayer().ped) ? menu::getLocalPlayer().mount : native::isPedInAnyVehicle(menu::getLocalPlayer().ped, 1) ? native::getVehiclePedIsIn(menu::getLocalPlayer().ped, 0) : menu::getLocalPlayer().ped;
		if (!vars.noclipMonitoring && (menu::util::getInput()->isPressed(1, VK_F8) || (menu::util::getInput()->isPressed(0, INPUT_FRONTEND_LS) && menu::util::getInput()->isPressed(0, INPUT_FRONTEND_RS)))) {//INPUT_FRONTEND_LS
			vars.noclipBind = !vars.noclipBind;
			native::setEntityCollision(entity, 1, 1); 
			native::freezeEntityPosition(entity, 0);
			native::setEntityVisible(entity, TRUE);
			native::clearAllHelpMessages();
			vars.noclipMonitoring = true;
		} else if (!menu::util::getInput()->isPressed(1, VK_F8)) {
			vars.noclipMonitoring = false;
		}

		if (!vars.noclipBind) {
			// display open bind here
		} else {
			menu::getRenderer()->displayTooltip("~INPUT_ATTACK~~INPUT_AIM~: Up/Down\n~INPUT_MOVE_LR~~INPUT_MOVE_UD~: Movement");
			native::setEntityVisible(entity, FALSE);
			math::vector3<float> camRot = native::getGameplayCamRotation(0);
			camRot.x = camRot.x - native::getControlNormal(0, INPUT_LOOK_UD);
			camRot.z = camRot.z - (native::getControlNormal(0, INPUT_LOOK_LR) * 7.f);
			native::setEntityCollision(entity, 1, 0);
			native::setEntityRotation(entity, { native::getEntityRotation(entity, 0).x, native::getEntityRotation(entity, 0).y, camRot.z }, 2, 1);
			// native::setGameplayRelativeHeading(.0f, 1065353216);
			// native::setGameplayRelativePitch(native::getGameplayRelativePitch(), .0f);
			native::freezeEntityPosition(entity, 1);
			math::vector3<float> offset = { .0f, .0f, .0f };
			offset.x = native::getControlNormal(0, INPUT_MOVE_LR) * vars.noclipSpeed;
			offset.y = -native::getControlNormal(0, INPUT_MOVE_UD) * vars.noclipSpeed;
			offset.z = (native::getControlNormal(0, INPUT_ATTACK) - native::getControlNormal(0, INPUT_AIM)) * vars.noclipSpeed;

			if (!offset.isZero()) {
				auto coords = native::getOffsetFromEntityInWorldCoords(entity, offset);
				native::setEntityCollision(entity, 0, 0);
				native::setEntityCoordsNoOffset(entity, coords.x, coords.y, coords.z, 0, 0, 0);
			}
		}
	}
}

void miscMenu::init() {
	setName("Misc");
	setParentSubmenu<mainMenu>();

	addString("~pa~F8~q~ to enable/disable noclip");

	addOption(submenuOption("Display")
		.addTranslation()
		.addSubmenu<miscDisplayMenu>());

	addOption(submenuOption("Heads up Display")
		.addTranslation()
		.addSubmenu<miscHeadsUpDisplayMenu>());

	addOption(submenuOption("Camera")
		.addTranslation()
		.addSubmenu<miscCameraMenu>());

	addOption(scrollOption<float>(SCROLL, "Metric Type")
		.addTranslation()
		.addScroll(vars.selectedMetric, 0, NUMOF(metric), metric).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Bypass Profanity Filter")
		.addTranslation()
		.addToggle(vars.bypassProfanityFilter).canBeSaved(name.getOriginal()));

	addOption(numberOption<float>(TOGGLE, "Noclip")
		.addTranslation()
		.addToggle(vars.noclip)
		.addNumber(vars.noclipSpeed, "%.01f", 0.2f).addMin(0.2f).addMax(10.f).canBeSaved(name.getOriginal())
		.addOnClick([this] {
			if (vars.noclip) {
				menu::getNotify()->bottom(getString("~pa~F8~q~ to enable/disable noclip"));
			}
		
			native::setEntityCollision(menu::getLocalPlayer().ped, 1, 1);
			native::freezeEntityPosition(menu::getLocalPlayer().ped, 0);
			native::setEntityVisible(menu::getLocalPlayer().ped, TRUE);
		})
		.addTooltip("~pa~F8~q~ or ~INPUT_FRONTEND_LS~ & ~INPUT_FRONTEND_RS~ to enable/disable noclip"));

	addOption(toggleOption("No Idle Kick")
		.addTranslation()
		.addToggle(vars.noIdleKick).canBeSaved(name.getOriginal()));
}

/*Called while in submenu*/
void miscMenu::update() {}

/*Called once on submenu open*/
void miscMenu::updateOnce() {}

/*Called always*/
void miscMenu::featureUpdate() {
	if (vars.noclip) {
		noclip();
	}

	if (vars.noIdleKick) {
		static uint32_t idkeKickTimer = 0;
		if (native::networkIsInSession() && native::isPlayerPlaying(menu::getLocalPlayer().id)) {
			menu::globals(1901947).at(50).at(16).as<int>() = 200000;   //-1549704562, 2000, "NPS_AFK_Start"
			menu::globals(1901947).at(50).at(17).as<int>() = 12000000; //-385707113, 120000, "NPS_AFK_Warning_0"
			menu::globals(1901947).at(50).at(18).as<int>() = 30000000; //-566198765, 300000, "NPS_AFK_Warning_1"
			menu::globals(1901947).at(50).at(19).as<int>() = 60000000; //-863708516, 600000, "NPS_AFK_Warning_2"
			menu::globals(1901947).at(50).at(20).as<int>() = 90000000; //-1835051675, 900000, "NPS_AFK_Kick"
			menu::globals(1901947).at(50).at(21).as<int>() = 200000;   //-1804689879, 2000, "NPS_IC_AFK_Start"
			menu::globals(1901947).at(50).at(22).as<int>() = 12000000; //-140783116, 120000, "NPS_IC_AFK_Warning_0"
			menu::globals(1901947).at(50).at(23).as<int>() = 18000000; //-516938467, 180000, "NPS_IC_AFK_Warning_1"
			menu::globals(1901947).at(50).at(24).as<int>() = 24000000; //-623175565, 240000, "NPS_IC_AFK_Warning_2"
			menu::globals(1901947).at(50).at(25).as<int>() = 30000000; //-2031369234, 300000, "NPS_IC_AFK_Kick"


			//menu::globals(1901947).at(789).at(0).as<float>() = .0f;
		//	menu::globals(1901947).at(789).at(1).as<float>() = .0f;
			//menu::globals(1901947).at(789).at(2).as<float>() = .0f;
			//Global_1901947.f_789 = func_403(-2037497938, 178944019, 30f, "MOONSHINE_BUSINESS_MINS_TO_CREATE_MOONSHINE_WEAK");
			//Global_1901947.f_789.f_1 = func_403(-2037497938, -41122219, 45f, "MOONSHINE_BUSINESS_MINS_TO_CREATE_MOONSHINE_DEFAULT");
			//Global_1901947.f_789.f_2 = func_403(-2037497938, -925810086, 60f, "MOONSHINE_BUSINESS_MINS_TO_CREATE_MOONSHINE_STRONG");
		}
	}
}

miscMenu* _instance;
miscMenu* miscMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new miscMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

miscMenu::~miscMenu() { delete _instance; }