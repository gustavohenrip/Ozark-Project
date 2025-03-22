#include "misc_display.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../misc.hpp"
#include "menu/base/util/stacked_text_render.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "menu/base/util/timers.hpp"
#include "utils/va.hpp"

using namespace miscDisplayMenuVars;

namespace miscDisplayMenuVars {
	vars_ vars;

	scrollStruct<int> fps[] = {
		"FPS", 0,
		"Average", 1,
		"Both", 2
	};
}

void miscDisplayMenu::init() {
	setName("Display");
	setParentSubmenu<miscMenu>();

	auto fpsOption = addOption(scrollOption<int>(TOGGLE, "FPS")
		.addTranslation()
		.addToggle(vars.displayFPS)
		.addScroll(vars.displayFPSVar, 0, NUMOF(fps), fps).canBeSaved(name.getOriginal())
		.addOnClick([] {
			if (vars.displayFPS) {
				menu::util::getStackedTextRenderer()->alloc("StackedRenderer_DisplayFPS");
			} else {
				menu::util::getStackedTextRenderer()->dealloc("StackedRenderer_DisplayFPS");
			}
		}));
	
	fpsOption->getOnClick()();

	auto positionOption = addOption(toggleOption("Position")
		.addTranslation()
		.addToggle(vars.displayPosition).canBeSaved(name.getOriginal())
		.addOnClick([] {
			if (vars.displayPosition) {
				menu::util::getStackedTextRenderer()->alloc("StackedRenderer_DisplayPosition");
			} else {
				menu::util::getStackedTextRenderer()->dealloc("StackedRenderer_DisplayPosition");
			}
		}));
	
	positionOption->getOnClick()();

	auto networkPoolOption = addOption(toggleOption("Network Pool")
		.addTranslation()
		.addToggle(vars.displayNetworkPool).canBeSaved(name.getOriginal())
		.addOnClick([] {
			if (vars.displayNetworkPool) {
				menu::util::getStackedTextRenderer()->alloc("StackedRenderer_DisplayNetworkPool");
			} else {
				menu::util::getStackedTextRenderer()->dealloc("StackedRenderer_DisplayNetworkPool");
			}
		}));

	networkPoolOption->getOnClick()();

	auto entityPoolOption = addOption(toggleOption("Entity Pools")
		.addTranslation()
		.addToggle(vars.displayPools).canBeSaved(name.getOriginal())
		.addOnClick([] {
			if (vars.displayPools) {
				menu::util::getStackedTextRenderer()->alloc("StackedRenderer_AttachmentPool");
				menu::util::getStackedTextRenderer()->alloc("StackedRenderer_ObjectPool");
				menu::util::getStackedTextRenderer()->alloc("StackedRenderer_PedPool");
				menu::util::getStackedTextRenderer()->alloc("StackedRenderer_PickupPool");
				menu::util::getStackedTextRenderer()->alloc("StackedRenderer_VehiclePool");
			} else {
				menu::util::getStackedTextRenderer()->dealloc("StackedRenderer_AttachmentPool");
				menu::util::getStackedTextRenderer()->dealloc("StackedRenderer_ObjectPool");
				menu::util::getStackedTextRenderer()->dealloc("StackedRenderer_PedPool");
				menu::util::getStackedTextRenderer()->dealloc("StackedRenderer_PickupPool");
				menu::util::getStackedTextRenderer()->dealloc("StackedRenderer_VehiclePool");
			}
		}));

	entityPoolOption->getOnClick()();
}

/*Called while in submenu*/
void miscDisplayMenu::update() {}

/*Called once on submenu open*/
void miscDisplayMenu::updateOnce() {}

