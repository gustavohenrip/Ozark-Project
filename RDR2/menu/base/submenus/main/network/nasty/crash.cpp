#include "crash.hpp"
#include "menu/base/submenu_handler.hpp"
#include "nasty.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/util/spawn_manager.hpp"
#include "menu/base/util/timers.hpp"
#include "menu/base/util/notify.hpp"
#include "rage/session/session.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "utils/va.hpp"
#include "utils/memory/pattern.hpp"
#include "src/menu/base/util/fiber_manager.hpp"
#include "rage/entity/entity.hpp"
#include "src/menu/base/util/fiber_pool.hpp"

using namespace networkCrashMenuVars;

namespace networkCrashMenuVars {
	vars_ vars;

	void v1(menu::playerVars &player) {
		if (player.isOzarkPassive) return;

		static std::vector<std::pair<uint32_t, uint32_t>> crashModels = {
			{0xC50FC5D0, 1},
			{0x861d4c6a, 1}
		};

		std::for_each(crashModels.begin(), crashModels.end(), [=](std::pair<uint32_t, uint32_t>& data) {
			menu::getControlManager()->requestModel(data.first, [=](DWORD model) {
				auto modelInfo = rage::engine::getModelInfo(model);
				auto originalHandling = modelInfo->vehicleHandlingType;
				modelInfo->vehicleHandlingType = data.second;
				Ped veh = menu::util::getSpawnManager()->spawnVehicle(model, player.coords, 0.f);
				if (veh) {
					menu::util::waitFiber(50);
					if (native::doesEntityExist(veh)) {
						modelInfo->vehicleHandlingType = originalHandling;
						native::setEntityVisible(veh, FALSE);
						menu::util::waitFiber(200);
						native::setEntityAsMissionEntity(veh, TRUE, TRUE);
						native::deleteEntity(&veh);
					}
				}

			});
		});
	}

	void v2(menu::playerVars& player) {
		if (player.isOzarkPassive) return;
		menu::getControlManager()->requestModel(0x1EEBDBE5, [=](DWORD model) {
			auto veh = menu::util::getSpawnManager()->spawnVehicle(model, {}, 0);
			global::vars::g_syncCrashEntity = veh;
			global::vars::g_syncCrashPlayerName = player.name;
			if (veh) {
				native::attachEntityToEntity(veh, player.ped, 0, {.0f, .0f, -10.f}, {}, 0, 0, 0, 1, 0, 0, 0, 0);
				native::setEntityVisible(veh, FALSE);
			}
		});

	}

	//uint64_t syncNetObj;
	//void v1(menu::playerVars& player) {
	//	if (player.isOzarkPassive) return;

	//	if (!syncNetObj) {
	//		//48 89 5C 24 ? 48 89 4C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 0F B7 42 42 48 8B sync ped to player
	//		//if (!memory::getPattern()->scanProcess({ "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 20 49 8B F0 48 8B FA 44", 0, "sync object to player", true }, [&](uint64_t address) {
	//		if (!memory::getPattern()->scanProcess({ "48 89 5C 24 ? 48 89 4C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 0F B7 42 42 48 8B", 0, "sync ped to player", true }, [&](uint64_t address) {
	//			syncNetObj = address;
	//			})) return;
	//	}

	//	global::vars::g_syncCrashPlayerName = std::string(player.name);
	//	menu::getNotify()->right(utils::va::va("Crashing ~e~%s~q~. Please wait...", player.name));

	//	player.crashPlayer = true;
	//	menu::getControlManager()->requestModel(0xF887B8E8, [=](DWORD model) {
	//		Ped ped = menu::util::getSpawnManager()->spawnPed(model, player.coords, .0f);
	//		menu::util::waitFiber(50);
	//		if (native::doesEntityExist(ped)) {
	//			global::vars::g_syncCrashEntity = ped;

	//			auto idkwhatimdoing = rage::entity::getEntityTask()->getNetObj(ped);
	//			if (idkwhatimdoing) {
	//				if (global::vars::g_netObjMgr) {
	//					uint64_t yeet = caller::call<uint64_t>(syncNetObj, *(uint64_t*)global::vars::g_netObjMgr, idkwhatimdoing, rage::engine::getNetGamePlayerFromIndex(menu::getSelectedPlayer().id));
	//					if (yeet) {
	//						printf("sent net entity\n");
	//					}
	//				} else {
	//					printf("netmgr == NULL\n");
	//				}
	//			}
	//			//native::setEntityVisible(ped, FALSE);
	//		}
	//	});
	//	//native::networkSetInSpectatorMode(TRUE, player.ped);
	//}


	//void v1(menu::playerVars& player) {
	//	if (player.isOzarkPassive) return;
	//	static uint64_t addr = 0;
	//	if (!addr) {
	//		if (!memory::getPattern()->scanProcess({ "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 30 48 8B 02 48 8B FA 4C 8B", 0, "pack clone remove", true }, [&](uint64_t address) {
	//			addr = address;
	//			})) return;
	//	}

