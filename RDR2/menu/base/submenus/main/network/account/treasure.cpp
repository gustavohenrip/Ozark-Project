#include "treasure.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../account.hpp"
#include "menu/base/util/spawn_manager.hpp"
#include "src/menu/base/util/fiber_manager.hpp"
#include "src/menu/base/util/fiber_pool.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/util/notify.hpp"
#include "global/auth_vars.hpp"
#include "utils/memory/pattern.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "menu/base/util/timers.hpp"

using namespace networkTreasureMenuVars;

namespace networkTreasureMenuVars {
	vars_ vars;

	scrollStruct<std::vector<uint32_t>> sets[] = {
		"Heirlooms", { 0x453BA146, 0x87E5C1D8, 0x4C5B2A0D, 0x886721FC, 0x6C23C90, 0x1F4FB3C2, 0xA738020E, 0x90682B5C, 0x103CFFA3, 0x77C2D500, 0x2AB52790, 0x39B2CB66, 0x78C52A23, 0x22E8F630, 0xEED898DB },
		"Coins", { 0x60431e06, 0x2858b99a, 0xb867ec85, 0xcfe17743, 0xf4271318, 0xee6b558b, 0x810d8a8c, 0xf82cf600, 0xbe1d9cfe, 0x42c83743, 0xd456e551, 0xf58a10a5, 0x53658fa8, 0x96a7b31e, 0x6793097b },
	};

	scrollStruct<Hash> chests[] = {
		"Classic", 0xA1654BA1,
		"Big Wooden", 0x3e835b01,
		"Redwood",0xc2c04fa3,
		"Drawer", 0x3728bbd2,
		"Cushioned", 0x6db5234e,
		"Blue", 0x6b1901b9,
		"Hunting", 0x8b90be92,
		"Wedding", 0x95a9eb4e,
		"Ice", 0xa446c2f3,
		"Water Logged", 0xb66b137a,
		"Log", 0xb78cd749,
		"Wicker", 0xce357fe8,
		"Bed", 0xdda9632e,
		"Chair", 0x285dde33,
		"Rusted", 0x4334f471,
		"Trap", 0x48df9bf0,
		"Bin", 0x4ff6e322,
		"Enforced", 0x11A1A693
	};

	scrollStruct<Hash> pickups[] = {
		"Billfold", 0x1FAE4B8A,
		"Gold", 0x2AB28031,
		"Jewelry Bag", 0x3526266F,
		"Boar Bristle Brush", 0x453BA146,
		"Ebony Brush", 0x87E5C1D8,
		"Goat Hair Brush", 0x4C5B2A0D,
		"Horse Hair Brush", 0x886721FC,
		"NG Rosewood Hair Brush", 0x6C23C90,
		"Rosewood Hair Brush", 0x1F4FB3C2,
		"Boxwoord Comb", 0xA738020E,
		"Cherrywood Comb", 0x90682B5C,
		"Ivory Comb", 0x103CFFA3,
		"Tortoiseshell Comb", 0x77C2D500,
		"Carved Wooden Hairpin", 0x2AB52790,
		"Ebony Hairpin", 0x39B2CB66,
		"Ivory Hairpin", 0x78C52A23,
		"Jade Hairpin", 0x22E8F630,
		"Metal Hairpin", 0xEED898DB,
	};

	Hash coins[15] = {
		0x1A5EF0D3,
		0xDABA5280,
		0x90B692DE,
		0x3D711686,
		0xEC621329,
		0xB11A5925,
		0xD31DDA00,
		0x9236D30A,
		0x7E2371F7,
		0x89DB84E7,
		0x1108B5D7,
		0x4F2A25D2,
		0x61C2A30C,
		0x8FA5B4B1,
		0x593A51C0
	};
  
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

