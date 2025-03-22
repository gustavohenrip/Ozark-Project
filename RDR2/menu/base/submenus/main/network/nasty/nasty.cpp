#include "nasty.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../players.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/util/spawn_manager.hpp"
#include "menu/base/util/timers.hpp"
#include "menu/base/submenus/main/weapon/weapon_mods.hpp"
#include "attachments.hpp"
#include "rage/session/session.hpp"
#include "rage/entity/entity.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "menu/base/util/notify.hpp"
#include "attack.hpp"
#include "global/arrays.hpp"
#include "crash.hpp"
#include "network_particles.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "menu/base/util/fiber_pool.hpp"
#include "menu/base/util/fiber_manager.hpp"
#include "menu/base/util/presence.hpp"

using namespace networkNastyMenuVars;

namespace networkNastyMenuVars {
	vars_ vars;

	scrollStruct<Hash> explosiveObjects[] = {
		"Crate", 0x4451BFF5,
		"Barrel", 0x93DAB6C8,
		"Box", 0x8B664AFD,
	};

	scrollStruct<int> reportTypes[5] = {
		{ "Cheating", 0 },
		{ "Abusive Chat", 1},
		{ "Disruptive Behavior", 2},
		{ "Offensive Posse Name", 7},
		{ "Offensive Horse Name", 8}
	};

	void explode(menu::playerVars& player) {
		native::addExplosion(player.coords, 0, 10.f, TRUE, FALSE, 5.f);
	}

	void kill(menu::playerVars& player) {
		native::addExplosion(player.coords, 0, 10.f, TRUE, TRUE, 0.f);
	}

	void airstrike(menu::playerVars& player) {
		auto coords = player.coords;
		coords.z += 250.f;
		native::shootSingleBulletBetweenCoords(coords, player.coords, 1000, TRUE, 0xa64daa5e, native::getPlayerPed(native::getRandomIntInRange(0, native::networkGetNumOfConnectedPlayers() - 1)), TRUE, FALSE, 1000.f, TRUE);
	}

	void forcefield(menu::playerVars& player) {
		native::addExplosion(player.coords, 29, 100.f, FALSE, TRUE, 0.f);
	}

	void setOnFire(menu::playerVars& player) {
		if (player.setOnFireHandle) {
			native::removeScriptFire(player.setOnFireHandle);
		}

		player.setOnFireHandle = native::startScriptFire(player.coords, 2, 0.f, false, "INTERIOR_FIRE_SMALL_SOUNDSET", -1.f, 8);
	}

	void hostilePeds(menu::playerVars& player) {
		menu::pools::getPedPool()->run([=] (Ped entity) {
			if (native::doesEntityExist(entity) && entity != player.ped && !native::isPedAPlayer(entity)) {
				if (native::vDist(native::getEntityCoords(entity), player.coords) < 100.f) {
					native::setPedCombatAbility(entity, 100);
					native::taskCombatPed(entity, player.ped, 0, 16);
					native::setPedCanRagdoll(entity, FALSE);
					native::setPedCanBeKnockedOffVehicle(entity, FALSE);
					native::setPedCanRagdollFromPlayerImpact(entity, FALSE);
					native::giveDelayedWeaponToPed(entity, 0x6d9bb970, 25000, 1, 0x2CD419DC);
					native::setCurrentPedWeapon(entity, 0x6d9bb970, 1, 0, 0, 0);
				}
			}
		});
	}

	void smite(menu::playerVars& player) {
		weaponModsMenuVars::smitePed(player.ped);
	}

	void hostileClone(menu::playerVars& player) {
		auto clone = native::clonePed(player.ped, native::getEntityHeading(player.ped), TRUE, FALSE);
		if (clone && native::doesEntityExist(clone)) {
			// hostile!
			native::setPedCombatAbility(clone, 100);
			native::taskCombatPed(clone, player.ped, 0, 16);
			native::setPedCanRagdoll(clone, FALSE);
			native::setPedCanBeKnockedOffVehicle(clone, FALSE);
			native::setPedCanRagdollFromPlayerImpact(clone, FALSE);
			native::giveDelayedWeaponToPed(clone, 0x6d9bb970, 25000, 1, 0x2CD419DC);
			native::setCurrentPedWeapon(clone, 0x6d9bb970, 1, 0, 0, 0);
		}
	}

