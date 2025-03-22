#include "objects.hpp"
#include "menu/base/submenu_handler.hpp"
#include "nearby.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "src/menu/base/util/control_manager.hpp"

using namespace objectMenuVars;

namespace objectMenuVars {
	vars_ vars;
	
	void esp() {
		menu::pools::getObjectPool()->run([](Object object) {
			menu::getRenderer()->drawLine(menu::getLocalPlayer().coords, native::getEntityCoords(object), { 255, 159, 116, 255 });
		});
	}

	void teleport(math::vector3<float> coords) {
		menu::pools::getObjectPool()->run([&](Object object) {
			native::setEntityCoords(object, coords);
		});
	}

	void deleteObjects() {
		menu::pools::getObjectPool()->run([=](Object object) {
			native::setEntityAsMissionEntity(object, 1, 1);
			native::setEntityCoords(object, { 4503.726f, -635.393f, 71.421f });
			native::deleteEntity2(&object);
		});
	}
}

void objectMenu::init() {
	setName("Object Manager");
	setParentSubmenu<nearbyMenu>();

	addOption(buttonOption("Teleport to Me")
		.addTranslation()
		.addOnClick([] { teleport(menu::getSelectedPlayer().coords); }));

	addOption(scrollOption<int>(SCROLLSELECT, "Teleport")
		.addTranslation()
		.addScroll(vars.teleportIndex, 0, NUMOF(vars.teleportPlayers), vars.teleportPlayers)
		.addOnClick([] { teleport(native::getEntityCoords(vars.teleportPlayers[vars.teleportIndex].result)); })
		.addOnUpdate([](scrollOption<int>* option) { option->addScroll(vars.teleportIndex, 0, vars.listSize, vars.teleportPlayers); }));

	addOption(buttonOption("Delete")
		.addTranslation()
		.addOnClick([] { deleteObjects(); }));

	addOption(breakOption("Config")
		.addTranslation());

	addOption(toggleOption("ESP")
		.addTranslation()
		.addToggle(vars.esp));
}

/*Called while in submenu*/
void objectMenu::update() {
	if (vars.esp) esp();
	menu::getPanelManager()->panelPool("Objects", menu::pools::getObjectPool()->getPoolPointer(), 3);

	int size = 0;
	menu::getPlayerManager()->foreach([&](menu::playerVars& var) {
		vars.teleportPlayers[size].name = var.name;
		vars.teleportPlayers[size].result = var.ped;
		size++;
	});

	vars.listSize = size;
}

/*Called once on submenu open*/
void objectMenu::updateOnce() {}

/*Called always*/
void objectMenu::featureUpdate() {}

objectMenu* _instance;
objectMenu* objectMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new objectMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

objectMenu::~objectMenu() { delete _instance; }