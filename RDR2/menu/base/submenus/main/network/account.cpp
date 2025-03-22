#include "account.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network.hpp"
#include "account/treasure.hpp"
#include "account/name_changer.hpp"
#include "account/rid_spoofing.hpp"
#include "account/ip_spoofing.hpp"
#include "menu/base/util/notify.hpp"
#include "account/role_booster.hpp"
#include "account/rank_spoofing.hpp"

using namespace networkAccountMenuVars;

namespace networkAccountMenuVars {
	vars_ vars;
}

void networkAccountMenu::init() {
	setName("Account");
	setParentSubmenu<networkMenu>();

	addOption(submenuOption("Treasure ~e~[RISK]")
		.addTranslation()
		.addSubmenu<networkTreasureMenu>()
		.addOnClick([] { menu::getNotify()->right("By entering this menu, you're acknowledging treasure is a ~e~risk"); }));

	addOption(submenuOption("Role Boosters ~e~[RISK]")
		.addTranslation()
		.addSubmenu<networkRoleBoosterMenu>()
		.addOnClick([] { menu::getNotify()->right("By entering this menu, you're acknowledging Role Boosting is a ~e~risk"); }));

	addOption(submenuOption("Spoofing: Name")
		.addTranslation()
		.addSubmenu<networkAccountNameChangerMenu>());

	addOption(submenuOption("Spoofing: Rockstar ID")
		.addTranslation()
		.addSubmenu<networkRockstarIDSpoofingMenu>());

	addOption(submenuOption("Spoofing: IP")
		.addTranslation()
		.addSubmenu<networkIPSpoofingMenu>());

	addOption(submenuOption("Spoofing: Rank")
		.addTranslation()
		.addSubmenu<networkAccountRankSpooferMenu>());
}

/*Called while in submenu*/
void networkAccountMenu::update() {}

/*Called once on submenu open*/
void networkAccountMenu::updateOnce() {}

/*Called always*/
void networkAccountMenu::featureUpdate() {}

networkAccountMenu* _instance;
networkAccountMenu* networkAccountMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkAccountMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkAccountMenu::~networkAccountMenu() { delete _instance; }