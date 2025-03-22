#include "all_players.hpp"
#include "menu/base/submenu_handler.hpp"
#include "menu/base/submenus/main/network.hpp"
#include "all_player_esp.hpp"
#include "all_nasty.hpp"
#include "all_nice.hpp"
#include "all_teleport.hpp"
#include "../network_message.hpp"
#include "menu/base/util/timers.hpp"
#include"rage/session/session.hpp"

using namespace networkAllPlayersMenuVars;

namespace networkAllPlayersMenuVars {
	vars_ vars;
}

void networkAllPlayersMenu::init() {
	setName("All Players");
	setParentSubmenu<networkMenu>();

	addOption(submenuOption("ESP")
		.addTranslation()
		.addSubmenu<networkAllEspMenu>());

	addOption(submenuOption("Teleport")
		.addTranslation()
		.addSubmenu<networkAllTeleportMenu>());

	addOption(submenuOption("Nasty")
		.addTranslation()
		.addSubmenu<networkAllNastyMenu>());

	addOption(submenuOption("Nice")
		.addTranslation()
		.addSubmenu<networkAllNiceMenu>());

	/*addOption(submenuOption("Message")
		.addTranslation()
		.addOnClick([] {
			networkMessageMenuVars::vars.submenuIdentifier = 2;
			networkMessageMenuVars::vars.rockstarID = 0;
		})
		.addSubmenu<networkMessageMenu>());*/

	addOption(toggleOption("Show Joining Players")
		.addTranslation()
		.addToggle(vars.showJoinLeave).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Reveal Players")
		.addToggle(vars.revealPlayers).canBeSaved(name.getOriginal())
		.addOnClick([] {
			menu::getPlayerManager()->foreach([=](menu::playerVars& lobby) {
				rage::network::netPlayerInfo* info = rage::network::getNetworkTask()->getNetPlayerInfo(lobby.id);
				//printf("Removing blip\n");
				native::removeBlip(&vars.playerBlip[info]);
				vars.playerBlip[info] = 0;
				}); }));

	addOption(breakOption("Exclude")
		.addTranslation());

	addOption(toggleOption("Ozark Users")
		.addTranslation()
		.addToggle(vars.excludeOzarkUsers).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Friends")
		.addTranslation()
		.addToggle(vars.excludeFriends).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Other Players")
		.addTranslation()
		.addToggle(vars.otherPlayers).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Me")
		.addTranslation()
		.addToggle(vars.me).canBeSaved(name.getOriginal()));
}

/*Called while in submenu*/
void networkAllPlayersMenu::update() {}

/*Called once on submenu open*/
void networkAllPlayersMenu::updateOnce() {}

/*Called always*/
void networkAllPlayersMenu::featureUpdate() {
	menu::getPlayerManager()->foreach([=](menu::playerVars& lobby) {
		if (vars.revealPlayers) {

			static int blipTimeout[32];
			rage::network::netPlayerInfo* info = rage::network::getNetworkTask()->getNetPlayerInfo(lobby.id);
			menu::util::getTimers()->runTimedFunction(&blipTimeout[lobby.id], 60000, [&] {
				native::removeBlip(&vars.playerBlip[info]);
				vars.playerBlip[info] = 0;
				});
			if (!vars.playerBlip[info]) {
				vars.playerBlip[info] = native::addBlipForEntity(-1749618580, native::getPlayerPedScriptIndex(lobby.id));
				//native::setBlipSprite(vars.playerBlip[info], 460296925, TRUE);
				native::setBlipNameToPlayerName(vars.playerBlip[info], lobby.id);
			}
		}
	});
}

networkAllPlayersMenu* _instance;
networkAllPlayersMenu* networkAllPlayersMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkAllPlayersMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkAllPlayersMenu::~networkAllPlayersMenu() { delete _instance; }