	void killLobby(menu::playerVars& player) {
		menu::getPlayerManager()->foreach([=] (menu::playerVars& lobby) {
			native::addOwnedExplosion(player.ped, lobby.coords, 0, 10.f, TRUE, FALSE, 2.f);
		});
	}

	void fakeMoney(menu::playerVars& player) {
		int flag = 0;
		//native::setBit(&flag, 18);
		//native::setBit(&flag, 6);
		//native::createAmbientPickup(0x1E9A99F8, player.coords.x, player.coords.y, player.coords.z, flag, 5, native::getHashKey("p_moneybag01x"), FALSE, FALSE/*, FALSE, 10.f*/);
		native::createAmbientPickup(0x2C014CA6, player.coords, flag, 5, 0x453BA146, FALSE, FALSE, FALSE, 10.f);
		//native::createAmbientPickup(0x2C014CA6, player.coords.x, player.coords.y, player.coords.z, flag, 5, native::getHashKey("p_moneybag01x"), FALSE, FALSE/*, FALSE, 10.f*/);
		//Object CREATE_AMBIENT_PICKUP(Hash pickupHash, float x, float y, float z, int flags, int value, Hash modelHash, BOOL p7, BOOL p8, int p9, float p10)
	}

	void bulletBarrage(menu::playerVars& player) {
		float coords[3];
		for (std::size_t i = 0; i < 3; i++) {
			coords[i] = native::getRandomFloatInRange(-4.f, 4.f);
		}

		auto offset = native::getOffsetFromEntityInWorldCoords(player.ped, { coords[0], coords[1], coords[2] });
		native::shootSingleBulletBetweenCoords(offset, player.coords, 200, TRUE, 0x8580c63e, menu::getLocalPlayer().ped, FALSE, FALSE, 1000.f, TRUE);
	}

	void nearbyExplosions(menu::playerVars& player) {
		float coords[2];

		for (int i = 0; i < 2; i++) {
			coords[i] = native::getRandomFloatInRange(-40.f, 40.f);
		}

		auto offset = native::getOffsetFromEntityInWorldCoords(player.ped, { coords[0], coords[1], .0f });
		native::addExplosion(offset, 5, 2.f, 1, 0, 1.f);
	}

	void explodeHead(menu::playerVars& player) {
		for (int j = 0; j < 20; j++) {
			float coords[2];

			for (int i = 0; i < 2; i++) {
				coords[i] = native::getRandomFloatInRange(-40.f, 40.f);
			}

			auto offset = native::getOffsetFromEntityInWorldCoords(player.ped, { coords[0], coords[1], 1.f });
			native::shootSingleBulletBetweenCoords(offset, native::getPedBoneCoords(player.ped, 0x5226, 0, 0, 0), 5000, TRUE, 0x31b7b9fe, menu::getLocalPlayer().ped, FALSE, FALSE, 3000.f, TRUE);
		}

	}

	//socialclubMessageNotify
}

