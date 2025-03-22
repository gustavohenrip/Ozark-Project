#include "all_nice.hpp"
#include "menu/base/submenu_handler.hpp"
#include "all_players.hpp"
#include "../nice/player_treasure.hpp"
#include "menu/base/submenus/main/weapon/explosion.hpp"

using namespace networkAllNiceMenuVars;

namespace networkAllNiceMenuVars {
	vars_ vars;
}

void networkAllNiceMenu::init() {
	setName("Nice");
	setParentSubmenu<networkAllPlayersMenu>();

	addOption(submenuOption("Treasure ~e~[RISK]")
		.addTranslation()
		.addOnClick([] { networkPlayerTreasureMenuVars::vars.allPlayers = true; })
		.addSubmenu<networkPlayerTreasureMenu>());

	addOption(scrollOption<int>(TOGGLE, "Explosive Ammo")
		.addTranslation()
		.addScroll(vars.explosiveType, 0, NUMOF(explosionMenuVars::explosions), explosionMenuVars::explosions)
		.addToggle(vars.explosiveAmmo));
}

/*Called while in submenu*/
void networkAllNiceMenu::update() {}

/*Called once on submenu open*/
void networkAllNiceMenu::updateOnce() {}

/*Called always*/
void networkAllNiceMenu::featureUpdate() {
	if (vars.explosiveAmmo) {
		menu::getPlayerManager()->foreach([=](menu::playerVars& lobby) {
			explosionMenuVars::explosionGun(lobby.ped, vars.explosiveType, 1.f, 1, 0, 0.f);
		});
	}
}

networkAllNiceMenu* _instance;
networkAllNiceMenu* networkAllNiceMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkAllNiceMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkAllNiceMenu::~networkAllNiceMenu() { delete _instance; }