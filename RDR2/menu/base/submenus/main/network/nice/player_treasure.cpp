#include "player_treasure.hpp"
#include "menu/base/submenu_handler.hpp"
#include "nice.hpp"
#include "../all/all_nice.hpp"
#include "../account/treasure.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "menu/base/util/fiber_pool.hpp"
#include "menu/base/util/fiber_manager.hpp"

using namespace networkPlayerTreasureMenuVars;

namespace networkPlayerTreasureMenuVars {
	vars_ vars;
	
	void give() {
		if (!vars.allPlayers) {
			networkTreasureMenuVars::treasure(menu::getSelectedPlayer().ped, networkTreasureMenuVars::chests[vars.chestType].result, 0x2AB28031, vars.chestCount, vars.barCount);
		} else {
			menu::util::getFiberPool()->run([] {
				menu::getPlayerManager()->foreach([=] (menu::playerVars& lobby) {
					networkTreasureMenuVars::treasure(lobby.ped, networkTreasureMenuVars::chests[vars.chestType].result, 0x2AB28031, vars.chestCount, vars.barCount);
					menu::util::waitFiber(100);
				});
			});
		}
	}
}

void networkPlayerTreasureMenu::init() {
	setName("Treasure");
	setParentSubmenu<networkNiceMenu>();

	addOption(buttonOption("Give Treasure")
		.addTranslation()
		.addOnClick(give));

	addOption(breakOption("Config")
		.addTranslation());

	addOption(scrollOption<Hash>(SCROLL, "Chest Type")
		.addTranslation()
		.addScroll(vars.chestType, 0, NUMOF(networkTreasureMenuVars::chests), networkTreasureMenuVars::chests));

	addOption(numberOption<int>(SCROLL, "Chest Count")
		.addTranslation()
		.addNumber(vars.chestCount, "%i", 1).addMin(1).addMax(32));

	addOption(numberOption<int>(SCROLL, "Pickups per Chest")
		.addTranslation()
		.addNumber(vars.barCount, "%i", 1).addMin(1).addMax(500));
}

/*Called while in submenu*/
void networkPlayerTreasureMenu::update() {
	if (!vars.allPlayers) {
		menu::getPanelManager()->panelPlayer(menu::getSelectedPlayer().id);
	}
}

/*Called once on submenu open*/
void networkPlayerTreasureMenu::updateOnce() {
	if (!vars.allPlayers) {
		setParentSubmenu<networkNiceMenu>();
	} else {
		setParentSubmenu<networkAllNiceMenu>();
	}
}

/*Called always*/
void networkPlayerTreasureMenu::featureUpdate() {}

networkPlayerTreasureMenu* _instance;
networkPlayerTreasureMenu* networkPlayerTreasureMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkPlayerTreasureMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkPlayerTreasureMenu::~networkPlayerTreasureMenu() { delete _instance; }