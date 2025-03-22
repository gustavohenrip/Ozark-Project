#include "spawn_ped_list.hpp"
#include "menu/base/submenu_handler.hpp"
#include "spawn_ped.hpp"

using namespace spawnPedListMenuVars;

namespace spawnPedListMenuVars {
	vars_ vars;
}

void spawnPedListMenu::init() {
	setName("Spawn Ped");
	setParentSubmenu<spawnPedMenu>();
}

/*Called while in submenu*/
void spawnPedListMenu::update() {}

/*Called once on submenu open*/
void spawnPedListMenu::updateOnce() {
	clearOptionsOffset(0);

	if (vars.list || vars.listVariant) {
		for (int i = 0; i < vars.size; i++) {
			addOption(buttonOption(vars.listVariants ? vars.listVariant[i].name : vars.list[i].name)
				.addOnClick([=] {
					if (!vars.listVariants) {
						spawnPedMenuVars::spawnPed(vars.list[i].name, vars.list[i].result);
					} else {
						spawnPedMenuVars::spawnPed(vars.listVariant[i].name, vars.listVariant[i].result.hash, vars.listVariant[i].result.variant);
					}
				}));
		}
	} else {
		addOption(buttonOption("An error occured populating list :(")
			.addTranslation());
	}
}

/*Called always*/
void spawnPedListMenu::featureUpdate() {}

spawnPedListMenu* _instance;
spawnPedListMenu* spawnPedListMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new spawnPedListMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

spawnPedListMenu::~spawnPedListMenu() { delete _instance; }