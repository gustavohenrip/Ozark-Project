#include "train.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../world.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "menu/base/util/notify.hpp"

using namespace trainMenuVars;

namespace trainMenuVars {
	vars_ vars;
	
	bool grabTrain(trainMenu* pthis) {
		vars.train = 0;
		menu::pools::getVehiclePool()->run([&](Vehicle vehicle) { if (native::isThisModelATrain(native::getEntityModel(vehicle))) { vars.train = vehicle; } });
		if (native::doesEntityExist(vars.train)) {
			return true;
		} 

		menu::getNotify()->right(pthis->getString("~e~Error~q~: Train not found"));
		return false;
	}
}

void trainMenu::init() {
	setName("Train");
	setParentSubmenu<worldMenu>();

	addString("~e~Error~q~: Train not found");

	addOption(buttonOption("Find Train")
		.addTranslation()
		.addOnClick([this] { vars.hasTrain = grabTrain(this); }));

	addOption(breakOption("Features")
		.addTranslation()
		.addRequirement([] { return vars.hasTrain; }));

	addOption(buttonOption("Drive")
		.addTranslation()
		.addOnClick([] { native::setPedIntoVehicle(menu::getLocalPlayer().ped, vars.train, -1); })
		.addRequirement([] { return vars.hasTrain; }));

	addOption(numberOption<float>(SCROLL, "Set Speed")
		.addTranslation()
		.addNumber(vars.speed, "%.0f", 1.f).addMin(1.f).addMax(9999.f).setScrollSpeed(2)
		.addOnClick([] { native::setTrainSpeed(vars.train, vars.speed); })
		.addRequirement([] { return vars.hasTrain; }));
}

/*Called while in submenu*/
void trainMenu::update() {}

/*Called once on submenu open*/
void trainMenu::updateOnce() {}

/*Called always*/
void trainMenu::featureUpdate() {}

trainMenu* _instance;
trainMenu* trainMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new trainMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

trainMenu::~trainMenu() { delete _instance; }