#include "aimbot.hpp"
#include "menu/base/submenu_handler.hpp"
#include "weapon_mods.hpp"
#include "rage/types/base_types.hpp"
#include "menu/base/util/timers.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "menu/base/submenus/main/network/player_esp.hpp"

using namespace aimbotMenuVars;

namespace aimbotMenuVars {
	vars_ vars;

	scrollStruct<int> aimFilter[] = {
		"Player", 0,
		"Ped", 1,
		"Occupied Mount", 2,
		"All", 3,
	};

	scrollStruct<int> aimType[] = {
		"Snap", 0,
		"Silent", 1
	};

	scrollStruct<int> aimTag[] = {
		"Head", 0x5226,
		"Neck", 52596,
		"Pelvis", 33219,
		"Chest", 53684,
		"Penis", 10208,
		"Left Hand", 36029,
		"Right Hand", 6286,
		"Left Foot",65245,
		"Right Foot",35502

	};

	scrollStruct<int> aimCondition[] = {
		"Visible", 0,
		"Logical", 1
	};

#undef max


	void aimbot() {
		Ped nearestPed = 0;
		static int timer = 0;
		float distance = std::numeric_limits<float>::max();

		menu::pools::getPedPool()->run([&](Ped ped) { 
			if (!native::isPedDeadOrDying(ped, FALSE) && ((vars.filter == 3) || (vars.filter == 0 && native::isPedAPlayer(ped)) ||
				(vars.filter == 1 && !native::isPedAPlayer(ped)))) {
				if ((native::isEntityOnScreen(ped) || vars.condition) && networkPlayerEspMenuVars::hasEntityClearLosToEntity(native::getGameplayCamCoord(), ped)) {
					float tmp = native::getEntityCoords(ped).getMathmaticalDistance(menu::getLocalPlayer().coords);
					if (tmp < distance) {
						distance = tmp;
						nearestPed = ped;
					}
				}
			}
		});
		if (native::getEntityCoords(nearestPed).getMathmaticalDistance(menu::getLocalPlayer().coords) > 500.f) {
			nearestPed = 0;
		}
		if (vars.paintTarget) {
			networkPlayerEspMenuVars::screenEsp(nearestPed, { 255, 0, 0, 100 });
		}
		//auto coords = native::getEntityCoords(nearestPed);
		if ((native::isPlayerFreeAiming(menu::getLocalPlayer().id) || !vars.aimingRequired) && native::doesEntityExist(nearestPed)) {
			auto boneCoords = native::getPedBoneCoords(nearestPed, aimTag[vars.tag].result, 0.f, 0.f, 0.f);
			if (!vars.type) {
				auto coords = native::getOffsetFromEntityGivenWorldCoords(menu::getLocalPlayer().ped, boneCoords);
				float heading = native::getHeadingFromVector2d({ coords.x, coords.y });
				float pitch = (std::atan((coords.z / std::sqrt(((coords.y * coords.y) + (coords.x * coords.x))))) * (180.0f / 3.14159265358979323846f));
				native::setGameplayRelativePitch(pitch, 1.f);
				native::setGameplayRelativeHeading(heading, 1.f);
				native::setCameraHeadingBounds(-180.f, 180.f);
			}

			if (native::isControlPressed(0, INPUT_ATTACK) || vars.autoFire) {
				menu::util::getTimers()->runTimedFunction(&timer, vars.ms, [&] {
					native::setPedShootsAtCoord(menu::getLocalPlayer().ped, boneCoords.x, boneCoords.y, boneCoords.z, 0, 0, 0, -1.f);
				});
			}
		} 
	}
}

void aimbotMenu::init() {
	setName("Aimbot");
	setParentSubmenu<weaponModsMenu>();

	addOption(toggleOption("Enable Aimbot")
		.addTranslation()
		.addToggle(vars.enable).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Paint Target")
		.addTranslation()
		.addToggle(vars.paintTarget).canBeSaved(name.getOriginal()));

	addOption(numberOption<int>(SCROLL, "Bullet Buffer")
		.addTranslation()
		.addNumber(vars.ms, "%ims", 20).addMin(0).addMax(2000).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Aiming Required")
		.addTranslation()
		.addToggle(vars.aimingRequired).canBeSaved(name.getOriginal()));

	addOption(scrollOption<int>(SCROLL, "Aim Filter")
		.addTranslation()
		.addScroll(vars.filter, 0, NUMOF(aimFilter), aimFilter).canBeSaved(name.getOriginal()));

	addOption(scrollOption<int>(SCROLL, "Aim Type")
		.addTranslation()
		.addScroll(vars.type, 0, NUMOF(aimType), aimType).canBeSaved(name.getOriginal()));

	addOption(scrollOption<int>(SCROLL, "Aim Tag")
		.addTranslation()
		.addScroll(vars.tag, 0, NUMOF(aimTag), aimTag).canBeSaved(name.getOriginal()));

	addOption(scrollOption<int>(SCROLL, "Aim Condition")
		.addTranslation()
		.addScroll(vars.condition, 0, NUMOF(aimCondition), aimCondition).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Auto Fire")
		.addTranslation()
		.addToggle(vars.autoFire).canBeSaved(name.getOriginal()));
}

/*Called while in submenu*/
void aimbotMenu::update() {}

/*Called once on submenu open*/
void aimbotMenu::updateOnce() {}

/*Called always*/
void aimbotMenu::featureUpdate() {
	if (vars.enable) {
		aimbot();
	}
}

aimbotMenu* _instance;
aimbotMenu* aimbotMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new aimbotMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

aimbotMenu::~aimbotMenu() { delete _instance; }