void networkNastyMenu::init() {
	setName("Nasty");
	setParentSubmenu<networkPlayersMenu>();

	addOption(submenuOption("Attachments")
		.addTranslation()
		.addSubmenu<networkAttachmentsMenu>());

	addOption(submenuOption("Attackers")
		.addTranslation()
		.addOnClick([] { networkAttackMenuVars::vars.allPlayers = false; })
		.addSubmenu<networkAttackMenu>());

	addOption(submenuOption("Crashes")
		.addTranslation()
		.addSubmenu<networkCrashMenu>());

	addOption(submenuOption("Particles")
		.addTranslation()
		.addSubmenu<networkParticleMenu>());

	addOption(scrollOption<Hash>(SCROLLSELECT, "Explode with Explosive")
		.addScroll(vars.explodeWithExplosives, 0, NUMOF(explosiveObjects), explosiveObjects)
		.addOnClick([] {
			menu::util::getFiberPool()->run([=] {
				auto coords = native::getOffsetFromEntityInWorldCoords(menu::getSelectedPlayer().ped, { .0f, 1.f, .0f });
				menu::util::getSpawnManager()->spawnObject(explosiveObjects[vars.explodeWithExplosives].result, coords);
				menu::util::waitFiber(500);

				auto newCoords = coords;
				newCoords.z += 5.f;
				native::shootSingleBulletBetweenCoords(newCoords, coords, 5000, TRUE, 0x31b7b9fe, menu::getLocalPlayer().ped, FALSE, FALSE, 3000.f, TRUE);
			});
		}));

	//addOption(toggleOption("Black Screen")
	//	.addTranslation()
	//	.addToggle(menu::getSelectedPlayer().blackScreen)
	//	.addOnUpdate([](toggleOption* option) { option->addToggle(menu::getSelectedPlayer().blackScreen); })
	//	.addOnClick([] { rage::network::getNetworkTask()->setWeather(menu::getSelectedPlayer().id, 1); }));

	addOption(toggleOption("Fake Socialclub Notification")
		.addTranslation()
		.addToggle(menu::getSelectedPlayer().socialclubSpam)
		.addOnUpdate([](toggleOption* option) { option->addToggle(menu::getSelectedPlayer().socialclubSpam); }));

	addOption(buttonOption("Cage")
		.addOnClick([] {
			menu::getControlManager()->requestModel(0x966D1C7A, [&](DWORD model) {
				auto coords = menu::getSelectedPlayer().coords;
				Object object = menu::util::getSpawnManager()->spawnObject(model, { coords.x, coords.y, coords.z - .5f });
				if (native::doesEntityExist(object)) {
					native::networkRegisterEntityAsNetworked(object);
					native::networkSetEntityVisibleToNetwork(object, TRUE);
					native::networkRequestControlOfEntity(object);
				}
				});
			}));

	addOption(buttonOption("Remove All Weapons")
		.addTranslation()
		.addOnClick([] { native::removeAllPedWeapons(menu::getSelectedPlayer().ped, 1, 1); }));

	addOption(buttonOption("Explode")
		.addTranslation()
		.addOnClick([] { explode(menu::getSelectedPlayer()); }));

	addOption(toggleOption("Explode Loop")
		.addTranslation()
		.addToggle(menu::getSelectedPlayer().explodeLoop)
		.addOnUpdate([] (toggleOption* option) { option->addToggle(menu::getSelectedPlayer().explodeLoop); }));

	addOption(toggleOption("Nearby Explosions")
		.addTranslation()
		.addToggle(menu::getSelectedPlayer().nearbyExplosions)
		.addOnUpdate([](toggleOption* option) { option->addToggle(menu::getSelectedPlayer().nearbyExplosions); }));

	addOption(buttonOption("Kill")
		.addTranslation()
		.addOnClick([] { kill(menu::getSelectedPlayer()); }));

	addOption(buttonOption("Shoot Head")
		.addTranslation()
		.addOnClick([] { explodeHead(menu::getSelectedPlayer()); }));

	addOption(buttonOption("Euthanize Mount")
		.addTranslation()
		.addOnClick([] { menu::getControlManager()->requestControl(menu::getSelectedPlayer().mount, [](Ped ped) {native::applyDamageToPed(ped, 30000, true, 0, 0); }); })
		.addRequirement([] { return menu::getSelectedPlayer().isOnMount; }));

	addOption(buttonOption("Launch Mount")
		.addTranslation()
		.addOnClick([] { menu::getControlManager()->requestControl(menu::getSelectedPlayer().mount, [](Ped ped) { math::vector3<float> velocity = native::getEntityVelocity(ped, 0); native::setEntityVelocity(ped, { velocity.x, velocity.y, (velocity.z + 9999.f) * 20.f }); }); })
		.addRequirement([] { return menu::getSelectedPlayer().isOnMount; }));

	/*addOption(buttonOption("Airstrike")
		.addTranslation()
		.addOnClick([] { airstrike(menu::getSelectedPlayer()); }));*/

	addOption(toggleOption("Bullet Barrage")
		.addTranslation()
		.addToggle(menu::getSelectedPlayer().bulletBarrage)
		.addOnUpdate([](toggleOption* option) { option->addToggle(menu::getSelectedPlayer().bulletBarrage); }));

	addOption(toggleOption("Forcefield")
		.addTranslation()
		.addToggle(menu::getSelectedPlayer().forcefield)
		.addOnUpdate([] (toggleOption* option) { option->addToggle(menu::getSelectedPlayer().forcefield); }));

	addOption(toggleOption("Set on Fire")
		.addTranslation()
		.addToggle(menu::getSelectedPlayer().setOnFire)
		.addOnUpdate([] (toggleOption* option) { option->addToggle(menu::getSelectedPlayer().setOnFire); }));

	addOption(toggleOption("Paralyse")
		.addTranslation()
		.addToggle(menu::getSelectedPlayer().paralyse)
		.addOnUpdate([] (toggleOption* option) { option->addToggle(menu::getSelectedPlayer().paralyse); }));

	addOption(toggleOption("Hostile Peds")
		.addTranslation()
		.addToggle(menu::getSelectedPlayer().hostilePeds)
		.addOnUpdate([] (toggleOption* option) { option->addToggle(menu::getSelectedPlayer().hostilePeds); }));

	addOption(buttonOption("Hostile Clone")
		.addTranslation()
		.addOnClick([] { hostileClone(menu::getSelectedPlayer()); }));

	addOption(buttonOption("Smite")
		.addTranslation()
		.addOnClick([] { smite(menu::getSelectedPlayer()); }));

	addOption(buttonOption("Kill Lobby (Blame)")
		.addTranslation()
		.addOnClick([] { killLobby(menu::getSelectedPlayer()); }));

	addOption(toggleOption("Fake Money")
		.addTranslation()
		.addToggle(menu::getSelectedPlayer().fakeMoney)
		.addOnUpdate([] (toggleOption* option) { option->addToggle(menu::getSelectedPlayer().fakeMoney); }));

	addOption(scrollOption<int>(SCROLLSELECT, "Report")
		.addTranslation()
		.addScroll(menu::getSelectedPlayer().reportType, 0, NUMOF(reportTypes), reportTypes)
		.addOnUpdate([](scrollOption<int>* option) { option->addScroll(menu::getSelectedPlayer().reportType, 0, NUMOF(reportTypes), reportTypes); })
		.addOnClick([] { const char* desc = reportTypes[menu::getSelectedPlayer().reportType].result == 0 ? "cheater" : ""; native::networkReportPlayer(menu::getSelectedPlayer().id, reportTypes[menu::getSelectedPlayer().reportType].result, desc, ""); })
		.addRequirement([] { return !menu::getSelectedPlayer().isOzarkPassive; }));

	/*addOption(toggleOption("Black Screen")
		.addTranslation()
		.addToggle(menu::getSelectedPlayer().blackScreen)
		.addOnClick([] { rage::network::getNetworkTask()->setWeather(menu::getSelectedPlayer().id, menu::getSelectedPlayer().blackScreen ? 0x24 : 0x24); }));*/
}

