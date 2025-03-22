#include "pickups.hpp"
#include "menu/base/submenu_handler.hpp"
#include "nearby.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "menu/base/submenus/main/teleport.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "rage/entity/entity.hpp"

using namespace pickupsMenuVars;

namespace pickupsMenuVars {
	vars_ vars;

	scrollStruct<int> teleport[2] = {
		{ "To Me", 0 },
		{ "Nearest Player", 1 }
	};

	void teleportType(int type) {
		menu::pools::getPickupPool()->run([=](Pickup pickup) {
			math::vector3<float> coords;
			switch (type) {
				case 0: native::setEntityCoords(pickup, menu::getLocalPlayer().coords); break;
				case 1: native::setEntityCoords(pickup, native::getEntityCoords(native::getPlayerPed(teleportMenuVars::getClosestPlayer()))); break;
				case 2: native::setEntityCoords(pickup, native::getEntityCoords(native::getPlayerPed(teleportMenuVars::getClosestPlayer()))); break;
			}
		});
	}

	void deletePickup(Pickup pickup) {
		if (native::networkIsInSession()) {
			rage::entity::getEntityTask()->changeNetObjOwner(pickup, menu::getLocalPlayer().id);
		}

		native::setEntityAsMissionEntity(pickup, TRUE, TRUE);
		native::deleteEntity(&pickup);
	}

	void esp() {
		menu::pools::getPickupPool()->run([] (Pickup pickup) {
			menu::getRenderer()->drawLine(menu::getLocalPlayer().coords, native::getEntityCoords(pickup), { 0, 159, 116, 255 } );
		});
	}
}

void pickupsMenu::init() {
	setName("Pickups");
	setParentSubmenu<nearbyMenu>();

	addOption(scrollOption<int>(SCROLLSELECT, "Teleport")
		.addTranslation()
		.addScroll(vars.teleportIndex, 0, NUMOF(teleport), teleport)
		.addOnClick([] { teleportType(vars.teleportIndex); }));

	addOption(buttonOption("Delete")
		.addTranslation()
		.addOnClick([] { menu::pools::getPickupPool()->run([=](Pickup pickup) { deletePickup(pickup); }); }));

	addOption(breakOption("Config")
		.addTranslation());

	addOption(toggleOption("ESP")
		.addTranslation()
		.addToggle(vars.esp));
}

/*Called while in submenu*/
void pickupsMenu::update() {
	if (vars.esp) esp();
	menu::getPanelManager()->panelPool("Pickup", menu::pools::getPickupPool()->getPoolPointer(), 4);
}

/*Called once on submenu open*/
void pickupsMenu::updateOnce() {}

/*Called always*/
void pickupsMenu::featureUpdate() {}

pickupsMenu* _instance;
pickupsMenu* pickupsMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new pickupsMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

pickupsMenu::~pickupsMenu() { delete _instance; }