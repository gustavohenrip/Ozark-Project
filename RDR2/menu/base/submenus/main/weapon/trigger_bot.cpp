#include "trigger_bot.hpp"
#include "menu/base/submenu_handler.hpp"
#include"weapon_mods.hpp"
#include "aimbot.hpp"
#include "menu/base/util/timers.hpp"


using namespace weaponTriggerBotMenuVars;

namespace weaponTriggerBotMenuVars {
	vars_ vars;
	
	void triggerBot(int bone) {
		Entity entity;
		static int timer = 0;
		if (native::getEntityPlayerIsfreeAimingAt(menu::getLocalPlayer().id, &entity)) {
			if (native::isEntityAPed(entity) && !native::isEntityDead(entity) && !native::isPedReloading(menu::getLocalPlayer().ped)) {
				if (vars.filter == 3) {
					goto triggerbot;
				} else if (vars.filter == 2) {
					if (native::isPedOnMount(entity)) {
						entity = native::getMount(entity);
					} else {
						return;
					}
				} else if (vars.filter == 1) {
					if (!native::isPedAPlayer(entity)) {
						goto triggerbot;
					} else {
						return;
					}
				} else if (vars.filter == 0) {
					if (native::isPedAPlayer(entity)) {
						goto triggerbot;
					} else {
						return;
					}
				}

			triggerbot:
				math::vector3<float> head = native::getPedBoneCoords(entity, bone, 0.f, 0.f, 0.f);//0x5226
				menu::util::getTimers()->runTimedFunction(&timer, vars.ms, [&] {
					native::setPedShootsAtCoord(menu::getLocalPlayer().ped, head.x, head.y, head.z, 0, 0, 0, -1.f);
				});
			}
		}
	}

	//void idkwhatimdoingtest() {
	//	math::vector3<float> coords;
	//	_i<int>(0xB90411F480457A6C, FALSE);
	//	native::setGameplayCoordHint(coords, -1, 0, 0, 0);
	//	native::taskLookAtCoord(menu::getLocalPlayer().ped, coords, -1, 2048, 0, 0);
	//}


}

void weaponTriggerBotMenu::init() {
	setName("Triggerbot");
	setParentSubmenu<weaponModsMenu>();

	addOption(toggleOption("Enable Triggerbot")
		.addTranslation()
		.addToggle(vars.triggerBot).canBeSaved(name.getOriginal()));

	addOption(breakOption("Config")
		.addRequirement([] { return vars.triggerBot; }));

	addOption(numberOption<int>(SCROLL, "Bullet Buffer")
		.addTranslation()
		.addNumber(vars.ms, "%ims", 20).addMin(0).addMax(2000).canBeSaved(name.getOriginal())
		.addRequirement([] { return vars.triggerBot; }));

	addOption(scrollOption<int>(SCROLL, "Bone")
		.addTranslation()
		.addScroll(vars.boneIndex, 0, NUMOF(aimbotMenuVars::aimTag), aimbotMenuVars::aimTag).canBeSaved(name.getOriginal())
		.addRequirement([] { return vars.triggerBot; }));

	addOption(scrollOption<int>(SCROLL, "Aim Filter")
		.addTranslation()
		.addScroll(vars.filter, 0, NUMOF(aimbotMenuVars::aimFilter), aimbotMenuVars::aimFilter).canBeSaved(name.getOriginal())
		.addRequirement([] { return vars.triggerBot; }));

}

/*Called while in submenu*/
void weaponTriggerBotMenu::update() {}

/*Called once on submenu open*/
void weaponTriggerBotMenu::updateOnce() {}

/*Called always*/
void weaponTriggerBotMenu::featureUpdate() {
	if (vars.triggerBot) {
		triggerBot(aimbotMenuVars::aimTag[vars.boneIndex].result);
	}
}

weaponTriggerBotMenu* _instance;
weaponTriggerBotMenu* weaponTriggerBotMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new weaponTriggerBotMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

weaponTriggerBotMenu::~weaponTriggerBotMenu() { delete _instance; }