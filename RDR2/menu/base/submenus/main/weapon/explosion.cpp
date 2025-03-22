#include "explosion.hpp"
#include "menu/base/submenu_handler.hpp"
#include "weapon_mods.hpp"
#include "menu/base/util/raycast.hpp"
#include "menu/base/util/camera.hpp"
#include "menu/base/util/player_manager.hpp"

using namespace explosionMenuVars;

namespace explosionMenuVars {
	vars_ vars;

	scrollStruct<int> explosions[] = {
		{ "Grenade", 0 },
		{ "Small", 1 },
		{ "Molotov", 2 },
		{ "Volatile Molotov", 3 },
		{ "Hi-Octane", 4 },
		{ "Vehicle", 5 },
		{ "Medium Explosion", 6 },
		{ "Gas Fire", 7 },
		{ "Steam", 8 },
		{ "Flame", 9 },
		{ "Water", 10 },
		{ "Boat", 11 },
		{ "Bullet", 12 },
		{ "Smoke", 13 },
		{ "Bz Gas", 14 },
		{ "Gas Canister", 15 },
		{ "Extinguisher", 16 },
		{ "Train", 17 },
		{ "Flame Explode", 18 },
		{ "Vehicle bullet", 19 },
		{ "Bird Shit", 20 },
		{ "Firework", 21 },
		{ "Torpedo", 22 },
		{ "Underwater Torpedo", 23 },
		{ "Lantern", 24 },
		{ "Dynamite", 25 },
		{ "Dynamite Stick", 26 },
		{ "Dynamite Volatile", 27 },
		{ "River Blast", 28 },
		{ "Placed Dynamite", 29 },
		{ "Fire Arrow", 30 },
		{ "Dynamite Arrow", 31 },
		{ "Phosforus Bullet", 32 },
		{ "Lightning Strike", 33 },
		{ "Tracking Arrow", 34 },
		{ "Poison", 35 },
	
	};

	void explosionGun(Ped owner,int type, float radius, bool audible, bool invisible, float cameraShake) {
		if (native::isPedShooting(menu::getLocalPlayer().ped) || 
				(weaponModsMenuVars::vars.rapidFire && (menu::util::getInput()->isPressed(0, 1877832124) || menu::util::getInput()->isPressed(1, VK_LBUTTON)))) {
			menu::raycast cast = menu::raycastInfront(9999.f);
			native::addOwnedExplosion(owner, cast.getCoords(), type, radius, audible, invisible, cameraShake);
		}
	}
}

void explosionMenu::init() {
	setName("Explosions");
	setParentSubmenu<weaponModsMenu>();

	//addOption(toggleOption("Explosive ")
	//	.addToggle(vars.triggerBot)
	//	.addTooltip(""));

	addOption(scrollOption<int>(TOGGLE, "Enable Explosion Gun")
		.addTranslation()
		.addScroll(vars.type, 0, NUMOF(explosions) - 1, explosions)
		.addToggle(vars.explosionGun)
		.addTooltip(""));

	addOption(breakOption("Config")
		.addTranslation());

	addOption(toggleOption("Audible")
		.addTranslation()
		.addToggle(vars.audible)
		.addTooltip(""));

	addOption(toggleOption("Invisible")
		.addTranslation()
		.addToggle(vars.invisible)
		.addTooltip(""));

	addOption(numberOption<float>(SCROLL, "Radius")
		.addTranslation()
		.addNumber(vars.radius, "%.1f", 0.1f).addMin(0.f).addMax(100.f)
		.addTooltip(""));

	addOption(numberOption<float>(SCROLL, "Camera Shake")
		.addTranslation()
		.addNumber(vars.cameraShake, "%.1f", 0.1f).addMin(0.f).addMax(100.f)
		.addTooltip(""));

	addOption(scrollOption<int>(TOGGLE, "Owner")
		.addTranslation()
		.addToggle(vars.changeOwner)
		.addScroll(vars.changeOwnerVal, 0, NUMOF(vars.changeOwnerList), vars.changeOwnerList)
		.addOnUpdate([] (scrollOption<int>* option) { option->addScroll(vars.changeOwnerVal, 0, vars.listSize, vars.changeOwnerList); }));
}

/*Called while in submenu*/
void explosionMenu::update() {
	int size = 0;
	menu::getPlayerManager()->foreach([&] (menu::playerVars& var) {
		vars.changeOwnerList[size].name = var.name;
		vars.changeOwnerList[size].result = var.ped;
		size++;
	});

	vars.listSize = size;
}

/*Called once on submenu open*/
void explosionMenu::updateOnce() {}

/*Called always*/
void explosionMenu::featureUpdate() {
	if (vars.explosionGun) {
		explosionGun(vars.changeOwner ? vars.changeOwnerList[vars.changeOwnerVal].result : menu::getLocalPlayer().ped, vars.type, vars.radius, vars.audible, vars.invisible, vars.cameraShake);
	}
}

explosionMenu* _instance;
explosionMenu* explosionMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new explosionMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

explosionMenu::~explosionMenu() { delete _instance; }
