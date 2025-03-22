#include "mini_games.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../world.hpp"
#include "../player.hpp"
#include "time.hpp"
#include "weather.hpp"
#include "menu/base/util/spawn_manager.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/util/timers.hpp"
#include "menu/base/renderer.hpp"
#include "menu/base/util/control_manager.hpp"
#include "src/menu/base/util/fiber_manager.hpp"
#include "src/menu/base/util/fiber_pool.hpp"
#include "menu/base/util/notify.hpp"

using namespace miniGamesMenuVars;

namespace miniGamesMenuVars {
	vars_ vars;
	
	//0x717c9f9 dead man
	//0x53367A8A dead woman corpse
	//0x5F3EE4D3 two headed skeleton
	//0xe533d2b4 another dead woman
	//0xd076c393 man corpse
	//0x4c6c6086 man corpse

	// TODO
	// wave system
	// points system
	// spawning system

	uint32_t undead[6] = {
		0x717c9f9,
		0x53367A8A,
		0x5F3EE4D3,
		0xe533d2b4,
		0xd076c393,
		0x4c6c6086
	};

	void minigameText(const char * text, math::vector2<float> screen, float scale, uint time, color color) {
		uint timer = GetTickCount() + time;
		menu::util::getFiberPool()->run([=] {
			while (timer > GetTickCount()) {
				menu::getRenderer()->drawText(text, { screen.x, screen.y }, scale, 4, color, JUSTIFY_CENTER, true);
				menu::util::waitFiber(0);
			}
		});
	}

