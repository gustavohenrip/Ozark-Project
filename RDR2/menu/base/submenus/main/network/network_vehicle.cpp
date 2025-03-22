#include "network_vehicle.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network/players.hpp"
#include "src/menu/base/util/fiber_manager.hpp"
#include "src/menu/base/util/fiber_pool.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "menu/base/util/control_manager.hpp"

using namespace networkVehicleMenuVars;

namespace networkVehicleMenuVars {
	vars_ vars;

	void launchVehicle(menu::playerVars& player) {
		if (player.inVehicle) {
			menu::getControlManager()->requestControl(player.vehicle, [](Entity vehicle) {
				math::vector3<float> velocity = native::getEntityVelocity(vehicle, 0);
				native::setEntityVelocity(vehicle, { velocity.x, velocity.y, (velocity.z + 9999.f) * 20.f });
			});
		}
	}

	void fixVehicle(menu::playerVars& player) {
		if (player.inVehicle) {
			menu::getControlManager()->requestControl(player.vehicle, [](Entity vehicle) {
				native::setVehicleFixed(vehicle);
			});
		}
	}
}

void networkVehicleMenu::init() {
	setName("Vehicle");
	setParentSubmenu<networkPlayersMenu>();

	addOption(buttonOption("No vehicle found")
		.addTranslation()
		.addRequirement([] { return !menu::getSelectedPlayer().inVehicle; })
		.addOnUpdate([](buttonOption* option) { option->addRequirement([] { return !menu::getSelectedPlayer().inVehicle; }); }));

	addOption(buttonOption("Fix Vehicle")
		.addTranslation()
		.addOnClick([] { fixVehicle(menu::getSelectedPlayer()); })
		.addRequirement([] { return menu::getSelectedPlayer().inVehicle; })
		.addOnUpdate([](buttonOption* option) { option->addRequirement([] { return menu::getSelectedPlayer().inVehicle; }); }));

	addOption(buttonOption("Launch")
		.addTranslation()
		.addOnClick([] { launchVehicle(menu::getSelectedPlayer()); })
		.addRequirement([] { return menu::getSelectedPlayer().inVehicle; })
		.addOnUpdate([](buttonOption* option) { option->addRequirement([] { return menu::getSelectedPlayer().inVehicle; }); }));

	addOption(buttonOption("Explode")
		.addTranslation()
		.addOnClick([] { native::explodeVehicle(menu::getSelectedPlayer().vehicle, TRUE, FALSE, 1); })
		.addRequirement([] { return menu::getSelectedPlayer().inVehicle; })
		.addOnUpdate([](buttonOption* option) { option->addRequirement([] { return menu::getSelectedPlayer().inVehicle; }); }));

}

/*Called while in submenu*/
void networkVehicleMenu::update() {
	menu::getPanelManager()->panelPlayer(menu::getSelectedPlayer().id);
}

/*Called once on submenu open*/
void networkVehicleMenu::updateOnce() {}

/*Called always*/
void networkVehicleMenu::featureUpdate() {}

networkVehicleMenu* _instance;
networkVehicleMenu* networkVehicleMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkVehicleMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkVehicleMenu::~networkVehicleMenu() { delete _instance; }