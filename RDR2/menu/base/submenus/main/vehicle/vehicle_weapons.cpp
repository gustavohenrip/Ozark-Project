#include "vehicle_weapons.hpp"
#include "menu/base/submenu_handler.hpp"
#include "vehicle_mods.hpp"
#include "menu/base/util/timers.hpp"

using namespace vehicleWeaponsMenuVars;

namespace vehicleWeaponsMenuVars {
	vars_ vars;

	scrollStruct<Hash> weapons[] = {
		{ "M1899 Pistol", 0x5b78b8dd },
		{ "Carbine Repeater", 0xf5175ba1 },
		{ "Cattleman Revolver", 0x169f59f7 },
		{ "Exotic Double Action", 0x23c706cd },
		{ "Lemat Revolver", 0x5b2d26b5 },
		{ "Golden Schofield", 0xe195d259 },
		{ "Boltaction Rifle", 0x772c8dd6 },
		{ "Exotic Doublebarrel", 0x2250e150 },
		{ "Bow", 0x88a8505c }
	};
}

void vehicleWeaponsMenu::init() {
	setName("Weapons");
	setParentSubmenu<vehicleModsMenu>();

	addOption(toggleOption("Enable Weapons")
		.addTranslation()
		.addToggle(vars.enabled).canBeSaved(name.getOriginal()));

	addOption(scrollOption<Hash>(SCROLL, "Weapon")
		.addTranslation()
		.addScroll(vars.weapon, 0, NUMOF(weapons), weapons).canBeSaved(name.getOriginal())
		.addRequirement([] { return vars.enabled; }));

	addOption(numberOption<float>(SCROLL, "Speed")
		.addTranslation()
		.addNumber(vars.speed, "%.0f", 100.f).addMin(1.f).addMax(10000.f).setScrollSpeed(10).canBeSaved(name.getOriginal())
		.addRequirement([] { return vars.enabled; }));

	addOption(toggleOption("Take the Blame")
		.addTranslation()
		.addToggle(vars.takeTheBlame).canBeSaved(name.getOriginal())
		.addRequirement([] { return vars.enabled; }));

	addOption(toggleOption("Guidelines")
		.addTranslation()
		.addToggle(vars.guidelines).canBeSaved(name.getOriginal())
		.addRequirement([] { return vars.enabled; }));
}

/*Called while in submenu*/
void vehicleWeaponsMenu::update() {}

/*Called once on submenu open*/
void vehicleWeaponsMenu::updateOnce() {}

/*Called always*/
void vehicleWeaponsMenu::featureUpdate() {
	if (menu::getLocalPlayer().inVehicle) {
		if (vars.enabled) {
			math::vector3<float> modelSizeMin;
			math::vector3<float> modelSizeMax;
			native::getModelDimensions(native::getEntityModel(menu::getLocalPlayer().vehicle), &modelSizeMin, &modelSizeMax);

			math::vector3<float> vehL = native::getOffsetFromEntityInWorldCoords(menu::getLocalPlayer().vehicle, { (modelSizeMax.x / 2.f) + 0.1f, modelSizeMax.y, 0.01f });
			math::vector3<float> vehR = native::getOffsetFromEntityInWorldCoords(menu::getLocalPlayer().vehicle, { (modelSizeMax.x / 2.f) * -1.f - 0.1f, modelSizeMax.y, 0.01f });
			math::vector3<float> vehLEnd = native::getOffsetFromEntityInWorldCoords(menu::getLocalPlayer().vehicle, { (modelSizeMax.x / 2.f) + 0.1f, 2000.f, 0.01f });
			math::vector3<float> vehREnd = native::getOffsetFromEntityInWorldCoords(menu::getLocalPlayer().vehicle, { (modelSizeMax.x / 2.f) * -1.f - 0.1f, 2000.f, 0.01f });

			if (native::isControlPressed(0, INPUT_JUMP)) {
				static int timer = 0;
				menu::util::getTimers()->runTimedFunction(&timer, 250, [&] {
					native::shootSingleBulletBetweenCoords(vehL, vehLEnd, 5000, FALSE, weapons[vars.weapon].result, 0, TRUE, FALSE, vars.speed, TRUE);
					native::shootSingleBulletBetweenCoords(vehR, vehREnd, 5000, FALSE, weapons[vars.weapon].result, 0, TRUE, FALSE, vars.speed, TRUE);
				});
			}

			if (vars.guidelines) {
				menu::getRenderer()->drawLine(vehL, vehLEnd, { 255, 0, 0, 255 });
				menu::getRenderer()->drawLine(vehR, vehREnd, { 255, 0, 0, 255 });
			}
		}
	}
}

vehicleWeaponsMenu* _instance;
vehicleWeaponsMenu* vehicleWeaponsMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new vehicleWeaponsMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

vehicleWeaponsMenu::~vehicleWeaponsMenu() { delete _instance; }