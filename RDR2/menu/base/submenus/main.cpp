#include "main.hpp"
#include "menu/base/submenu_handler.hpp"
#include "main/player.hpp"
#include "main/weapon.hpp"
#include "main/vehicle.hpp"
#include "main/teleport.hpp"
#include "main/world.hpp"
#include "main/spawn.hpp"
#include "main/misc.hpp"
#include "main/settings.hpp"
#include "main/network.hpp"
#include "main/search_results.hpp"
#include "menu/base/util/global.hpp"
#include "rage/session/session.hpp"
#include "rage/entity/entity.hpp"
#include "menu/base/util/spawn_manager.hpp"
#include "main/scripthook.hpp"
#include "menu/base/util/timers.hpp"
#include "menu/base/util/notify.hpp"
#include "global/auth_vars.hpp"
#include "utils/memory/pattern.hpp"
#include "menu/base/util/presence.hpp"
#include "utils/log.hpp"
#include "rage/api/socialclub.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/hooks/hooks.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "rage/entity/entity.hpp"
#include "utils/va.hpp"
#include "src/menu/base/util/fiber_manager.hpp"
#include "src/menu/base/util/fiber_pool.hpp"
#include "utils/hooking/hooking.hpp"
#include "utils/thread_pool.hpp"

using namespace mainMenuVars;

namespace mainMenuVars {
	vars_ vars;

	void searchOptions(const char* text) {
		if (!text) return;

		searchResultsMenu::getInstance()->clearOptionsOffset(0);

		std::string str(text);
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);

		bool foundSomething = false;
		for (auto& sub : menu::getSubmenuHandler()->getSubmenus()) {
			if (!sub->canBeSearched) continue;

			std::vector<std::shared_ptr<option>> tempOptions;
			for (auto& op : sub->options) {
				if (op->isVisible()) {
					std::string name = op->getName().get();
					std::transform(name.begin(), name.end(), name.begin(), ::tolower);

					if (name.find(str) != std::string::npos) {
						if (!op->isBreak()) {
							tempOptions.push_back(op);
							foundSomething = true;
						}
					}
				}
			}

			if (tempOptions.size() > 0) {
				searchResultsMenu::getInstance()->addOption(breakOption(sub->name.get()));
				for (auto& op : tempOptions) {
					searchResultsMenu::getInstance()->addOption(op);
				}
			}
		}

