#include "rid_spoofing.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../account.hpp"
#include "global/auth_vars.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/notify.hpp"

using namespace networkRockstarIDSpoofingMenuVars;

namespace networkRockstarIDSpoofingMenuVars {
	vars_ vars;
	
	void spoofRID(uint64_t rid, bool def = false) {
		if (def) {
			utils::getConfig()->writeUInt64("Spoofing: Rockstar ID", "RID", global::auth::g_rockstarId);
			return;
		}

		vars.spoofingRockstarID = rid;
		if (vars.configSaveRockstarID) {
			utils::getConfig()->writeUInt64("Spoofing: Rockstar ID", "RID", rid);
		}
	}
}

void networkRockstarIDSpoofingMenu::init() {
	setName("Spoofing: Rockstar ID");
	setParentSubmenu<networkAccountMenu>();

	vars.spoofingRockstarID = global::auth::g_rockstarId;

	addString("~e~Error~q~: You can't set your Rockstar ID to 0!");

	addOption(buttonOption("Custom Input")
		.addTranslation()
		.addKeyboard("Enter rockstar ID", 16, [this] (const char* name) {
			vars.tempSpoofingRockstarID = atoll(name);
			if (vars.tempSpoofingRockstarID == 0) {
				menu::getNotify()->right(getString("~e~Error~q~: You can't set your Rockstar ID to 0!"));
			}
		}));

	addOption(buttonOption("Reset to Default")
		.addTranslation()
		.addOnClick([] { spoofRID(global::auth::g_rockstarId); }));

	addOption(toggleOption("Save to Config")
		.addTranslation()
		.addToggle(vars.configSaveRockstarID).canBeSaved(name.getOriginal())
		.addOnClick([] { spoofRID(0, true); }));

	vars.applyButton = addOption(buttonOption("")
		.addOnClick([] { spoofRID(vars.tempSpoofingRockstarID); }));

	if (vars.configSaveRockstarID) {
		vars.spoofingRockstarID = utils::getConfig()->readUInt64("Spoofing: Rockstar ID", "RID", vars.spoofingRockstarID);
	}
}

/*Called while in submenu*/
void networkRockstarIDSpoofingMenu::update() {
	snprintf(vars.previewName, 50, "Apply: %I64d", vars.tempSpoofingRockstarID);
	vars.applyButton->setName(vars.previewName);

	if (vars.spoofingRockstarID == 0) {
		vars.spoofingRockstarID = global::auth::g_rockstarId;
	}

	if (vars.tempSpoofingRockstarID == 0) {
		vars.tempSpoofingRockstarID = global::auth::g_rockstarId;
	}
}

/*Called once on submenu open*/
void networkRockstarIDSpoofingMenu::updateOnce() {}

/*Called always*/
void networkRockstarIDSpoofingMenu::featureUpdate() {}

networkRockstarIDSpoofingMenu* _instance;
networkRockstarIDSpoofingMenu* networkRockstarIDSpoofingMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkRockstarIDSpoofingMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkRockstarIDSpoofingMenu::~networkRockstarIDSpoofingMenu() { delete _instance; }