/*Called always*/
void miscDisplayMenu::featureUpdate() {
	static int timer = 0;
	menu::util::getTimers()->runTimedFunction(&timer, 1000, [] {
		vars.drawFPSCache[1] = vars.drawFPSCache[0];
		vars.drawFPSCache[0] = rage::engine::getFrameCount();
	});

	if (vars.displayPosition) {
		auto coords = menu::getLocalPlayer().coords;
		menu::util::getStackedTextRenderer()->update("StackedRenderer_DisplayPosition", utils::va::va("Position: x=~o~%.3f~q~ y=~o~%.3f~q~ z=~o~%.3f~q~", coords.x, coords.y, coords.z));
	}

	if (vars.displayNetworkPool) {
		auto pool = menu::pools::getNetEventQueuePool()->getPoolPointer();

		if (pool) {
			if (pool->unk > 0 && pool->pool) {
				int populated = 0;
				for (int i = 0; i < pool->count; i++) {
					if (~(*(uint8_t*)(pool->unk + i) >> 7) & 1) {
						populated++;
					}
				}

				menu::util::getStackedTextRenderer()->update("StackedRenderer_DisplayNetworkPool", utils::va::va("Outgoing Network Events: ~o~%i/%i", populated, pool->count));
			}
		}
	}

	if (vars.displayPools) {
		std::vector<std::pair<std::string, std::pair<int, int>>> info;

		std::pair<std::string, entityPool*> pools[] = {
			{ "Attachment Pool", menu::pools::getAttachmentPool()->getPoolPointer() },
			{ "Object Pool", menu::pools::getObjectPool()->getPoolPointer() },
			{ "Ped Pool", menu::pools::getPedPool()->getPoolPointer() },
			{ "Pickup Pool", menu::pools::getPickupPool()->getPoolPointer() },
			{ "Vehicle Pool", menu::pools::getVehiclePool()->getPoolPointer() }
		};

		for (int i = 0; i < 5; i++) {
			auto pool = pools[i];
			if (pool.second) {
				if (pool.second->unk <= 0) continue;
				if (!pool.second->pool) continue;

				int populated = 0;
				for (int i = 0; i < pool.second->count; i++) {
					if (~(*(uint8_t*)(pool.second->unk + i) >> 7) & 1) {
						populated++;
					}
				}

				info.push_back(std::make_pair(pool.first, std::make_pair(pool.second->count, populated)));
			}
		}

		for (std::size_t i = 0; i < info.size(); i++) {
			std::string name = pools[i].first;
			name.erase(remove(name.begin(), name.end(), ' '), name.end());
			menu::util::getStackedTextRenderer()->update(("StackedRenderer_" + name), utils::va::va("%s: ~o~%i/%i", info[i].first.c_str(), info[i].second.second, info[i].second.first));
		}
	}

	if (vars.displayFPS) {
		int FPS = vars.drawFPSCache[0] - vars.drawFPSCache[1] - 1;

		if (vars.drawFPSAverageCount < 900) {
			vars.drawFPSAverageTotal += FPS;
			vars.drawFPSAverageCount++;
		} else {
			vars.drawFPSAverageTotal = vars.drawFPSAverageCount = 0;
		}

		if (vars.drawFPSAverageTotal != 0 && vars.drawFPSAverageCount != 0) {
			vars.drawFPSAverage = vars.drawFPSAverageTotal / vars.drawFPSAverageCount;
		}

		char buffer[50];
		if (vars.displayFPSVar == 0) {
			sprintf(buffer, "FPS: %s%i", FPS < 60 ? "~e~" : FPS > 100 ? "~t6~" : "", FPS);
		} else if (vars.displayFPSVar == 1) {
			sprintf(buffer, "Average FPS: %s%i", vars.drawFPSAverage < 60 ? "~e~" : vars.drawFPSAverage > 100 ? "~t6~" : "", vars.drawFPSAverage);
		} else {
			sprintf(buffer, "FPS: %s%i~q~ - Average: %s%i", FPS < 60 ? "~e~" : FPS > 100 ? "~t6~" : "", FPS, vars.drawFPSAverage < 60 ? "~e~" : vars.drawFPSAverage > 100 ? "~t6~" : "", vars.drawFPSAverage);
		}

		menu::util::getStackedTextRenderer()->update("StackedRenderer_DisplayFPS", buffer);
	}
}

miscDisplayMenu* _instance;
miscDisplayMenu* miscDisplayMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new miscDisplayMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

miscDisplayMenu::~miscDisplayMenu() { delete _instance; }