		if (foundSomething) {
			menu::getSubmenuHandler()->setSubmenu(searchResultsMenu::getInstance());
		} else {
			menu::getNotify()->bottom("No options found");
		}
	}

	void transaction(Hash action, uint32_t *data, int unkIndex) {
		int transactionHandle = 0;
		if (_i<BOOL>(0xF039EC27F4490E96, &transactionHandle, native::getHashKey("BASKET"), action)) { // _CASHINVENTORY_TRANSACTION_START
			if (_i<BOOL>(0x3745C002F5A21C45, transactionHandle, data, unkIndex)) { // _CASHINVENTORY_TRANSACTION_ADD_ITEM
				if (_i<BOOL>(0x592BC00BF6629BE7, transactionHandle)) { // _CASHINVENTORY_TRANSACTION_CHECKOUT
					printf("Checkout success\n");
				} else {
					printf("checkout failed\n");
				}
			} else {
				printf("add item failed\n");
			}
		} else {
			printf("transaction start failed\n");
		}
	}
	int getInventoryItemAmount(Hash hash) {
		Hash itemHash = 0;
		auto invPtr = caller::call<uint64_t>(global::vars::g_getInventoryList, *(uint64_t*)global::vars::g_inventoryPool, 2); // 2 is net inv id lol maybe
		if (invPtr) {
			for (std::uint16_t i = 0; i < *(std::uint16_t*)(invPtr + 0x50); i++) {
				auto itemPtr = *(uint64_t*)(*(uint64_t*)(invPtr + 0x48) + (i * 8));
				if (itemPtr) {
					auto itemInfoPtr = *(uint64_t*)(itemPtr + 0x38);
					if (itemInfoPtr) {
						itemHash = *(Hash*)(itemInfoPtr + 0x8);
					}
					else {
						itemHash = 0;
					}
					if (itemHash == hash) {
						return *(uint32_t*)(itemPtr + 0x18);
					}
				}
			}
		}
		return 0;
	}

	void setInventoryItemAmount(Hash hash, uint32_t value) {
		Hash itemHash = 0;
		auto invPtr = caller::call<uint64_t>(global::vars::g_getInventoryList, *(uint64_t*)global::vars::g_inventoryPool, 2); // 2 is net inv id lol maybe
		if (invPtr) {
			for (std::uint16_t i = 0; i < *(std::uint16_t*)(invPtr + 0x50); i++) {
				auto itemPtr = *(uint64_t*)(*(uint64_t*)(invPtr + 0x48) + (i * 8));
				if (itemPtr) {
					auto itemInfoPtr = *(uint64_t*)(itemPtr + 0x38);
					if (itemInfoPtr) {
						itemHash = *(Hash*)(itemInfoPtr + 0x8);
					}
					else {
						itemHash = 0;
					}
					if (itemHash == hash) {
						*(uint32_t*)(itemPtr + 0x18) = value;
					}
				}
			}
		}
	}
	bool monitorPool = false;
	void monitorInventoryPool() {
		static uint32_t excludes[] = {
			0x11D4D81C,
			0xB392591E,
			0x3E6C4053,
			0x3B2921F7,
			0x49C6E248,
			0xFE37E519,
			0x2905FBFB,
			0x58211DD,
			0x8A5A83E1,
			0xCA73B790,
			0x8E919F27,
			0xC72D9606,
			0x8C655849,
			0xD90F70BC,
			0xD2F834D8,
			0xE1CA07A,
			0xE2FFAF76,
			0xDBD9BF7,
			0x85F81459,
			0x8C655849,
			0xE8C06C94,
			0x321BA159,
			0xECAF54CB,
			0x7B14D38D,
			0x85F81459,
			0x7BA5E56E,0x743D4F54,0xEB356099,0xEB356099, 0x83C5E860, 0x83C5E860,0x743D4F54,0xEB356099,0xEB356099,0x83C5E860, 0x83C5E860,0x743D4F54,
			0x7BA5E56E,0xD2F834D8,0x321BA159,0xE2FFAF76, 0x7B14D38D,0xC72D9606,0xECAF54CB,0xC72D9606, 0xE8C06C94,0xEBD06161,0x886C55D7,0x321BA159,0xD90F70BC,0xF0F9B023, 0xDBD9BF7, 0x8E919F27,0xEBD06161, 0x573F9C58 ,0x573F9C58,0xC72D9606,
		};
		static std::unordered_map<Hash, uint32_t> inventoryAmount;
		Hash itemHash = 0;
		auto invPtr = caller::call<uint64_t>(global::vars::g_getInventoryList, *(uint64_t*)global::vars::g_inventoryPool, 2); // 2 is net inv id lol maybe
		if (invPtr) {
			for (std::uint16_t i = 0; i < *(std::uint16_t*)(invPtr + 0x50); i++) {
				auto itemPtr = *(uint64_t*)(*(uint64_t*)(invPtr + 0x48) + (i * 8));
				if (itemPtr) {
					auto itemInfoPtr = *(uint64_t*)(itemPtr + 0x38);
					if (itemInfoPtr) {
						itemHash = *(Hash*)(itemInfoPtr + 0x8);
					}
					else {
						itemHash = 0;
					}
					auto amount = *(uint32_t*)(itemPtr + 0x18);
					for (int i = 0; i < 57; i++) {
						if (itemHash == excludes[i]) {
							goto skip;
						}
					}
					if (inventoryAmount[itemHash] != amount) {
						inventoryAmount[itemHash] = amount;
						printf("label: %s | hash: 0x%X | amount: %i\n", _i<const char*>(0xBD5DD5EAE2B6CE14, itemHash), itemHash, *(uint32_t*)(itemPtr + 0x18));
					}
				skip:
					printf("");
				}
			}
		}
	}
	void dumpInventoryPool() {
		Hash itemHash = 0;
		auto invPtr = caller::call<uint64_t>(global::vars::g_getInventoryList, *(uint64_t*)global::vars::g_inventoryPool, 2); // 2 is net inv id lol maybe
		if (invPtr) {
			for (std::uint16_t i = 0; i < *(std::uint16_t*)(invPtr + 0x50); i++) {
				auto itemPtr = *(uint64_t*)(*(uint64_t*)(invPtr + 0x48) + (i * 8));
				if (itemPtr) {
					auto itemInfoPtr = *(uint64_t*)(itemPtr + 0x38);
					if (itemInfoPtr) {
						itemHash = *(Hash*)(itemInfoPtr + 0x8);
					}
					else {
						itemHash = 0;
					}
					printf("label: %s | hash: 0x%X | amount: %i\n",_i<const char*>(0xBD5DD5EAE2B6CE14, itemHash), itemHash, *(uint32_t*)(itemPtr + 0x18));
				}
			}
		}
	}
}

