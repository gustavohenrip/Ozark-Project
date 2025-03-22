#include "weapon.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../main.hpp"
#include "weapon/weapon_mods.hpp"
#include "weapon/weapons.hpp"
#include "rage/engine.hpp"

using namespace weaponMenuVars;

namespace weaponMenuVars {
	vars_ vars;
}

void weaponMenu::init() {
	setName("Weapon");
	setParentSubmenu<mainMenu>();

	addOption(submenuOption("Weapons")
		.addTranslation()
		.addSubmenu<weaponsMenu>());

	addOption(submenuOption("Weapon Mods")
		.addTranslation()
		.addSubmenu<weaponModsMenu>());
}

/*Called while in submenu*/
void weaponMenu::update() {}

/*Called once on submenu open*/
void weaponMenu::updateOnce() {
	if (!global::vars::g_weaponHashes.size()) {
		global::vars::g_weaponHashes = rage::engine::getHashListFromHashTable((*(WORD*)global::vars::g_weaponHashListCount) - 1, *(uint64_t*)global::vars::g_weaponHashList);
	}

	if (!global::vars::g_weaponComponentHashes.size()) {
		global::vars::g_weaponComponentHashes = rage::engine::getHashListFromHashTable((*(WORD*)global::vars::g_weaponComponentHashListCount) - 1, *(uint64_t*)global::vars::g_weaponComponentHashList);
	}
}

/*Called always*/
void weaponMenu::featureUpdate() {}

weaponMenu* _instance;
weaponMenu* weaponMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new weaponMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

weaponMenu::~weaponMenu() { delete _instance; }