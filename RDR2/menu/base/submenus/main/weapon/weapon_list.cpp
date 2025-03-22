#include "weapon_list.hpp"
#include "menu/base/submenu_handler.hpp"
#include "weapons.hpp"
#include "global/arrays.hpp"
#include "weapon_list_full.hpp"

using namespace weaponsListMenuVars;

namespace weaponsListMenuVars {
	vars_ vars;
}

void weaponsListMenu::init() {
	setName("Weapon List");
	setParentSubmenu<weaponsMenu>();

	addOption(submenuOption("Full Weapon List")
		.addTranslation()
		.addSubmenu<weaponsListFullMenu>());

	addOption(breakOption("Favorites")
		.addTranslation());

	for (std::size_t i = 0; i < NUMOF(global::arrays::g_weaponList); i++) {
		addOption(buttonOption(global::arrays::g_weaponList[i].name)
			.addOnClick([=] { weaponsMenuVars::givePedWeapon(menu::getLocalPlayer().ped, global::arrays::g_weaponList[i].result, 9999); }));
	}
}

/*Called while in submenu*/
void weaponsListMenu::update() {}

/*Called once on submenu open*/
void weaponsListMenu::updateOnce() {}

/*Called always*/
void weaponsListMenu::featureUpdate() {}

weaponsListMenu* _instance;
weaponsListMenu* weaponsListMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new weaponsListMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

weaponsListMenu::~weaponsListMenu() { delete _instance; }