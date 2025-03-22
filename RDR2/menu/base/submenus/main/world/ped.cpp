#include "ped.hpp"
#include "menu/base/submenu_handler.hpp"
#include "nearby.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "menu/base/submenus/main/weapon/weapon_mods.hpp"
#include "menu/base/submenus/main/teleport.hpp"
#include "src/menu/base/util/fiber_manager.hpp"
#include "src/menu/base/util/control_manager.hpp"
#include "src/menu/base/util/fiber_pool.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "menu/base/renderer.hpp"

using namespace pedMenuVars;

namespace pedMenuVars {
	vars_ vars;

	scrollStruct<int> teleport[3] = {
		{ "To Me", 0 },
		{ "Up Above", 1 },
		{ "Nearest Player", 2 }
	};

	bool isPedDead(Ped ped) {
		if (!vars.grabDeadPeds) {
			return native::isEntityDead(ped);
		}

		return false;
	}

	void teleportType(int type) {
		menu::pools::getPedPool()->run([=](Ped ped) {
			if (!isPedDead(ped)) {
				menu::getControlManager()->requestControl(ped, [=](Ped p) {
					math::vector3<float> coords;
					switch (type) {
					case 0:
						coords = menu::getLocalPlayer().coords;
						native::setEntityCoords(p, { coords.x, coords.y, coords.z + 2.f });
						break;
					case 1:
						coords = native::getEntityCoords(p);
						native::setEntityCoords(p, { coords.x, coords.y, coords.z });
						break;
					case 2:
						coords = native::getEntityCoords(native::getPlayerPed(teleportMenuVars::getClosestPlayer()));
						native::setEntityCoords(p, { coords.x, coords.y, coords.z });
						break;
					}
				});
			}
		});
	}

	void setBodyguard(Ped ped) {
		menu::getControlManager()->requestControl(ped, [=](Ped p) {
			int myGroup = native::getPlayerGroup(menu::getLocalPlayer().id);
			native::setPedAsGroupMember(p, myGroup);
			native::setPedCombatAbility(p, 100);
		});
	}

	void launchUp(Entity entity) {
		if (native::doesEntityExist(entity)) {
			menu::getControlManager()->requestControl(entity, [=](Ped ped) {
				native::setPedToRagdoll(ped, -1, -1, 0, 1, 1, 0);
				auto coords = native::getEntityVelocity(ped, 0);
				native::setEntityVelocity(ped, { coords.x, coords.y, (coords.z + 9999.f) * 999.f });
			});
		}
	}

	void esp() {
		menu::pools::getPedPool()->run([] (Ped ped) {
			if (!isPedDead(ped)) {
				menu::getRenderer()->drawLine(menu::getLocalPlayer().coords, native::getEntityCoords(ped), { 204, 0, 0, 255 });
			}
		});
	}
}

void pedMenu::init() {
	setName("Ped Manager");
	setParentSubmenu<nearbyMenu>();

	addOption(scrollOption<int>(SCROLLSELECT, "Teleport")
		.addTranslation()
		.addScroll(vars.teleportIndex, 0, NUMOF(teleport), teleport)
		.addOnClick([] { teleportType(vars.teleportIndex); }));

	addOption(buttonOption("Kill")
		.addTranslation()
		.addOnClick([] { 
			menu::pools::getPedPool()->run([](Ped ped) { if (!isPedDead(ped)) { menu::getControlManager()->requestControl(ped, [=](Ped ped) {native::explodePedHead(ped, 0x31b7b9fe); }); } }); }));

	addOption(buttonOption("Smite")
		.addTranslation()
		.addOnClick([] { 
			menu::util::getFiberPool()->run([=] { 
				menu::pools::getPedPool()->run([](Ped ped) { if (!isPedDead(ped)) { menu::getControlManager()->requestControl(ped, [=](Ped ped) { weaponModsMenuVars::smitePed(ped); menu::util::waitFiber(60); }); } }); }); }));

	addOption(buttonOption("Launch")
		.addTranslation()
		.addOnClick([] { menu::pools::getPedPool()->run([](Ped ped) { if (!isPedDead(ped)) { launchUp(ped); } }); }));

	addOption(numberOption<float>(TOGGLE, "Scale")
		.addTranslation()
		.addNumber(vars.scale, "%.2f", 0.01f).addMin(0.01f).addMax(20.f).setScrollSpeed(10)
		.addToggle(vars.scaleToggle));

	addOption(buttonOption("Sprint to Me")
		.addTranslation()
		.addOnClick([] {
			menu::pools::getPedPool()->run([](Ped ped) { menu::getControlManager()->requestControl(ped, [=](Ped ped) { native::taskGoStraightToCoord(ped, menu::getLocalPlayer().coords, 10.f, 99999, 0.f, 0.f); }); }); }));

	addOption(buttonOption("Jump")
		.addTranslation()
		.addOnClick([] {
			menu::pools::getPedPool()->run([](Ped ped) { menu::getControlManager()->requestControl(ped, [=](Ped ped) { native::taskJump(ped, FALSE); }); }); }));

	addOption(buttonOption("Put Hands Up")
		.addTranslation()
		.addOnClick([] {
			menu::pools::getPedPool()->run([](Ped ped) { menu::getControlManager()->requestControl(ped, [=](Ped ped) { native::taskHandsUp(ped, 5000, 0, 0, 0); }); }); }));

	addOption(buttonOption("Employ as Bodyguard")
		.addTranslation()
		.addOnClick([] { 
			menu::pools::getPedPool()->run([](Ped ped) { menu::getControlManager()->requestControl(ped, [=](Ped ped) { setBodyguard(ped); }); }); }));

	addOption(buttonOption("Delete")
		.addTranslation()
		.addOnClick([] { 
		menu::pools::getPedPool()->run([=] (Ped ped) {
			native::setEntityAsMissionEntity(ped, 1, 1);
			native::setEntityCoords(ped, { 4503.726f, -635.393f, 71.421f });
			native::deleteEntity2(&ped);
		});
	}));

	addOption(breakOption("Config")
		.addTranslation());

	addOption(toggleOption("Grab Dead Peds")
		.addTranslation()
		.addToggle(vars.grabDeadPeds));

	addOption(toggleOption("ESP")
		.addTranslation()
		.addToggle(vars.esp));
}

/*Called while in submenu*/
void pedMenu::update() {
	if (vars.esp) esp();
	menu::getPanelManager()->panelPool("Ped", menu::pools::getPedPool()->getPoolPointer(), 1);
}

/*Called once on submenu open*/
void pedMenu::updateOnce() {}

/*Called always*/
void pedMenu::featureUpdate() {
	if (vars.scaleToggle) {
		menu::pools::getPedPool()->run([](Ped ped) {
			if (vars.scaleToggle) {
				if (!isPedDead(ped)) {
					native::setPedScale(ped, vars.scale);
				}
			}
		});
	}
}

pedMenu* _instance;
pedMenu* pedMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new pedMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

pedMenu::~pedMenu() { delete _instance; }