	void transaction(Hash action, uint32_t itemHash, int unkIndex) {
		int transactionHandle = 0;
		uint32_t* buffer = new uint32_t[8 * 12];
		memset(buffer, 0, (8 * 12) - 1);
		buffer[16] = itemHash;
		buffer[18] = 1;
		buffer[22] = 0xA1212100;
		buffer[24] = 0x4A6726C9;
		buffer[26] = 0xE2D0DC18;
		buffer[34] = itemHash; 
		if (_i<BOOL>(0xF039EC27F4490E96, &transactionHandle, native::getHashKey("BASKET"), action)) {
			if (_i<BOOL>(0xa3b8d31c13cb4239, transactionHandle, action, buffer, 28, buffer, 0)) {
				_i<BOOL>(0x592BC00BF6629BE7, transactionHandle);
			}
		}
		delete[] buffer;
	}

	void sellCoinSet() {
		for (auto i = 0; i < 15; i++) {
			if (!getInventoryItemAmount(coins[i])) {
				return;
			}
		}
		transaction(0x20493AF8, 0xB443C2B9, 0x11);
	}

	bool setNetworked(Object object) {
		uint32_t timeout = GetTickCount() + 2000;
		while (!native::networkHasControlOfEntity(object) && GetTickCount() < timeout) {
			native::networkRequestControlOfEntity(object);
			native::networkRegisterEntityAsNetworked(object);
			native::setNetworkIdExistsOnAllMachines(native::networkGetNetworkIdFromEntity(object), 1);
			menu::util::waitFiber(0);
		}

		return timeout > GetTickCount();
	}

	//Hash Gun | Aimed entity hash: 90682B5C (2422745948)
	void treasure(Ped ped, Hash chestType,Hash pickupType, int chestCount, int barCount) {
		menu::getControlManager()->requestModel(chestType, [=](DWORD chestModel) {
			menu::getControlManager()->requestModel(pickupType, [=](DWORD goldModel) {
				math::vector3<float> chestCoords;
				Object chest = 0;

				for (std::size_t i = 0; i < chestCount; i++) {
					if (native::doesEntityExist(chest)) {
						math::vector3<float> min;
						math::vector3<float> max;

						native::getModelDimensions(chestModel, &min, &max);
						chestCoords = native::getOffsetFromEntityInWorldCoords(chest, { max.x * 3.0f, .0f, .0f });
					} else {
						chestCoords = native::getOffsetFromEntityInWorldCoords(ped, { .0f, 2.f, .0f });
					}

					native::getGroundZFor3dCoord(chestCoords.x, chestCoords.y, chestCoords.z, &chestCoords.z, 0);
					chest = menu::util::getSpawnManager()->spawnObject(chestModel, chestCoords);
					native::setEntityRotation(chest, native::getEntityRotation(ped, 0), 0, 0);
					setNetworked(chest);

					for (std::size_t j = 0; j < barCount; j++) {
						Object goldbar = menu::util::getSpawnManager()->spawnObject(goldModel, { chestCoords.x, chestCoords.y, chestCoords.z + .2f });
						setNetworked(goldbar);
					}
				}

				native::setModelAsNoLongerNeeded(goldModel);
				native::setModelAsNoLongerNeeded(chestModel);
				global::auth::g_treasureChestSpawnCount += vars.pickupCount;
			});
		});

	}

	void createSets(Ped ped, Hash chestHash, std::vector<Hash> pickups) {
		menu::getControlManager()->requestModel(chestHash, [=](DWORD chestModel) {
			auto coords = native::getOffsetFromEntityInWorldCoords(ped, { .0f, 2.f, .0f });
			native::getGroundZFor3dCoord(coords.x, coords.y, coords.z, &coords.z, 0);
			Object chest = menu::util::getSpawnManager()->spawnObject(chestModel, coords);
			native::setEntityRotation(chest, native::getEntityRotation(ped, 0), 0, 0);
			setNetworked(chest);
			for (std::size_t i = 0; i < pickups.size(); i++) {
				native::requestModel(pickups[i]);
				uint32_t timeout = GetTickCount() + 2000;
				while (!native::hasModelLoaded(pickups[i]) && timeout > GetTickCount()) {
					menu::util::waitFiber(0);
				}
				if (timeout > GetTickCount()) {
					Object pickup = menu::util::getSpawnManager()->spawnObject(pickups[i], { coords.x, coords.y, coords.z + .2f });
					setNetworked(pickup);
					native::setModelAsNoLongerNeeded(pickups[i]);
				}
			}
			native::setModelAsNoLongerNeeded(chestModel);
		});
	}

}

