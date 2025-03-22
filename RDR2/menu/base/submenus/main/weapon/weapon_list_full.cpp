#include "weapon_list_full.hpp"
#include "menu/base/submenu_handler.hpp"
#include "weapon_list.hpp"
#include "global/arrays.hpp"
#include "weapons.hpp"

using namespace weaponsListFullMenuVars;

namespace weaponsListFullMenuVars {
	vars_ vars;
}

void weaponsListFullMenu::init() {
	setName("Full Weapon List");
	setParentSubmenu<weaponsListMenu>();
}

/*Called while in submenu*/
void weaponsListFullMenu::update() {}

/*Called once on submenu open*/
void weaponsListFullMenu::updateOnce() {
	clearOptionsOffset(0);

	for (std::size_t i = 0; i < global::vars::g_weaponHashes.size(); i++) {
		if (global::vars::g_weaponHashes[i]) {
			auto name = native::getWeaponName(global::vars::g_weaponHashes[i]);
			if (name) {
				name = native::getLabelText(name);
				if (name) {
					addOption(buttonOption(std::string(name == NULL ? "~Unnamed Weapon~" : name))
						.addOnClick([=] { weaponsMenuVars::givePedWeapon(menu::getLocalPlayer().ped, global::vars::g_weaponHashes[i], 9999); }));
				}
			}
		}
	}
}

/*Called always*/
void weaponsListFullMenu::featureUpdate() {}

weaponsListFullMenu* _instance;
weaponsListFullMenu* weaponsListFullMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new weaponsListFullMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

weaponsListFullMenu::~weaponsListFullMenu() { delete _instance; }