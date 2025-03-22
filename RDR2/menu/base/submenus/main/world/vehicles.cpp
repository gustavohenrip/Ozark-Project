#include "vehicles.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../world/nearby.hpp"
#include "menu/base/util/raycast.hpp"
#include "menu/base/util/camera.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "rage/entity/entity.hpp"
#include "menu/base/submenus/main/teleport.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "ped.hpp"

using namespace vehiclesMenuVars;

namespace vehiclesMenuVars {
	vars_ vars;

	scrollStruct<int> teleport[3] = {
		{ "Aimed", 0 },
		{ "To Me", 1 },
		{ "Nearest Player", 2 }
	};

	void toAimedPos() {
		menu::raycast cast = menu::raycastInfront(9999.f);
		if (native::isPlayerFreeAiming(menu::getLocalPlayer().id)) {
			auto coords = cast.getCoords();
			menu::pools::getVehiclePool()->run([=](Vehicle vehicle) { native::setEntityCoords(vehicle, coords); });
		}
	}

	void teleportType(int type) {
		menu::raycast cast = menu::raycastInfront(9999.f);
		auto rayCoords = cast.getCoords();
		menu::pools::getVehiclePool()->run([=](Vehicle vehicle)-> void {
			math::vector3<float> coords;
			switch (type) {
				case 0: native::setEntityCoords(vehicle, rayCoords); break;
				case 1: native::setEntityCoords(vehicle, menu::getLocalPlayer().coords); break;
				case 2: native::setEntityCoords(vehicle, native::getEntityCoords(native::getPlayerPed(teleportMenuVars::getClosestPlayer()))); break;
			}
		});
	}

	void esp() {
		menu::pools::getVehiclePool()->run([](Vehicle vehicle) {
			menu::getRenderer()->drawLine(menu::getLocalPlayer().coords, native::getEntityCoords(vehicle), { 254, 243, 144, 255 });
		});
	}
}

void vehiclesMenu::init() {
	setName("Vehicles");
	setParentSubmenu<nearbyMenu>();

	addOption(scrollOption<int>(SCROLLSELECT, "Teleport")
		.addTranslation()
		.addScroll(vars.teleportIndex, 0, NUMOF(teleport), teleport)
		.addOnClick([] { teleportType(vars.teleportIndex); }));

	addOption(buttonOption("Launch")
		.addTranslation()
		.addOnClick([] {
			menu::pools::getVehiclePool()->run([](Vehicle vehicle) {
				pedMenuVars::launchUp(vehicle);
			});
		}));

	addOption(buttonOption("Delete")
		.addTranslation()
		.addOnClick([] {
		menu::pools::getVehiclePool()->run([=] (Vehicle vehicle) {
			native::setEntityAsMissionEntity(vehicle, 1, 1);
			native::setEntityCoords(vehicle, { 4503.726f, -635.393f, 71.421f });
			native::deleteEntity2(&vehicle);
		});
	}));

	addOption(breakOption("Config")
		.addTranslation());

	addOption(toggleOption("ESP")
		.addTranslation()
		.addToggle(vars.esp));
}

/*Called while in submenu*/
void vehiclesMenu::update() {
	if (vars.esp) esp();
	menu::getPanelManager()->panelPool("Vehicle", menu::pools::getVehiclePool()->getPoolPointer(), 2);

	// rgb color: { 254, 243, 144, 255 }
}

/*Called once on submenu open*/
void vehiclesMenu::updateOnce() {}

/*Called always*/
void vehiclesMenu::featureUpdate() {}

vehiclesMenu* _instance;
vehiclesMenu* vehiclesMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new vehiclesMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

vehiclesMenu::~vehiclesMenu() { delete _instance; }