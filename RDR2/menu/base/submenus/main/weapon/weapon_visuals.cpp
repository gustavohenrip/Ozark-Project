#include "weapon_visuals.hpp"
#include "menu/base/submenu_handler.hpp"
#include "weapon_mods.hpp"
#include "weapon_list.hpp"
#include "global/arrays.hpp"
#include "rage/engine.hpp"

using namespace visualMenuVars;

namespace visualMenuVars {
	vars_ vars;

	void drawCrosshair() {
		native::drawRect(0.5f, 0.5f, 0.013f, 0.002f, 74, 127, 184, 255);
		native::drawRect(0.5f, 0.5f, 0.0008f, 0.023f, 74, 127, 184, 255);
	}
}

void visualMenu::init() {
	setName("Visuals");
	setParentSubmenu<weaponModsMenu>();

	addOption(toggleOption("Crosshair")
		.addTranslation()
		.addToggle(vars.crosshair).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Invisibility")
		.addTranslation()
		.addToggle(vars.invisibility)
		.addOnClick([] { native::setEntityVisible(native::getCurrentPedWeaponEntityIndex(menu::getLocalPlayer().ped), !vars.invisibility); }));

	addOption(numberOption<int>(TOGGLE, "Weapon Opacity")
		.addTranslation()
		.addNumber(vars.weaponOpacityVar, "%i%%", 20).addMin(0).addMax(100)
		.addToggle(vars.weaponOpacity)
		.addOnClick([] {native::setEntityAlpha(native::getCurrentPedWeaponEntityIndex(menu::getLocalPlayer().ped), 255); }));
}

/*Called while in submenu*/
void visualMenu::update() {}

/*Called once on submenu open*/
void visualMenu::updateOnce() {}

/*Called always*/
void visualMenu::featureUpdate() {
	if (vars.invisibility) {
		Weapon weapon = native::getCurrentPedWeaponEntityIndex(menu::getLocalPlayer().ped);
		if (weapon) {
			native::setEntityVisible(weapon, 0);
		}

	}

	if (vars.weaponOpacity) {
		Weapon weapon = native::getCurrentPedWeaponEntityIndex(menu::getLocalPlayer().ped);
		if (weapon) {
			native::setEntityAlpha(weapon, (int)(255.f *((float)vars.weaponOpacityVar * .01f)));
		}
	}

	if (vars.crosshair) {
		drawCrosshair();
	}
}

visualMenu* _instance;
visualMenu* visualMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new visualMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

visualMenu::~visualMenu() { delete _instance; }