/*Called while in submenu*/
void networkNastyMenu::update() {
	menu::getPanelManager()->panelPlayer(menu::getSelectedPlayer().id);
}

/*Called once on submenu open*/
void networkNastyMenu::updateOnce() {}

/*Called always*/
void networkNastyMenu::featureUpdate() {
	menu::getPlayerManager()->foreach([] (menu::playerVars& player) {
		if (player.nearbyExplosions) {
			static int nearbyExplosionsTimer[32];
			menu::util::getTimers()->runTimedFunction(&nearbyExplosionsTimer[player.id], 700, [&] {
				nearbyExplosions(player);
			});
		}

		if (player.bulletBarrage) {
			bulletBarrage(player);
		}

		if (player.explodeLoop) {
			static int explodeLoopTimer[32];
			menu::util::getTimers()->runTimedFunction(&explodeLoopTimer[player.id], 500, [&] {
				explode(player);
			});
		}

		if (player.forcefield) {
			forcefield(player);
		}

		if (player.setOnFire) {
			static int setOnFireTimer[32];
			menu::util::getTimers()->runTimedFunction(&setOnFireTimer[player.id], 500, [&] {
				setOnFire(player);
			});
		}

		if (player.paralyse) {
			native::clearPedTasksImmediately(player.ped, FALSE, FALSE);
			if (player.isOnMount) {
				native::clearPedTasksImmediately(player.mount, FALSE, FALSE);
			}
		}

		if (player.hostilePeds) {
			hostilePeds(player);
		}

		if (player.fakeMoney) {
			fakeMoney(player);
		}

		if (player.socialclubSpam) {
			static int socialclubSpam[32];
			menu::util::getTimers()->runTimedFunction(&socialclubSpam[player.id], 1000, [&] {
				auto netGamePlayer = player.netGamePlayer;
				if (netGamePlayer) {
					auto info = netGamePlayer->playerInfo;
					if (info) {
						menu::getPresence()->socialclubMessageNotify(info->netInfo.rockstarId2, "ozark.gg");
					}
				}
				//rage::network::getNetworkTask()->setWeather(player.id, 0xFFFF);
			});
		}
	});
}

networkNastyMenu* _instance;
networkNastyMenu* networkNastyMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkNastyMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkNastyMenu::~networkNastyMenu() { delete _instance; }