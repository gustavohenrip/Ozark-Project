#include "role_booster.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../account.hpp"
#include "menu/base/util/spawn_manager.hpp"
#include "src/menu/base/util/fiber_manager.hpp"
#include "src/menu/base/util/fiber_pool.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/util/notify.hpp"
#include "global/auth_vars.hpp"
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
#include "menu/base/util/presence.hpp"


using namespace networkRoleBoosterMenuVars;

namespace networkRoleBoosterMenuVars {
	vars_ vars;

	void setInventoryItemAmount(Hash hash, int32_t amt) {
		Hash itemHash = 0;
		auto invPtr = caller::call<uint64_t>(global::vars::g_getInventoryList, *(uint64_t*)global::vars::g_inventoryPool, 2); // 2 is net inv id lol maybe
		if (invPtr) {
			for (std::uint16_t i = 0; i < *(std::uint16_t*)(invPtr + 0x50); i++) {
				auto itemPtr = *(uint64_t*)(*(uint64_t*)(invPtr + 0x48) + (i * 8));
				if (itemPtr) {
					auto itemInfoPtr = *(uint64_t*)(itemPtr + 0x38);
					if (itemInfoPtr) {
						itemHash = *(Hash*)(itemInfoPtr + 0x8);
					} else {
						itemHash = 0;
					}
					if (itemHash == hash) {
						*(uint32_t*)(itemPtr + 0x18) = amt;
					}
				}
			}
		}
	}

}

void networkRoleBoosterMenu::init() {
	setName("Boosters");
	setParentSubmenu<networkAccountMenu>();
	
	addOption(toggleOption("Moonshine Booster")
		.addTranslation()
		.addToggle(global::vars::g_moonshinerBooster)
		.addTooltip("This will speed up moonshine production\nYou'll have to change your session for this to work."));

	addOption(toggleOption("Trader Booster")
		.addTranslation()
		.addToggle(vars.traderBooster)
		.addOnClick([] {
			//menu::globals(1901947).at(584).at(11).as<int>() = 0; // mins per goods
			//menu::globals(1901947).at(584).at(13).as<int>() = 0; // mins for resupply
			////menu::globals(1901947).at(584).at(9).as<int>() = 10000000; // max materials
			//menu::globals(1901947).at(584).at(8).as<int>() = 1000; // camp shop ms failsafe timeout
		})
		.addTooltip("This will auto set trader goods to max"));

	//addOption(buttonOption("Horse Bond Booster")
	//	.addTranslation()
	//	.addOnClick([] {
	//		for (int32_t i = 1; i < 27; i++) {
	//			menu::globals(1901947).at(681).at(i).as<float>() = 500.f;
	//		}
	//	})
	//	.addTooltip("Pat your horse and feed it a few times after pressing this option."));


	addOption(buttonOption("Spoof Max Role")
		.addTranslation()
		.addOnClick([] {
			vars.unlockAllMaxRoleAwards = true;
			//vars.originalRoleRank
			//vars.originalRoleRank[0] = menu::globals(1293346).at(3009 + 1).at(7).as<int>(); // bounty hunter //-999519443
			//vars.originalRoleRank[1] = menu::globals(1293346).at(3009 + 1).at(12).as<int>(); // collector // 128291088
			//vars.originalRoleRank[2] = menu::globals(1293346).at(3009 + 1).at(11).as<int>(); // trader // -1399517481
			//vars.originalRoleRank[3] = menu::globals(1293346).at(3009 + 1).at(15).as<int>(); // moonshiner  // 1371028520
			//vars.originalRoleRank[4] = menu::globals(1293346).at(3009 + 1).at(18).as<int>(); //naturalist // -296143963
			//vars.originalRoleRank[5] = menu::globals(1293346).at(3009 + 1).at(22).as<int>(); //halloween // -296143963
			//for (int i = 0; i < 6; i++) {
			//	printf("rank[%i]: %i\n", i, vars.originalRoleRank[i]);
			//}
			//menu::getPresence()->refreshSession([] {
			//	vars.unlockAllMaxRoleAwards = true;
			//});
		})
		.addTooltip("This spoofs all roles to max. will give you a reward for all of them only once."));

}

/*Called while in submenu*/
void networkRoleBoosterMenu::update() {}

/*Called once on submenu open*/
void networkRoleBoosterMenu::updateOnce() {}

/*Called always*/
void networkRoleBoosterMenu::featureUpdate() {
	if (vars.traderBooster) {
		setInventoryItemAmount(-735397297, 100);
	}
	if (vars.moonshinerBooster) {
		//setInventoryItemAmount(0x3155EAD4, 0);
		//setInventoryItemAmount(0xDF78113E, 20);
	}

	if (vars.unlockAllMaxRoleAwards) {
		setInventoryItemAmount(-999519443, 30);
		setInventoryItemAmount(128291088, 20);
		setInventoryItemAmount(-1399517481, 20);
		setInventoryItemAmount(1371028520, 20);
		setInventoryItemAmount(-296143963, 20);
		setInventoryItemAmount(0xCBD99B01, 100); //outlaw pass
		//setInventoryItemAmount(-361115047, 20); // halloween
		//setInventoryItemAmount(1936286816, 20); // halloween
		//setInventoryItemAmount(-697100746, 20); 
		//setInventoryItemAmount(705255263, 20);
		//setInventoryItemAmount(-471215553, 20);
		//setInventoryItemAmount(1479488426, 20);
		//setInventoryItemAmount(-170363364, 20);
		//setInventoryItemAmount(-575193412, 20);
		//setInventoryItemAmount(1364208910, 20);
		//setInventoryItemAmount(1936286816, 20);
		//setInventoryItemAmount(0x354893D4, 19001);
		//setInventoryItemAmount(0xF4DA7BF3, 19001);

		/*
		label:  | hash: 0xF4DA7BF3 | amount: 2001
		label: Wood Cabin | hash: 0x9E26C4D9 | amount: 1
		label:  | hash: 0xEA79D259 | amount: 3
		label:  | hash: 0x73696460 | amount: 3
		label: Gold Bar | hash: 0x2E16264C | amount: 958
		label:  | hash: 0x354893D4 | amount: 2001
		
		*/

		//static auto timer = GetTickCount64() + 10000;
		//if (timer < GetTickCount64()) {
		//	vars.unlockAllMaxRoleAwards = false;
		//	menu::globals(1293346).at(3009 + 1).at(7).as<int>() = vars.originalRoleRank[0]; // bounty hunter //-999519443
		//	menu::globals(1293346).at(3009 + 1).at(12).as<int>() = vars.originalRoleRank[1]; // collector // 128291088
		//	menu::globals(1293346).at(3009 + 1).at(11).as<int>() = vars.originalRoleRank[2]; // trader // -1399517481
		//	menu::globals(1293346).at(3009 + 1).at(15).as<int>() = vars.originalRoleRank[3]; // moonshiner  // 1371028520
		//	menu::globals(1293346).at(3009 + 1).at(18).as<int>() = vars.originalRoleRank[4]; //naturalist // -296143963
		//}
	}
}

networkRoleBoosterMenu* _instance;
networkRoleBoosterMenu* networkRoleBoosterMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkRoleBoosterMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkRoleBoosterMenu::~networkRoleBoosterMenu() { delete _instance; }