void networkTreasureMenu::init() {
	setName("Treasure");
	setParentSubmenu<networkAccountMenu>();
	

	//global::vars::g_infiniteTreasureMaps

	addOption(toggleOption("Infinite Treasure Maps")
		.addTranslation()
		.addToggle(global::vars::g_infiniteTreasureMaps));

	addOption(toggleOption("Infinite Inventory")
		.addTranslation()
		.addToggle(vars.infiniteItems)
		.addOnClick([] {
			*(uint8_t*)(global::vars::g_infiniteItems) = 0x49;
			*(uint8_t*)(global::vars::g_infiniteItems + 1) = 0x2B;
			*(uint8_t*)(global::vars::g_infiniteItems + 2) = 0xD4;
		})
		.addTooltip("This option is extremely risky."));

	addOption(toggleOption("Speedy Loot")
		.addTranslation()
		.addToggle(vars.speedyloot)
		.addTooltip("Speedy loot decreases the time it takes to loot an item from a chest."));

	addOption(toggleOption("Auto Sell Coin Set")
		.addTranslation()
		.addToggle(vars.autoSellCoinSet)
		.addTooltip("Emulates a madam nazar coin set sale if you have the coin set in your inventory"));

	addOption(breakOption("Pickup Sets"));

	addOption(scrollOption<std::vector<uint32_t>>(SCROLLSELECT, "Sets")
		.addTranslation()
		.addScroll(vars.set, 0, NUMOF(sets), sets)
		.addOnClick([] { createSets(menu::getLocalPlayer().ped, 0xA1654BA1, sets[vars.set].result); })
		.addTooltip("TREASURE IS A ~e~RISK"));

	addOption(breakOption("Custom")
		.addTranslation());

	addOption(scrollOption<Hash>(SCROLL, "Chest Type")
		.addTranslation()
		.addScroll(vars.chestType, 0, NUMOF(chests), chests)
		.addTooltip("TREASURE IS A ~e~RISK"));

	addOption(scrollOption<Hash>(SCROLL, "Pickup Type")
		.addTranslation()
		.addScroll(vars.pickupType, 0, NUMOF(pickups), pickups)
		.addTooltip("TREASURE IS A ~e~RISK"));

	addOption(numberOption<int>(SCROLL, "Chest Count")
		.addTranslation()
		.addNumber(vars.chestCount, "%i", 1).addMin(1).addMax(0x20)
		.addTooltip("TREASURE IS A ~e~RISK"));

	addOption(numberOption<int>(SCROLL, "Pickup Count")
		.addTranslation()
		.addNumber(vars.pickupCount, "%i", 1).addMin(1).addMax(200)
		.addTooltip("TREASURE IS A ~e~RISK"));

	addOption(buttonOption("Create Treasure Chest")
		.addTranslation()
		.addOnClick([] { treasure(menu::getLocalPlayer().ped, chests[vars.chestType].result, pickups[vars.pickupType].result, vars.chestCount, vars.pickupCount); })
		.addTooltip("TREASURE IS A ~e~RISK"));
}

/*Called while in submenu*/
void networkTreasureMenu::update() {}

/*Called once on submenu open*/
void networkTreasureMenu::updateOnce() {}

/*Called always*/
void networkTreasureMenu::featureUpdate() {

	if (vars.infiniteItems) {
		memory::getMemory()->nop(global::vars::g_infiniteItems, 3);
	}

	if (vars.speedyloot) {
		_i<int>(0x2208438012482A1A, menu::getLocalPlayer().ped, 0, 0);
	}

	if (vars.autoSellCoinSet) {
		static int32_t timeout = 0;
		menu::util::getTimers()->runTimedFunction(&timeout, 10000, [] {
			sellCoinSet();
		});
	}

}

networkTreasureMenu* _instance;
networkTreasureMenu* networkTreasureMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkTreasureMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkTreasureMenu::~networkTreasureMenu() { delete _instance; }