#include "train_locations.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../teleport.hpp"

using namespace trainLocationsMenuVars;

namespace trainLocationsMenuVars {
	vars_ vars;

	scrollStruct<math::vector3<float>> stations[6] = {
		{ "Flatneck Station", { -334.879944f, -353.938690f, 0.f } },
		{ "Bacchus Station", { 599.141113f, 1697.377441f, 0.f } },
		{ "Wallace Station / General Store", {  - 1306.688354f, 398.571930f, 0.f } },
		{ "Riggs Station", { -1101.344727f, -586.271057f, 0.f } },
		{ "Benedict Point", { -5229.225098f, -3474.161133f, 0.f } },
		{ "Valentine", { -158.035507f, 622.515320f, 0.f } },
	};
}

void trainLocationsMenu::init() {
	setName("Train Stations");
	setParentSubmenu<teleportMenu>();

	for (std::size_t i = 0; i < NUMOF(stations); i++) {
		addOption(buttonOption(stations[i].name)
			.addOnClick([=] { teleportMenuVars::teleportToLocation(stations[i].result); }));
	}
}

/*Called while in submenu*/
void trainLocationsMenu::update() {}

/*Called once on submenu open*/
void trainLocationsMenu::updateOnce() {}

/*Called always*/
void trainLocationsMenu::featureUpdate() {}

trainLocationsMenu* _instance;
trainLocationsMenu* trainLocationsMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new trainLocationsMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

trainLocationsMenu::~trainLocationsMenu() { delete _instance; }