#include "rank_spoofing.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../account.hpp"
#include "utils/caller.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/util/timers.hpp"

using namespace networkAccountRankSpooferMenuVars;

namespace networkAccountRankSpooferMenuVars {
	vars_ vars;

	void spoofRank(uint32_t rank) {
		auto invPtr = caller::call<uint64_t>(global::vars::g_getInventoryList, *(uint64_t*)global::vars::g_inventoryPool, 2);

		if (invPtr) {
			auto rankPtr = *(uint64_t*)(invPtr + (8 * 4) + 0x1D8);
			if (rankPtr) {
				if (!vars.originalRank) {
					vars.originalRank = *(uint32_t*)(rankPtr + 0x18);
				}
				*(uint32_t*)(rankPtr + 0x18) = rank;
				if (vars.saveToConfig) {
					utils::getConfig()->writeInt("Spoofing: Rank", "Rank", rank);
				}
			}
		}
	}

	int32_t _1337(int64_t *instance) {
		static std::vector<int> order = { 1, 13, 133, 1337, 337, 37 ,7 };
		*instance = *instance + 1;
		return order[*instance % order.size()];
	}

}

void networkAccountRankSpooferMenu::init() {
	setName("Spoofing: Rank");
	setParentSubmenu<networkAccountMenu>();

	addOption(buttonOption("Custom Input")
		.addTranslation()
		.addKeyboard("Enter rank", 10, [](const char* rank) { 
			vars.spoofedRank = (uint32_t)std::stoi(rank);
		}));

	addOption(buttonOption("Reset to Default")
		.addTranslation()
		.addOnClick([] { 
			vars.spoofedRank = vars.originalRank;
			spoofRank(vars.originalRank);
		}));

	addOption(toggleOption("Save to Config")
		.addTranslation()
		.addToggle(vars.saveToConfig).canBeSaved(name.getOriginal()));

	vars.previewButton = addOption(buttonOption("")
		.addOnClick([] { spoofRank(vars.spoofedRank); }));

	addOption(breakOption("Cycling"));

	vars.hackerButton = addOption(toggleOption("Hacker")
		.addOnClick([] { vars.saveToConfig = false; })
		.addToggle(vars._1337));

	if (vars.saveToConfig) {
		vars.spoofedRank = utils::getConfig()->readInt("Spoofing: Rank", "Rank", vars.spoofedRank);
	}
}

/*Called while in submenu*/
void networkAccountRankSpooferMenu::update() {
	snprintf(vars.previewName, 50, "Apply: %i", vars.spoofedRank);
	vars.previewButton->setName(vars.previewName);

	static int timer = 0;
	static int64_t instance = 0;
	menu::util::getTimers()->runTimedFunction(&timer, 500, [] {
		snprintf(vars.hackerName, 50, "Hacker: %i", _1337(&instance));
	});
	vars.hackerButton->setName(vars.hackerName);
}

/*Called once on submenu open*/
void networkAccountRankSpooferMenu::updateOnce() {}

/*Called always*/
void networkAccountRankSpooferMenu::featureUpdate() {

	if (vars.saveToConfig) {
		spoofRank(vars.spoofedRank);
	}

	if (vars._1337) {
		static int timer = 0;
		static int64_t instance = 0;
		menu::util::getTimers()->runTimedFunction(&timer, 500, [] {
			spoofRank(_1337(&instance));
		});
	}

}

networkAccountRankSpooferMenu* _instance;
networkAccountRankSpooferMenu* networkAccountRankSpooferMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkAccountRankSpooferMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkAccountRankSpooferMenu::~networkAccountRankSpooferMenu() { delete _instance; }