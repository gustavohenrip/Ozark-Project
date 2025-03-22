#include "popular_locations.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../teleport.hpp"

using namespace popularLocationsMenuVars;

namespace popularLocationsMenuVars {
	vars_ vars;

	scrollStruct<math::vector3<float>> list[4] = {
		{ "Sisika Penitentiary", {3363.448242f, -681.281067f, 46.475f} },
		{ "Fort Wallace", { 362.430481f, 1461.045532f, 179.191f } },
		{ "Macfarlane's Ranch", { -2375.800537f, -2392.845459f, 61.392f} },
		{ "Thieves Landing", {-1423.340210f, -2292.360596f, 43.028f } },
	};
}

void popularLocationsMenu::init() {
	setName("Popular");
	setParentSubmenu<teleportMenu>();

	for (std::size_t i = 0; i < NUMOF(list); i++) {
		addOption(buttonOption(list[i].name)
			.addOnClick([=] { teleportMenuVars::teleportToLocation(list[i].result); }));
	}
}

/*Called while in submenu*/
void popularLocationsMenu::update() {}

/*Called once on submenu open*/
void popularLocationsMenu::updateOnce() {}

/*Called always*/
void popularLocationsMenu::featureUpdate() {}

popularLocationsMenu* _instance;
popularLocationsMenu* popularLocationsMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new popularLocationsMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

popularLocationsMenu::~popularLocationsMenu() { delete _instance; }