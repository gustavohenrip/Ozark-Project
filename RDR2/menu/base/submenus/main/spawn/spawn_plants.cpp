#include "spawn_plants.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../spawn.hpp"
#include "menu/base/util/fiber_pool.hpp"
#include "menu/base/util/fiber_manager.hpp"
#include "menu/base/util/notify.hpp"


using namespace spawnPlantsMenuVars;

namespace spawnPlantsMenuVars {
	vars_ vars;

	scrollStruct<Hash> plants[] = {
		"Acuna's Star Orchid", 0xB67F7FEA,
		"Alaskin Ginseng", 0x791CB060,
		"American Ginseng", 0xA8C87FE6,
		"Bay Bolete", 0xD0EE5547,
		"Blackberry", 0x218ACC70,
		"Blackcurrant", 0x8A7249DF,
		"Burdock Root", 0xB12DCCFC,
		"Chanterelle", 0x52329977,
		"Cigar Orchid", 0x2729F09C,
		"Clamshell Orchid", 0xC0304A5E,
		"Common Bulrush", 0xE008DF3,
		"Creeping Thyme", 0x58F87EA3,
		"Desert Sage", 0x934CDF53,
		"Dragon's Mouth Orchid", 0x6083EBAD,
		"English Mace", 0x131DAC12,
		"Evergreen Huckleberry", 0xCFA5E0D2,
		"Ghost Orchid", 0xE71B4B16,
		"Golden Currant", 0xC8B01F47,
		"Hummingbird Sage", 0x2781F602,
		"Indian Tobacco", 0x7C9228C5,
		"Lady of the Night Orchid", 0x528A26DA,
		"Lady Slipper Orchid", 0x1234C200,
		"Milkweed", 0x7E2FD03E,
		"Moccasin Flower Orchid", 0x40A3D479,
		"Night Scented Orchid", 0xD28A216B,
		"Oleander Sage", 0xAB009D3B,
		"Oregano", 0x52902D4F,
		"Parasol Mushroom", 0xB716E0C5,
		"Prairie Poppy", 0x22A9A3F5,
		"Queen's Orchid", 0xB33CCE55,
		"Ram's Head", 0xDBDD6BA6,
		"Rat Tail Orchid", 0x2C2C6F30,
		"Raspberry", 0x1B68A274,
		"Red Sage", 0xD3D1E587,
		"Sparrow's Egg Orchid", 0xCCA325A8,
		"Spider Orchid", 0x77CDB9AC,
		"Vanilla Flower", 0x40DF2FB0,
		"Violet Snowdrop", 0x1F3E2AA9,
		"Agarita", 0x35835260,
	};


	void spawnPlant(Hash hash, math::vector3<float> coords) {
		static bool isJobRunning = false;
		if (!isJobRunning) {
			menu::util::getFiberPool()->run([=] {
				isJobRunning = true;
				native::requestComposite(hash);
				auto timer = GetTickCount64() + 1000;
				while (!native::hasCompositeLoaded(hash) && timer > GetTickCount64()) {
					menu::util::waitFiber(0);
				}
				if (timer < GetTickCount64()) {
					isJobRunning = false;
					return;
				} else {
					int unk;
					int bit = 0;
					bit |= 2;
					auto handle = native::createComposite(hash, coords, .0f, 0, &unk, -1);
					native::networkSetEntityVisibleToNetwork(handle, TRUE);
					native::networkRequestControlOfEntity(handle);
					native::setNetworkIdExistsOnAllMachines(native::networkGetNetworkIdFromEntity(handle), TRUE);
					isJobRunning = false;
				}

			});
		}
		else {
			menu::getNotify()->right("~e~Warning~q~: Plant spawn job is still active!");
		}
	}


}

void spawnPlantsMenu::init() {
	setName("Plants");
	setParentSubmenu<spawnMenu>();

	for (int i = 0; i < NUMOF(plants); i++) {
		addOption(buttonOption(plants[i].name)
			.addOnClick([=] { 
				spawnPlant(plants[i].result, native::getOffsetFromEntityInWorldCoords(menu::getLocalPlayer().ped, { .0f, 1.4f, .0f }));
			}));
	}

}

/*Called while in submenu*/
void spawnPlantsMenu::update() {
}

/*Called once on submenu open*/
void spawnPlantsMenu::updateOnce() {}

/*Called always*/
void spawnPlantsMenu::featureUpdate() {
}

spawnPlantsMenu* _instance;
spawnPlantsMenu* spawnPlantsMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new spawnPlantsMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

spawnPlantsMenu::~spawnPlantsMenu() { delete _instance; }