void mainMenu::init() {
	setName("Main Menu");

	addOption(submenuOption("Player")
		.addTranslation()
		.addSubmenu<playerMenu>());

	addOption(submenuOption("Network")
		.addTranslation()
		.addSubmenu<networkMenu>());

	addOption(submenuOption("Weapon")
		.addTranslation()
		.addSubmenu<weaponMenu>());

	addOption(submenuOption("Vehicle")
		.addTranslation()
		.addSubmenu<vehicleMenu>());

	addOption(submenuOption("Teleport")
		.addTranslation()
		.addSubmenu<teleportMenu>());

	addOption(submenuOption("World")
		.addTranslation()
		.addSubmenu<worldMenu>());

	addOption(submenuOption("Spawn")
		.addTranslation()
		.addSubmenu<spawnMenu>());

	addOption(submenuOption("Miscellaneous")
		.addTranslation()
		.addSubmenu<miscMenu>());

	addOption(submenuOption("ScriptHook")
		.addTranslation()
		.addSubmenu<scriptHookMenu>());

	addOption(submenuOption("Settings")
		.addTranslation()
		.addSubmenu<settingsMenu>());

	addOption(buttonOption("Search")
		.addTranslation()
		.addKeyboard("Enter option to search for", 100, searchOptions));

#ifdef DEV
	addOption(breakOption("Testing"));

	addOption(buttonOption("RID -> Name")
		.addKeyboard("Enter rid", 100, [] (const char* msg) {
			rage::api::getSocialclub()->getProfileFromRID(std::atoll(msg), [=] (bool success, rage::network::rockstarAccount acc) {
				dev("%s -> %s", msg, acc.name.c_str());
			});
		}));

	addOption(buttonOption("Name -> RID")
		.addKeyboard("Enter name", 100, [] (const char* msg) {
		rage::api::getSocialclub()->getProfileFromName(msg, [=] (bool success, rage::network::rockstarAccount acc) {
			dev("%s -> %i", msg, acc.rockstarId);
		});
	}));

	addOption(buttonOption("idkwhatimdoing")
		.addOnClick([] {
			//0x3B3D11CD9FFCDFC9 set mission finished

			_i<int>(0x3B3D11CD9FFCDFC9);

			// C086FF658B2E51DA create posse(*ptr)

			// 0xc08aff658b2e51da get current posse info(&out)

			// C08EFF658B2E51DB kick posse member (posse info, player handle)

			// C08DEF658B2E51DA join posse (unk integer)

			/*setInventoryItemAmount(0x3155EAD4, 0);
			setInventoryItemAmount(0xDF78113E, 20);*/

	/*		setInventoryItemAmount(0x3155EAD4, 20);
			setInventoryItemAmount(0xDF78113E, 0);*/
			//0xDF78113E 
			//auto hash = 0xeb319577;
			//native::requestModel(hash);

			//Object idkwhatimdoing = native::createObject(hash, { .0f, .0f, .0f }, 1, 0, 0, 0, 0);
			//if (idkwhatimdoing) {
			//	auto modelInfo = rage::engine::getModelInfo(hash);
			//	auto netObj = (rage::network::netObject*)rage::entity::getEntityTask()->getNetObj(idkwhatimdoing);
			//	if (modelInfo) {
			//		printf("Type32: %X | Type: %X\n", *(int32_t*)(modelInfo + 0x3E0), netObj->objectType);
			//	}
			//}

			/*
			network_register_player_broadcast_variables(Global_1145091, 1121, 67);
			func_185(_0x690806bc83bc8ca2((*Global_1145091)[0]), 1121, "g_mpPlayerPersona");
			*/


			//statId[0]: 0x908C41F0 | statId[1]: 0x0 | edit: 6250

			//unsigned int statId[2] = { 0x620C8405 , 0 };
			//_i<int>(0xBC3422DC91667621, 6250, 752097756);
			//_i<int>(0x6a0184e904cdf25e, &statId, 6250);
			//int nigguh = 1;
			// _i<int>(0x767FBC2AC802EF3E, &statId, &nigguh);
			// printf("%i\n", nigguh);
		/*	int32_t* buffer = new int32_t[8 * 12];
			int32_t* buffer_2 = new int32_t[8 * 12];
			auto res = _i<int>(0xCB5D11F9508A928D, 2, buffer, buffer_2, 0xD2F834D8, 1084182731, 1, -334626412);
			printf("gang: %i\n", res);
			delete[] buffer;
			delete[] buffer_2;
			int32_t* buffer_3 = new int32_t[10];
			_i<int>(0xFE90ABBCBFDC13B2, 0xD2F834D8, buffer_3);
			for (int i = 0; i < 6; i++) {
				printf("%i, ", buffer_3[i]);
			}
			printf("\n");
			delete[] buffer_3;*/
			//auto ptr = menu::globals(1145091).at(menu::getLocalPlayer().id, 35).get<int>();
			//lobal_1293346->f_3009[iParam0]
			//_i<int>(0x0FBBFFC891A97C81, 2);
			//Global_1572887->f_13 is online
			/*menu::globals(1901947).at(766).at(8).as<int>() = 1;
			menu::globals(1901947).at(766).at(7).as<int>() = 1;
			menu::globals(1901947).at(766).at(6).as<int>() = 1;*/

			//	Global_1293346->f_3009[iParam0] = iVar1;
			//printf("Ptr: %llX\n", (uint64_t)ptr);
			//printf("fuck: %i\n", menu::globals(1293346).at(3009 + 1).at(11).as<int>());
			//menu::globals(1293346).at(3009 + 1).at(7).as<int>() = 20; // bounty hunter //-999519443
			//setInventoryItemAmount(-999519443, 20);
			//menu::globals(1293346).at(3009 + 1).at(12).as<int>() = 20; // collector // 128291088
			//setInventoryItemAmount(128291088, 20);
			//menu::globals(1293346).at(3009 + 1).at(11).as<int>() = 20; // trader // -1399517481
			//setInventoryItemAmount(-1399517481, 20);
			//menu::globals(1293346).at(3009 + 1).at(15).as<int>() = 20; // moonshiner  // 1371028520
			//setInventoryItemAmount(1371028520, 20);
			//menu::globals(1293346).at(3009 + 1).at(18).as<int>() = 20; //naturalist // -296143963
			//setInventoryItemAmount(-296143963, 20);

			//setInventoryItemAmount(-999519443, 20);
			//setInventoryItemAmount(128291088, 20);
			//setInventoryItemAmount(-1399517481, 20);
			//setInventoryItemAmount(1371028520, 20);
			//setInventoryItemAmount(-296143963, 20);
			//Global_1901947->f_766.f_8
			//Global_1901947->f_766.f_7
			//Global_1901947->f_766.f_6

			

			}));

	addOption(buttonOption("dump inventory pool")
		.addOnClick([] {
			dumpInventoryPool();

		}));
	addOption(toggleOption("monitor inventory pool")
	.addToggle(monitorPool));

#endif
}

