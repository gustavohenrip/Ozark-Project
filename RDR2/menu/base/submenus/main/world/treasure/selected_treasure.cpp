#include "selected_treasure.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../treasure_finder.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "menu/base/submenus/main/network/account/treasure.hpp"
#include "menu/base/util/control_manager.hpp"

using namespace selectedTreasureMenuVars;

namespace selectedTreasureMenuVars {
	vars_ vars;

	void teleportToMe() {
		std::vector<Object> pickups;
		menu::pools::getObjectPool()->run([&] (Object object) {
			auto distance = native::getDistanceBetweenCoords(native::getEntityCoords(vars.selectedChest), native::getEntityCoords(object), TRUE);
			if (distance <= .4f) {
				pickups.push_back(object);
			}
		});

		auto coords = native::getOffsetFromEntityInWorldCoords(menu::getLocalPlayer().ped, { .0f, 2.f, .0f });
		native::getGroundZFor3dCoord(coords.x, coords.y, coords.z, &coords.z, FALSE);

		menu::getControlManager()->requestControl(vars.selectedChest, [=] (Object obj) {
			native::setEntityCoords(vars.selectedChest, coords);
		});

		coords.z += .2f;
		for (auto& pickup : pickups) {
			menu::getControlManager()->requestControl(pickup, [=] (Object obj) {
				native::setEntityCoords(obj, coords);
			});
		}
	}
}

void selectedTreasureMenu::init() {
	setName("Treasure Finder");
	setParentSubmenu<treasureFinderMenu>();

	addOption(buttonOption("Teleport to Chest")
		.addTranslation()
		.addOnClick([] { native::setEntityCoords(menu::getLocalPlayer().entity, native::getEntityCoords(vars.selectedChest)); }));

	addOption(buttonOption("Teleport to Me")
		.addTranslation()
		.addOnClick(teleportToMe));

	addOption(buttonOption("Delete")
		.addTranslation()
		.addOnClick([] {
			menu::getControlManager()->requestControl(vars.selectedChest, [=] (Object obj) {
				native::setEntityAsMissionEntity(obj, TRUE, TRUE);
				native::deleteEntity(&obj);
			});
		}));
}

/*Called while in submenu*/
void selectedTreasureMenu::update() {
	if (!native::doesEntityExist(vars.selectedChest)) {
		menu::getSubmenuHandler()->setSubmenuPrevious(true);
		return;
	}

	menu::getRenderer()->drawLine(menu::getLocalPlayer().coords, native::getEntityCoords(vars.selectedChest), color(255, 255, 100));
}

/*Called once on submenu open*/
void selectedTreasureMenu::updateOnce() {}

/*Called always*/
void selectedTreasureMenu::featureUpdate() {}

selectedTreasureMenu* _instance;
selectedTreasureMenu* selectedTreasureMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new selectedTreasureMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

selectedTreasureMenu::~selectedTreasureMenu() { delete _instance; }