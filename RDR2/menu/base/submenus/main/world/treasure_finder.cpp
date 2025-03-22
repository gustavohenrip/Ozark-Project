#include "treasure_finder.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../world.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "menu/base/submenus/main/network/account/treasure.hpp"
#include "utils/va.hpp"
#include "menu/base/util/timers.hpp"
#include "treasure/selected_treasure.hpp"

using namespace treasureFinderMenuVars;

namespace treasureFinderMenuVars {
	vars_ vars;
}

void treasureFinderMenu::init() {
	setName("Treasure Finder");
	setParentSubmenu<worldMenu>();
}

/*Called while in submenu*/
void treasureFinderMenu::update() {
	static int timer = 0;
	menu::util::getTimers()->runTimedFunction(&timer, 500, [this] {
		clearOptionsOffset(0);

		menu::pools::getObjectPool()->run([this] (Object object) {
			Hash model = native::getEntityModel(object);
			for (auto& chest : networkTreasureMenuVars::chests) {
				if (chest.result == model) {
					addOption(submenuOption(utils::va::va("[%.2f ft] %s", native::getEntityCoords(object).getMathmaticalDistance(menu::getLocalPlayer().coords), chest.name.c_str()))
						.addSubmenu<selectedTreasureMenu>()
						.addOnClick([=] { selectedTreasureMenuVars::vars.selectedChest = object; })
						.addOnHover([=] { menu::getRenderer()->drawLine(menu::getLocalPlayer().coords, native::getEntityCoords(object), color(255, 255, 100)); }));
					break;
				}
			}

			if (model == 0x2AB28031) {
				addOption(submenuOption(utils::va::va("[%.2f ft] Gold Bar Pickup", native::getEntityCoords(object).getMathmaticalDistance(menu::getLocalPlayer().coords)))
					.addSubmenu<selectedTreasureMenu>()
					.addOnClick([=] { selectedTreasureMenuVars::vars.selectedChest = object; })
					.addOnHover([=] { menu::getRenderer()->drawLine(menu::getLocalPlayer().coords, native::getEntityCoords(object), color(255, 255, 100)); }));
			}
		});

		if (options.size() <= 0) {
			addOption(buttonOption("No treasure found near you :(")
				.addTranslation());
		}
	});
}

/*Called once on submenu open*/
void treasureFinderMenu::updateOnce() {}

/*Called always*/
void treasureFinderMenu::featureUpdate() {}

treasureFinderMenu* _instance;
treasureFinderMenu* treasureFinderMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new treasureFinderMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

treasureFinderMenu::~treasureFinderMenu() { delete _instance; }