void mainMenu::update() {}

/*Called once on submenu open*/
void mainMenu::updateOnce() {}

/*Called always*/
void mainMenu::featureUpdate() {
	static int timer = 0;
	menu::util::getTimers()->runTimedFunction(&timer, 1000, [] {
		if (native::networkIsInSession()) {
			global::auth::g_secondsInMP++;
		}
	});

	if (monitorPool) {
		monitorInventoryPool();
		//setInventoryItemAmount(0xB3945FB9, 5);
		//0xB3945FB9
	}
	//setInventoryItemAmount(0xB3945FB9, 5);
	//if (setModel) {
	//	//if (!menu::globals(1139381).at(4796).at(9).as<int>()) {
	//	//	menu::globals(1139381).at(4796).at(9).as<int>() = 1;
	//	//}
	//	//if (!menu::globals(1139381).at(4796).as<int>()) {
	//	//	menu::globals(1139381).at(4796).as<int>() = 1;
	//	//}
	//	//menu::globals(1139381).at(4796).at(51).at(2).as<int>() = 0;
	//	//menu::globals(1102219).at(3578).as<int>() = 0;
	//	//menu::globals(1572864).at(17).as<int>() = 0;
	//	//menu::globals(1102219).at(0).as<int>() = -1;


	//	//Global_1572864->f_21
	//	static int gayIndex = -10;
	//	if (gayIndex != menu::globals(1572864).at(1).as<int>()) {
	//		gayIndex = menu::globals(1572864).at(1).as<int>();
	//		printf("gayIndex = %i \n", gayIndex);
	//	}

	//	static int kickIndex = 0;
	//	if (kickIndex != menu::globals(1572864).at(17).as<int>()) {
	//		kickIndex = menu::globals(1572864).at(17).as<int>();
	//		printf("kickIndex = %i | unk: %i \n", kickIndex, menu::globals(1572864).at(18).as<int>());
	//	}


	//	static int fuckMe = -10;
	//	if (fuckMe != menu::globals(1572887).at(266).as<int>()) {
	//		fuckMe = menu::globals(1572887).at(266).as<int>();
	//		printf("fuck me lol = %i\n", fuckMe);
	//	}

	//	//Global_1102219

	//	//static int hehe1 = menu::globals(1572864).at(17).as<int>();
	//	//if (hehe1 != menu::globals(1572864).at(17).as<int>()) {
	//	//	menu::globals(1572864).at(17).as<int>() = hehe1;
	//	//}

	//	//static int hehe2 = menu::globals(1572864).at(18).as<int>();
	//	//if (hehe2 != menu::globals(1572864).at(18).as<int>()) {
	//	//	menu::globals(1572864).at(18).as<int>() = hehe2;
	//	//}

	//	//static int hehe3 = menu::globals(1572864).at(3).as<int>();
	//	//if (hehe3 != menu::globals(1572864).at(3).as<int>()) {
	//	//	menu::globals(1572864).at(3).as<int>() = hehe3;
	//	//}

	//	//static int hehe4 = menu::globals(1572864).at(1).as<int>();
	//	//if (hehe4 != menu::globals(1572864).at(1).as<int>()) {
	//	//	menu::globals(1572864).at(1).as<int>() = hehe4;
	//	//}
	//	//static int buffer[5] = {0,0,0,0,0};
	//	//static bool runYeet = false;
	//	//if (!runYeet) {
	//	//	runYeet = true;
	//	//	int fuck = 0;
	//	//	while (fuck <= 2) {
	//	//		buffer[fuck] = menu::globals(1572887).at(405).at(1, fuck).as<int>();
	//	//		fuck++;
	//	//	}
	//	//}
	//	//int fuck = 0;
	//	//while (fuck <= 2) {
	//	//	menu::globals(1572887).at(405).at(1, fuck).as<int>() = buffer[fuck];
	//	//	fuck++;
	//	//}
	//	/*
	//	
	//		iVar0 = 0;
	//		while (iVar0 <= 2)
	//		{
	//			Global_1572887->f_405.f_1[iVar0] = 0;
	//			iVar0++;
	//		}

	//	*/

	//	//if (menu::globals(1572864).at(0).as<int>() != menu::globals(1572864).at(1).as<int>()) {
	//	//	menu::globals(1572864).at(1).as<int>() = menu::globals(1572864).at(0).as<int>();
	//	//}
	//	//if (menu::globals(1572864).at(3).as<int>() != 0) {
	//	//	menu::globals(1572864).at(3).as<int>() = 0;
	//	//}
	//	//menu::globals(1572887).at(266).at(91).as<int>() = 0;
	//	menu::globals(1572864).at(1).as<int>() = 3;
	//	//Global_1572864->f_1

	//	//Global_1572887->f_266


	//	//Global_1196898->f_78[iVar0]->f_5
	//	//Global_1196898->f_78[iVar0]->f_4

	//	//menu::globals(1952637).at(1).as<int>() = 0;
	//	//menu::globals(1102219).at(0).as<int>() = -1;
	//	//Global_1572864->f_17
	//	//menu::globals(1572864).at(21).as<int>() = 3;
	//	//if (native::getEntityModel(menu::getLocalPlayer().ped) == 0x80D04451) {
	//	//	menu::globals(1102219).at(3578).as<int>() = native::getHashKey("mp_male");
	//	//	setModel = false;
	//	//}
	//	//else {
	//	//menu::globals(1139381).at(4796).as<int>() = 1; // pass check
	//	//menu::globals(1139381).at(4796).at(1).as<int>() = 1; // pass check
	//	//menu::globals(1139381).at(4796).at(16).at(3).as<int>() = 0; // make the function return
	//	//menu::globals(1102219).at(3578).as<int>() = 0x39c84a35;

	//	//}
	//}
}


mainMenu* _instance;
mainMenu* mainMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new mainMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

mainMenu::~mainMenu() { delete _instance; }