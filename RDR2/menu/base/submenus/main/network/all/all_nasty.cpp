#include "all_nasty.hpp"
#include "menu/base/submenu_handler.hpp"
#include "all_players.hpp"
#include "../nasty/nasty.hpp"
#include "menu/base/util/control_manager.hpp"
#include "../nasty/attack.hpp"
#include "all_particles.hpp"
#include "menu/base/util/timers.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/util/fiber_pool.hpp"
#include "menu/base/util/fiber_manager.hpp"
#include "rage/session/session.hpp"
#include "menu/base/util/spawn_manager.hpp"

using namespace networkAllNastyMenuVars;

namespace networkAllNastyMenuVars {
	vars_ vars;

	void explode() {
		menu::getPlayerManager()->foreach([=](menu::playerVars& lobby) {
			if (lobby.id != menu::getLocalPlayer().id) {
				native::addExplosion(lobby.coords, 1, 1.f, 1, 0, .4f);
			}
		});	
	}

	void bulletBarrage() {
		menu::getPlayerManager()->foreach([=](menu::playerVars& lobby) {
			if (lobby.id != menu::getLocalPlayer().id) {
				networkNastyMenuVars::bulletBarrage(lobby);
			}
		});
	}

	void paralyse() {
		menu::getPlayerManager()->foreach([=](menu::playerVars& lobby) {
			if (lobby.id != menu::getLocalPlayer().id) {
				native::clearPedTasksImmediately(lobby.entity, FALSE, FALSE);
			}
		});
	}

	void fakeMoney() {
		menu::getPlayerManager()->foreach([=](menu::playerVars& lobby) {
			if (lobby.id != menu::getLocalPlayer().id) {
				networkNastyMenuVars::fakeMoney(lobby);
			}
		});
	}
}

void networkAllNastyMenu::init() {
	setName("Nasty");
	setParentSubmenu<networkAllPlayersMenu>();

	addOption(submenuOption("Attackers")
		.addTranslation()
		.addOnClick([] { networkAttackMenuVars::vars.allPlayers = true; })
		.addSubmenu<networkAttackMenu>());

	addOption(submenuOption("Particles")
		.addTranslation()
		.addSubmenu<networkAllParticleMenu>());

	addOption(buttonOption("Remove All Weapons") 
		.addTranslation()
		.addOnClick([] { menu::getPlayerManager()->foreach([=](menu::playerVars& lobby) { native::removeAllPedWeapons(lobby.ped, 1, 1); }); }));

	addOption(buttonOption("Cage")
		.addOnClick([] {
			menu::getPlayerManager()->foreach([=](menu::playerVars& lobby) {
				menu::getControlManager()->requestModel(0x966D1C7A, [&](DWORD model) {
					auto coords = lobby.coords;
					menu::util::getSpawnManager()->spawnObject(model, { coords.x, coords.y, coords.z - .2f });
				});
			});
		}));

	//addOption(buttonOption("Crash")
	//	.addTranslation()
	//	.addOnClick([] {
	//		menu::getNotify()->right("Crashing ~e~lobby~q~. please wait...");
	//		rage::network::getNetworkTask()->setWeather(0, 0x24, true);
	//	}));

	addOption(toggleOption("Explode Loop")
		.addTranslation()
		.addToggle(vars.loopedExplode));

	addOption(buttonOption("Explode")
		.addTranslation()
		.addOnClick([] { explode(); }));

	addOption(buttonOption("Kill")
		.addTranslation()
		.addOnClick([] { menu::getPlayerManager()->foreach([=](menu::playerVars& lobby) { if (lobby.id != menu::getLocalPlayer().id) { networkNastyMenuVars::kill(lobby); } }); }));

	addOption(buttonOption("Smite")
		.addTranslation()
		.addOnClick([] { menu::getPlayerManager()->foreach([=](menu::playerVars& lobby) { if (lobby.id != menu::getLocalPlayer().id) { networkNastyMenuVars::smite(lobby); } }); }));

	addOption(buttonOption("Kill Horse")
		.addTranslation()
		.addOnClick([] { menu::getPlayerManager()->foreach([=](menu::playerVars& lobby) { 
			if (lobby.id != menu::getLocalPlayer().id) { if (native::isPedOnMount(lobby.ped)) { menu::getControlManager()->requestControl(native::getMount(lobby.ped), [](Ped mount) { native::applyDamageToPed(mount, 30000, true, 0, 0);	});	} }});}));
	
	addOption(toggleOption("Fake Money")
		.addTranslation()
		.addToggle(vars.fakeMoney));

	addOption(toggleOption("Paralyse")
		.addTranslation()
		.addToggle(vars.paralyse));

	addOption(toggleOption("Bullet Barrage")
		.addTranslation()
		.addToggle(vars.bulletBarrage));
}

/*Called while in submenu*/
void networkAllNastyMenu::update() {}

/*Called once on submenu open*/
void networkAllNastyMenu::updateOnce() {}

/*Called always*/
void networkAllNastyMenu::featureUpdate() {
	if (vars.fakeMoney) {
		fakeMoney();
	}

	if (vars.loopedExplode) {
		if (vars.explosionTimer < GetTickCount()) {
			vars.explosionTimer = GetTickCount() + 500;
			explode();
		}
	}

	if (vars.paralyse) {
		paralyse();
	}

	if (vars.bulletBarrage) {
		bulletBarrage();
	}

	////95314435 rockstar admin
	//menu::util::getTimers()->runTimedFunction()


}

networkAllNastyMenu* _instance;
networkAllNastyMenu* networkAllNastyMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkAllNastyMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkAllNastyMenu::~networkAllNastyMenu() { delete _instance; }