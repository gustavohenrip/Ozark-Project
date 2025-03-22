#include "post_office_locations.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../teleport.hpp"

using namespace postOfficeLocationsMenuVars;

namespace postOfficeLocationsMenuVars {
	vars_ vars;
	
	scrollStruct<math::vector3<float>> postOffice[7] = {
		{ "Blackwater", { -866.145264f, -1332.811890f, 43.375f } },
		{ "Valentine", { -183.825241f, 616.845764f, 113.445f } },
		{ "Emerald Station", {1528.033447f, 443.874390f, 90.218f } },
		{ "Annesburg", { 2925.186768f, 1285.521362f, 44.431f } },
		{ "Rhodes", {1237.988403f, -1289.200195f, 76.911f } },
		{ "Van Horn", { 2996.714355f, 573.376099f, 44.283f } },
		{ "Saint Dennis", { 2736.017578f, -1389.087891f, 46.196f } },
	};
}

void postOfficeLocationsMenu::init() {
	setName("Post Offices");
	setParentSubmenu<teleportMenu>();

	for (std::size_t i = 0; i < NUMOF(postOffice); i++) {
		addOption(buttonOption(postOffice[i].name)
			.addOnClick([=] { teleportMenuVars::teleportToLocation(postOffice[i].result); }));
	}
}

/*Called while in submenu*/
void postOfficeLocationsMenu::update() {}

/*Called once on submenu open*/
void postOfficeLocationsMenu::updateOnce() {}

/*Called always*/
void postOfficeLocationsMenu::featureUpdate() {}

postOfficeLocationsMenu* _instance;
postOfficeLocationsMenu* postOfficeLocationsMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new postOfficeLocationsMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

postOfficeLocationsMenu::~postOfficeLocationsMenu() { delete _instance; }