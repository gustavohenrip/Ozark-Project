#include "spawn_manage_selected.hpp"
#include "menu/base/submenu_handler.hpp"
#include "spawn_ped.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/submenus/main/weapon/weapons.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "menu/base/submenus/main/weapon/weapon_mods.hpp"

using namespace spawnManageSelectedMenuVars;

namespace spawnManageSelectedMenuVars {
	vars_ vars;
}

void spawnManageSelectedMenu::init() {
	setName("Spawn Manager");
	setParentSubmenu<spawnPedMenu>();

	addString("Manage Spawned Peds");
	addString("Manage Spawned Vehicles");

	addString("~e~Error~q~: Entity does not exist!");
	addString("Sprint to Me");
	addString("Jump");
	addString("Put Hands Up");
	addString("Employ as Bodyguard");
	addString("Give Weapon");
	addString("Kill");
	addString("Smite");
	addString("Scale");

	addString("Place on Ground");
	addString("Creation Overlay");
	addString("Warp Into");
	addString("Teleport to Me");
	addString("Teleport to Entity");
	addString("Invincibility");
	addString("Attach to Player");
	addString("Detach");
	addString("Delete");
}

/*Called while in submenu*/
void spawnManageSelectedMenu::update() {
	if (!native::doesEntityExist(vars.handle)) {
		menu::getNotify()->right(getString("~e~Error~q~: Entity does not exist!"));
		menu::getSubmenuHandler()->setSubmenuPrevious();
	}

	int size = 0;
	menu::getPlayerManager()->foreach([&] (menu::playerVars& var) {
		vars.attachList[size].name = var.name;
		vars.attachList[size].result = var.ped;
		size++;
	});

	vars.listSize = size;

	if (vars.type == 1) {
		menu::getPanelManager()->panelEntity(std::string("~e~" + std::string(vars.pedData->name)).c_str(), vars.handle, vars.pedData->creationTime, 1);
	} else if (vars.type == 2) {
		menu::getPanelManager()->panelEntity(std::string("~o~" + std::string(vars.vehicleData->name)).c_str(), vars.handle, vars.vehicleData->creationTime, 2);
	}
}

/*Called once on submenu open*/
void spawnManageSelectedMenu::updateOnce() {
	clearOptionsOffset(0);

	// ped
	if (vars.type == 1) {
		setName(getString("Manage Spawned Peds"), false);
		setParentSubmenu<spawnPedMenu>();

		// Animations?

		addOption(buttonOption(getString("Sprint to Me"))
			.addOnClick([] {
				native::taskGoStraightToCoord(vars.handle, menu::getLocalPlayer().coords, 10.f, 99999, 0.f, 0.f);
			}));

		addOption(buttonOption(getString("Jump"))
			.addOnClick([] {
				native::taskJump(vars.handle, FALSE);
			}));

		addOption(buttonOption(getString("Put Hands Up"))
			.addOnClick([] {
				native::taskHandsUp(vars.handle, 5000, 0, 0, 0);
			}));

		addOption(buttonOption(getString("Employ as Bodyguard"))
			.addOnClick([] {
				int myGroup = native::getPlayerGroup(menu::getLocalPlayer().id);
				native::setPedAsGroupMember(vars.handle, myGroup);
				native::setPedCombatAbility(vars.handle, 100);
			}));

		addOption(scrollOption<Hash>(SCROLLSELECT, getString("Give Weapon"))
			.addScroll(vars.weapon, 0, NUMOF(global::arrays::g_weaponList), global::arrays::g_weaponList)
			.addOnClick([] { weaponsMenuVars::givePedWeapon(vars.handle, global::arrays::g_weaponList[vars.weapon].result, 1000); }));

		addOption(buttonOption(getString("Kill"))
			.addOnClick([] { native::applyDamageToPed(vars.handle, 30000, true, 0, 0); }));

		addOption(buttonOption(getString("Smite"))
			.addOnClick([] { weaponModsMenuVars::smitePed(vars.handle); }));

		addOption(numberOption<float>(SCROLL, getString("Scale"))
			.addNumber(*vars.scale, "%.2f", 0.01f).addMin(0.01f).addMax(20.f).setScrollSpeed(10)
			.addOnClick([] { native::setPedScale(vars.handle, *vars.scale); }));

		addOption(buttonOption("Remove all Weapons")
			.addOnClick([] { native::removeAllPedWeapons(vars.handle, 1, 1); }));
	}

	// vehicle
	if (vars.type == 2) {
		setName(getString("Manage Spawned Vehicles"), false);
		setParentSubmenu<spawnVehicleMenu>();

		addOption(buttonOption(getString("Place on Ground"))
			.addOnClick([] {
				native::setVehicleOnGroundProperly(vars.handle);
			}));
	}

	addOption(toggleOption(getString("Creation Overlay"))
		.addToggle(*vars.creationOverlay));

	addOption(buttonOption(getString("Warp Into"))
		.addOnClick([] { native::setPedIntoVehicle(menu::getLocalPlayer().ped, vars.handle, -1); }));

	addOption(buttonOption(getString("Teleport to Me"))
		.addOnClick([] { native::setEntityCoords(vars.handle, menu::getLocalPlayer().coords); }));

	addOption(buttonOption(getString("Teleport to Entity"))
		.addOnClick([] { native::setEntityCoords(menu::getLocalPlayer().entity, native::getEntityCoords(vars.handle)); }));

	addOption(toggleOption(getString("Invincibility"))
		.addToggle(*vars.invincibility));

	addOption(scrollOption<int>(SCROLLSELECT, getString("Attach to Player"))
		.addOnClick([] { native::attachEntityToEntity(vars.handle, menu::getPlayer(vars.attachList[vars.attachVal].result).ped, 0, { 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }, FALSE, TRUE, TRUE, TRUE, 0, TRUE, TRUE, TRUE); })
		.addScroll(vars.attachVal, 0, NUMOF(vars.attachList), vars.attachList)
		.addOnUpdate([] (scrollOption<int>* option) { option->addScroll(vars.attachVal, 0, vars.listSize, vars.attachList); })
		.addRequirement([] () -> bool { return native::networkIsInSession(); }));

	addOption(buttonOption(getString("Detach"))
		.addOnClick([] { native::detachEntity(vars.handle, FALSE, FALSE); })
		.addRequirement([] () -> bool { return native::isEntityAttached(vars.handle) && native::networkIsInSession(); }));

	addOption(buttonOption(getString("Delete"))
		.addOnClick([] { 
			native::setEntityAsMissionEntity(vars.handle, TRUE, TRUE);
			native::deleteEntity(&vars.handle);
		}));
}

/*Called always*/
void spawnManageSelectedMenu::featureUpdate() {}

spawnManageSelectedMenu* _instance;
spawnManageSelectedMenu* spawnManageSelectedMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new spawnManageSelectedMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

spawnManageSelectedMenu::~spawnManageSelectedMenu() { delete _instance; }