	void undeadNightmare() {
		// create undead
		static uint levelTimer = 0;
		static uint levelProgressionBuffer = 0;
		static int level = 1;
		if (vars.stopMiniGame) {
			level = 1;
			vars.undeadNightmareLevel = 1;
			levelTimer = 0;
			vars.undeadNightmare = false;
			vars.stopMiniGame = false;
			for (std::size_t i = 0; i < vars.undeadPeds[0].size(); i++) {
				native::networkRequestControlOfEntity(vars.undeadPeds[0][i]);
				native::setEntityAsMissionEntity(vars.undeadPeds[0][i], TRUE, TRUE);
				auto entity = vars.undeadPeds[0][i];
				native::deleteEntity(&entity);
			}
			vars.undeadPeds[0].clear();
			vars.undeadPeds[1].clear();
			weatherMenuVars::setWeather(0x614a1f91, true);
			timeMenuVars::setTime({ 10, 0, 0 }, true);
			return;
		}
		if (vars.undeadNightmareLevel <= vars.undeadNightmareMaxLevel) {
			if (level == vars.undeadNightmareLevel) {
				if (levelProgressionBuffer < GetTickCount()) {
					for (std::size_t j = 0; j < (vars.undeadNightmareLevel * 5); j++) {
						Hash hash = undead[native::getRandomIntInRange(0, 5)];
						menu::getControlManager()->requestModel(hash, [=](DWORD modelHash) {
							std::vector<float> random;
							for (std::size_t i = 0; i < 2; i++) {
								random.push_back(native::getRandomFloatInRange(j % 2 ? 15.f : -20.f, j % 2 ? 20.f : -15.f));
							}
							auto offset = native::getOffsetFromEntityInWorldCoords(menu::getLocalPlayer().ped, { random[0], random[1], .0f });
							native::getGroundZFor3dCoord(offset.x, offset.y, offset.z, &offset.z, 0);
							menu::util::getFiberPool()->run([=] {
								Ped ped = menu::util::getSpawnManager()->spawnPed(modelHash, offset, 0.f);
								uint timer = GetTickCount() + 1000;
								while (!native::doesEntityExist(ped) && timer > GetTickCount()) {
									menu::util::waitFiber(0);
								}
								if (timer < GetTickCount()) {
									return;
								}
								native::setPedMaxHealth(ped, (vars.undeadNightmareMaxLevel * 200));
								native::setEntityHealth(ped, (level * 200), 1);
								native::setPedAsGroupMember(ped, 69);
								native::setPedCombatAbility(ped, (level * 10));
								native::taskCombatPed(ped, menu::getLocalPlayer().ped, 0, 16);
								vars.undeadPeds[0].push_back(ped);
								vars.undeadPeds[1].push_back(ped);
								native::setModelAsNoLongerNeeded(modelHash);
								});
							random.clear();
							});
					}
					menu::getNotify()->bottom("Level ~e~" + std::to_string(level));
					level += 1;
					levelTimer = GetTickCount() + 240000;
				}
			} else {
				if (vars.undeadPeds[0].size()/* == (vars.undeadNightmareLevel * 5)*/) {
					if (vars.undeadPeds[1].size() && levelTimer > GetTickCount()) {
						for (std::size_t i = 0; i < vars.undeadPeds[1].size(); i++) {
							if (native::isPedDeadOrDying(vars.undeadPeds[1][i], FALSE)) {
								math::vector2<float> screen;
								if (native::getScreenCoordFromWorldCoord(native::getEntityCoords(vars.undeadPeds[1][i]), &screen.x, &screen.y)) {
									screen.y -= .075f;
									minigameText("+5", screen, 0.27f, 800, { 255, 247, 28, 200 });
								}
								vars.undeadPeds[1].erase(vars.undeadPeds[1].begin() + i);

							}

							static int timer[200];
							menu::util::getTimers()->runTimedFunction(&timer[i], 6000, [&] {
								native::setPedCombatRange(vars.undeadPeds[1][i], 1000);
								native::taskCombatPed(vars.undeadPeds[1][i], native::getPlayerPed(native::getNearestPlayerToEntity(vars.undeadPeds[1][i], 0, 0)), 0, 16);
							});
						}
					} else {
						if (levelTimer < GetTickCount()) {
							vars.undeadNightmareLevel = 1;
							level = 1;
						} else {
							vars.undeadNightmareLevel++;
						}
						for (std::size_t i = 0; i < vars.undeadPeds[0].size(); i++) {
							native::networkRequestControlOfEntity(vars.undeadPeds[0][i]);
							native::setEntityAsMissionEntity(vars.undeadPeds[0][i], TRUE, TRUE);
							auto entity = vars.undeadPeds[0][i];
							native::deleteEntity(&entity);
						}
						levelProgressionBuffer = GetTickCount() + 8000;
						menu::getNotify()->bottom("Level ~e~" + std::to_string(vars.undeadNightmareLevel - 1) + " ~t6~Completed!");
						vars.undeadPeds[0].clear();
						vars.undeadPeds[1].clear();
					}
				}
			}
		} else {
			menu::getNotify()->bottom("~e~Undead Nightmare ~t6~Completed!");
			vars.stopMiniGame = true;
		}
	}

}

void miniGamesMenu::init() {
	setName("Mini Games");
	setParentSubmenu<worldMenu>();

	addOption(buttonOption("Stop Mini Game")
		.addTranslation()
		.addOnClick([] { vars.stopMiniGame = true; })
		.addRequirement([] { return vars.undeadNightmare; }));

	addOption(breakOption("Mini Games")
		.addTranslation()
		.addRequirement([] { return vars.undeadNightmare; }));

	addOption(buttonOption("Undead Nightmare")
		.addTranslation()
		.addOnClick([] {
			weatherMenuVars::setWeather(0x5974e8e5, true);// misty
			timeMenuVars::setTime({ 22, 0, 0 }, true);
			if (playerMenuVars::vars.invincibility) {
				playerMenuVars::vars.invincibility = false;
				//menu::getNotify()->bottom("Invincibility has been ~e~Disabled");
			}
			vars.undeadNightmare = true;
		}));
}

/*Called while in submenu*/
void miniGamesMenu::update() {}

/*Called once on submenu open*/
void miniGamesMenu::updateOnce() {}

/*Called always*/
void miniGamesMenu::featureUpdate() {
	if (vars.undeadNightmare) {
		undeadNightmare();
	}

}

miniGamesMenu* _instance;
miniGamesMenu* miniGamesMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new miniGamesMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

miniGamesMenu::~miniGamesMenu() { delete _instance; }