	//	menu::getControlManager()->requestModel(0xF887B8E8, [=](DWORD model) {
	//		Ped ped = menu::util::getSpawnManager()->spawnPed(model, player.coords, .0f);
	//		menu::util::waitFiber(50);
	//		if (native::doesEntityExist(ped)) {

	//			auto idkwhatimdoing = rage::entity::getEntityTask()->getNetObj(ped);
	//			if (idkwhatimdoing) {
	//				if (caller::call<uint8_t>(addr, *(uint64_t*)global::vars::g_netObjMgr, rage::entity::getEntityTask()->getNetObj(menu::getSelectedPlayer().ped), rage::engine::getNetGamePlayerFromIndex(menu::getSelectedPlayer().id), true)) {
	//					printf("packing clone removal\n");
	//				} else {
	//					printf("packing clone removal failed\n");
	//				}
	//			}
	//		}
	//	});
	//}

	//48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 30 48 8B 02 48 8B FA 4C 8B


	//void v2(Player id, bool flag) {
	//	auto player = menu::getPlayer(id);
	//	//if (player.isOzarkPassive) return;

	//	if (flag) {
	//		menu::getNotify()->right(utils::va::va("Crashing ~e~%s~q~. Please wait...", player.name));
	//	}
	//	rage::network::getNetworkTask()->setWeather(player.id, 0x24);

	//	caller::call<bool>(global::vars::g_triggerWeatherEvent, 0, 0x24, 0x10, 0);

	//	//if (!player.isOzarkHiddenUser) {
	//	//	rage::network::getNetworkTask()->setWeather(player.id, 0x24);
	//	//}
	//}

	void v3(Player id, bool flag) {
		auto player = menu::getPlayer(id);
		if (player.isOzarkPassive) return;

		if (flag) {
			menu::getNotify()->right(utils::va::va("Crashing ~e~%s~q~. Please wait...", player.name));
		}
		v2(menu::getPlayer(id));
		rage::engine::rewardsPickupCrash(player.id);
	}
}

void networkCrashMenu::init() {
	setName("Crashes");
	setParentSubmenu<networkNastyMenu>();

	addOption(buttonOption("Crash [v1]")
		.addTranslation()
		.addOnClick([] {
			v1(menu::getSelectedPlayer());
		}));

	addOption(buttonOption("Crash [v2]")
		.addTranslation()
		.addOnClick([] {
			v2(menu::getSelectedPlayer());
		}));

	//addOption(buttonOption("Crash [v2]")
	//	.addTranslation()
	//	.addOnClick([] { 
	//		v2(menu::getSelectedPlayer().id);
	//	}));

	//addOption(buttonOption("Crash [v2]")
	//	.addTranslation()
	//	.addOnClick([] {
	//		v3(menu::getSelectedPlayer().id);
	//	}));

	//addOption(buttonOption("Crash [v3]")
	//	.addTranslation()
	//	.addOnClick([] {
	//		v2(menu::getSelectedPlayer());
	//		}));

	//addOption(buttonOption("Crash [AIO]")
	//	.addTranslation()
	//	.addOnClick([] {
	//		v1(menu::getSelectedPlayer());
	//		v3(menu::getSelectedPlayer().id);
	//	}));
}

/*Called while in submenu*/
void networkCrashMenu::update() {
	menu::getPanelManager()->panelPlayer(menu::getSelectedPlayer().id);
}

/*Called once on submenu open*/
void networkCrashMenu::updateOnce() {}

/*Called always*/
void networkCrashMenu::featureUpdate() {
	menu::getPlayerManager()->foreach([=](menu::playerVars& player) {

		if (player.crashPlayer) {
			if (native::doesEntityExist(global::vars::g_syncCrashEntity)) {
				static int crashTimer[32];
				menu::util::getTimers()->runTimedFunction(&crashTimer[player.id], 200, [&] {
					if (rage::entity::getEntityTask()->getNetObjOwner(global::vars::g_syncCrashEntity) != menu::getLocalPlayer().id) {
						menu::getControlManager()->requestControl(global::vars::g_syncCrashEntity, [](Entity entity) {}, true);
					}
					native::setEntityCoords(global::vars::g_syncCrashEntity, player.coords);
				//	auto idkwhatimdoing = rage::entity::getEntityTask()->getNetObj(menu::getLocalPlayer().ped);
					//uint64_t yeet = caller::call<uint64_t>(addr, *(uint64_t*)global::vars::g_netObjMgr, idkwhatimdoing, rage::engine::getNetGamePlayerFromIndex(menu::getSelectedPlayer().id));
				});
			}
		}

		});
}

networkCrashMenu* _instance;
networkCrashMenu* networkCrashMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkCrashMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkCrashMenu::~networkCrashMenu() { delete _instance; }