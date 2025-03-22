#include "model.hpp"
#include "menu/base/submenu_handler.hpp"
#include "appearance.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/util/global.hpp"
#include "menu/base/util/fiber_manager.hpp"
#include "model_change.hpp"

using namespace modelMenuVars;

namespace modelMenuVars {
	vars_ vars;

	void changeModel(Hash hash) {
		global::vars::g_changeModel = true;
		menu::getControlManager()->requestModel(hash, [] (Hash h) {
			native::setPlayerModel(menu::getLocalPlayer().id, h, 0);
			//auto backup = menu::globals(1935630).at(2).as<Hash>();
			auto backup = menu::globals(1102219).at(3578).as<Hash>();
			//Global_1102219->f_3578
			menu::globals(1102219).at(3578).as<Hash>() = h;
			//menu::globals(1935630).at(2).as<Hash>() = h;
			//menu::globals(40).at(39).as<Hash>() = h;
			menu::util::waitFiber(250);

			native::setPedDefaultOutfit(menu::getLocalPlayer().ped, TRUE);
			if (native::getEntityModel(menu::getLocalPlayer().ped) != h) {
				menu::globals(1935630).at(2).as<Hash>() = backup;
				//menu::globals(40).at(39).as<Hash>() = backup;
			}
		});
	}

	//void changeModel(Hash hash) {
	//	menu::getControlManager()->requestModel(hash, [](Hash h) {
	//		native::setPlayerModel(menu::getLocalPlayer().id, h, 0);
	//	});
	//}
}

void modelMenu::init() {
	setName("Model");
	setParentSubmenu<appearanceMenu>();

	addOption(buttonOption("Refresh Model")
		.addTranslation()
		.addOnClick([] {
			native::clearPedBloodDamage(menu::getLocalPlayer().ped);
			native::clearPedDecorations(menu::getLocalPlayer().ped);
			native::clearPedDamageDecalByZone(menu::getLocalPlayer().ped, 10, "ALL");
			native::clearPedDirt(menu::getLocalPlayer().ped);
			native::clearPedWetness(menu::getLocalPlayer().ped);
		}));

	addOption(buttonOption("Custom Input")
		.addTranslation()
		.addKeyboard("Enter hash or name of model", 100, [] (const char* res) {
			if (res) {
				if (res[0] == '0' && (res[1] == 'x' || res[1] == 'X')) {
					// hash
					changeModel((DWORD)_strtoui64(res, 0, 0));
				} else {
					std::string str(res);
					if (std::find_if(str.begin(), str.end(), [] (unsigned char c) { return !std::isdigit(c); }) == str.end()) {
						// numeric
						changeModel((DWORD)atoi(res));
					} else {
						// str
						changeModel(native::getHashKey(res));
					}
				}
			}
		}));

	addOption(breakOption("Models"));

	addOption(submenuOption("Online")
		.addSubmenu<modelChangeMenu>()
		.addOnClick([] { modelChangeMenuVars::vars.list = global::arrays::g_pedModelOnline; modelChangeMenuVars::vars.size = NUMOF(global::arrays::g_pedModelOnline); })
		.addRequirement([] { return native::networkIsInSession(); }));

	addOption(submenuOption("Alligators")
		.addSubmenu<modelChangeMenu>()
		.addOnClick([] { modelChangeMenuVars::vars.list = global::arrays::g_pedModelAlligators; modelChangeMenuVars::vars.size = NUMOF(global::arrays::g_pedModelAlligators); }));

	addOption(submenuOption("Dogs")
		.addSubmenu<modelChangeMenu>()
		.addOnClick([] { modelChangeMenuVars::vars.list = global::arrays::g_pedModelDogs; modelChangeMenuVars::vars.size = NUMOF(global::arrays::g_pedModelDogs); }));

	addOption(submenuOption("Sparrows")
		.addSubmenu<modelChangeMenu>()
		.addOnClick([] { modelChangeMenuVars::vars.list = global::arrays::g_pedModelSparrows; modelChangeMenuVars::vars.size = NUMOF(global::arrays::g_pedModelSparrows); })
		.addRequirement([] { return !native::networkIsInSession(); }));

	addOption(submenuOption("Snakes")
		.addSubmenu<modelChangeMenu>()
		.addOnClick([] { modelChangeMenuVars::vars.list = global::arrays::g_pedModelSnake; modelChangeMenuVars::vars.size = NUMOF(global::arrays::g_pedModelSnake); }));

	addOption(submenuOption("Males")
		.addSubmenu<modelChangeMenu>()
		.addOnClick([] { modelChangeMenuVars::vars.list = global::arrays::g_pedModelMales; modelChangeMenuVars::vars.size = NUMOF(global::arrays::g_pedModelMales); }));

	addOption(submenuOption("Females")
		.addSubmenu<modelChangeMenu>()
		.addOnClick([] { modelChangeMenuVars::vars.list = global::arrays::g_pedModelFemales; modelChangeMenuVars::vars.size = NUMOF(global::arrays::g_pedModelFemales); }));

	addOption(submenuOption("Horses")
		.addSubmenu<modelChangeMenu>()
		.addOnClick([] { modelChangeMenuVars::vars.list = global::arrays::g_pedModelHorses; modelChangeMenuVars::vars.size = NUMOF(global::arrays::g_pedModelHorses); }));
}

/*Called while in submenu*/
void modelMenu::update() {}

/*Called once on submenu open*/
void modelMenu::updateOnce() {}

/*Called always*/
void modelMenu::featureUpdate() {}

modelMenu* _instance;
modelMenu* modelMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new modelMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

modelMenu::~modelMenu() { delete _instance; }