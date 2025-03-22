#include "spawn_manage.hpp"
#include "menu/base/submenu_handler.hpp"
#include "spawn_manage_selected.hpp"
#include "spawn_ped.hpp"
#include "spawn_vehicle.hpp"
#include "utils/va.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "menu/base/util/timers.hpp"
#include "menu/base/util/notify.hpp"

using namespace spawnManageMenuVars;

namespace spawnManageMenuVars {
	vars_ vars;
}

void spawnManageMenu::init() {
	setName("Manage Entity");
	setParentSubmenu<spawnManageMenu>();

	addString("~e~Error~q~: You don't have any spawned vehicles to manage");
	addString("~e~Error~q~: You don't have any spawned peds to manage");
}

/*Called while in submenu*/
void spawnManageMenu::update() {
	if (menu::getSubmenuHandler()->getPreviousSubmenu() == spawnVehicleMenu::getInstance()) {
		if (spawnVehicleMenuVars::vars.spawnedVehicles.size() == 0) {
			menu::getNotify()->right(getString("~e~Error~q~: You don't have any spawned vehicles to manage"));
			setParentSubmenu<spawnVehicleMenu>();
			menu::getSubmenuHandler()->setSubmenuPrevious();
		} else {
			if (menu::getBase()->currentOption <= spawnVehicleMenuVars::vars.spawnedVehicles.size()) {
				auto vehicle = spawnVehicleMenuVars::vars.spawnedVehicles[menu::getBase()->currentOption];
				menu::getPanelManager()->panelEntity(std::string("~o~" + std::string(vehicle.name)).c_str(), vehicle.handle, vehicle.creationTime, 2);
			}
		}
	}

	if (menu::getSubmenuHandler()->getPreviousSubmenu() == spawnPedMenu::getInstance()) {
		if (spawnPedMenuVars::vars.spawnedPeds.size() == 0) {
			menu::getNotify()->right(getString("~e~Error~q~: You don't have any spawned peds to manage"));
			setParentSubmenu<spawnPedMenu>();
			menu::getSubmenuHandler()->setSubmenuPrevious();
		} else {
			if (menu::getBase()->currentOption <= spawnPedMenuVars::vars.spawnedPeds.size()) {
				auto ped = spawnPedMenuVars::vars.spawnedPeds[menu::getBase()->currentOption];
				menu::getPanelManager()->panelEntity(std::string("~e~" + std::string(ped.name)).c_str(), ped.handle, ped.creationTime, 1);
			}
		}
	}

	static bool run = true;

	if (run) {
		run = false;
		clearOptionsOffset(0);

		if (menu::getSubmenuHandler()->getPreviousSubmenu() == spawnPedMenu::getInstance()) {
			setName("Manage Spawned Peds");
			setParentSubmenu<spawnPedMenu>();

			for (std::size_t i = 0; i < spawnPedMenuVars::vars.spawnedPeds.size(); i++) {
				addOption(submenuOption(utils::va::va("[%i] %s", i + 1, spawnPedMenuVars::vars.spawnedPeds[i].name))
					.addSubmenu<spawnManageSelectedMenu>()
					.addOnClick([=] {
						spawnManageSelectedMenuVars::vars.type = 1;
						spawnManageSelectedMenuVars::vars.handle = spawnPedMenuVars::vars.spawnedPeds[i].handle;
						spawnManageSelectedMenuVars::vars.invincibility = &spawnPedMenuVars::vars.spawnedPeds[i].invincibility;
						spawnManageSelectedMenuVars::vars.scale = &spawnPedMenuVars::vars.spawnedPeds[i].scale;
						spawnManageSelectedMenuVars::vars.creationOverlay = &spawnPedMenuVars::vars.spawnedPeds[i].creationOverlay;
						spawnManageSelectedMenuVars::vars.pedData = &spawnPedMenuVars::vars.spawnedPeds[i];
					}));
			}
		} else if (menu::getSubmenuHandler()->getPreviousSubmenu() == spawnVehicleMenu::getInstance()) {
			setName("Manage Spawned Vehicles");
			setParentSubmenu<spawnVehicleMenu>();

			for (std::size_t i = 0; i < spawnVehicleMenuVars::vars.spawnedVehicles.size(); i++) {
				addOption(submenuOption(utils::va::va("[%i] %s", i + 1, spawnVehicleMenuVars::vars.spawnedVehicles[i].name))
					.addSubmenu<spawnManageSelectedMenu>()
					.addOnClick([=] {
						spawnManageSelectedMenuVars::vars.type = 2;
						spawnManageSelectedMenuVars::vars.handle = spawnVehicleMenuVars::vars.spawnedVehicles[i].handle;
						spawnManageSelectedMenuVars::vars.invincibility = &spawnVehicleMenuVars::vars.spawnedVehicles[i].invincibility;
						spawnManageSelectedMenuVars::vars.creationOverlay = &spawnVehicleMenuVars::vars.spawnedVehicles[i].creationOverlay;
						spawnManageSelectedMenuVars::vars.vehicleData = &spawnVehicleMenuVars::vars.spawnedVehicles[i];
					}));
			}
		}
	}

	static int timer = 0;
	menu::util::getTimers()->runTimedFunction(&timer, 500, [] {
		run = true;
	});
}

/*Called once on submenu open*/
void spawnManageMenu::updateOnce() {}

/*Called always*/
void spawnManageMenu::featureUpdate() {}

spawnManageMenu* _instance;
spawnManageMenu* spawnManageMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new spawnManageMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

spawnManageMenu::~spawnManageMenu() { delete _instance; }